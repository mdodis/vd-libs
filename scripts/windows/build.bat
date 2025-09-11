@echo off
setlocal EnableDelayedExpansion
set SCRIPT_DIR=%~dp0

REM --ARGUMENTS---------------------------------------------------------------------------------------------------------
set BUILD_MODE="debug"
set SAMPLE_TO_COMPILE=""
set COMPILE_PROGRAMS=0

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
) else if "%~1"=="-p" (
    set COMPILE_PROGRAMS=1
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

set INC_DIR=%PRJ_DIR%
set SRC_DIR=%PRJ_DIR%\samples

if not %SAMPLE_TO_COMPILE%=="" (
    cl /utf-8 /Zi /Od /I %INC_DIR% /W4 /GS- /nologo %SRC_DIR%\%SAMPLE_TO_COMPILE%.c /Fe:%SAMPLE_TO_COMPILE%.exe 
)

if %COMPILE_PROGRAMS%==1 (
    cl /Zi /Od /I %INC_DIR% /W4 /GS- /nologo %PRJ_DIR%/programs/embed.c /Fe:embed.exe 
    cl /Zi /Od /I %INC_DIR% /W4 /GS- /nologo %PRJ_DIR%/programs/printf_specifiers.c /Fe:print_specifiers.exe 
    cl /Zi /Od /I %INC_DIR% /W4 /GS- /nologo %PRJ_DIR%/programs/fontello.c /Fe:fontello.exe 
    cl /Zi /Od /I %INC_DIR% /W4 /GS- /nologo %PRJ_DIR%/programs/ryu.c /Fe:ryu.exe 
)

popd
popd
