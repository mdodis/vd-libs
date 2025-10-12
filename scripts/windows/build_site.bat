@echo off
setlocal EnableDelayedExpansion
set SCRIPT_DIR=%~dp0

pushd %SCRIPT_DIR%\..\..\
set PRJ_DIR=%cd%
set SRC_DIR=%PRJ_DIR%\site

if not exist build mkdir build
pushd build
cl /nologo /Od /Zi /I %PRJ_DIR% %SRC_DIR%\docubuild.c /Fe: docubuild.exe
set DOCUBUILD=%cd%\docubuild.exe
popd

%DOCUBUILD% .\site\ -o .\site-base\

popd
