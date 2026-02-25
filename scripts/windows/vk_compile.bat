@echo off
setlocal EnableDelayedExpansion
set SCRIPT_DIR=%~dp0
set SLANG=%VULKAN_SDK%\Bin\slangc 
pushd %SCRIPT_DIR%\..\..\
if not exist _spirv mkdir _spirv

%SLANG% glsl/vk_shader_basic.slang ^
    -target spirv -profile spirv_1_4 -emit-spirv-directly ^
    -fvk-use-entrypoint-name -entry vert_main -entry frag_main -o _spirv\vk_shader_basic.spirv

popd
