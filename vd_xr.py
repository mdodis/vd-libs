import dataclasses
import urllib.request
import subprocess
import os

def git_clone(repo_url, target_dir=None):
	cmd = ["git", "clone", repo_url]
	if target_dir:
		cmd.append(target_dir)
	result = subprocess.run(cmd, capture_output=True, text=True)
	print("stdout:\n", result.stdout)
	print("stderr:\n", result.stderr)
	if result.returncode != 0:
		raise RuntimeError(f"git clone failed: {result.stderr}")


try:
    git_clone("https://github.com/KhronosGroup/OpenXR-SDK-Source.git", target_dir="./temp_openxr_source")
except:
	pass

os.chdir(os.path.join("./temp_openxr_source/", "specification/scripts"))

# Import cgenerator from the cloned repo's specification directory
import sys
sys.path.insert(0, os.getcwd())
sys.path.insert(0, '../../src/scripts')
import xml.etree.ElementTree as etree
from cgenerator import CGeneratorOptions, COutputGenerator
from apiconventions import APIConventions
from reg import Registry
from reflib import logDiag, logWarn, logErr, setLogFile
from automatic_source_generator import AutomaticSourceGeneratorOptions
from automatic_source_generator import (AutomaticSourceOutputGenerator,
                                        undecorate)

from automatic_source_generator import AutomaticSourceOutputGenerator, CurrentExtensionTracker
from generator import write
# UtilitySourceOutputGenerator - subclass of AutomaticSourceOutputGenerator.


