@echo off
setlocal EnableDelayedExpansion
set SCRIPT_DIR=%~dp0

REM --ARGUMENTS---------------------------------------------------------------------------------------------------------
set BUILD_MODE="debug"
set SAMPLE_TO_COMPILE=""
set COMPILE_PROGRAMS=0
set PROGRAM_TO_COMPILE=""
set CSEXT=".c"
set USECLANG=0

:parse_args
if "%~1"=="" goto after_parse

if "%~1"=="-m" (
    if "%~2"=="debug" (
        set BUILD_MODE=debug
    ) else if "%~2"=="release" (
        set BUILD_MODE=release
    ) else (
        echo Invalid build mode
        exit /b 1
    )
    shift
    shift

) else if "%~1"=="-l" (
    set USECLANG=1
    shift

) else if "%~1"=="-cpp" (
    set CSEXT=.cpp
    shift

) else if "%~1"=="-p" (
    set COMPILE_PROGRAMS=1
    set PROGRAM_TO_COMPILE=%~2
    shift
    shift

) else if "%~1"=="-s" (
    set SAMPLE_TO_COMPILE=%~2
    shift
    shift
) else (
    echo Unknown option: %~1
    exit /b 1
)

goto parse_args

:after_parse

pushd %SCRIPT_DIR%\..\..\
set PRJ_DIR=%cd%
if not exist build mkdir build
pushd build

set INC_DIR=%PRJ_DIR%
set EXT_DIR=%PRJ_DIR%\ext
set SRC_DIR=%PRJ_DIR%\samples
set CL_FLAGS=/utf-8 /std:c11 /I %INC_DIR% /W4 /GS- /nologo /I %EXT_DIR%  /D_CRT_SECURE_NO_WARNINGS /DUNICODE /wd4201
set CL_DEBUG_FLAGS=/Zi /Od
set CL_RELEASE_FLAGS=/O2
set CL_LINK_FLAGS=

set LV_FLAGS=-std=c99 -I %INC_DIR% -Wall -Werror -I %EXT_DIR% -Wno-unused-function
set LV_DEBUG_FLAGS=-O0 -g
set LV_RELEASE_FLAGS=-O2
set LV_LINK_FLAGS=

set VK_INCLUDE_PATH=%VULKAN_SDK%\Include
set EXTRA_LINK_ARGS=""

if not %VULKAN_SDK%=="" (
    set CL_FLAGS=%CL_FLAGS% /I %VK_INCLUDE_PATH%
    set LV_FLAGS=%LV_FLAGS% -I %VK_INCLUDE_PATH%
    set CL_LINK_FLAGS="/link %VULKAN_SDK%\Lib\vulkan-1.lib"
    set LV_LINK_FLAGS="%VULKAN_SDK%\Lib\vulkan-1.lib"
)

set FF_FLAGS=%CL_FLAGS%
set FF_DEBUG_FLAGS=%CL_DEBUG_FLAGS%
set FF_RELEASE_FLAGS=%CL_RELEASE_FLAGS%
set FF_LINK_FLAGS=%CL_LINK_FLAGS%
set COMPILER=cl

set FF_NAME_PROGRAM=/Fe:

if %USECLANG%==1 (
    set COMPILER=clang
    set FF_FLAGS=%LV_FLAGS%
    set FF_DEBUG_FLAGS=%LV_DEBUG_FLAGS%
    set FF_RELEASE_FLAGS=%LV_RELEASE_FLAGS%
    set FF_NAME_PROGRAM=-o
    set FF_LINK_FLAGS=%LV_LINK_FLAGS%
)

if %BUILD_MODE%=="release" (
    set FF_FLAGS=%FF_FLAGS% %FF_RELEASE_FALGS%
) else (
    set FF_FLAGS=%FF_FLAGS% %FF_DEBUG_FLAGS%
)

if not %SAMPLE_TO_COMPILE%=="" (
    %COMPILER% %FF_FLAGS% ^
        %SRC_DIR%\%SAMPLE_TO_COMPILE%%CSEXT% ^
        %FF_NAME_PROGRAM% %SAMPLE_TO_COMPILE%.exe ^
        %FF_LINK_FLAGS%
)

if %COMPILE_PROGRAMS%==1 (
    %COMPILER% %FF_FLAGS% %PRJ_DIR%/programs/%PROGRAM_TO_COMPILE%.c %FF_NAME_PROGRAM% %PROGRAM_TO_COMPILE%.exe
)

popd
popd
