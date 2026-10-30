"""
Author: Michael Dodis <michaeldodisgr@gmail.com>
This file  builds an android NativeActivity
"""
import argparse
import platform
import re
import subprocess
import sys
import os
import shutil
import stat
import sys
from pathlib import Path

def make_clean_dir(path):
    """Delete path if it exists (including read-only files on Windows), then create it empty."""
    def force_remove(func, p, _):
        os.chmod(p, stat.S_IWRITE)
        func(p)

    path = Path(path)
    if path.exists():
        if sys.version_info >= (3, 12):
            shutil.rmtree(path, onexc=force_remove)
        else:
            shutil.rmtree(path, onerror=force_remove)
    path.mkdir(parents=True)
    return path

_CLANG_API_RE = re.compile(r"^\w+-linux-android(?:eabi)?(\d+)-clang")

def ndk_api_range(bin_path):
    """Return (min_api, max_api) from the target-prefixed clang wrappers in an NDK bin folder."""
    apis = {
        int(m.group(1))
        for f in Path(bin_path).iterdir()
        if (m := _CLANG_API_RE.match(f.name))
    }
    if not apis:
        raise FileNotFoundError(f"no <triple><api>-clang wrappers found in {bin_path}")
    return min(apis), max(apis)

def is_windows():
    return platform.system() == "Windows"

def hostname_arch():
    system = platform.system().lower()
    arch = platform.machine().lower()

    if system == "windows":
        os_name = "windows"
    elif system == "linux":
        os_name = "linux"
    elif system == "darwin":
        os_name = "macos"
    else:
        raise RuntimeError(f"Unsupported operating system: {system}")

    if arch in ("x86_64", "amd64"):
        arch = "x86_64"
    elif arch in ("aarch64", "arm64"):
        arch = "aarch64"
    elif arch in ("i386", "i686", "x86"):
        arch = "x86"
    else:
        raise RuntimeError(f"Unsupported architecture: {arch}")
    return f"{os_name}-{arch}"

def run_command(args):
    """Run a command with stdout and stderr both sent to stderr. Return True if it succeeded."""
    sys.stdout.flush()
    try:
        return subprocess.run([str(a) for a in args], stdout=sys.stderr, stderr=sys.stderr).returncode == 0
    except OSError as e:
        print(f"Failed to run \"{args[0]}\": {e}", file=sys.stderr)
        return False

def build(app_name, package_name, output_apk, filename, cpp, cflags, lflags, build_number, version, arch, minver, tgtver, intermediate, opt_env):

    # Language
    target_language = "C"
    target_compiler_name = "clang"
    if cpp:
        target_language = "C++"
        target_compiler_name = "clang++"

    script_extension = ""
    if is_windows():
        script_extension = ".cmd"

    if intermediate == None:
        intermediate = f"{package_name}_i"

    llvm_toolchain_path = Path(opt_env["sdk_ndk"]) / "toolchains" / "llvm" / "prebuilt" / hostname_arch() / "bin" 

    # Choose smallest minver if not set, highest for tgtver if not set
    if minver==-1 or tgtver==-1:
        min, max = ndk_api_range(llvm_toolchain_path)
        if minver==-1:
            minver=min
        if tgtver==-1:
            tgtver=max

    # NDK uses "androidVER" for everything except for armv7a, which is "androideabiVER"
    android_abi_prefix = "android"
    if arch == "armv7a":
        android_abi_prefix = "androideabi"

    compiler_path = llvm_toolchain_path / f"{arch}-linux-{android_abi_prefix}{minver}-{target_compiler_name}{script_extension}"
    compiler_flags = cflags.replace("\n", " ").replace("\r", "")
    compiler_links = lflags.replace("\n", " ").replace("\r", "")
    shared_lib = Path(intermediate) / Path(f"lib{app_name}.so")

    print(f"----------------------------------------------------------------------------------------")
    print(f"Building {target_language} \"{app_name}\" ({package_name}), v{version}b{build_number}")
    print(f"Min SDK Version:    {minver}")
    print(f"Target SDK Version: {tgtver}")
    print(f"SDK Build Tools:    \"{opt_env['sdk_build_tools']}\"")
    print(f"Compiler:           \"{compiler_path}\"")
    print(f"C Flags:            \"{compiler_flags}\"")
    print(f"Link Flags:         \"{compiler_links}\"")
    print(f"Intermediate:       \"{intermediate}\"")
    print(f"----------------------------------------------------------------------------------------")

    make_clean_dir(intermediate)

    print(f"Building shared library...")
    run_command([compiler_path, "-shared" , compiler_flags, filename, compiler_links, "-fPIC", "-o", shared_lib])
    pass


if __name__=="__main__":
    parser = argparse.ArgumentParser(prog="android_build.py",
                                     description="Builds android NativeActivities")

    # Build Settings
    parser.add_argument("filename")
    parser.add_argument("-n", "--app-name",     type=str,  required=False, default="Native Android App",                  help="Application Name")
    parser.add_argument("-p", "--package-name", type=str,  required=False, default="com.contoso.test.native_android_app", help="Package Name")
    parser.add_argument("-o", "--output",       type=str,  required=False, default="./native_android_app.apk",            help="Output file")
    parser.add_argument("-b", "--build-number", type=int,  required=False, default=1,                                     help="Internal build number")
    parser.add_argument("-v", "--version",      type=str,  required=False, default="1.0",                                 help="Version")
    parser.add_argument("-a", "--architecture", type=str,  required=False, default="aarch64",                             help="Architecture", choices=["aarch64", "i686", "armv7a", "x86_64"])
    parser.add_argument("--cpp",                action="store_true",                                                      help="Use C++ compiler")
    parser.add_argument("--cflags",             type=str,  required=False, default="",                                    help="Additional compiler flags")
    parser.add_argument("--lflags",             type=str,  required=False, default="",                                    help="Additional linker flags")
    parser.add_argument("--min-sdk-version",    type=int,  required=False, default=-1,                                    help="Min SDK Version (by default uses lowest available)")
    parser.add_argument("--target-sdk-version", type=int,  required=False, default=-1,                                    help="Target SDK Version (by default uses max available)")
    parser.add_argument("-i", "--intermediate", type=str,  required=False, default=None,                                  help="Intermediate directory for building. Defaults to Package Name_i")

    # Environment Settings
    parser.add_argument("--sdk-build-tools",    type=str, required=False, default=None,                                   help="SDK Build Tools path (aapt2, zipalign). Uses env if not set")
    parser.add_argument("--sdk-ndk",            type=str, required=False, default=None,                                   help="NDK root path (Android/Sdk/ndk/<version>/). Uses env if not set")
    args = parser.parse_args()

    opt_env = dict()
    opt_env["sdk_build_tools"] = args.sdk_build_tools
    opt_env["sdk_ndk"] = args.sdk_ndk
    build(args.app_name, args.package_name, args.output,
          args.filename, args.cpp, args.cflags, args.lflags,
          args.build_number, args.version,
          args.architecture,
          args.min_sdk_version, args.target_sdk_version,
          args.intermediate,
          opt_env)