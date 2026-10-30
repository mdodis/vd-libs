"""
Author: Michael Dodis <michaeldodisgr@gmail.com>
This file  builds an android NativeActivity.

To use this, you need:
- Android SDK with the JDK, build tools, and NDK.

My intention with this is to make it kind of like a compiler invocation to create an android app, which is what it
should have been, and not this ridiculous nonsense that we have to do here.

Like, Windows exes can have the icon bundled in 'em. Why not this piece of garbage?

Anyhow. It compiles alongside your code the android_native_app_glue.c file which includes some stuff so that you can
have an android_main function, which is what you need to implement. See its header for details.
"""
import argparse
import platform
import re
import subprocess
import sys
import os
import shlex
import shutil
import stat
import sys
from pathlib import Path
import xml.etree.ElementTree as ET

ANDROID_NS = "http://schemas.android.com/apk/res/android"
ET.register_namespace("android", ANDROID_NS)
_CLANG_API_RE = re.compile(r"^\w+-linux-android(?:eabi)?(\d+)-clang")

# NDK clang target arch -> the lib/<abi>/ folder name the APK installer looks in
ANDROID_ABIS = {
    "aarch64": "arm64-v8a",
    "armv7a":  "armeabi-v7a",
    "i686":    "x86",
    "x86_64":  "x86_64",
}

def android_attr(name):
    return f"{{{ANDROID_NS}}}{name}"

def android_manifest(app_name, package_name, build_number, version, native_library, has_icon, theme, minver, tgtver):

    allow_backup = "false"
    has_code = "false"
    activity_name = "android.app.NativeActivity"
    config_changes = "orientation|keyboardHidden"

    manifest = ET.Element("manifest", {
        "package": package_name,
        android_attr("versionCode"): str(build_number),
        android_attr("versionName"): str(version),
    })

    # ET.SubElement(manifest, "uses-feature", {
    #     android_attr("glEsVersion"): gl_es_version,
    #     android_attr("required"): str(gl_es_required).lower(),
    # })

    ET.SubElement(manifest, "uses-sdk", {
        android_attr("minSdkVersion"): str(minver),
        android_attr("targetSdkVersion"): str(tgtver),
    })

    application_attrs = {
        android_attr("allowBackup"): str(allow_backup).lower(),
        android_attr("hasCode"): str(has_code).lower(),
        android_attr("label"): f"{app_name}",
        android_attr("theme"): f"@android:style/{theme}",
    }

    if has_icon:
        application_attrs[android_attr("icon")] = "@mipmap/ic_launcher"

    application = ET.SubElement(manifest, "application", application_attrs)

    activity = ET.SubElement(application, "activity", {
        android_attr("name"): activity_name,
        android_attr("configChanges"): config_changes,
        android_attr("exported"): "true",
    })

    ET.SubElement(activity, "meta-data", {
        android_attr("name"): "android.app.lib_name",
        android_attr("value"): native_library,
    })

    intent_filter = ET.SubElement(activity, "intent-filter")

    ET.SubElement(intent_filter, "action", {
        android_attr("name"): "android.intent.action.MAIN",
    })

    ET.SubElement(intent_filter, "category", {
        android_attr("name"): "android.intent.category.LAUNCHER",
    })

    tree = ET.ElementTree(manifest)
    ET.indent(tree, space="    ")
    return ET.tostring(
        manifest,
        encoding="unicode",
        xml_declaration=False)

def mkdir_p(path):
    Path(path).mkdir(parents=True, exist_ok=True)

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

def ndk_toolchain_path(ndk):
    return Path(ndk) / "toolchains" / "llvm" / "prebuilt" / hostname_arch() / "bin"

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

def env_paths(*names):
    """Return the paths held by whichever of the given environment variables are set."""
    return [Path(os.environ[n]) for n in names if os.environ.get(n)]

def version_key(name):
    """Sort key for versioned dir names (36.1.0, 35.0.0-rc1, android-36), with release candidates below their release."""
    release, _, prerelease = name.partition("-rc")
    return tuple(int(n) for n in re.findall(r"\d+", release)), not prerelease

def latest_subdir(path, required):
    """Return the highest-versioned subdirectory of path that contains required, or None."""
    path = Path(path)
    if not path.is_dir():
        return None
    candidates = [d for d in path.iterdir() if (d / required).exists()]
    return max(candidates, key=lambda d: version_key(d.name), default=None)

