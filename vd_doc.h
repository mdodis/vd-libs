/**
 * \internal
 * vd_doc.h - A documentation listing generator
 * 
 * zlib License
 * 
 * (C) Copyright 2025-2026 Michael Dodis (michaeldodisgr@gmail.com)
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#ifndef VD_DOC_H
#define VD_DOC_H
#define VD_DOC_VERSION_MAJOR 0
#define VD_DOC_VERSION_MINOR 0
#define VD_DOC_VERSION_PATCH 1
#define VD_DOC_VERSION       ((VD_DOC_VERSION_MAJOR << 16) | (VD_DOC_VERSION_MINOR << 8) | (VD_DOC_VERSION_PATCH))

#ifndef VD_DOC_PARSER
#define VD_DOC_PARSER 0
#endif // !VD_DOC_PARSER

#ifndef VD_DOC_MACRO_ABBREVIATIONS
#define VD_DOC_MACRO_ABBREVIATIONS VD_MACRO_ABBREVIATIONS
#endif // !VD_DOC_MACRO_ABBREVIATIONS

#define VD_DOC(...)

#if VD_DOC_MACRO_ABBREVIATIONS
#define DOC(...) VD_DOC(__VA_ARGS__)
#endif // VD_DOC_MACRO_ABBREVIATIONS


#endif // !VD_DOC_H