class UtilitySourceOutputGenerator(AutomaticSourceOutputGenerator):
    """Generate loader source using XML element attributes from registry"""

    # Override the base class header warning so the comment indicates this file.
    #   self            the UtilitySourceOutputGenerator object
    def outputGeneratedHeaderWarning(self):
        # REUSE-IgnoreStart
        generated_warning = ''
        generated_warning += '// Copyright (c) 2017-2025 The Khronos Group Inc.\n'
        generated_warning += '// Copyright (c) 2017-2019, Valve Corporation\n'
        generated_warning += '// Copyright (c) 2017-2019, LunarG, Inc.\n\n'
        # Broken string is to avoid confusing the REUSE tool here.
        generated_warning += '// SPDX-License-' + 'Identifier: Apache-2.0 OR MIT\n\n'
        generated_warning += '// *********** THIS FILE IS GENERATED - DO NOT EDIT ***********\n'
        generated_warning += '//     See utility_source_generator.py for modifications\n'
        generated_warning += '// ************************************************************\n'
        # REUSE-IgnoreEnd
        write(generated_warning, file=self.outFile)

    # Call the base class to properly begin the file, and then add
    # the file-specific header information.
    #   self            the UtilitySourceOutputGenerator object
    #   gen_opts        the UtilitySourceGeneratorOptions object
    def beginFile(self, genOpts):
        AutomaticSourceOutputGenerator.beginFile(self, genOpts)
        assert self.genOpts
        assert self.genOpts.filename

        preamble = ''

        if self.genOpts.filename.endswith('.h'):
            # All .h start the same
            preamble += '#pragma once\n\n'

        elif self.genOpts.filename.endswith('.c'):
            # All .c files start the same
            header = self.genOpts.filename.replace('.c', '.h')
            # preamble += f'#include "{header}"\n\n'
        else:
            raise RuntimeError(f"Unknown filename extension! {self.genOpts.filename}")

        # The different .h files have different includes
        # if self.genOpts.filename == 'xr_generated_dispatch_table_core.h':
        #     preamble += '#include <openxr/openxr.h>\n'

        # elif self.genOpts.filename == 'xr_generated_dispatch_table.h':
        #     preamble += '#include "xr_dependencies.h"\n'
        #     preamble += '#include <openxr/openxr.h>\n'
        #     preamble += '#include <openxr/openxr_platform.h>\n'

        preamble += '\n'

        write(preamble, file=self.outFile)

    # Write out all the information for the appropriate file,
    # and then call down to the base class to wrap everything up.
    #   self            the UtilitySourceOutputGenerator object
    def endFile(self):
        assert self.genOpts
        assert self.genOpts.filename

        file_data = ''

        file_data += '#ifdef __cplusplus\n'
        file_data += 'extern "C" { \n'
        file_data += '#endif\n'

        if self.genOpts.filename.endswith('.h'):
            file_data += self.outputDispatchTable()
            file_data += self.outputDispatchPrototypes()

        elif self.genOpts.filename.endswith('.c'):
            file_data += self.outputDispatchTableHelper()

        else:
            raise RuntimeError(f"Unknown filename extension! {self.genOpts.filename}")

        file_data += '\n'
        file_data += '#ifdef __cplusplus\n'
        file_data += '} // extern "C"\n'
        file_data += '#endif\n'

        write(file_data, file=self.outFile)

        # Finish processing in superclass
        AutomaticSourceOutputGenerator.endFile(self)

    # Write out a prototype for a C-style command to populate a Dispatch table
    #   self            the ApiDumpOutputGenerator object
    def outputDispatchPrototypes(self):
        table_helper = '\n'
        table_helper += '// Prototype for dispatch table helper function\n'

        if self.genOpts.filename == 'xr_generated_dispatch_table_core.h':
            table_helper += 'void GeneratedXrPopulateDispatchTableCore(struct XrGeneratedDispatchTableCore *table,\n'
        else:
            table_helper += 'void GeneratedXrPopulateDispatchTable(struct XrGeneratedDispatchTable *table,\n'

        table_helper += '                                      XrInstance instance,\n'
        table_helper += '                                      PFN_xrGetInstanceProcAddr get_inst_proc_addr);\n'
        return table_helper

    def _feature_name_to_core_version(self, ext_name: str):
        return ext_name[len(self.conventions.api_version_prefix):].replace("_", ".")

    # Write out a C-style structure used to store the Dispatch table information
    #   self            the ApiDumpOutputGenerator object
    def outputDispatchTable(self):
        assert self.genOpts
        commands = []
        table = ''
        cur_extension = CurrentExtensionTracker(self.conventions.api_version_prefix)

        table += '// Generated dispatch table\n'
        if self.genOpts.filename == 'xr_generated_dispatch_table_core.h':
            table += 'typedef struct XrGeneratedDispatchTableCore XrGeneratedDispatchTableCore;\n'
            table += 'struct XrGeneratedDispatchTableCore {\n'
        else:
            table += 'typedef struct XrGeneratedDispatchTable XrGeneratedDispatchTable;\n'
            table += 'struct XrGeneratedDispatchTable {\n'

        # functions implemented for the loader are different
        LOADER_FUNCTIONS = [
            'xrCreateApiLayerInstance',
            'xrNegotiateLoaderRuntimeInterface',
            'xrNegotiateLoaderApiLayerInterface',
        ]
        # Loop through both core commands, and extension commands
        # Outputting the core commands first, and then the extension commands.
        for x in range(0, 2):
            if x == 0:
                commands = self.core_commands
            else:
                commands = self.ext_commands

            for cur_cmd in commands:
                if self.genOpts.filename == 'xr_generated_dispatch_table_core.h':
                    if self.isCoreExtensionName(cur_cmd.ext_name):
                        pass
                    # Loader implements XR_EXT_debug_utils
                    elif cur_cmd.ext_name == 'XR_EXT_debug_utils':
                        pass
                    else:
                        # Skip anything that is not core or XR_EXT_debug_utils in the loader dispatch table
                        continue

                # Skip loader-use-only functions in dispatch tables.
                if cur_cmd.name in LOADER_FUNCTIONS:
                    continue

                # If we've switched to a new "feature" print out a comment on what it is.  Usually,
                # this is a group of core commands or a group of commands in an extension.
                assert cur_cmd.ext_name
                table += cur_extension.format_if_extension_changed(cur_cmd.ext_name, "\n    // ---- {} commands\n")

                # Remove 'xr' from proto name
                base_name = cur_cmd.name[2:]

                # If a protect statement exists, use it.
                if cur_cmd.protect_value:
                    table += f'#if {cur_cmd.protect_string}\n'

                # Write out each command using it's function pointer for each command
                table += f'    PFN_{cur_cmd.name} {base_name};\n'

                # If a protect statement exists, wrap it up.
                if cur_cmd.protect_value:
                    table += f'#endif // {cur_cmd.protect_string}\n'
        table += '};\n\n'
        return table

    # Write out the helper function that will populate a dispatch table using
    # an instance handle and a corresponding xrGetInstanceProcAddr command.
    #   self            the ApiDumpOutputGenerator object
    def outputDispatchTableHelper(self):
        assert self.genOpts
        commands = []
        table_helper = ''
        cur_extension = CurrentExtensionTracker(self.conventions.api_version_prefix)

        table_helper += '// Helper function to populate an instance dispatch table\n'
        if self.genOpts.filename == 'xr_generated_dispatch_table_core.c':
            table_helper += 'void GeneratedXrPopulateDispatchTableCore(struct XrGeneratedDispatchTableCore *table,\n'
        else:
            table_helper += 'void GeneratedXrPopulateDispatchTable(struct XrGeneratedDispatchTable *table,\n'
        table_helper += '                                      XrInstance instance,\n'
        table_helper += '                                      PFN_xrGetInstanceProcAddr get_inst_proc_addr) {\n'

        # Loop through both core commands, and extension commands
        # Outputting the core commands first, and then the extension commands.
        for x in range(0, 2):
            if x == 0:
                commands = self.core_commands
            else:
                commands = self.ext_commands

            for cur_cmd in commands:
                # If the command is only manually implemented in the loader,
                # it is not needed anywhere else, so skip it.
                if cur_cmd.name in self.no_trampoline_or_terminator:
                    continue

                if self.genOpts.filename == 'xr_generated_dispatch_table_core.c':
                    if self.isCoreExtensionName(cur_cmd.ext_name):
                        pass
                    # Loader implements XR_EXT_debug_utils
                    elif cur_cmd.ext_name == 'XR_EXT_debug_utils':
                        pass
                    else:
                        # Skip anything that is not core or XR_EXT_debug_utils in the loader dispatch table
                        continue

                # If we've switched to a new "feature" print out a comment on what it is.  Usually,
                # this is a group of core commands or a group of commands in an extension.
                assert cur_cmd.ext_name
                table_helper += cur_extension.format_if_extension_changed(cur_cmd.ext_name, "\n    // ---- {} commands\n")

                # Remove 'xr' from proto name
                base_name = cur_cmd.name[2:]

                if cur_cmd.protect_value:
                    table_helper += f'#if {cur_cmd.protect_string}\n'

                if cur_cmd.name == 'xrGetInstanceProcAddr':
                    # If the command we're filling in is the xrGetInstanceProcAddr command, use
                    # the one passed into this helper function.
                    table_helper += '    table->GetInstanceProcAddr = get_inst_proc_addr;\n'
                else:
                    # Otherwise, fill in the dispatch table with an xrGetInstanceProcAddr call
                    # to the appropriate command.
                    table_helper += '    (get_inst_proc_addr(instance, "%s", (PFN_xrVoidFunction*)&table->%s));\n' % (
                        cur_cmd.name, base_name)

                if cur_cmd.protect_value:
                    table_helper += f'#endif // {cur_cmd.protect_string}\n'
        table_helper += '}\n\n'
        return table_helper