def android_sdk_roots():
    """Candidate SDK roots: $ANDROID_HOME, $ANDROID_SDK_ROOT, then where Android Studio installs the SDK by default."""
    home = Path.home()
    system = platform.system()
    if system == "Windows":
        defaults = [Path(os.environ.get("LOCALAPPDATA", home / "AppData" / "Local")) / "Android" / "Sdk"]
    elif system == "Darwin":
        defaults = [home / "Library" / "Android" / "sdk"]
    else:
        defaults = [home / "Android" / "Sdk"]
    return env_paths("ANDROID_HOME", "ANDROID_SDK_ROOT") + defaults

def find_android_sdk_build_tools():
    aapt = "aapt.exe" if is_windows() else "aapt"
    for sdk in android_sdk_roots():
        if found := latest_subdir(sdk / "build-tools", aapt):
            return found
    return None

def find_android_sdk_ndk():
    marker = Path("toolchains") / "llvm" / "prebuilt"
    for ndk in env_paths("ANDROID_NDK_HOME", "ANDROID_NDK_ROOT", "ANDROID_NDK"):
        if (ndk / marker).is_dir():
            return ndk
    for sdk in android_sdk_roots():
        if found := latest_subdir(sdk / "ndk", marker):
            return found
        # Pre side-by-side NDK installs
        if (sdk / "ndk-bundle" / marker).is_dir():
            return sdk / "ndk-bundle"
    return None

def find_android_studio_tools():
    keytool = "keytool.exe" if is_windows() else "keytool"
    home = Path.home()
    system = platform.system()
    if system == "Windows":
        program_files = Path(os.environ.get("ProgramFiles", "C:/Program Files"))
        local_app_data = Path(os.environ.get("LOCALAPPDATA", home / "AppData" / "Local"))
        defaults = [
            program_files / "Android" / "Android Studio" / "jbr" / "bin",
            local_app_data / "Programs" / "Android Studio" / "jbr" / "bin",
        ]
    elif system == "Darwin":
        defaults = [Path(apps) / "Android Studio.app" / "Contents" / "jbr" / "Contents" / "Home" / "bin" for apps in ("/Applications", home / "Applications")]
    else:
        defaults = [Path(d) / "jbr" / "bin" for d in ("/opt/android-studio", "/usr/local/android-studio", home / "android-studio", "/snap/android-studio/current")]
    for tools in [p / "bin" for p in env_paths("JAVA_HOME")] + defaults:
        if (tools / keytool).is_file():
            return tools
    return None

def find_android_sdk_platforms():
    for sdk in android_sdk_roots():
        if found := latest_subdir(sdk / "platforms", "android.jar"):
            return found
    return None

def run_command(args, env=None):
    """Run a command with stdout and stderr both sent to stderr. Return True if it succeeded."""
    sys.stdout.flush()
    try:
        return subprocess.run([str(a) for a in args], stdout=sys.stderr, stderr=sys.stderr, env=env).returncode == 0
    except OSError as e:
        print(f"Failed to run \"{args[0]}\": {e}", file=sys.stderr)
        return False

def dumpstr_to_file(path, content):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8")

def copy_file(src, dest_dir, new_name):
    dest_dir = Path(dest_dir)
    dest_dir.mkdir(parents=True, exist_ok=True)
    shutil.copy2(src, dest_dir / new_name)

def remove_file_if_exists(path):
    Path(path).unlink(missing_ok=True)

def split_flags(flags):
    """Split a flags string into separate arguments like a shell would: '-lfoo -DNAME="a b"' -> ['-lfoo', '-DNAME=a b']."""
    lexer = shlex.shlex(flags, posix=True)
    lexer.whitespace_split = True
    lexer.commenters = ""
    # Backslashes are path separators on Windows, not escapes
    if is_windows():
        lexer.escape = ""
    return list(lexer)

def quote_args(args):
    return " ".join(f'"{a}"' for a in args)

def build_arch(arch, ndk, minver, cpp, filename, cflags, lflags, output):
    """Compile filename and the NDK's native app glue into a shared library for one architecture. Return True if it succeeded."""
    compiler_name = "clang"
    if cpp:
        compiler_name = "clang++"
    script_extension = ""
    if is_windows():
        script_extension = ".cmd"

    # NDK uses "androidVER" for everything except for armv7a, which is "androideabiVER"
    android_abi_prefix = "android"
    if arch == "armv7a":
        android_abi_prefix = "androideabi"

    compiler_path = ndk_toolchain_path(ndk) / f"{arch}-linux-{android_abi_prefix}{minver}-{compiler_name}{script_extension}"
    app_glue_path = Path(ndk) / "sources" / "android" / "native_app_glue"

    mkdir_p(Path(output).parent)
    print(f"Building shared library for {arch} ({ANDROID_ABIS[arch]}) with \"{compiler_path}\"...")
    return run_command([compiler_path, "-shared", *cflags, "-I", app_glue_path, app_glue_path / "android_native_app_glue.c", filename, *lflags, "-fPIC", "-o", output])

