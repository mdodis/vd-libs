![vd-libs logo](./site-base/assets/vd-logo-p@1x.png)

A collection of header-only libraries and utility programs

[Documentation](https://mdodis.github.io/vd-libs/)

## Libraries

| Library                                                   | Description                                                                                                                          | Logo                                                           |
| --------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------- |
| **IN PROGRESS**                                           |                                                                                                                                      |                                                                |
| [vd_fw.h](./vd_fw.h)                                      | C Windowing library                                                                                                                  | <img src="./site-base/assets/vd-fw-logo.svg/" width="64">      |
| [vd.h](./vd.h)                                            | C base for programing, includes a bunch of data structures, a test system, allocators etc...                                         | <img src="./site-base/assets/vd-base-logo.svg/" width="64">    |
| [vd_cg.h](./vd_cg.h)                                      | C math library                                                                                                                       | <img src="./site-base/assets/vd-cg-logo.svg/" width="64">      |
| [vd_docuspec.h](./vd_docuspec.h)                          | Kinda like Markdown, but simpler                                                                                                     | <img src="./site-base/assets/vd-cgla-logo.svg/" width="64">    |
| **VERY IN PROGRESS**                                      |                                                                                                                                      |                                                                |
| [vd_cgla.h](./vd_cgla.h)                                  | Aliases for enums from common graphics APIs                                                                                          | <img src="./site-base/assets/vd-cgla-logo.svg/" width="64">    |
| [vd_debug.h](./vd_debug.h)                                | C profiling library                                                                                                                  | <img src="./site-base/assets/vd-debug-logo.svg/" width="64">   |
| [vd_inix.h](./vd_inix.h)                                  | C parser for ini-extended format (ini with multiline strings '@> @<')                                                                | <img src="./site-base/assets/vd-inix-logo.svg/" width="64">    |
| [vd_input.h](./vd_input.h)                                | Aliases for enum from input APIs into a common set                                                                                   | <img src="./site-base/assets/vd-input-logo.svg/" width="64">   |
| [vd_ui.h](./vd_ui.h)                                      | **WIP** immediate mode UI library, inspired by [Ryan Fleury's posts on Substack](https://www.rfleury.com/p/posts-table-of-contents)  | <img src="./site-base/assets/vd-ui-logo.svg/" width="64">      |
| [vd_um.h](./vd_um.h)                                      | **WIP** immediate mode 3D shape drawing and gizmo library                                                                            | <img src="./site-base/assets/vd-um-logo.svg/" width="64">      |
| [vd_vk.h](./vd_vk.h)                                      | C Vulkan utility library                                                                                                             | <img src="./site-base/assets/vd-vk-logo.svg/" width="64">      |
| **UTILITIES**                                             |                                                                                                                                      |                                                                |
| [vd_imgui.h](./vd_imgui.h)                                | Handwritten translation layer for [Dear ImGui](https://github.com/ocornut/imgui), which can be adapted into fptr struct              |                                                                |

## Programs

| Program                                                     | Description                                                                                                                        |
| ----------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------- |
| [docuspec_html.c](./programs/docuspec_html.c)               | **WIP** Example of generating html with vd_docuspec                                                                                |
| [embed.c](./programs/embed.c)                               | Takes in input file path, spits out c unsigned char array for embedding into applications                                          |
| [fontello.c](./programs/fontello.c)                         | Takes in a [Fontello](https://fontello.com/) config.json file, outputs C compatible enums                                          |
| [gamecontrollerdb_gen.c](./programs/gamecontrollerdb_gen.c) | vd_fw Builds RGCDB based on input file                                                                                             |
| [printf_specifiers.c](./programs/printf_specifiers.c)       | **WIP** listing of all printf specifiers to help with writing your own printf implementation                                       |
| [ryu.c](./programs/ryu.c)                                   | **WIP** implementation of ryu floating point printing algorithm                                                                    |
| [sembd.c](./programs/sembd.c)                               | Takes in an input file path, spits out C macro string aligned nicely                                                               |
| [test.c](./programs/test.c)                                 | Runs all of the tests                                                                                                              |
| [windisplays.c](./programs/windisplays.c)                   | Lists all of the Windows display adapters with friendly names                                                                      |
| [winhidpi_dump.c](./programs/winhidpi_dump.c)               | Dumps all HID devices on Windows                                                                                                   |

## Samples
| Name                                                                         | Description                                                                                                       |
| ---------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------- |
| [gl_window.c](./samples/gl_window.c)                                         | vd_fw Basic window with OpenGL                                                                                    |
| [gl_drag_area.c](./samples/gl_drag_area.c)                                   | vd_fw Sample for borderless window drag area                                                                      |
| [gl_rectangles.c](./samples/gl_rectangles.c)                                 | vd_fw OpenGL 2D rectangles                                                                                        |
| [gl_camera.c](./samples/gl_camera.c)                                         | vd_fw Camera                                                                                                      |
| [gl_cube.c](./samples/gl_cube.c)                                             | vd_fw Cube                                                                                                        |
| [gl_cubeapp.c](./samples/gl_cubeapp.c)                                       | vd_fw Sample from page tutorial                                                                                   |
| [gl_gamepad.c](./samples/gl_gamepad.c)                                       | vd_fw Gamepad Testing Utility                                                                                     |
| [gl_hotload.c](./samples/gl_hotload.c)                                       | vd_fw OpenGL shader hot reloading                                                                                 |
| [gl_10.c](./samples/gl_10.c)                                                 | vd_fw OpenGL 1.0                                                                                                  |
| [gl_ed.c](./samples/gl_ed.c)                                                 | vd_fw Sample of an editor-like program                                                                            |
| [dx_window.cpp](./samples/dx_window.cpp)                                     | vd_fw DirectX11                                                                                                   |
| [fw_all.cpp](./samples/fw_all.cpp)                                           | vd_fw OpenGL/DirectX11                                                                                            |
| [win32_direct_manipulation.cpp](./samples/win32_direct_manipulation.cpp)     | Direct Manipulation API usage                                                                                     |
| [gl_dialogs.c](./samples/gl_dialogs.c)                                       | **WIP** vd_dlg example                                                                                            |
| [ui_basic.c](./samples/ui_basic.c)                                           | **WIP** vd_ui example                                                                                             |
| [um_basic.c](./samples/um_basic.c)                                           | **WIP** vd_um example                                                                                             |
| [au_sin.c](./samples/au_sin.c)                                               | **WIP** Audio Library sample                                                                                      |

## Compilation

### Windows

#### Requirements

- MSVC

#### Compile a C sample
```bash
.\\scripts\\windows\\build.bat -m debug -s <sample>
```

#### Compile a C program
```bash
.\\scripts\\windows\\build.bat -m debug -p <program name>
```

#### Compile a C++ sample
```bash
.\\scripts\\windows\\build.bat -m debug -cpp -s <sample>
```

#### Compile a C++ program
```bash
.\\scripts\\windows\\build.bat -m debug -cpp -p <program name>
```
### MacOS

#### Requirements

- XCode Command Line Tools & SDK

#### Compile a sample
```bash
.\\scripts\\macos\\build.bat -s <sample>
```

#### Compile a program
```bash
.\\scripts\\macos\\build.bat -p <program name>
```