MANUAL_LOADER_FUNCS = set((
    'xrNegotiateLoaderRuntimeInterface',
    'xrNegotiateLoaderApiLayerInterface',
    'xrCreateApiLayerInstance',

    'xrGetInstanceProcAddr',
    'xrEnumerateApiLayerProperties',
    'xrEnumerateInstanceExtensionProperties',
    'xrCreateInstance',
    'xrDestroyInstance',

    # For XR_EXT_debug_utils:
    'xrCreateDebugUtilsMessengerEXT',
    'xrDestroyDebugUtilsMessengerEXT',
    'xrSessionBeginDebugUtilsLabelRegionEXT',
    'xrSessionEndDebugUtilsLabelRegionEXT',
    'xrSessionInsertDebugUtilsLabelEXT',

    # For XR_KHR_loader_init:
    'xrInitializeLoaderKHR',
))

EXTENSIONS_LOADER_IMPLEMENTS = [
    'XR_EXT_debug_utils'
]

class VdLoaderSourceOutputGenerator(AutomaticSourceOutputGenerator):
    def getProto(self, cur_cmd):
        return cur_cmd.cdecl.replace("XRAPI_ATTR", 'VD_XR_API XRAPI_ATTR')

    def beginFile(self, genOpts):
        AutomaticSourceOutputGenerator.beginFile(self, genOpts)
    def endFile(self):

        file_data = ''
        file_data += self.write_all_procs()
        write(file_data, file=self.outFile)
        AutomaticSourceOutputGenerator.endFile(self)

    def write_all_procs(self):
        generated_funcs = '\n// Automatically generated instance trampolines and terminators\n'
        for cur_cmd in self.core_commands:
            if cur_cmd.name in MANUAL_LOADER_FUNCS:
                continue

            base_name = cur_cmd.name[2:]
            has_return = False

            if cur_cmd.is_create_connect or cur_cmd.is_destroy_disconnect:
                has_return = True
            elif cur_cmd.return_type is not None:
                has_return = True
            
            tramp_variable_defines = ''
            tramp_param_replace = []
            for count, param in enumerate(cur_cmd.params):
                param_cdecl = param.cdecl
                is_const = False
                const_check = param_cdecl.strip()
                if const_check[:5].lower() == "const":
                    is_const = True
                
                pointer_count = self.paramPointerCount(param.cdecl, param.type, param.name)
                array_dimen = self.paramArrayDimension(param.cdecl, param.type, param.name)

                static_array_sizes = []
                if param.is_static_array:
                    static_array_sizes = param.static_array_sizes

                cmd_tramp_param_name = param.name
                cmd_tramp_is_handle = param.is_handle

                if count == 0:
                    if param.is_handle:
                        base_handle_name = undecorate(param.type)
                        first_handle_name = self.getFirstHandleName(param)

                        tramp_variable_defines += '    XrResult result;\n'
                        tramp_variable_defines += '    {\n'

                tramp_param_replace.append(
                    dataclasses.replace(param,
                                        name=cmd_tramp_param_name,
                                        is_const=is_const,
                                        is_handle=cmd_tramp_is_handle,
                                        static_array_sizes=static_array_sizes,
                                        array_dimen=array_dimen,
                                        pointer_count=pointer_count,
                                        valid_extension_structs=None))
                count = count + 1
            if cur_cmd.protect_value:
                generated_funcs += f'#if {cur_cmd.protect_string}\n'
            decl = self.getProto(cur_cmd).replace(";", " {\n")

            generated_funcs += decl
            generated_funcs += tramp_variable_defines

            if has_return:
                generated_funcs += '        result = '
            else:
                generated_funcs += '        '
            
            generated_funcs += 'Vd_Xr_G.dispatch_table->'
            generated_funcs += base_name
            generated_funcs += '('

            count = 0
            for param in tramp_param_replace:
                if count > 0:
                    generated_funcs += ', '
                generated_funcs += param.name
                count = count +1

            generated_funcs += ');\n'

            generated_funcs += '    }\n'

            if has_return:
                generated_funcs += '    return result;\n'

            generated_funcs += '}\n'

            if cur_cmd.protect_value:
                generated_funcs += f"#endif /* {cur_cmd.protect_string} */\n"

            generated_funcs += '\n'
        return generated_funcs


