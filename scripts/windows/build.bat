@echo off
setlocal EnableDelayedExpansion
set SCRIPT_DIR=%~dp0

pushd %SCRIPT_DIR%\..\..\
if not exist build mkdir build
pushd build

REM samples/ui_basic.c------------------
set SRC_DIR=..\samples

cl /Zi /Od /nologo %SRC_DIR%\ui_basic.c /Fe:ui_basic.exe /link OpenGL32.lib
REM samples/ui_basic.c------------------

popd
popd
