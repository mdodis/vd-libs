@echo off
setlocal EnableDelayedExpansion
set SCRIPT_DIR=%~dp0

pushd %SCRIPT_DIR%\..\..\
set PRJ_DIR=%cd%
if not exist build-test mkdir build-test
pushd build-test
set INC_DIR=%PRJ_DIR%
set SRC_DIR=%PRJ_DIR%
set CFLAGS=/Zi /Od /W4 /std:c++20 /GS- /nologo /D_CRT_SECURE_NO_WARNINGS /wd4505 /I %PRJ_DIR% /I %PRJ_DIR%\ext

cl %CFLAGS% %SRC_DIR%\tests\vd_fw_test.cpp
cl %CFLAGS% %SRC_DIR%\tests\vd_docuspec_test.cpp
cl %CFLAGS% %SRC_DIR%\tests\vd_io_net_test.cpp

popd
popd