@echo off
setlocal EnableDelayedExpansion
set SCRIPT_DIR=%~dp0

pushd %SCRIPT_DIR%\..\..\
set PRJ_DIR=%cd%
if not exist build-test mkdir build-test
pushd build-test
set INC_DIR=%PRJ_DIR%
set SRC_DIR=%PRJ_DIR%
set CFLAGS=/Zi /Od /W4 /GS- /nologo /I %PRJ_DIR% /I %PRJ_DIR%\ext

cl %CFLAGS% %SRC_DIR%\tests\vd_fw_test.c

popd
popd