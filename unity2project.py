# Author: Michael Dodis <michaeldodisgr@gmail.com>
# Makes a visual studio project based on a source c file
# It's pretty janky and slow, but it's a relatively ok way to do projects based on
# input batch file
from __future__ import annotations

import argparse
import os
import re
import uuid
from pathlib import Path
from xml.sax.saxutils import escape


_INCLUDE_RE = re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]')
_VC_PROJECT_TYPE_GUID = "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}"
_CONFIGURATIONS = (
    ("Debug", "Win32"),
    ("Release", "Win32"),
    ("Debug", "x64"),
    ("Release", "x64"),
)
_SOURCE_SUFFIXES = {".c", ".cc", ".cpp", ".cxx"}
_HEADER_SUFFIXES = {".h", ".hh", ".hpp", ".hxx", ".inc", ".inl"}


def find_all_c_includes(c_file_path: str) -> list[str]:
    cwd = Path.cwd().resolve()
    start_path = (cwd / c_file_path).resolve() if not os.path.isabs(c_file_path) else Path(c_file_path).resolve()

    include_roots = [cwd]
    include_roots.extend(path for path in cwd.rglob("*") if path.is_dir())

    include_index: dict[str, Path] = {}
    for path in cwd.rglob("*"):
        if not path.is_file():
            continue

        relative_path = path.relative_to(cwd)
        relative_posix = relative_path.as_posix()
        include_index.setdefault(relative_posix, path)
        include_index.setdefault(path.name, path)

    visited: set[Path] = set()
    discovered: list[str] = []
    discovered_set: set[str] = set()

    def resolve_include(include_name: str, including_file: Path) -> Path | None:
        direct_path = (including_file.parent / include_name).resolve()
        if direct_path.is_file():
            return direct_path

        indexed_match = include_index.get(include_name)
        if indexed_match is not None:
            return indexed_match

        normalized_name = include_name.replace("\\", "/")
        indexed_match = include_index.get(normalized_name)
        if indexed_match is not None:
            return indexed_match

        for root in include_roots:
            candidate = root / include_name
            if candidate.is_file():
                return candidate.resolve()

        return None

    def walk(file_path: Path) -> None:
        resolved_file = file_path.resolve()
        if resolved_file in visited or not resolved_file.is_file():
            return
        visited.add(resolved_file)

        try:
            contents = resolved_file.read_text(encoding="utf-8", errors="ignore").splitlines()
        except OSError:
            return

        for line in contents:
            match = _INCLUDE_RE.match(line)
            if not match:
                continue

            include_name = match.group(1).strip()
            included_file = resolve_include(include_name, resolved_file)
            if included_file is None:
                continue

            try:
                relative_include = included_file.relative_to(cwd).as_posix()
            except ValueError:
                continue

            if relative_include not in discovered_set:
                discovered_set.add(relative_include)
                discovered.append(relative_include)

            walk(included_file)

    walk(start_path)
    return discovered


def _sanitize_name(value: str) -> str:
    sanitized = re.sub(r"[^A-Za-z0-9_.-]+", "_", value).strip("._-")
    return sanitized or "target"


def _to_windows_path(path: Path) -> str:
    return str(path).replace("/", "\\")


def _default_solution_name() -> str:
    return _sanitize_name(f"{Path.cwd().resolve().name}-unity")


def _project_name_for_target(c_file: str, used_names: set[str]) -> str:
    source_path = Path(c_file)
    candidate_parts = [source_path.stem]
    if source_path.parent != Path("."):
        candidate_parts.insert(0, _sanitize_name(str(source_path.parent).replace("\\", "_").replace("/", "_")))

    project_name = _sanitize_name("_".join(part for part in candidate_parts if part))
    unique_name = project_name
    suffix = 2
    while unique_name in used_names:
        unique_name = f"{project_name}_{suffix}"
        suffix += 1

    used_names.add(unique_name)
    return unique_name