def build(app_name, package_name, output_apk, filename, cpp, cflags, lflags, build_number, version, archs, minver, tgtver, intermediate, data, keystore, dname, alias, theme, icons, opt_env):

    finders = {
        "sdk_build_tools": find_android_sdk_build_tools,
        "sdk_ndk":         find_android_sdk_ndk,
        "studio_tools":    find_android_studio_tools,
        "sdk_platforms":   find_android_sdk_platforms,
    }
    missing = []
    for key, finder in finders.items():
        if not opt_env[key]:
            opt_env[key] = finder()
        if not opt_env[key]:
            missing.append(f"--{key.replace('_', '-')}")
    if missing:
        print(f"Could not find {', '.join(missing)} from environment variables or default install paths. Pass them explicitly.", file=sys.stderr)
        sys.exit(1)

    # Language
    target_language = "C"
    if cpp:
        target_language = "C++"

    executable_extension = ""
    if is_windows():
        executable_extension = ".exe"

    batch_extension = ""
    if is_windows():
        batch_extension = ".bat"

    if intermediate == None:
        intermediate = f"{package_name}_i"

    llvm_toolchain_path = ndk_toolchain_path(opt_env["sdk_ndk"])

    # Choose smallest minver if not set, highest for tgtver if not set
    if minver==-1 or tgtver==-1:
        min, max = ndk_api_range(llvm_toolchain_path)
        if minver==-1:
            minver=min
        if tgtver==-1:
            tgtver=max

    compiler_flags = split_flags(cflags)
    compiler_links = split_flags(lflags)

    manifest_path = Path(intermediate) / "AndroidManifest.xml"

    aapt        = Path(opt_env['sdk_build_tools']) / f"aapt{executable_extension}"
    zipalign    = Path(opt_env['sdk_build_tools']) / f"zipalign{executable_extension}"
    keytool     = Path(opt_env['studio_tools']) / f"keytool{executable_extension}"
    apksigner   = Path(opt_env['sdk_build_tools']) / f"apksigner{batch_extension}"
    android_jar = Path(opt_env['sdk_platforms']) / "android.jar"

    print(f"Building {target_language} \"{app_name}\" ({package_name}), v{version}b{build_number}")
    print(f"----------------------------------------------------------------------------------------")
    print(f"Min SDK Version:    {minver}")
    print(f"Target SDK Version: {tgtver}")
    print(f"SDK Build Tools:    \"{opt_env['sdk_build_tools']}\"")
    print(f"Architectures:      {', '.join(f'{a} ({ANDROID_ABIS[a]})' for a in archs)}")
    print(f"NDK Toolchain:      \"{llvm_toolchain_path}\"")
    print(f"C Flags:            {quote_args(compiler_flags)}")
    print(f"Link Flags:         {quote_args(compiler_links)}")
    print(f"Intermediate:       \"{intermediate}\"")
    print(f"aapt:               \"{aapt}\"")
    print(f"zipalign:           \"{zipalign}\"")
    print(f"keytool:            \"{keytool}\"")
    print(f"apksigner:          \"{apksigner}\"")
    print(f"android.jar:        \"{android_jar}\"")
    print(f"----------------------------------------------------------------------------------------")

    mkdir_p(intermediate)

    # Laid out like the APK root, since aapt adds everything under it as-is. Cleaned so ABIs from older builds don't linger.
    apk_root = make_clean_dir(Path(intermediate) / "apk")
    for arch in archs:
        shared_lib = apk_root / "lib" / ANDROID_ABIS[arch] / f"lib{app_name}.so"
        if not build_arch(arch, opt_env["sdk_ndk"], minver, cpp, filename, compiler_flags, compiler_links, shared_lib):
            print(f"Failed to build shared library for {arch}", file=sys.stderr)
            sys.exit(1)

    resources_directory = Path(intermediate) / "res"

    print(f"Building resources...")
    def make_icon_path_and_copy_if_exists(name, path):
        if path is not None:
            target_dir = resources_directory / name
            mkdir_p(target_dir)
            copy_file(path, target_dir, "ic_launcher.png")
            return True
        return False

    has_icon = False
    has_icon = has_icon or make_icon_path_and_copy_if_exists("mipmap-mdpi", icons[0])
    has_icon = has_icon or make_icon_path_and_copy_if_exists("mipmap-hdpi", icons[1])
    has_icon = has_icon or make_icon_path_and_copy_if_exists("mipmap-xhdpi", icons[2])
    has_icon = has_icon or make_icon_path_and_copy_if_exists("mipmap-xxhdpi", icons[3])
    has_icon = has_icon or make_icon_path_and_copy_if_exists("mipmap-xxxhdpi", icons[4])

    print(f"Writing manifest...")
    manifest = android_manifest(app_name, package_name, build_number, version, app_name, has_icon, theme, minver, tgtver)
    dumpstr_to_file(manifest_path, manifest)

    print(f"Packaging with aapt...")
    aapt_cmd = [aapt, "package", "--debug-mode", "-f", "-M", f'{manifest_path}', "-S", f'{resources_directory}', "-I", f'{android_jar}', "-F", f'{output_apk}.unaligned.apk']
    if data is not None:
        aapt_cmd += ["-A", data]
    # Raw files dir is positional, so it goes after every option
    aapt_cmd += [apk_root]
    run_command(aapt_cmd)

    keystore_is_debug = False
    if keystore is None:
        keystore_is_debug = True
        keystore = "debug.keystore"
        if Path(keystore).is_file():
            print(f"Using {keystore}...")
        else:
            print(f"Generating {keystore}...")
            run_command([keytool, "-genkeypair", "-keystore", keystore, "-storepass", "android", "-keypass", "android", "-alias", alias, "-dname", dname, "-keyalg", "RSA", "-keysize", "2048", "-validity", "20000"])
    else:
        if alias is None:
            print("Using custom keystore. need to provide -alias i.e. which key in the store to use.")
            exit()
        print(f"Using provided keystore: {keystore}...")

    print(f"Aligning package...")
    run_command([zipalign, "4", f'{output_apk}.unaligned.apk', f'{output_apk}.aligned.apk'])

    print(f"Signing package...")
    apksigner_cmd = [apksigner, "sign", "--ks", keystore, "--ks-key-alias", alias]
    if keystore_is_debug:
        apksigner_cmd += ["--ks-pass", "pass:android", "--key-pass", "pass:android"]
    # apksigner stops parsing options at the first positional, so the input apk must come last
    apksigner_cmd += ["--out", f'{output_apk}.apk', f'{output_apk}.aligned.apk']
    # apksigner is a wrapper script that needs a JVM; point it at the same JDK keytool comes from
    apksigner_env = {**os.environ, "JAVA_HOME": str(Path(opt_env['studio_tools']).parent)}
    run_command(apksigner_cmd, env=apksigner_env)

    remove_file_if_exists(f'{output_apk}.unaligned.apk')
    remove_file_if_exists(f'{output_apk}.aligned.apk')

    print(f"Done")
    pass


