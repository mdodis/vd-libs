![vd-libs logo](./assets/vd-logo-v.svg)

A collection of header-only libraries and utility programs
## Libraries

| Library                                                   | Description                                                                                                                          | Logo                                                           |
| --------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------- |
| [vd.h](./vd.h)                                            | C base for programing, includes a bunch of data structures, a test system, allocators etc...                                         | <img src="./assets/vd-base-logo.svg/" width="64">              |
| [vd_cg.h](./vd_cg.h)                                      | C math library                                                                                                                       | <img src="./assets/vd-cg-logo.svg/" width="64">                |
| [vd_cgla.h](./vd_cgla.h)                                  | Aliases for enums from common graphics APIs                                                                                          | <img src="./assets/vd-cgla-logo.svg/" width="64">              |
| [vd_debug.h](./vd_debug.h)                                | C profiling library                                                                                                                  | <img src="./assets/vd-debug-logo.svg/" width="64">             |
| [vd_fw.h](./vd_fw.h)                                      | Simple 1 call = window + OpenGL context library for Window & MacOS (linux WIP)                                                       | <img src="./assets/vd-fw-logo.svg/" width="64">                |
| [vd_inix.h](./vd_inix.h)                                  | C parser for ini-extended format (ini with multiline strings '@> @<')                                                                | <img src="./assets/vd-inix-logo.svg/" width="64">              |
| [vd_input.h](./vd_input.h)                                | Aliases for enum from input APIs into a common set                                                                                   | <img src="./assets/vd-input-logo.svg/" width="64">             |
| [vd_ui.h](./vd_ui.h)                                      | **WIP** immediate mode UI library, inspired by [Ryan Fleury's posts on Substack](https://www.rfleury.com/p/posts-table-of-contents)  | <img src="./assets/vd-ui-logo.svg/" width="64">                |
| [vd_vk.h](./vd_vk.h)                                      | C Vulkan utility library                                                                                                             | <img src="./assets/vd-vk-logo.svg/" width="64">                |
| **UTILITIES**                                             |                                                                                                                                      |                                                                |
| [vd_imgui.h](./vd_imgui.h)                                | Handwritten translation layer for [Dear ImGui](https://github.com/ocornut/imgui), which can be adapted into fptr struct              |                                                                |

## Programs

| Program                                                   | Description                                                                                                                          |
| --------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------ |
| [embed](./programs/embed.c)                               | Takes in input file path, spits out c unsigned char array for embedding into applications                                            |
| [fontello](./programs/fontello.c)                         | Takes in a [Fontello](https://fontello.com/) config.json file, outputs C compatible enums                                            |
| [printf_specifiers](./programs/printf_specifiers.c)       | **WIP** listing of all printf specifiers to help with writing your own printf implementation                                         |
| [ryu](./programs/ryu.c)                                   | **WIP** implementation of ryu floating point printing algorithm                                                                      |

## Samples

| Name                                                      | Description                                                                                                                          |
| --------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------ |
| [gl_window](./samples/gl_window.c)                        | Window Sample with vd_fw.h in OpenGL                                                                                                 |
| [gl_cube](./samples/gl_cube.c)                            | Cube Sample with vd_fw.h in OpenGL                                                                                                   |
| [ui_basic](./samples/ui_basic.c)                          | **WIP** listing of all printf specifiers to help with writing your own printf implementation                                         |

## Compilation

### Windows

#### Requirements

- MSVC

#### Compile a sample
```bash
.\\scripts\\windows\\build.bat -m debug -s <sample>
```

#### Compile all programs
```bash
.\\scripts\\windows\\build.bat -m debug -p
```
