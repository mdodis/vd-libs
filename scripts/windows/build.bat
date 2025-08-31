@echo off
setlocal EnableDelayedExpansion
set SCRIPT_DIR=%~dp0

pushd %SCRIPT_DIR%\..\..\
set PRJ_DIR=%cd%
if not exist build mkdir build
pushd build

REM samples/ui_basic.c------------------
set INC_DIR=%PRJ_DIR%
set SRC_DIR=%PRJ_DIR%\samples

cl /Zi /Od /I %INC_DIR% /W4  /nologo %SRC_DIR%\ui_basic.c /Fe:ui_basic.exe /link User32.lib OpenGL32.lib
REM samples/ui_basic.c------------------

popd
popd
