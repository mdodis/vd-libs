@echo off
setlocal EnableDelayedExpansion
set SCRIPT_DIR=%~dp0

pushd %SCRIPT_DIR%\..\..\
set PRJ_DIR=%cd%
if not exist build-test mkdir build-test
pushd build-test
set INC_DIR=%PRJ_DIR%
set SRC_DIR=%PRJ_DIR%

cl /Zi /Od  /W4 /GS- /std:c11 /nologo %SRC_DIR%\test.c /link

popd
popd