conventions = APIConventions()

openxr_h = [
    COutputGenerator,
    CGeneratorOptions(
        conventions       = conventions,
        filename          = 'openxr.h',
        directory         = '../../../openxr_generated',
        apiname           = 'openxr',
        profile           = None,
        versions          = r'.*',
        emitversions      = r'.*',
        defaultExtensions = 'openxr',
        addExtensions     = None,
        removeExtensions  = None,
        emitExtensions    = r'.*',
        prefixText        = [
            '/** openxr.h',
            '**/'
        ],
        genFuncPointers   = True,
        protectFile       = True,
        protectFeature    = False,
        protectProto      = '#ifndef',
        protectProtoStr   = 'XR_NO_PROTOTYPES',
        protectExtensionProto      = '#ifdef',
        protectExtensionProtoStr   = 'XR_EXTENSION_PROTOTYPES',
        apicall           = 'XRAPI_ATTR ',
        apientry          = 'XRAPI_CALL ',
        apientryp         = 'XRAPI_PTR *',
        alignFuncParam    = 48,
        genAliasMacro     = True,
        genStructExtendsComment = True,
        aliasMacro        = 'XR_MAY_ALIAS')
]

loader_c = [
    VdLoaderSourceOutputGenerator,
    AutomaticSourceGeneratorOptions(
        conventions=conventions,
        filename='xr_generated_loader.cpp',
        directory='../../../openxr_generated',
        apiname='openxr',
        profile=None,
        versions=r'.*',
        emitversions=r'.*',
        defaultExtensions='openxr',
        addExtensions=None,
        removeExtensions=None,
        emitExtensions=r'.*',
        prefixText=[
			'/** xr_generated_loader.cpp',
			'**/'],
        protectFeature=False,
        protectProto='#ifndef',
        protectProtoStr='XR_NO_PROTOTYPES',
        apicall='XRAPI_ATTR ',
        apientry='XRAPI_CALL ',
        apientryp='XRAPI_PTR *',
        alignFuncParam=48)
]

