/**
 * \internal
 * vd_debug.h - Quick debugging utilities
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
#ifndef VD_DEBUG_H
#define VD_DEBUG_H
#define VD_DEBUG_VERSION_MAJOR 0
#define VD_DEBUG_VERSION_MINOR 0
#define VD_DEBUG_VERSION_PATCH 1
#define VD_DEBUG_VERSION       ((VD_DEBUG_VERSION_MAJOR << 16) | (VD_DEBUG_VERSION_MINOR << 8) | (VD_DEBUG_VERSION_PATCH))

#ifndef VD_H
#error "vd_debug.h requires vd.h"
#endif // !VD_H

#ifndef VD_DEBUG_ENABLE
#define VD_DEBUG_ENABLE 0
#endif // !VD_DEBUG_ENABLE

#ifndef VD_DEBUG_MAX_ZONES
#define VD_DEBUG_MAX_ZONES 1024
#endif // !VD_DEBUG_MAX_ZONES

#ifndef VD_DEBUG_MAX_FRAMES
#define VD_DEBUG_MAX_FRAMES 60
#endif // !VD_DEBUG_MAX_FRAMES

#define VD_DEBUG_ID(name) VD_STRING_JOIN2(vd__debug__id, name)

typedef struct __VD_Debug_Zone {
    const char *name;
    VdHiTime    begin_time;
    const char *file;
    Vdu64      line;
    const char *function;
    Vdu64      id;
    VdHiTime   end_time;
} VdDebugZone;

typedef struct __VD_Debug_Frame {
    Vdu32       num_zones;
    Vdu32       current_zone_index;
    VdDebugZone zones[VD_DEBUG_MAX_ZONES];
} VdDebugFrame;

extern void          vd__debug_init(void);
extern void          vd__debug_frame_begin(void);
extern VdDebugZone*  vd__debug_zone_begin(const char *id, const char *file, Vdu64 line, const char *function, Vdu64 unique_id);
extern void          vd__debug_zone_end(VdDebugZone *zone);
extern void          vd__debug_frame_end(void);

extern void          vd_debug_pause(Vdb32 pause);
extern Vdb32         vd_debug_is_paused(void);
extern Vdusize       vd_debug_get_num_frames(void);
extern VdDebugFrame* vd_debug_get_frame(Vdusize i);

extern Vdusize       vd_debug_frame_get_num_zones(VdDebugFrame *frame);
extern VdDebugZone*  vd_debug_frame_get_zone(VdDebugFrame *frame, Vdusize i);

extern Vdf64         vd_debug_zone_get_ms(VdDebugZone *zone);

#if VD_DEBUG_ENABLE
#define VD_DEBUG_INIT()           vd__debug_init()
#define VD_DEBUG_FRAME_BEGIN()    vd__debug_frame_begin()
#define VD_DEBUG_ZONE_BEGIN(name) VdDebugZone *VD_DEBUG_ID(name) = vd__debug_zone_begin(#name, __FILE__, __LINE__, __FUNCTION__, __COUNTER__)
#define VD_DEBUG_ZONE_END(name)   vd__debug_zone_end(VD_DEBUG_ID(name))
#define VD_DEBUG_FRAME_END()      vd__debug_frame_end()
#else
#define VD_DEBUG_INIT()
#define VD_DEBUG_FRAME_BEGIN()
#define VD_DEBUG_ZONE_BEGIN(name)
#define VD_DEBUG_ZONE_END(name)
#define VD_DEBUG_FRAME_END()
#endif // VD_DEBUG_ENABLE, else

#if VD_MACRO_ABBREVIATIONS
#define DEBUG_INIT                  VD_DEBUG_INIT
#define DEBUG_FRAME_BEGIN           VD_DEBUG_FRAME_BEGIN
#define DEBUG_ZONE_BEGIN            VD_DEBUG_ZONE_BEGIN
#define DEBUG_ZONE_END              VD_DEBUG_ZONE_END
#define DEBUG_FRAME_END             VD_DEBUG_FRAME_END
#define DebugFrame                  VdDebugFrame
#define DebugZone                   VdDebugZone
#define debug_pause                 vd_debug_pause
#define debug_is_paused             vd_debug_is_paused
#define debug_get_num_frames        vd_debug_get_num_frames
#define debug_get_frame             vd_debug_get_frame
#define debug_frame_get_num_zones   vd_debug_frame_get_num_zones
#define debug_frame_get_zone        vd_debug_frame_get_zone
#define debug_zone_get_ms           vd_debug_zone_get_ms
#endif // VD_MACRO_ABBREVIATIONS

#endif // !VD_DEBUG_H

#ifdef VD_DEBUG_IMPL

typedef struct __VD_Debug_State {
    VdArena       debug_arena;
    VdDebugFrame  *frames;
    Vdusize       curr_frame;
    Vdb32         will_pause;
    Vdb32         paused;
} Vd__DebugState;

static Vd__DebugState Vd__Debug_State;
#define VD_DEBUG_GET_STATE()  (Vd__Debug_State)
#define VD_DEBUG_CURR_FRAME() (VD_DEBUG_GET_STATE().frames[VD_DEBUG_GET_STATE().curr_frame])

void vd__debug_init(void)
{
    Vdusize arena_size = VD_DEBUG_MAX_FRAMES * sizeof(VdDebugFrame) * 2;
    vd_arena_init(
        &VD_DEBUG_GET_STATE().debug_arena,
        VD_MEMSET(VD_MALLOC(arena_size), 0, arena_size), arena_size);

    VD_DEBUG_GET_STATE().frames = (VdDebugFrame*)vd_arena_alloc(&VD_DEBUG_GET_STATE().debug_arena, VD_DEBUG_MAX_FRAMES * sizeof(VdDebugFrame));
}

void vd__debug_frame_begin(void)
{
    if (VD_DEBUG_GET_STATE().will_pause != VD_DEBUG_GET_STATE().paused) {
        VD_DEBUG_GET_STATE().paused = VD_DEBUG_GET_STATE().will_pause;
    }
}

VdDebugZone *vd__debug_zone_begin(const char *id, const char *file, Vdu64 line, const char *function, Vdu64 unique_id)
{
    VD_DEBUG_CURR_FRAME().current_zone_index++;

    if (VD_DEBUG_GET_STATE().paused) {
        return 0;    
    }

    VdDebugZone *zn = &VD_DEBUG_CURR_FRAME().zones[VD_DEBUG_CURR_FRAME().num_zones++];
    zn->name = id;
    zn->file = file;
    zn->line = line;
    zn->function = function;
    zn->begin_time = vd_hitime_get();
    zn->id = unique_id;
    return zn;
}

void vd__debug_zone_end(VdDebugZone *zone)
{
    VD_ASSERT(VD_DEBUG_CURR_FRAME().current_zone_index != 0);
    VD_DEBUG_CURR_FRAME().current_zone_index--;
    if (zone == 0) {
        return;
    }

    zone->end_time = vd_hitime_get();
}

void vd__debug_frame_end(void)
{
    VD_ASSERT(VD_DEBUG_CURR_FRAME().current_zone_index == 0 && "A VD_DEBUG_ZONE_BEGIN/VD_DEBUG_ZONE_END mismatch has occurred");
    if (VD_DEBUG_GET_STATE().paused) {
        return;
    }

    VD_DEBUG_GET_STATE().curr_frame++;
    if (VD_DEBUG_GET_STATE().curr_frame > VD_DEBUG_MAX_FRAMES) {
        VD_DEBUG_GET_STATE().curr_frame = 0;
    }
    VD_DEBUG_CURR_FRAME().num_zones = 0;
}

void vd_debug_pause(Vdb32 pause)
{
    VD_DEBUG_GET_STATE().will_pause = pause;
}

Vdb32 vd_debug_is_paused(void)
{
    return VD_DEBUG_GET_STATE().will_pause;
}

Vdusize vd_debug_get_num_frames(void)
{
    return (VD_DEBUG_MAX_FRAMES - 1);
}

VdDebugFrame *vd_debug_get_frame(Vdusize i)
{
    return &VD_DEBUG_GET_STATE().frames[(VD_DEBUG_GET_STATE().curr_frame + 1 + i) % VD_DEBUG_MAX_FRAMES];
}

Vdusize vd_debug_frame_get_num_zones(VdDebugFrame *frame)
{
    return frame->num_zones;
}

VdDebugZone *vd_debug_frame_get_zone(VdDebugFrame *frame, Vdusize i)
{
    return &frame->zones[i];
}

Vdf64 vd_debug_zone_get_ms(VdDebugZone *zone)
{
    Vdf64 ms = vd_hitime_fms64(vd_hitime_sub(zone->end_time, zone->begin_time));
    return ms;
}

#undef VD_DEBUG_CURR_FRAME
#undef VD_DEBUG_GET_STATE
#endif // VD_DEBUG_IMPL