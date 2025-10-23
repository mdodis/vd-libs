@echo off
setlocal EnableDelayedExpansion
set SCRIPT_DIR=%~dp0

REM --ARGUMENTS---------------------------------------------------------------------------------------------------------
set BUILD_MODE="debug"
set SAMPLE_TO_COMPILE=""
set COMPILE_PROGRAMS=0
set PROGRAM_TO_COMPILE=""
set CSEXT=".c"

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
) else if "%~1"=="-cpp" (
    set CSEXT=".cpp"
) else if "%~1"=="-p" (
    set COMPILE_PROGRAMS=1
    set PROGRAM_TO_COMPILE=%~2
    shift
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
set EXT_DIR=%PRJ_DIR%\ext
set SRC_DIR=%PRJ_DIR%\samples
set CL_FLAGS=/utf-8 /I %INC_DIR% /W4 /GS- /nologo /I %EXT_DIR%  /D_CRT_SECURE_NO_WARNINGS
set CL_DEBUG_FLAGS=/Zi /Od
set CL_RELEASE_FALGS=/O2

if %BUILD_MODE%=="release" (
    set CL_FLAGS=%CL_FLAGS% %CL_RELEASE_FALGS%
) else (
    set CL_FLAGS=%CL_FLAGS% %CL_DEBUG_FLAGS%
)

if not %SAMPLE_TO_COMPILE%=="" (
    cl %CL_FLAGS% %SRC_DIR%\%SAMPLE_TO_COMPILE%%CSEXT% /Fe:%SAMPLE_TO_COMPILE%.exe 
)

if %COMPILE_PROGRAMS%==1 (
    cl %CL_FLAGS% %PRJ_DIR%/programs/%PROGRAM_TO_COMPILE%.c /Fe:%PROGRAM_TO_COMPILE%.exe
)

popd
popd