dispatch_table_h = [
    UtilitySourceOutputGenerator,
    AutomaticSourceGeneratorOptions(
        conventions=conventions,
        filename="xr_generated_dispatch_table.h",
        directory='../../../openxr_generated',
        apiname='openxr',
        profile=None,
        prefixText=[
			'/** xr_generated_dispatch_table.h',
			'**/'],
        versions=r'.*',
        emitversions=r'.*',
        defaultExtensions='openxr',
        addExtensions=None,
        removeExtensions=None,
        emitExtensions=r'.*')
]

dispatch_table_c = [
    UtilitySourceOutputGenerator,
    AutomaticSourceGeneratorOptions(
        conventions=conventions,
        filename="xr_generated_dispatch_table.c",
        directory='../../../openxr_generated',
        apiname='openxr',
        profile=None,
        prefixText=[
			'/** xr_generated_dispatch_table.c',
			'**/'],
        versions=r'.*',
        emitversions=r'.*',
        defaultExtensions='openxr',
        addExtensions=None,
        removeExtensions=None,
        emitExtensions=r'.*')
]

def run_generator(config):
    gen = config[0](errFile=sys.stdout,
                    warnFile=sys.stdout,
                    diagFile=None)
    options = config[1]
    reg = Registry(gen, options)

    tree = etree.parse('../registry/xr.xml')
    reg.loadElementTree(tree)
    reg.apiGen()

run_generator(openxr_h)
run_generator(loader_c)
run_generator(dispatch_table_h)
run_generator(dispatch_table_c)

file_dict = {
    "$$openxr.h$$": "../../../openxr_generated/openxr.h",
    "$$xr_generated_dispatch_table.h$$": "../../../openxr_generated/xr_generated_dispatch_table.h",
    "$$xr_generated_dispatch_table.c$$": "../../../openxr_generated/xr_generated_dispatch_table.c",
    "$$xr_generated_loader.cpp$$": "../../../openxr_generated/xr_generated_loader.cpp"
}

def write_lines_to_file(outfile, filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
          outfile.write(f.read())

# Write custom content and append openxr.h to a new file
new_file_path = '../../../vd_xr.proto.h'
file = open('../../../vd_xr.proto.h', 'r', encoding='utf-8')
out = open('../../../vd_xr.h', 'w', encoding='utf-8')
for line in file:
        if line.startswith('$$'):
            macro = line.strip()
            if macro not in file_dict:
                raise RuntimeError(f"{macro} not recognized for replacement!")
            
            with open(file_dict[macro], 'r', encoding='utf-8') as f:
                for xrline in f:
                    if xrline.startswith('#include "openxr_platform_defines.h"'):
                        write_lines_to_file(out, "../../include/openxr/openxr_platform_defines.h")
                    elif xrline.startswith('#include "xr_dependencies.h'):
                        write_lines_to_file(out, "../../src/common/xr_dependencies.h")
                    elif xrline.startswith('#include <openxr/openxr.h>'):
                        pass
                    elif xrline.startswith('#include <openxr/openxr_platform.h>'):
                        pass
                    else:
                        out.write(xrline)
        else:
            out.write(line)
        

file.close()
out.close()