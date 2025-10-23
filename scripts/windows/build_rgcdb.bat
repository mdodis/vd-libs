@echo off
setlocal EnableDelayedExpansion
set SCRIPT_DIR=%~dp0

pushd %SCRIPT_DIR%\..\..\
set PRJ_DIR=%cd%
if not exist build mkdir build
pushd build
cl /nologo /Zi /Od /I %PRJ_DIR% %PRJ_DIR%\programs\gamecontrollerdb_gen.c /Fe:gamecontrollerdb_gen.exe
popd

set RGCDBGEN=%PRJ_DIR%\build\gamecontrollerdb_gen.exe

%RGCDBGEN% assets\builtin.rgcdb Windows > builtin.rgcdb.c
popd
