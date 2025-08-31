@echo off
setlocal EnableDelayedExpansion
set SCRIPT_DIR=%~dp0

REM --ARGUMENTS---------------------------------------------------------------------------------------------------------
set BUILD_MODE="debug"
set COMPILE_ALL=0
set SAMPLE_TO_COMPILE="ui_basic"

:parse_args
if "%~1"=="" goto after_parse

if "%~1"=="-m" (
    if "%~2"=="debug" (
        set BUILD_MODE="debug"
        shift
    ) else if "%~2"=="release" (
        set BUILD_MODE="release"
        shift
    ) else (
        echo Invalid build mode
        exit /b 1
    )
) else if "%~1"=="-a" (
    set COMPILE_ALL=1
) else if "%~1"=="-s" (
    set SAMPLE_TO_COMPILE=%~2
    shift
) else (
    echo Unknown option: %~1
)

shift
goto parse_args
:after_parse

pushd %SCRIPT_DIR%\..\..\
set PRJ_DIR=%cd%
if not exist build mkdir build
pushd build

REM --SAMPLES - UI BASIC------------------------------------------------------------------------------------------------
set INC_DIR=%PRJ_DIR%
set SRC_DIR=%PRJ_DIR%\samples

if %SAMPLE_TO_COMPILE%==ui_basic (
    cl /Zi /Od /I %INC_DIR% /W4 /GS- /nologo %SRC_DIR%\ui_basic.c /Fe:ui_basic.exe 
)

REM --SAMPLES - GL WINDOW-----------------------------------------------------------------------------------------------
if %SAMPLE_TO_COMPILE%==gl_window (
    cl /Zi /Od /I %INC_DIR% /W4 /GS- /nologo %SRC_DIR%\gl_window.c /Fe:gl_window.exe 
)

REM --SAMPLES - GL CUBE-------------------------------------------------------------------------------------------------
if %SAMPLE_TO_COMPILE%==gl_cube (
    cl /Zi /Od /I %INC_DIR% /W4 /GS- /nologo %SRC_DIR%\gl_cube.c /Fe:gl_cube.exe 
)


popd
popd