if __name__=="__main__":
    parser = argparse.ArgumentParser(prog="android_build.py",
                                     description="Builds android NativeActivities")

    parser.add_argument("filename")

    # Build Settings
    comp = parser.add_argument_group("Build Options")
    comp.add_argument("-n", "--app-name",       type=str,  required=False, default="Native Android App",                  help="Application Name")
    comp.add_argument("-p", "--package-name",   type=str,  required=False, default="com.contoso.test.native_android_app", help="Package Name")
    comp.add_argument("-o", "--output",         type=str,  required=False, default="./native_android_app",                help="Output file (APK without .apk extension)")
    comp.add_argument("-b", "--build-number",   type=int,  required=False, default=1,                                     help="Internal build number")
    comp.add_argument("-v", "--version",        type=str,  required=False, default="1.0",                                 help="Version")
    comp.add_argument("-a", "--architecture",   type=str,  required=False, default=None,                                  help="Architectures to build for, e.g. -a aarch64 x86_64. Defaults to aarch64", choices=list(ANDROID_ABIS), action="extend", nargs="+")
    comp.add_argument("--cpp",                  action="store_true",                                                      help="Use C++ compiler")
    comp.add_argument("--cflags",               type=str,  required=False, default="",                                    help="Additional compiler flags, split like a shell command line, e.g. --cflags \"-O2 -DNAME='a b'\". A single flag needs =, e.g. --cflags=-O2")
    comp.add_argument("--lflags",               type=str,  required=False, default="",                                    help="Additional linker flags, split like a shell command line, e.g. --lflags \"-landroid -llog\". A single flag needs =, e.g. --lflags=-landroid")
    comp.add_argument("--min-sdk-version",      type=int,  required=False, default=-1,                                    help="Min SDK Version (by default uses lowest available)")
    comp.add_argument("--target-sdk-version",   type=int,  required=False, default=-1,                                    help="Target SDK Version (by default uses max available)")
    comp.add_argument("-i", "--intermediate",   type=str,  required=False, default=None,                                  help="Intermediate directory for building. Defaults to Package Name_i")
    comp.add_argument("-d", "--data",           type=str,  required=False, default=None,                                  help="Optional data directory to bundle")
    comp.add_argument("-k", "--keystore",       type=str,  required=False, default=None,                                  help="Pass this to sign with key. It may prompt for password. By default it generates a debug keystore")
    comp.add_argument("--dname",                type=str,  required=False, default="CN=Developer, C=AA",                  help="-dname passed to keytool")
    comp.add_argument("--alias",                type=str,  required=False, default="androiddebugkey",                     help="-alias passed to keytool and apksigner")
    comp.add_argument("--theme",                type=str,  required=False, default="Theme.Holo.NoActionBar.Fullscreen",   help="The theme. Defaults to Theme.NoActionBar.Fullscreen (fullscreen with no action bar)")

    # Icons
    icon = parser.add_argument_group("Icons")
    icon.add_argument("--icon-mdpi",            type=str,  required=False, default=None,                                  help="mdpi icon png (copied to <intermediate>/res/mipmap-mdpi/ic_launcher.png)")
    icon.add_argument("--icon-hdpi",            type=str,  required=False, default=None,                                  help="hdpi icon png (copied to <intermediate>/res/mipmap-hdpi/ic_launcher.png)")
    icon.add_argument("--icon-xhdpi",           type=str,  required=False, default=None,                                  help="xhdpi icon png (copied to <intermediate>/res/mipmap-xhdpi/ic_launcher.png)")
    icon.add_argument("--icon-xxhdpi",          type=str,  required=False, default=None,                                  help="xxhdpi icon png (copied to <intermediate>/res/mipmap-xxhdpi/ic_launcher.png)")
    icon.add_argument("--icon-xxxhdpi",         type=str,  required=False, default=None,                                  help="xxxhdpi icon png (copied to <intermediate>/res/mipmap-xxxhdpi/ic_launcher.png)")

    # Environment Settings
    env = parser.add_argument_group("Environment")
    env.add_argument("--studio-tools",          type=str, required=False, default=None,                                   help="Studio Tools path (<Android Studio Installation>/jbr/bin). Needed for keytool, and as JAVA_HOME for apksigner. Uses $JAVA_HOME/bin, then the default Android Studio install if not set")
    env.add_argument("--sdk-build-tools",       type=str, required=False, default=None,                                   help="SDK Build Tools path (Android/Sdk/build-tools/<version>/). Needed for aapt, zipalign, apksigner. Uses the latest in $ANDROID_HOME, $ANDROID_SDK_ROOT, then the default SDK path if not set")
    env.add_argument("--sdk-ndk",               type=str, required=False, default=None,                                   help="NDK root path (Android/Sdk/ndk/<version>/). Needed for compiler. Uses $ANDROID_NDK_HOME, $ANDROID_NDK_ROOT, $ANDROID_NDK, then the latest in the SDK if not set")
    env.add_argument("--sdk-platforms",         type=str, required=False, default=None,                                   help="Platforms path (Android/Sdk/platforms/android-<version>). Needed for the android.jar file. Uses the latest in the SDK if not set")
    args = parser.parse_args()

    opt_env = dict()
    opt_env["sdk_build_tools"] = args.sdk_build_tools
    opt_env["sdk_ndk"] = args.sdk_ndk
    opt_env["studio_tools"] = args.studio_tools
    opt_env["sdk_platforms"] = args.sdk_platforms

    icons = [
        args.icon_mdpi,
        args.icon_hdpi,
        args.icon_xhdpi,
        args.icon_xxhdpi,
        args.icon_xxxhdpi,
    ]
    # Not an argparse default: "extend" would append to it instead of replacing it
    archs = list(dict.fromkeys(args.architecture or ["aarch64"]))

    build(args.app_name, args.package_name, args.output,
          args.filename, args.cpp, args.cflags, args.lflags,
          args.build_number, args.version,
          archs,
          args.min_sdk_version, args.target_sdk_version,
          args.intermediate,
          args.data,
          args.keystore,
          args.dname,
          args.alias,
          args.theme,
          icons,
          opt_env)