def _make_guid(namespace: str, value: str) -> str:
    return "{" + str(uuid.uuid5(uuid.NAMESPACE_URL, f"{namespace}:{value}")).upper() + "}"


def _project_file_entry_tag(path: Path) -> str:
    suffix = path.suffix.lower()
    if suffix in _SOURCE_SUFFIXES:
        return "ClCompile"
    if suffix in _HEADER_SUFFIXES:
        return "ClInclude"
    return "None"


def _project_filter_name(path: Path) -> str:
    suffix = path.suffix.lower()
    if suffix in _SOURCE_SUFFIXES:
        return "Source Files"
    if suffix in _HEADER_SUFFIXES:
        return "Header Files"
    return "Other Files"


def _relative_from(path: Path, start: Path) -> str:
    return _to_windows_path(Path(os.path.relpath(path, start)))


def _write_text(path: Path, contents: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(contents, encoding="utf-8", newline="\r\n")


def _normalize_target(target: list[str] | tuple[str, ...]) -> tuple[str, str, str | None]:
    if len(target) == 2:
        c_file, command = target
        return c_file, command, None
    if len(target) == 3:
        c_file, command, executable_path = target
        return c_file, command, executable_path
    raise ValueError("Each --target must provide CFILE COMMAND [EXECUTABLE]")


def _collect_target_files(c_file: str) -> list[Path]:
    cwd = Path.cwd().resolve()
    source_path = (cwd / c_file).resolve() if not os.path.isabs(c_file) else Path(c_file).resolve()
    if not source_path.is_file():
        raise FileNotFoundError(f"Target source file does not exist: {c_file}")

    ordered_files: list[Path] = [source_path]
    seen = {source_path}

    for include_path in find_all_c_includes(str(source_path)):
        resolved = (cwd / include_path).resolve()
        if resolved in seen or not resolved.is_file():
            continue
        seen.add(resolved)
        ordered_files.append(resolved)

    return ordered_files


def _vcxproj_contents(
    project_guid: str,
    project_name: str,
    build_command: str,
    file_entries: list[tuple[str, str]],
    include_path: str,
    executable_path: str | None,
) -> str:
    escaped_command = escape(build_command)
    escaped_include_path = escape(include_path)
    escaped_executable_path = escape(executable_path) if executable_path else None

    config_items = []
    config_groups = []
    property_sheets = []
    nmake_groups = []
    for configuration, platform in _CONFIGURATIONS:
        config_items.append(
            f"""    <ProjectConfiguration Include="{configuration}|{platform}">
      <Configuration>{configuration}</Configuration>
      <Platform>{platform}</Platform>
    </ProjectConfiguration>"""
        )
        use_debug = "true" if configuration == "Debug" else "false"
        config_groups.append(
            f"""  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='{configuration}|{platform}'" Label="Configuration">
    <ConfigurationType>Makefile</ConfigurationType>
    <UseDebugLibraries>{use_debug}</UseDebugLibraries>
    <PlatformToolset>v143</PlatformToolset>
  </PropertyGroup>"""
        )
        property_sheets.append(
            f"""  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='{configuration}|{platform}'">
    <Import Project="$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>"""
        )
        executable_xml = ""
        if escaped_executable_path:
            executable_xml = (
                f"\n    <NMakeOutput>{escaped_executable_path}</NMakeOutput>"
                f"\n    <LocalDebuggerCommand>{escaped_executable_path}</LocalDebuggerCommand>"
            )
        nmake_groups.append(
            f"""  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='{configuration}|{platform}'">
    <NMakeBuildCommandLine>{escaped_command}</NMakeBuildCommandLine>
    <NMakeReBuildCommandLine>{escaped_command}</NMakeReBuildCommandLine>
    <NMakeCleanCommandLine>cmd /c echo No clean command configured for {escape(project_name)}</NMakeCleanCommandLine>
    {executable_xml}
    <IncludePath>{escaped_include_path};$(IncludePath)</IncludePath>
  </PropertyGroup>"""
        )

    items_by_tag: dict[str, list[str]] = {}
    for tag, relative_path in file_entries:
        items_by_tag.setdefault(tag, []).append(
            f'    <{tag} Include="{escape(relative_path)}" />'
        )

    item_groups = []
    for tag in ("ClInclude", "ClCompile", "None"):
        entries = items_by_tag.get(tag)
        if not entries:
            continue
        item_groups.append("  <ItemGroup>\n" + "\n".join(entries) + "\n  </ItemGroup>")

    return f"""<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
{chr(10).join(config_items)}
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <VCProjectVersion>17.0</VCProjectVersion>
    <ProjectGuid>{project_guid}</ProjectGuid>
    <Keyword>Win32Proj</Keyword>
    <ProjectName>{escape(project_name)}</ProjectName>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.Default.props" />
{chr(10).join(config_groups)}
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings">
  </ImportGroup>
  <ImportGroup Label="Shared">
  </ImportGroup>
{chr(10).join(property_sheets)}
  <PropertyGroup Label="UserMacros" />
{chr(10).join(nmake_groups)}
  <ItemDefinitionGroup>
  </ItemDefinitionGroup>
{chr(10).join(item_groups)}
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.targets" />
  <ImportGroup Label="ExtensionTargets">
  </ImportGroup>
</Project>
"""


def _filters_contents(file_entries: list[tuple[str, str]]) -> str:
    filters = {
        "Source Files": _make_guid("filter", "Source Files"),
        "Header Files": _make_guid("filter", "Header Files"),
        "Other Files": _make_guid("filter", "Other Files"),
    }

    filter_items = [
        f"""    <Filter Include="{name}">
      <UniqueIdentifier>{guid}</UniqueIdentifier>
    </Filter>"""
        for name, guid in filters.items()
    ]

    entries_by_tag: dict[str, list[str]] = {}
    for tag, relative_path in file_entries:
        filter_name = _project_filter_name(Path(relative_path))
        entries_by_tag.setdefault(tag, []).append(
            f"""    <{tag} Include="{escape(relative_path)}">
      <Filter>{filter_name}</Filter>
    </{tag}>"""
        )

    item_groups = ['  <ItemGroup>\n' + "\n".join(filter_items) + "\n  </ItemGroup>"]
    for tag in ("ClInclude", "ClCompile", "None"):
        entries = entries_by_tag.get(tag)
        if not entries:
            continue
        item_groups.append("  <ItemGroup>\n" + "\n".join(entries) + "\n  </ItemGroup>")

    return f"""<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
{chr(10).join(item_groups)}
</Project>
"""


def _solution_contents(solution_name: str, solution_guid: str, projects: list[dict[str, str]]) -> str:
    project_entries = []
    project_configs = []

    for project in projects:
        project_entries.append(
            f'Project("{_VC_PROJECT_TYPE_GUID}") = "{project["name"]}", "{project["project_path"]}", "{project["guid"]}"\nEndProject'
        )
        for solution_configuration, solution_platform in (
            ("Debug", "x64"),
            ("Debug", "x86"),
            ("Release", "x64"),
            ("Release", "x86"),
        ):
            project_platform = "Win32" if solution_platform == "x86" else "x64"
            project_configs.append(
                f'\t\t{project["guid"]}.{solution_configuration}|{solution_platform}.ActiveCfg = {solution_configuration}|{project_platform}'
            )
            project_configs.append(
                f'\t\t{project["guid"]}.{solution_configuration}|{solution_platform}.Build.0 = {solution_configuration}|{project_platform}'
            )

    return f"""Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio Version 17
VisualStudioVersion = 17.14.36408.4
MinimumVisualStudioVersion = 10.0.40219.1
{chr(10).join(project_entries)}
Global
\tGlobalSection(SolutionConfigurationPlatforms) = preSolution
\t\tDebug|x64 = Debug|x64
\t\tDebug|x86 = Debug|x86
\t\tRelease|x64 = Release|x64
\t\tRelease|x86 = Release|x86
\tEndGlobalSection
\tGlobalSection(ProjectConfigurationPlatforms) = postSolution
{chr(10).join(project_configs)}
\tEndGlobalSection
\tGlobalSection(SolutionProperties) = preSolution
\t\tHideSolutionNode = FALSE
\tEndGlobalSection
\tGlobalSection(ExtensibilityGlobals) = postSolution
\t\tSolutionGuid = {solution_guid}
\tEndGlobalSection
EndGlobal
"""


def generator_vs(
    targets: list[list[str]] | list[tuple[str, ...]],
    solution_name: str | None = None,
    output_dir: str | None = None,
) -> Path:
    if not targets:
        raise ValueError("No targets were supplied")

    cwd = Path.cwd().resolve()
    resolved_solution_name = _sanitize_name(solution_name) if solution_name else _default_solution_name()
    if output_dir:
        output_root = (cwd / output_dir).resolve() if not os.path.isabs(output_dir) else Path(output_dir).resolve()
    else:
        output_root = cwd / "devtools" / "visual-studio" / resolved_solution_name

    solution_path = output_root / f"{resolved_solution_name}.sln"
    solution_guid = _make_guid("solution", str(solution_path))

    used_names: set[str] = set()
    projects: list[dict[str, str]] = []

    for target in targets:
        c_file, command, executable_path = _normalize_target(target)
        project_name = _project_name_for_target(c_file, used_names)
        project_dir = output_root / project_name
        project_path = project_dir / f"{project_name}.vcxproj"
        filters_path = project_dir / f"{project_name}.vcxproj.filters"
        project_guid = _make_guid("project", str(project_path))

        target_files = _collect_target_files(c_file)
        file_entries = [
            (_project_file_entry_tag(path), _relative_from(path, project_dir))
            for path in target_files
        ]

        build_command = f'cd /d "{_to_windows_path(cwd)}" && {command}'
        include_path = _to_windows_path(cwd)
        resolved_executable_path = None
        if executable_path:
            resolved_executable = (cwd / executable_path).resolve() if not os.path.isabs(executable_path) else Path(executable_path).resolve()
            resolved_executable_path = _to_windows_path(resolved_executable)

        _write_text(
            project_path,
            _vcxproj_contents(
                project_guid=project_guid,
                project_name=project_name,
                build_command=build_command,
                file_entries=file_entries,
                include_path=include_path,
                executable_path=resolved_executable_path,
            ),
        )
        _write_text(filters_path, _filters_contents(file_entries))

        projects.append(
            {"name": project_name, "guid": project_guid, "project_path": _relative_from(project_path, output_root)}
        )

    _write_text(solution_path, _solution_contents(resolved_solution_name, solution_guid, projects))
    return solution_path


def main() -> int:
    parser = argparse.ArgumentParser(
        prog="unity2project",
        description="Pass in c/c++ source files you compile and commands. Creates editor project",
        epilog="Author: Michael Dodis <michaeldodisgr@gmail.com>",
    )
    parser.add_argument(
        "--generator",
        choices=["vs"],
        default="vs",
        help="Choose the project generator (default: vs (Visual Studio Solution))",
    )
    parser.add_argument(
        "--target",
        nargs="+",
        metavar="TARGET_PART",
        action="append",
        help="Target as CFILE COMMAND [EXECUTABLE] (can be used multiple times)",
    )
    parser.add_argument(
        "--name",
        help="Name for the generated solution file and default output folder",
    )
    parser.add_argument(
        "--output-dir",
        help="Directory where the generated project files should be written",
    )

    args = parser.parse_args()
    if not args.target:
        parser.error("At least one --target CFILE COMMAND [EXECUTABLE] entry is required")
    for target in args.target:
        if len(target) not in (2, 3):
            parser.error("Each --target must be CFILE COMMAND [EXECUTABLE]")

    generators = {
        "vs": generator_vs,
    }

    solution_path = generators[args.generator](
        args.target,
        solution_name=args.name,
        output_dir=args.output_dir,
    )
    print(f"Generated {solution_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
