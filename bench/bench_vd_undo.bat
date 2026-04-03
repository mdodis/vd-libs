@echo off
setlocal EnableDelayedExpansion
set SCRIPT_DIR=%~dp0


pushd %SCRIPT_DIR%
set SOURCE_DIR=%cd%
pushd ..
set INC_DIR=%cd%
if not exist build mkdir build

set MODE_FLAGS=/Zi /Od
rem set MODE_FLAGS=/Zi /O2

pushd build
cl ^
    /nologo /Za /EHsc ^
    /I %INC_DIR% ^
    %MODE_FLAGS% ^
    %SOURCE_DIR%\bench_vd_undo.c ^
    /Fe:bench_vd_undo.exe
popd

popd
popd

