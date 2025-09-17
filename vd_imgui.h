/**
 * \internal
 * vd_imgui.h - A C translation layer for Dear ImGui
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
 *
 * NOTE
 * This library is only used for translating imgui functions for USAGE only and not
 * for set-up (i.e. using backends, creating context, rendering the draw list).
 * For that, you will have to drop down to C++ land.
 *
 * @todo:
 * - Add a fully abbreviated version of every function given a user's VdIMGUIProcs getter function
 */
#ifndef VD_IMGUI_H
#define VD_IMGUI_H
#define VD_IMGUI_VERSION_MAJOR 0
#define VD_IMGUI_VERSION_MINOR 0
#define VD_IMGUI_VERSION_PATCH 1
#define VD_IMGUI_VERSION       ((VD_IMGUI_VERSION_MAJOR << 16) | (VD_IMGUI_VERSION_MINOR << 8) | (VD_IMGUI_VERSION_PATCH))

#ifndef VD_H
#error "vd_imgui.h requires vd.h"
#endif // !VD_H

#include <stdarg.h>

#ifndef VD_IMGUI_API
#define VD_IMGUI_API
#endif // !VD_IMGUI_API

#ifndef VD_IMGUI_FUNCTIONS_GLOBAL
#define VD_IMGUI_FUNCTIONS_GLOBAL 0
#endif // !VD_IMGUI_FUNCTIONS_GLOBAL

#ifndef VD_IMGUI_MAX_DRAW_LIST_SPLIITERS
#define VD_IMGUI_MAX_DRAW_LIST_SPLIITERS 32
#endif // !VD_IMGUI_MAX_DRAW_LIST_SPLIITERS

typedef void*        VdIMGUIDrawList;
typedef void*        VdIMGUISplitter;
typedef unsigned int VdIMGUIID;

enum {
    IMGUI_WINDOW_FLAGS_NONE                           = 0,
    IMGUI_WINDOW_FLAGS_NO_TITLEBAR                    = 1 << 0,   // Disable title-bar
    IMGUI_WINDOW_FLAGS_NO_RESIZE                      = 1 << 1,   // Disable user resizing with the lower-right grip
    IMGUI_WINDOW_FLAGS_NO_MOVE                        = 1 << 2,   // Disable user moving the window
    IMGUI_WINDOW_FLAGS_NO_SCROLLBAR                   = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
    IMGUI_WINDOW_FLAGS_NO_SCROLL_WITH_MOUSE           = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
    IMGUI_WINDOW_FLAGS_NO_COLLAPSE                    = 1 << 5,   // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
    IMGUI_WINDOW_FLAGS_ALWAYS_AUTO_RESIZE             = 1 << 6,   // Resize every window to its content every frame
    IMGUI_WINDOW_FLAGS_NO_BACKGROUND                  = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
    IMGUI_WINDOW_FLAGS_NO_SAVED_SETTINGS              = 1 << 8,   // Never load/save settings in .ini file
    IMGUI_WINDOW_FLAGS_NO_MOUSE_INPUTS                = 1 << 9,   // Disable catching mouse, hovering test with pass through.
    IMGUI_WINDOW_FLAGS_MENU_BAR                       = 1 << 10,  // Has a menu-bar
    IMGUI_WINDOW_FLAGS_HORIZONTAL_SCROLLBAR           = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
    IMGUI_WINDOW_FLAGS_NO_FOCUS_ON_APPEARING          = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
    IMGUI_WINDOW_FLAGS_NO_BRING_TO_FRONT_ON_FOCUS     = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
    IMGUI_WINDOW_FLAGS_ALWAYS_VERTICAL_SCROLLBAR      = 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
    IMGUI_WINDOW_FLAGS_ALWAYS_HORIZONTAL_SCROLLBAR    = 1 << 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
    IMGUI_WINDOW_FLAGS_NO_NAV_INPUTS                  = 1 << 16,  // No keyboard/gamepad navigation within the window
    IMGUI_WINDOW_FLAGS_NO_NAV_FOCUS                   = 1 << 17,  // No focusing toward this window with keyboard/gamepad navigation (e.g. skipped by CTRL+TAB)
    IMGUI_WINDOW_FLAGS_UNSAVED_DOCUMENT               = 1 << 18,  // Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
    IMGUI_WINDOW_FLAGS_NO_DOCKING                     = 1 << 19,  // Disable docking of this window
    IMGUI_WINDOW_FLAGS_NO_NAV                         = IMGUI_WINDOW_FLAGS_NO_NAV_INPUTS | IMGUI_WINDOW_FLAGS_NO_NAV_FOCUS,
    IMGUI_WINDOW_FLAGS_NO_DECORATION                  = IMGUI_WINDOW_FLAGS_NO_TITLEBAR | IMGUI_WINDOW_FLAGS_NO_RESIZE | IMGUI_WINDOW_FLAGS_NO_SCROLLBAR | IMGUI_WINDOW_FLAGS_NO_COLLAPSE,
    IMGUI_WINDOW_FLAGS_NO_INPUTS                      = IMGUI_WINDOW_FLAGS_NO_MOUSE_INPUTS | IMGUI_WINDOW_FLAGS_NO_NAV_INPUTS | IMGUI_WINDOW_FLAGS_NO_NAV_FOCUS,
};
typedef int VdIMGUIWindowFlags;

enum {
    IMGUI_CHILD_FLAGS_NONE                       = 0,
    IMGUI_CHILD_FLAGS_BORDERS                    = 1 << 0,   // Show an outer border and enable WindowPadding. (IMPORTANT: this is always == 1 == true for legacy reason)
    IMGUI_CHILD_FLAGS_ALWAYS_USE_WINDOW_PADDING  = 1 << 1,   // Pad with style.WindowPadding even if no border are drawn (no padding by default for non-bordered child windows because it makes more sense)
    IMGUI_CHILD_FLAGS_RESIZE_X                   = 1 << 2,   // Allow resize from right border (layout direction). Enable .ini saving (unless ImGuiWindowFlags_NoSavedSettings passed to window flags)
    IMGUI_CHILD_FLAGS_RESIZE_Y                   = 1 << 3,   // Allow resize from bottom border (layout direction). "
    IMGUI_CHILD_FLAGS_AUTO_RESIZE_X              = 1 << 4,   // Enable auto-resizing width. Read "IMPORTANT: Size measurement" details above.
    IMGUI_CHILD_FLAGS_AUTO_RESIZE_Y              = 1 << 5,   // Enable auto-resizing height. Read "IMPORTANT: Size measurement" details above.
    IMGUI_CHILD_FLAGS_ALWAYS_AUTO_RESIZE         = 1 << 6,   // Combined with AutoResizeX/AutoResizeY. Always measure size even when child is hidden, always return true, always disable clipping optimization! NOT RECOMMENDED.
    IMGUI_CHILD_FLAGS_FRAME_STYLE                = 1 << 7,   // Style the child window like a framed item: use FrameBg, FrameRounding, FrameBorderSize, FramePadding instead of ChildBg, ChildRounding, ChildBorderSize, WindowPadding.
};
typedef int VdIMGUIChildFlags;

enum {
    IMGUI_STYLE_VAR_ALPHA,                                   // float     Alpha
    IMGUI_STYLE_VAR_DISABLED_ALPHA,                          // float     DisabledAlpha
    IMGUI_STYLE_VAR_WINDOW_PADDING,                          // ImVec2    WindowPadding
    IMGUI_STYLE_VAR_WINDOW_ROUNDING,                         // float     WindowRounding
    IMGUI_STYLE_VAR_WINDOW_BORDER_SIZE,                      // float     WindowBorderSize
    IMGUI_STYLE_VAR_WINDOW_MIN_SIZE,                         // ImVec2    WindowMinSize
    IMGUI_STYLE_VAR_WINDOW_TITLE_ALIGN,                      // ImVec2    WindowTitleAlign
    IMGUI_STYLE_VAR_CHILD_ROUNDING,                          // float     ChildRounding
    IMGUI_STYLE_VAR_CHILD_BORDER_SIZE,                       // float     ChildBorderSize
    IMGUI_STYLE_VAR_POPUP_ROUNDING,                          // float     PopupRounding
    IMGUI_STYLE_VAR_POPUP_BORDER_SIZE,                       // float     PopupBorderSize
    IMGUI_STYLE_VAR_FRAME_PADDING,                           // ImVec2    FramePadding
    IMGUI_STYLE_VAR_FRAME_ROUNDING,                          // float     FrameRounding
    IMGUI_STYLE_VAR_FRAME_BORDER_SIZE,                       // float     FrameBorderSize
    IMGUI_STYLE_VAR_ITEM_SPACING,                            // ImVec2    ItemSpacing
    IMGUI_STYLE_VAR_ITEM_INNER_SPACING,                      // ImVec2    ItemInnerSpacing
    IMGUI_STYLE_VAR_INDENT_SPACING,                          // float     IndentSpacing
    IMGUI_STYLE_VAR_CELL_PADDING,                            // ImVec2    CellPadding
    IMGUI_STYLE_VAR_SCROLLBAR_SIZE,                          // float     ScrollbarSize
    IMGUI_STYLE_VAR_SCROLLBAR_ROUNDING,                      // float     ScrollbarRounding
    IMGUI_STYLE_VAR_GRAB_MIN_SIZE,                           // float     GrabMinSize
    IMGUI_STYLE_VAR_GRAB_ROUNDING,                           // float     GrabRounding
    IMGUI_STYLE_VAR_IMAGE_BORDER_SIZE,                       // float     ImageBorderSize
    IMGUI_STYLE_VAR_TAB_ROUNDING,                            // float     TabRounding
    IMGUI_STYLE_VAR_TAB_BORDER_SIZE,                         // float     TabBorderSize
    IMGUI_STYLE_VAR_TAB_MIN_WIDTH_BASE,                      // float     TabMinWidthBase
    IMGUI_STYLE_VAR_TAB_MIN_WIDTH_SHRINK,                    // float     TabMinWidthShrink
    IMGUI_STYLE_VAR_TAB_BAR_BORDER_SIZE,                     // float     TabBarBorderSize
    IMGUI_STYLE_VAR_TAB_BAR_OVERLINE_SIZE,                   // float     TabBarOverlineSize
    IMGUI_STYLE_VAR_TABLE_ANGLED_HEADERS_ANGLE,              // float     TableAngledHeadersAngle
    IMGUI_STYLE_VAR_TABLE_ANGLED_HEADERS_TEXT_ALIGN,         // ImVec2  TableAngledHeadersTextAlign
    IMGUI_STYLE_VAR_TREE_LINES_SIZE,                         // float     TreeLinesSize
    IMGUI_STYLE_VAR_TREE_LINES_ROUNDING,                     // float     TreeLinesRounding
    IMGUI_STYLE_VAR_BUTTON_TEXT_ALIGN,                       // ImVec2    ButtonTextAlign
    IMGUI_STYLE_VAR_SELECTABLE_TEXT_ALIGN,                   // ImVec2    SelectableTextAlign
    IMGUI_STYLE_VAR_SEPARATOR_TEXT_BORDER_SIZE,              // float     SeparatorTextBorderSize
    IMGUI_STYLE_VAR_SEPARATOR_TEXT_ALIGN,                    // ImVec2    SeparatorTextAlign
    IMGUI_STYLE_VAR_SEPARATOR_TEXT_PADDING,                  // ImVec2    SeparatorTextPadding
    IMGUI_STYLE_VAR_COUNT
};
typedef int VdIMGUIStyleVar;

enum {
    IMGUI_COL_TEXT,
    IMGUI_COL_TEXT_DISABLED,
    IMGUI_COL_WINDOW_BG,                    // Background of normal windows
    IMGUI_COL_CHILD_BG,                     // Background of child windows
    IMGUI_COL_POPUP_BG,                     // Background of popups, menus, tooltips windows
    IMGUI_COL_BORDER,
    IMGUI_COL_BORDER_SHADOW,
    IMGUI_COL_FRAME_BG,                     // Background of checkbox, radio button, plot, slider, text input
    IMGUI_COL_FRAME_BG_HOVERED,
    IMGUI_COL_FRAME_BG_ACTIVE,
    IMGUI_COL_TITLE_BG,                     // Title bar
    IMGUI_COL_TITLE_BG_ACTIVE,              // Title bar when focused
    IMGUI_COL_TITLE_BG_COLLAPSED,           // Title bar when collapsed
    IMGUI_COL_MENU_BAR_BG,
    IMGUI_COL_SCROLLBAR_BG,
    IMGUI_COL_SCROLLBAR_GRAB,
    IMGUI_COL_SCROLLBAR_GRAB_HOVERED,
    IMGUI_COL_SCROLLBAR_GRAB_ACTIVE,
    IMGUI_COL_CHECK_MARK,                   // Checkbox tick and RadioButton circle
    IMGUI_COL_SLIDER_GRAB,
    IMGUI_COL_SLIDER_GRAB_ACTIVE,
    IMGUI_COL_BUTTON,
    IMGUI_COL_BUTTON_HOVERED,
    IMGUI_COL_BUTTON_ACTIVE,
    IMGUI_COL_HEADER,                       // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
    IMGUI_COL_HEADER_HOVERED,
    IMGUI_COL_HEADER_ACTIVE,
    IMGUI_COL_SEPARATOR,
    IMGUI_COL_SEPARATOR_HOVERED,
    IMGUI_COL_SEPARATOR_ACTIVE,
    IMGUI_COL_RESIZE_GRIP,                  // Resize grip in lower-right and lower-left corners of windows.
    IMGUI_COL_RESIZE_GRIP_HOVERED,
    IMGUI_COL_RESIZE_GRIP_ACTIVE,
    IMGUI_COL_INPUT_TEXT_CURSOR,            // InputText cursor/caret
    IMGUI_COL_TAB_HOVERED,                  // Tab background, when hovered
    IMGUI_COL_TAB,                          // Tab background, when tab-bar is focused & tab is unselected
    IMGUI_COL_TAB_SELECTED,                 // Tab background, when tab-bar is focused & tab is selected
    IMGUI_COL_TAB_SELECTED_OVERLINE,        // Tab horizontal overline, when tab-bar is focused & tab is selected
    IMGUI_COL_TAB_DIMMED,                   // Tab background, when tab-bar is unfocused & tab is unselected
    IMGUI_COL_TAB_DIMMED_SELECTED,          // Tab background, when tab-bar is unfocused & tab is selected
    IMGUI_COL_TAB_DIMMED_SELECTED_OVERLINE, //..horizontal overline, when tab-bar is unfocused & tab is selected
    IMGUI_COL_PLOT_LINES,
    IMGUI_COL_PLOT_LINES_HOVERED,
    IMGUI_COL_PLOT_HISTOGRAM,
    IMGUI_COL_PLOT_HISTOGRAM_HOVERED,
    IMGUI_COL_TABLE_HEADER_BG,              // Table header background
    IMGUI_COL_TABLE_BORDER_STRONG,          // Table outer and header borders (prefer using Alpha=1.0 here)
    IMGUI_COL_TABLE_BORDER_LIGHT,           // Table inner borders (prefer using Alpha=1.0 here)
    IMGUI_COL_TABLE_ROW_BG,                 // Table row background (even rows)
    IMGUI_COL_TABLE_ROW_BG_ALT,             // Table row background (odd rows)
    IMGUI_COL_TEXT_LINK,                    // Hyperlink color
    IMGUI_COL_TEXT_SELECTED_BG,             // Selected text inside an InputText
    IMGUI_COL_TREE_LINES,                   // Tree node hierarchy outlines when using ImGuiTreeNodeFlags_DrawLines
    IMGUI_COL_DRAG_DROP_TARGET,             // Rectangle highlighting a drop target
    IMGUI_COL_NAV_CURSOR,                   // Color of keyboard/gamepad navigation cursor/rectangle, when visible
    IMGUI_COL_NAV_WINDOWING_HIGHLIGHT,      // Highlight window when using CTRL+TAB
    IMGUI_COL_NAV_WINDOWING_DIM_BG,         // Darken/colorize entire screen behind the CTRL+TAB window list, when active
    IMGUI_COL_MODAL_WINDOW_DIM_BG,          // Darken/colorize entire screen behind a modal window, when one is active
    IMGUI_COL_COUNT,
};
typedef int VdIMGUICol;

enum {
    IMGUI_DRAW_FLAGS_NONE                        = 0,
    IMGUI_DRAW_FLAGS_CLOSED                      = 1 << 0, // PathStroke(), AddPolyline(): specify that shape should be closed (Important: this is always == 1 for legacy reason)
    IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_LEFT      = 1 << 4, // AddRect(), AddRectFilled(), PathRect(): enable rounding top-left corner only (when rounding > 0.0f, we default to all corners). Was 0x01.
    IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_RIGHT     = 1 << 5, // AddRect(), AddRectFilled(), PathRect(): enable rounding top-right corner only (when rounding > 0.0f, we default to all corners). Was 0x02.
    IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_LEFT   = 1 << 6, // AddRect(), AddRectFilled(), PathRect(): enable rounding bottom-left corner only (when rounding > 0.0f, we default to all corners). Was 0x04.
    IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_RIGHT  = 1 << 7, // AddRect(), AddRectFilled(), PathRect(): enable rounding bottom-right corner only (when rounding > 0.0f, we default to all corners). Wax 0x08.
    IMGUI_DRAW_FLAGS_ROUND_CORNERS_NONE          = 1 << 8, // AddRect(), AddRectFilled(), PathRect(): disable rounding on all corners (when rounding > 0.0f). This is NOT zero, NOT an implicit flag!
    IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP           = IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_LEFT | IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_RIGHT,
    IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM        = IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_LEFT | IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_RIGHT,
    IMGUI_DRAW_FLAGS_ROUND_CORNERS_LEFT          = IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_LEFT | IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_LEFT,
    IMGUI_DRAW_FLAGS_ROUND_CORNERS_RIGHT         = IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_RIGHT | IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_RIGHT,
    IMGUI_DRAW_FLAGS_ROUND_CORNERS_ALL           = IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_LEFT | IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_RIGHT | IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_LEFT | IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_RIGHT,
    IMGUI_DRAW_FLAGS_ROUND_CORNERS_DEFAULT_      = IMGUI_DRAW_FLAGS_ROUND_CORNERS_ALL, // Default to ALL corners if none of the _RoundCornersXX flags are specified.
    IMGUI_DRAW_FLAGS_ROUND_CORNERS_MASK_         = IMGUI_DRAW_FLAGS_ROUND_CORNERS_ALL | IMGUI_DRAW_FLAGS_ROUND_CORNERS_NONE,
};
typedef int VdIMGUIDrawFlags;

enum {
    IMGUI_SLIDER_FLAGS_NONE                  = 0,
    IMGUI_SLIDER_FLAGS_LOGARITHMIC           = 1 << 5,       // Make the widget logarithmic (linear otherwise). Consider using ImGuiSliderFlags_NoRoundToFormat with this if using a format-string with small amount of digits.
    IMGUI_SLIDER_FLAGS_NO_ROUND_TO_FORMAT    = 1 << 6,       // Disable rounding underlying value to match precision of the display format string (e.g. %.3f values are rounded to those 3 digits).
    IMGUI_SLIDER_FLAGS_NO_INPUT              = 1 << 7,       // Disable CTRL+Click or Enter key allowing to input text directly into the widget.
    IMGUI_SLIDER_FLAGS_WRAP_AROUND           = 1 << 8,       // Enable wrapping around from max to min and from min to max. Only supported by DragXXX() functions for now.
    IMGUI_SLIDER_FLAGS_CLAMP_ON_INPUT        = 1 << 9,       // Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.
    IMGUI_SLIDER_FLAGS_CLAMP_ZERO_RANGE      = 1 << 10,      // Clamp even if min==max==0.0f. Otherwise due to legacy reason DragXXX functions don't clamp with those values. When your clamping limits are dynamic you almost always want to use it.
    IMGUI_SLIDER_FLAGS_NO_SPEED_TWEAKS       = 1 << 11,      // Disable keyboard modifiers altering tweak speed. Useful if you want to alter tweak speed yourself based on your own logic.
    IMGUI_SLIDER_FLAGS_ALWAYS_CLAMP          = IMGUI_SLIDER_FLAGS_CLAMP_ON_INPUT | IMGUI_SLIDER_FLAGS_CLAMP_ZERO_RANGE,
};
typedef int VdIMGUISliderFlags;

enum {
    //ImGuiHoveredFlags_DockHierarchy               = 1 << 4,   // IsWindowHovered() only: Consider docking hierarchy (treat dockspace host as parent of docked window) (when used with _ChildWindows or _RootWindow)
    //ImGuiHoveredFlags_AllowWhenBlockedByModal     = 1 << 6,   // Return true even if a modal popup window is normally blocking access to this item/window. FIXME-TODO: Unavailable yet.
    IMGUI_HOVERED_FLAGS_NONE                               = 0,        // Return true if directly over the item/window, not obstructed by another window, not obstructed by an active popup or modal blocking inputs under them.
    IMGUI_HOVERED_FLAGS_CHILD_WINDOWS                      = 1 << 0,   // IsWindowHovered() only: Return true if any children of the window is hovered
    IMGUI_HOVERED_FLAGS_ROOT_WINDOW                        = 1 << 1,   // IsWindowHovered() only: Test from root window (top most parent of the current hierarchy)
    IMGUI_HOVERED_FLAGS_ANY_WINDOW                         = 1 << 2,   // IsWindowHovered() only: Return true if any window is hovered
    IMGUI_HOVERED_FLAGS_NO_POPUP_HIERARCHY                 = 1 << 3,   // IsWindowHovered() only: Do not consider popup hierarchy (do not treat popup emitter as parent of popup) (when used with _ChildWindows or _RootWindow)
    IMGUI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_POPUP        = 1 << 5,   // Return true even if a popup window is normally blocking access to this item/window
    IMGUI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_ACTIVE_ITEM  = 1 << 7,   // Return true even if an active item is blocking access to this item/window. Useful for Drag and Drop patterns.
    IMGUI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED_BY_ITEM      = 1 << 8,   // IsItemHovered() only: Return true even if the item uses AllowOverlap mode and is overlapped by another hoverable item.
    IMGUI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED_BY_WINDOW    = 1 << 9,   // IsItemHovered() only: Return true even if the position is obstructed or overlapped by another window.
    IMGUI_HOVERED_FLAGS_ALLOW_WHEN_DISABLED                = 1 << 10,  // IsItemHovered() only: Return true even if the item is disabled
    IMGUI_HOVERED_FLAGS_NO_NAV_OVERRIDE                    = 1 << 11,  // IsItemHovered() only: Disable using keyboard/gamepad navigation state when active, always query mouse
    IMGUI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED              = IMGUI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED_BY_ITEM | IMGUI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED_BY_WINDOW,
    IMGUI_HOVERED_FLAGS_RECT_ONLY                          = IMGUI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_POPUP | IMGUI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_ACTIVE_ITEM | IMGUI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED,
    IMGUI_HOVERED_FLAGS_ROOT_AND_CHILD_WINDOWS             = IMGUI_HOVERED_FLAGS_ROOT_WINDOW | IMGUI_HOVERED_FLAGS_CHILD_WINDOWS,

    // Tooltips mode
    // - typically used in IsItemHovered() + SetTooltip() sequence.
    // - this is a shortcut to pull flags from 'style.HoverFlagsForTooltipMouse' or 'style.HoverFlagsForTooltipNav' where you can reconfigure desired behavior.
    //   e.g. 'TooltipHoveredFlagsForMouse' defaults to 'ImGuiHoveredFlags_Stationary | ImGuiHoveredFlags_DelayShort'.
    // - for frequently actioned or hovered items providing a tooltip, you want may to use ImGuiHoveredFlags_ForTooltip (stationary + delay) so the tooltip doesn't show too often.
    // - for items which main purpose is to be hovered, or items with low affordance, or in less consistent apps, prefer no delay or shorter delay.
    IMGUI_HOVERED_FLAGS_FOR_TOOLTIP                        = 1 << 12,  // Shortcut for standard flags when using IsItemHovered() + SetTooltip() sequence.

    // (Advanced) Mouse Hovering delays.
    // - generally you can use ImGuiHoveredFlags_ForTooltip to use application-standardized flags.
    // - use those if you need specific overrides.
    IMGUI_HOVERED_FLAGS_STATIONARY                         = 1 << 13,  // Require mouse to be stationary for style.HoverStationaryDelay (~0.15 sec) _at least one time_. After this, can move on same item/window. Using the stationary test tends to reduces the need for a long delay.
    IMGUI_HOVERED_FLAGS_DELAY_NONE                         = 1 << 14,  // IsItemHovered() only: Return true immediately (default). As this is the default you generally ignore this.
    IMGUI_HOVERED_FLAGS_DELAY_SHORT                        = 1 << 15,  // IsItemHovered() only: Return true after style.HoverDelayShort elapsed (~0.15 sec) (shared between items) + requires mouse to be stationary for style.HoverStationaryDelay (once per item).
    IMGUI_HOVERED_FLAGS_DELAY_NORMAL                       = 1 << 16,  // IsItemHovered() only: Return true after style.HoverDelayNormal elapsed (~0.40 sec) (shared between items) + requires mouse to be stationary for style.HoverStationaryDelay (once per item).
    IMGUI_HOVERED_FLAGS_NO_SHARED_DELAY                    = 1 << 17,  // IsItemHovered() only: Disable shared delay system where moving from one item to the next keeps the previous timer for a short time (standard for tooltips with long delays)
};
typedef int VdIMGUIHoveredFlags;

enum {
    IMGUI_MOUSE_BUTTON_LEFT   = 0,
    IMGUI_MOUSE_BUTTON_RIGHT  = 1,
    IMGUI_MOUSE_BUTTON_MIDDLE = 2,
};
typedef int VdIMGUIMouseButton;

enum {
    IMGUI_FOCUSED_FLAGS_NONE                    = 0,
    IMGUI_FOCUSED_FLAGS_CHILD_WINDOWS           = 1 << 0,   // Return true if any children of the window is focused
    IMGUI_FOCUSED_FLAGS_ROOT_WINDOW             = 1 << 1,   // Test from root window (top most parent of the current hierarchy)
    IMGUI_FOCUSED_FLAGS_ANY_WINDOW              = 1 << 2,   // Return true if any window is focused. Important: If you are trying to tell how to dispatch your low-level inputs, do NOT use this. Use 'io.WantCaptureMouse' instead! Please read the FAQ!
    IMGUI_FOCUSED_FLAGS_NO_POPUP_HIERARCHY      = 1 << 3,   // Do not consider popup hierarchy (do not treat popup emitter as parent of popup) (when used with _ChildWindows or _RootWindow)
    //IMGUI_FOCUSED_FLAGS_DOCK_HIERARCHY        = 1 << 4,   // Consider docking hierarchy (treat dockspace host as parent of docked window) (when used with _ChildWindows or _RootWindow)
    IMGUI_FOCUSED_FLAGS_ROOT_AND_CHILD_WINDOWS  = IMGUI_FOCUSED_FLAGS_ROOT_WINDOW | IMGUI_FOCUSED_FLAGS_CHILD_WINDOWS,
};
typedef int VdIMGUIFocusedFlags;

enum {
    IMGUI_COND_NONE           = 0,        // No condition (always set the variable), same as _Always
    IMGUI_COND_ALWAYS         = 1 << 0,   // No condition (always set the variable), same as _None
    IMGUI_COND_ONCE           = 1 << 1,   // Set the variable once per runtime session (only the first call will succeed)
    IMGUI_COND_FIRST_USE_EVER = 1 << 2,   // Set the variable if the object/window has no persistently saved data (no entry in .ini file)
    IMGUI_COND_APPEARING      = 1 << 3,   // Set the variable if the object/window is appearing after being hidden/inactive (or the first time)
};
typedef int VdIMGUICond;

enum {
    // Keyboard
    IMGUI_KEY_NONE = 0,
    IMGUI_KEY_NAMED_KEY_BEGIN = 512,  // First valid key value (other than 0)

    IMGUI_KEY_TAB = 512,             // == ImGuiKey_NamedKey_BEGIN
    IMGUI_KEY_LEFT_ARROW,
    IMGUI_KEY_RIGHT_ARROW,
    IMGUI_KEY_UP_ARROW,
    IMGUI_KEY_DOWN_ARROW,
    IMGUI_KEY_PAGE_UP,
    IMGUI_KEY_PAGE_DOWN,
    IMGUI_KEY_HOME,
    IMGUI_KEY_END,
    IMGUI_KEY_INSERT,
    IMGUI_KEY_DELETE,
    IMGUI_KEY_BACKSPACE,
    IMGUI_KEY_SPACE,
    IMGUI_KEY_ENTER,
    IMGUI_KEY_ESCAPE,
    IMGUI_KEY_LEFT_CTRL,   IMGUI_KEY_LEFT_SHIFT,  IMGUI_KEY_LEFT_ALT,  IMGUI_KEY_LEFT_SUPER,
    IMGUI_KEY_RIGHT_CTRL,  IMGUI_KEY_RIGHT_SHIFT, IMGUI_KEY_RIGHT_ALT, IMGUI_KEY_RIGHT_SUPER,
    IMGUI_KEY_MENU,
    IMGUI_KEY_0,           IMGUI_KEY_1,   IMGUI_KEY_2,   IMGUI_KEY_3,   IMGUI_KEY_4,   IMGUI_KEY_5, IMGUI_KEY_6, IMGUI_KEY_7, IMGUI_KEY_8, IMGUI_KEY_9,
    IMGUI_KEY_A,           IMGUI_KEY_B,   IMGUI_KEY_C,   IMGUI_KEY_D,   IMGUI_KEY_E,   IMGUI_KEY_F, IMGUI_KEY_G, IMGUI_KEY_H, IMGUI_KEY_I, IMGUI_KEY_J,
    IMGUI_KEY_K,           IMGUI_KEY_L,   IMGUI_KEY_M,   IMGUI_KEY_N,   IMGUI_KEY_O,   IMGUI_KEY_P, IMGUI_KEY_Q, IMGUI_KEY_R, IMGUI_KEY_S, IMGUI_KEY_T,
    IMGUI_KEY_U,           IMGUI_KEY_V,   IMGUI_KEY_W,   IMGUI_KEY_X,   IMGUI_KEY_Y,   IMGUI_KEY_Z,
    IMGUI_KEY_F1,          IMGUI_KEY_F2,  IMGUI_KEY_F3,  IMGUI_KEY_F4,  IMGUI_KEY_F5,  IMGUI_KEY_F6,
    IMGUI_KEY_F7,          IMGUI_KEY_F8,  IMGUI_KEY_F9,  IMGUI_KEY_F10, IMGUI_KEY_F11, IMGUI_KEY_F12,
    IMGUI_KEY_F13,         IMGUI_KEY_F14, IMGUI_KEY_F15, IMGUI_KEY_F16, IMGUI_KEY_F17, IMGUI_KEY_F18,
    IMGUI_KEY_F19,         IMGUI_KEY_F20, IMGUI_KEY_F21, IMGUI_KEY_F22, IMGUI_KEY_F23, IMGUI_KEY_F24,
    IMGUI_KEY_APOSTROPHE,        // '
    IMGUI_KEY_COMMA,             // ,
    IMGUI_KEY_MINUS,             // -
    IMGUI_KEY_PERIOD,            // .
    IMGUI_KEY_SLASH,             // /
    IMGUI_KEY_SEMICOLON,         // ;
    IMGUI_KEY_EQUAL,             // =
    IMGUI_KEY_LEFT_BRACKET,      // [
    IMGUI_KEY_BACKSLASH,         // \ (this text inhibit multiline comment caused by backslash)
    IMGUI_KEY_RIGHT_BRACKET,     // ]
    IMGUI_KEY_GRAVE_ACCENT,      // `
    IMGUI_KEY_CAPS_LOCK,
    IMGUI_KEY_SCROLL_LOCK,
    IMGUI_KEY_NUM_LOCK,
    IMGUI_KEY_PRINT_SCREEN,
    IMGUI_KEY_PAUSE,
    IMGUI_KEY_KEYPAD0,     IMGUI_KEY_KEYPAD1, IMGUI_KEY_KEYPAD2, IMGUI_KEY_KEYPAD3, IMGUI_KEY_KEYPAD4,
    IMGUI_KEY_KEYPAD5,     IMGUI_KEY_KEYPAD6, IMGUI_KEY_KEYPAD7, IMGUI_KEY_KEYPAD8, IMGUI_KEY_KEYPAD9,
    IMGUI_KEY_KEYPAD_DECIMAL,
    IMGUI_KEY_KEYPAD_DIVIDE,
    IMGUI_KEY_KEYPAD_MULTIPLY,
    IMGUI_KEY_KEYPAD_SUBTRACT,
    IMGUI_KEY_KEYPAD_ADD,
    IMGUI_KEY_KEYPAD_ENTER,
    IMGUI_KEY_KEYPAD_EQUAL,
    IMGUI_KEY_APP_BACK,               // Available on some keyboard/mouses. Often referred as "Browser Back"
    IMGUI_KEY_APP_FORWARD,
    IMGUI_KEY_OEM102,                // Non-US backslash.

    // Gamepad
    // (analog values are 0.0f to 1.0f)
    // (download controller mapping PNG/PSD at http://dearimgui.com/controls_sheets)
    //                                     // XBOX        | SWITCH  | PLAYSTA. | -> ACTION
    IMGUI_KEY_GAMEPAD_START,          // Menu        | +       | Options  |
    IMGUI_KEY_GAMEPAD_BACK,           // View        | -       | Share    |
    IMGUI_KEY_GAMEPAD_FACE_LEFT,      // X           | Y       | Square   | Tap: Toggle Menu. Hold: Windowing mode (Focus/Move/Resize windows)
    IMGUI_KEY_GAMEPAD_FACE_RIGHT,     // B           | A       | Circle   | Cancel / Close / Exit
    IMGUI_KEY_GAMEPAD_FACE_UP,        // Y           | X       | Triangle | Text Input / On-screen Keyboard
    IMGUI_KEY_GAMEPAD_FACE_DOWN,      // A           | B       | Cross    | Activate / Open / Toggle / Tweak
    IMGUI_KEY_GAMEPAD_DPAD_LEFT,      // D-pad Left  | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    IMGUI_KEY_GAMEPAD_DPAD_RIGHT,     // D-pad Right | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    IMGUI_KEY_GAMEPAD_DPAD_UP,        // D-pad Up    | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    IMGUI_KEY_GAMEPAD_DPAD_DOWN,      // D-pad Down  | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    IMGUI_KEY_GAMEPAD_L1,             // L Bumper    | L       | L1       | Tweak Slower / Focus Previous (in Windowing mode)
    IMGUI_KEY_GAMEPAD_R1,             // R Bumper    | R       | R1       | Tweak Faster / Focus Next (in Windowing mode)
    IMGUI_KEY_GAMEPAD_L2,             // L Trigger   | ZL      | L2       | [Analog]
    IMGUI_KEY_GAMEPAD_R2,             // R Trigger   | ZR      | R2       | [Analog]
    IMGUI_KEY_GAMEPAD_L3,             // L Stick     | L3      | L3       |
    IMGUI_KEY_GAMEPAD_R3,             // R Stick     | R3      | R3       |
    IMGUI_KEY_GAMEPAD_L_STICK_LEFT,   //             |         |          | [Analog] Move Window (in Windowing mode)
    IMGUI_KEY_GAMEPAD_L_STICK_RIGHT,  //             |         |          | [Analog] Move Window (in Windowing mode)
    IMGUI_KEY_GAMEPAD_L_STICK_UP,     //             |         |          | [Analog] Move Window (in Windowing mode)
    IMGUI_KEY_GAMEPAD_L_STICK_DOWN,   //             |         |          | [Analog] Move Window (in Windowing mode)
    IMGUI_KEY_GAMEPAD_R_STICK_LEFT,   //             |         |          | [Analog]
    IMGUI_KEY_GAMEPAD_R_STICK_RIGHT,  //             |         |          | [Analog]
    IMGUI_KEY_GAMEPAD_R_STICK_UP,     //             |         |          | [Analog]
    IMGUI_KEY_GAMEPAD_R_STICK_DOWN,   //             |         |          | [Analog]

    // Aliases: Mouse Buttons (auto-submitted from AddMouseButtonEvent() calls)
    // - This is mirroring the data also written to io.MouseDown[], io.MouseWheel, in a format allowing them to be accessed via standard key API.
    IMGUI_KEY_MOUSE_LEFT, IMGUI_KEY_MOUSE_RIGHT, IMGUI_KEY_MOUSE_MIDDLE, IMGUI_KEY_MOUSE_X1, IMGUI_KEY_MOUSE_X2, IMGUI_KEY_MOUSE_WHEEL_X, IMGUI_KEY_MOUSE_WHEEL_Y,

    // Keyboard Modifiers (explicitly submitted by backend via AddKeyEvent() calls)
    // - Any functions taking a ImGuiKeyChord parameter can binary-or those with regular keys, e.g. Shortcut(ImGuiMod_Ctrl | ImGuiKey_S).
    // - Those are written back into io.KeyCtrl, io.KeyShift, io.KeyAlt, io.KeySuper for convenience,
    //   but may be accessed via standard key API such as IsKeyPressed(), IsKeyReleased(), querying duration etc.
    // - Code polling every key (e.g. an interface to detect a key press for input mapping) might want to ignore those
    //   and prefer using the real keys (e.g. ImGuiKey_LeftCtrl, ImGuiKey_RightCtrl instead of ImGuiMod_Ctrl).
    // - In theory the value of keyboard modifiers should be roughly equivalent to a logical or of the equivalent left/right keys.
    //   In practice: it's complicated; mods are often provided from different sources. Keyboard layout, IME, sticky keys and
    //   backends tend to interfere and break that equivalence. The safer decision is to relay that ambiguity down to the end-user...
    // - On macOS, we swap Cmd(Super) and Ctrl keys at the time of the io.AddKeyEvent() call.
    IMGUI_MOD_NONE                   = 0,
    IMGUI_MOD_CTRL                   = 1 << 12, // Ctrl (non-macOS), Cmd (macOS)
    IMGUI_MOD_SHIFT                  = 1 << 13, // Shift
    IMGUI_MOD_ALT                    = 1 << 14, // Option/Menu
    IMGUI_MOD_SUPER                  = 1 << 15, // Windows/Super (non-macOS), Ctrl (macOS)
    IMGUI_MOD_MASK_                  = 0xF000,  // 4-bits
};
typedef int VdIMGUIKey;

enum {
    IMGUI_MOUSE_SOURCE_MOUSE = 0,         // Input is coming from an actual mouse.
    IMGUI_MOUSE_SOURCE_TOUCH_SCREEN,      // Input is coming from a touch screen (no hovering prior to initial press, less precise initial press aiming, dual-axis wheeling possible).
    IMGUI_MOUSE_SOURCE_PEN,               // Input is coming from a pressure/magnetic pen (often used in conjunction with high-sampling rates).
};
typedef int VdIMGUIMouseSource;

enum {
    IMGUI_BUTTON_FLAGS_NONE                   = 0,
    IMGUI_BUTTON_FLAGS_MOUSE_BUTTON_LEFT      = 1 << 0,   // React on left mouse button (default)
    IMGUI_BUTTON_FLAGS_MOUSE_BUTTON_RIGHT     = 1 << 1,   // React on right mouse button
    IMGUI_BUTTON_FLAGS_MOUSE_BUTTON_MIDDLE    = 1 << 2,   // React on center mouse button
    IMGUI_BUTTON_FLAGS_ENABLE_NAV             = 1 << 3,   // InvisibleButton(): do not disable navigation/tabbing. Otherwise disabled by default.
};
typedef int VdIMGUIButtonFlags;

enum {
    IMGUI_DIR_NONE    = -1,
    IMGUI_DIR_LEFT    = 0,
    IMGUI_DIR_RIGHT   = 1,
    IMGUI_DIR_UP      = 2,
    IMGUI_DIR_DOWN    = 3,
};
typedef int VdIMGUIDir;

/* ----WINDOWS------------------------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_BEGIN(PNAME)                                  Vdb32            PNAME(const char *name, Vdb32 *p_open /* = NULL */, VdIMGUIWindowFlags flags /* = 0 */)
#define VD_PROC_IMGUI_END(PNAME)                                    void             PNAME(void)

/* ----CHILD WINDOWS------------------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_BEGIN_CHILD(PNAME)                            Vdb32            PNAME(const char *str_id, Vdf32 size_x /* = 0.f */, Vdf32 size_y /* = 0.f */, VdIMGUIChildFlags child_flags /* = 0 */, VdIMGUIWindowFlags window_flags /* = 0 */)
#define VD_PROC_IMGUI_BEGIN_CHILD_ID(PNAME)                         Vdb32            PNAME(VdIMGUIID id,     Vdf32 size_x /* = 0.f */, Vdf32 size_y /* = 0.f */, VdIMGUIChildFlags child_flags /* = 0 */, VdIMGUIWindowFlags window_flags /* = 0 */)
#define VD_PROC_IMGUI_END_CHILD(PNAME)                              void             PNAME(void)

/* ----WINDOWS UTILITIES--------------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_IS_WINDOW_APPEARING(PNAME)                    Vdb32            PNAME(void)
#define VD_PROC_IMGUI_IS_WINDOW_COLLAPSED(PNAME)                    Vdb32            PNAME(void)
#define VD_PROC_IMGUI_IS_WINDOW_FOCUSED(PNAME)                      Vdb32            PNAME(VdIMGUIFocusedFlags flags /* = 0 */)
#define VD_PROC_IMGUI_IS_WINDOW_HOVERED(PNAME)                      Vdb32            PNAME(VdIMGUIHoveredFlags flags /* = 0 */)
#define VD_PROC_IMGUI_GET_WINDOW_DRAW_LIST(PNAME)                   VdIMGUIDrawList  PNAME(void)
#define VD_PROC_IMGUI_GET_WINDOW_POS(PNAME)                         void             PNAME(Vdf32 *x, Vdf32 *y)
#define VD_PROC_IMGUI_GET_WINDOW_SIZE(PNAME)                        void             PNAME(Vdf32 *x, Vdf32 *y)

/* ----WINDOW MANIPULATION------------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_POS(PNAME)                    void             PNAME(Vdf32 pos_x,      Vdf32 pos_y,      VdIMGUICond cond /* = 0 */, Vdf32 pivot_x /* = 0.f */, Vdf32 pivot_y /* = 0.f */)
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_SIZE(PNAME)                   void             PNAME(Vdf32 size_x,     Vdf32 size_y,     VdIMGUICond cond /* = 0 */)
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_SIZE_CONSTRAINTS(PNAME)       void             PNAME(Vdf32 size_min_x, Vdf32 size_min_y, Vdf32 size_max_x,           Vdf32 size_max_y) // @todo(mdodis): custom callback
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_CONTENT_SIZE(PNAME)           void             PNAME(Vdf32 size_x,     Vdf32 size_y)
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_COLLAPSED(PNAME)              void             PNAME(Vdb32 collapsed,  VdIMGUICond cond /* = 0 */)
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_FOCUS(PNAME)                  void             PNAME(void)
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_SCROLL(PNAME)                 void             PNAME(Vdf32 scroll_x,   Vdf32 scroll_y)
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_BG_ALPHA(PNAME)               void             PNAME(Vdf32 alpha)
// @todo(mdodis):
// - IMGUI_API void          SetWindowPos(const ImVec2& pos, ImGuiCond cond = 0);                        // (not recommended) set current window position - call within Begin()/End(). prefer using SetNextWindowPos(), as this may incur tearing and side-effects.
// - IMGUI_API void          SetWindowSize(const ImVec2& size, ImGuiCond cond = 0);                      // (not recommended) set current window size - call within Begin()/End(). set to ImVec2(0, 0) to force an auto-fit. prefer using SetNextWindowSize(), as this may incur tearing and minor side-effects.
// - IMGUI_API void          SetWindowCollapsed(bool collapsed, ImGuiCond cond = 0);                     // (not recommended) set current window collapsed state. prefer using SetNextWindowCollapsed().
// - IMGUI_API void          SetWindowFocus();                                                           // (not recommended) set current window to be focused / top-most. prefer using SetNextWindowFocus().
// - IMGUI_API void          SetWindowPos(const char* name, const ImVec2& pos, ImGuiCond cond = 0);      // set named window position.
// - IMGUI_API void          SetWindowSize(const char* name, const ImVec2& size, ImGuiCond cond = 0);    // set named window size. set axis to 0.0f to force an auto-fit on this axis.
// - IMGUI_API void          SetWindowCollapsed(const char* name, bool collapsed, ImGuiCond cond = 0);   // set named window collapsed state
// - IMGUI_API void          SetWindowFocus(const char* name);                                           // set named window to be focused / top-most. use NULL to remove focus.

/* ----WINDOW SCROLLING---------------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_GET_SCROLL(PNAME)                             void             PNAME(Vdf32 *x, Vdf32 *y)
#define VD_PROC_IMGUI_SET_SCROLL(PNAME)                             void             PNAME(Vdf32 *x, Vdf32 *y)
#define VD_PROC_IMGUI_GET_SCROLL_MAX(PNAME)                         void             PNAME(Vdf32 *x, Vdf32 *y)
#define VD_PROC_IMGUI_SET_SCROLL_HERE(PNAME)                        void             PNAME(Vdf32 *x, Vdf32 center_x_ratio /* = 0.5f */, Vdf32 *y, Vdf32 center_y_ratio /* = 0.5f */)
#define VD_PROC_IMGUI_SET_SCROLL_FROM_POS(PNAME)                    void             PNAME(Vdf32 *x, Vdf32 center_x_ratio /* = 0.5f */, Vdf32 *y, Vdf32 center_y_ratio /* = 0.5f */)

/* ----PARAMETERS STACKS (FONT)-------------------------------------------------------------------------------------- */
// @todo(mdodis): Parameters stacks (font)
// - IMGUI_API void          PushFont(ImFont* font, float font_size_base_unscaled);          // Use NULL as a shortcut to keep current font. Use 0.0f to keep current size.
// - IMGUI_API void          PopFont();
// - IMGUI_API ImFont*       GetFont();                                                      // get current font
// - IMGUI_API float         GetFontSize();                                                  // get current scaled font size (= height in pixels). AFTER global scale factors applied. *IMPORTANT* DO NOT PASS THIS VALUE TO PushFont()! Use ImGui::GetStyle().FontSizeBase to get value before global scale factors.
// - IMGUI_API ImFontBaked*  GetFontBaked();                                                 // get current font bound at current size // == GetFont()->GetFontBaked(GetFontSize())

/* ----PARAMETERS STACKS (SHARED)------------------------------------------------------------------------------------ */
#define VD_PROC_IMGUI_PUSH_STYLE_COLORU32(PNAME)                    void             PNAME(VdIMGUICol idx,      Vdu32 col)
#define VD_PROC_IMGUI_PUSH_STYLE_COLORRGBA(PNAME)                   void             PNAME(VdIMGUICol idx,      Vdf32 r,     Vdf32 g, Vdf32 b, Vdf32 a)
#define VD_PROC_IMGUI_POP_STYLE_COLOR(PNAME)                        void             PNAME(Vdi32 count /* = 1 */)
#define VD_PROC_IMGUI_PUSH_STYLE_VARF(PNAME)                        void             PNAME(VdIMGUIStyleVar idx, Vdf32 val)
#define VD_PROC_IMGUI_PUSH_STYLE_VAR2F(PNAME)                       void             PNAME(VdIMGUIStyleVar idx, Vdf32 val_x, Vdf32 val_y)
#define VD_PROC_IMGUI_POP_STYLE_VAR(PNAME)                          void             PNAME(Vdi32 count /* = 1 */)
// @todo(mdodis): Rest
// - IMGUI_API void          PushItemFlag(ImGuiItemFlags option, bool enabled);              // modify specified shared item flag, e.g. PushItemFlag(ImGuiItemFlags_NoTabStop, true)
// - IMGUI_API void          PopItemFlag();

/* ----PARAMETERS STACKS (CURRENT WINDOW)---------------------------------------------------------------------------- */
// @todo(mdodis): Parameters stacks (current window)
// - IMGUI_API void          PushItemWidth(float item_width);                                // push width of items for common large "item+label" widgets. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -FLT_MIN always align width to the right side).
// - IMGUI_API void          PopItemWidth();
// - IMGUI_API void          SetNextItemWidth(float item_width);                             // set width of the _next_ common large "item+label" widget. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -FLT_MIN always align width to the right side)
// - IMGUI_API float         CalcItemWidth();                                                // width of item given pushed settings and current cursor position. NOT necessarily the width of last item unlike most 'Item' functions.
// - IMGUI_API void          PushTextWrapPos(float wrap_local_pos_x = 0.0f);                 // push word-wrapping position for Text*() commands. < 0.0f: no wrapping; 0.0f: wrap to end of window (or column); > 0.0f: wrap at 'wrap_pos_x' position in window local space
// - IMGUI_API void          PopTextWrapPos();

/* ----STYLE READ ACCESS--------------------------------------------------------------------------------------------- */
// Style read access
// - IMGUI_API ImVec2        GetFontTexUvWhitePixel();                                       // get UV coordinate for a white pixel, useful to draw custom shapes via the ImDrawList API
// - IMGUI_API ImU32         GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f);              // retrieve given style color with style alpha applied and optional extra alpha multiplier, packed as a 32-bit value suitable for ImDrawList
// - IMGUI_API ImU32         GetColorU32(const ImVec4& col);                                 // retrieve given color with style alpha applied, packed as a 32-bit value suitable for ImDrawList
// - IMGUI_API ImU32         GetColorU32(ImU32 col, float alpha_mul = 1.0f);                 // retrieve given color with style alpha applied, packed as a 32-bit value suitable for ImDrawList
// - IMGUI_API const ImVec4& GetStyleColorVec4(ImGuiCol idx);                                // retrieve style color as stored in ImGuiStyle structure. use to feed back into PushStyleColor(), otherwise use GetColorU32() to get style color with style alpha baked in.

/* ----LAYOUT CURSOR POSITIONING------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_GET_CURSOR_SCREEN_POS(PNAME)                  void             PNAME(Vdf32 *x, Vdf32 *y)
#define VD_PROC_IMGUI_SET_CURSOR_SCREEN_POS(PNAME)                  void             PNAME(Vdf32 x,  Vdf32 y)
#define VD_PROC_IMGUI_GET_CONTENT_REGION_AVAIL(PNAME)               void             PNAME(Vdf32 *x, Vdf32 *y)
#define VD_PROC_IMGUI_GET_CURSOR_POS(PNAME)                         void             PNAME(Vdf32 *x, Vdf32 *y)
#define VD_PROC_IMGUI_SET_CURSOR_POS(PNAME)                         void             PNAME(Vdf32 x,  Vdf32 y)
#define VD_PROC_IMGUI_GET_CURSOR_START_POS(PNAME)                   void             PNAME(Vdf32 *x, Vdf32 *y)

/* ----OTHER LAYOUT FUNCTIONS---------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_SEPARATOR(PNAME)                              void             PNAME(void)
#define VD_PROC_IMGUI_SAME_LINE(PNAME)                              void             PNAME(Vdf32 offset_from_start_x /* = 0.f */, Vdf32 spacing /* = -1.f */)
#define VD_PROC_IMGUI_NEW_LINE(PNAME)                               void             PNAME(void)
#define VD_PROC_IMGUI_SPACING(PNAME)                                void             PNAME(void)
#define VD_PROC_IMGUI_DUMMY(PNAME)                                  void             PNAME(Vdf32 size_x,                          Vdf32 size_y)
#define VD_PROC_IMGUI_INDENT(PNAME)                                 void             PNAME(Vdf32 indent_w /* = 0.f */)
#define VD_PROC_IMGUI_UNINDENT(PNAME)                               void             PNAME(Vdf32 indent_w /* = 0.f */)
#define VD_PROC_IMGUI_BEGIN_GROUP(PNAME)                            void             PNAME(void)
#define VD_PROC_IMGUI_END_GROUP(PNAME)                              void             PNAME(void)
#define VD_PROC_IMGUI_ALIGN_TEXT_TO_FRAME_PADDING(PNAME)            void             PNAME(void)
#define VD_PROC_IMGUI_GET_TEXT_LINE_HEIGHT(PNAME)                   Vdf32            PNAME(void)
#define VD_PROC_IMGUI_GET_TEXT_LINE_HEIGHT_WITH_SPACING(PNAME)      Vdf32            PNAME(void)
#define VD_PROC_IMGUI_GET_FRAME_HEIGHT(PNAME)                       Vdf32            PNAME(void)
#define VD_PROC_IMGUI_GET_FRAME_HEIGHT_WITH_SPACING(PNAME)          Vdf32            PNAME(void)

/* ----ID STACK/SCOPES----------------------------------------------------------------------------------------------- */
// @todo(mdodis): ID stack/scopes
// - IMGUI_API void          PushID(const char* str_id);                                     // push string into the ID stack (will hash string).
// - IMGUI_API void          PushID(const char* str_id_begin, const char* str_id_end);       // push string into the ID stack (will hash string).
// - IMGUI_API void          PushID(const void* ptr_id);                                     // push pointer into the ID stack (will hash pointer).
// - IMGUI_API void          PushID(int int_id);                                             // push integer into the ID stack (will hash integer).
// - IMGUI_API void          PopID();                                                        // pop from the ID stack.
// - IMGUI_API ImGuiID       GetID(const char* str_id);                                      // calculate unique ID (hash of whole ID stack + given parameter). e.g. if you want to query into ImGuiStorage yourself
// - IMGUI_API ImGuiID       GetID(const char* str_id_begin, const char* str_id_end);
// - IMGUI_API ImGuiID       GetID(const void* ptr_id);
// - IMGUI_API ImGuiID       GetID(int int_id);

/* ----WIDGETS: TEXT------------------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_TEXT_UNFORMATTED(PNAME)                       void             PNAME(const char *text, const char *text_end /*= NULL */)
#define VD_PROC_IMGUI_TEXTV(PNAME)                                  void             PNAME(const char *fmt, va_list args)
#define VD_PROC_IMGUI_TEXT_COLOREDV(PNAME)                          void             PNAME(Vdf32 r, Vdf32 g, Vdf32 b, Vdf32 a, const char *fmt, va_list args)
#define VD_PROC_IMGUI_TEXT_DISABLEDV(PNAME)                         void             PNAME(const char *fmt, va_list args)
#define VD_PROC_IMGUI_TEXT_WRAPPEDV(PNAME)                          void             PNAME(const char *fmt, va_list args)
#define VD_PROC_IMGUI_LABEL_TEXTV(PNAME)                            void             PNAME(const char *label, const char *fmt, va_list args)
#define VD_PROC_IMGUI_BULLET_TEXTV(PNAME)                           void             PNAME(const char *label, const char *fmt, va_list args)
#define VD_PROC_IMGUI_SEPARATOR_TEXT(PNAME)                         void             PNAME(const char *label)

/* ----WIDGETS: MAIN------------------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_BUTTON(PNAME)                                 Vdb32            PNAME(const char *label,  Vdf32 size_x /* = 0.f */, Vdf32 size_y /* = 0.f */)
#define VD_PROC_IMGUI_SMALL_BUTTON(PNAME)                           Vdb32            PNAME(const char *label)
#define VD_PROC_IMGUI_INVISIBLE_BUTTON(PNAME)                       Vdb32            PNAME(const char *str_id, Vdf32 size_x,             Vdf32 size_y,      VdIMGUIButtonFlags flags /* = 0 */)
#define VD_PROC_IMGUI_ARROW_BUTTON(PNAME)                           Vdb32            PNAME(const char *str_id, VdIMGUIDir dir)
#define VD_PROC_IMGUI_CHECKBOX(PNAME)                               Vdb32            PNAME(const char *label,  Vdb32 *v)
#define VD_PROC_IMGUI_CHECKBOX_FLAGSI(PNAME)                        Vdb32            PNAME(const char *label,  Vdi32 *flags,             Vdi32 flags_value)
#define VD_PROC_IMGUI_CHECKBOX_FLAGSU(PNAME)                        Vdb32            PNAME(const char *label,  Vdu32 *flags,             Vdu32 flags_value)
#define VD_PROC_IMGUI_RADIO_BUTTON(PNAME)                           Vdb32            PNAME(const char *label,  Vdb32 active)
#define VD_PROC_IMGUI_RADIO_BUTTONI(PNAME)                          Vdb32            PNAME(const char *label,  Vdi32 *v,                 Vdi32 v_button)
#define VD_PROC_IMGUI_PROGRESS_BAR(PNAME)                           void             PNAME(Vdf32 fraction)
#define VD_PROC_IMGUI_BULLET(PNAME)                                 void             PNAME(void)
#define VD_PROC_IMGUI_TEXT_LINK(PNAME)                              Vdb32            PNAME(const char *label)

// @todo(mdodis): Widgets: Images
// - IMGUI_API void          Image(ImTextureRef tex_ref, const ImVec2& image_size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1));
// - IMGUI_API void          ImageWithBg(ImTextureRef tex_ref, const ImVec2& image_size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
// - IMGUI_API bool          ImageButton(const char* str_id, ImTextureRef tex_ref, const ImVec2& image_size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));

// @todo(mdodis): Widgets: Combo Box (Dropdown)
// - IMGUI_API bool          BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
// - IMGUI_API void          EndCombo(); // only call EndCombo() if BeginCombo() returns true!
// - IMGUI_API bool          Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
// - IMGUI_API bool          Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);      // Separate items with \0 within a string, end item-list with \0\0. e.g. "One\0Two\0Three\0"
// - IMGUI_API bool          Combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items = -1);

// @todo(mdodis): Widgets: Drag Sliders
// - IMGUI_API bool          DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);     // If v_min >= v_max we have no bound
// - IMGUI_API bool          DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", const char* format_max = NULL, ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          DragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);  // If v_min >= v_max we have no bound
// - IMGUI_API bool          DragInt2(const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          DragInt3(const char* label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          DragInt4(const char* label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", const char* format_max = NULL, ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          DragScalar(const char* label, ImGuiDataType data_type, void* p_data, float v_speed = 1.0f, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          DragScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, float v_speed = 1.0f, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, ImGuiSliderFlags flags = 0);

// @todo(mdodis): Widgets: Regular Sliders
// - IMGUI_API bool          SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);     // adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
// - IMGUI_API bool          SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          SliderAngle(const char* label, float* v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, const char* format = "%.0f deg", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          SliderScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          VSliderInt(const char* label, const ImVec2& size, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
// - IMGUI_API bool          VSliderScalar(const char* label, const ImVec2& size, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);

// @todo(mdodis): Widgets: Input with Keyboard
// - IMGUI_API bool          InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
// - IMGUI_API bool          InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
// - IMGUI_API bool          InputTextWithHint(const char* label, const char* hint, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
// - IMGUI_API bool          InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
// - IMGUI_API bool          InputFloat2(const char* label, float v[2], const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
// - IMGUI_API bool          InputFloat3(const char* label, float v[3], const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
// - IMGUI_API bool          InputFloat4(const char* label, float v[4], const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
// - IMGUI_API bool          InputInt(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0);
// - IMGUI_API bool          InputInt2(const char* label, int v[2], ImGuiInputTextFlags flags = 0);
// - IMGUI_API bool          InputInt3(const char* label, int v[3], ImGuiInputTextFlags flags = 0);
// - IMGUI_API bool          InputInt4(const char* label, int v[4], ImGuiInputTextFlags flags = 0);
// - IMGUI_API bool          InputDouble(const char* label, double* v, double step = 0.0, double step_fast = 0.0, const char* format = "%.6f", ImGuiInputTextFlags flags = 0);
// - IMGUI_API bool          InputScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, ImGuiInputTextFlags flags = 0);
// - IMGUI_API bool          InputScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, ImGuiInputTextFlags flags = 0);

// @todo(mdodis): Widgets: Color Editor/Picker (tip: the ColorEdit* functions have a little color square that can be left-clicked to open a picker, and right-clicked to open an option menu.)
// - IMGUI_API bool          ColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
// - IMGUI_API bool          ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
// - IMGUI_API bool          ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
// - IMGUI_API bool          ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = NULL);
// - IMGUI_API bool          ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, const ImVec2& size = ImVec2(0, 0)); // display a color square/button, hover for details, return true when pressed.
// - IMGUI_API void          SetColorEditOptions(ImGuiColorEditFlags flags);                     // initialize current options (generally on application startup) if you want to select a default format, picker type, etc. User will be able to change many settings, unless you pass the _NoOptions flag to your calls.

// @todo(mdodis): Widgets: Trees
// - IMGUI_API bool          TreeNode(const char* label);
// - IMGUI_API bool          TreeNode(const char* str_id, const char* fmt, ...) IM_FMTARGS(2);   // helper variation to easily decorelate the id from the displayed string. Read the FAQ about why and how to use ID. to align arbitrary text at the same level as a TreeNode() you can use Bullet().
// - IMGUI_API bool          TreeNode(const void* ptr_id, const char* fmt, ...) IM_FMTARGS(2);   // "
// - IMGUI_API bool          TreeNodeV(const char* str_id, const char* fmt, va_list args) IM_FMTLIST(2);
// - IMGUI_API bool          TreeNodeV(const void* ptr_id, const char* fmt, va_list args) IM_FMTLIST(2);
// - IMGUI_API bool          TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0);
// - IMGUI_API bool          TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
// - IMGUI_API bool          TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
// - IMGUI_API bool          TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);
// - IMGUI_API bool          TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);
// - IMGUI_API void          TreePush(const char* str_id);                                       // ~ Indent()+PushID(). Already called by TreeNode() when returning true, but you can call TreePush/TreePop yourself if desired.
// - IMGUI_API void          TreePush(const void* ptr_id);                                       // "
// - IMGUI_API void          TreePop();                                                          // ~ Unindent()+PopID()
// - IMGUI_API float         GetTreeNodeToLabelSpacing();                                        // horizontal distance preceding label when using TreeNode*() or Bullet() == (g.FontSize + style.FramePadding.x*2) for a regular unframed TreeNode
// - IMGUI_API bool          CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0);  // if returning 'true' the header is open. doesn't indent nor push on ID stack. user doesn't have to call TreePop().
// - IMGUI_API bool          CollapsingHeader(const char* label, bool* p_visible, ImGuiTreeNodeFlags flags = 0); // when 'p_visible != NULL': if '*p_visible==true' display an additional small close button on upper right of the header which will set the bool to false when clicked, if '*p_visible==false' don't display the header.
// - IMGUI_API void          SetNextItemOpen(bool is_open, ImGuiCond cond = 0);                  // set next TreeNode/CollapsingHeader open state.
// - IMGUI_API void          SetNextItemStorageID(ImGuiID storage_id);                           // set id to use for open/close storage (default to same as item id).

// @todo(mdodis): Widgets: Selectables
// - IMGUI_API bool          Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0)); // "bool selected" carry the selection state (read-only). Selectable() is clicked is returns true so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
// - IMGUI_API bool          Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));      // "bool* p_selected" point to the selection state (read-write), as a convenient helper.

// @todo(mdodis): Multi-selection system for Selectable(), Checkbox(), TreeNode() functions [BETA]
// - IMGUI_API ImGuiMultiSelectIO*   BeginMultiSelect(ImGuiMultiSelectFlags flags, int selection_size = -1, int items_count = -1);
// - IMGUI_API ImGuiMultiSelectIO*   EndMultiSelect();
// - IMGUI_API void                  SetNextItemSelectionUserData(ImGuiSelectionUserData selection_user_data);
// - IMGUI_API bool                  IsItemToggledSelection();                                   // Was the last item selection state toggled? Useful if you need the per-item information _before_ reaching EndMultiSelect(). We only returns toggle _event_ in order to handle clipping correctly.

// @todo(mdodis): Widgets: List Boxes
// - IMGUI_API bool          BeginListBox(const char* label, const ImVec2& size = ImVec2(0, 0)); // open a framed scrolling region
// - IMGUI_API void          EndListBox();                                                       // only call EndListBox() if BeginListBox() returned true!
// - IMGUI_API bool          ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1);
// - IMGUI_API bool          ListBox(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int height_in_items = -1);

// @todo(mdodis): Widgets: Data Plotting
// - Consider using ImPlot (https://github.com/epezent/implot) which is much better!
// - IMGUI_API void          PlotLines(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
// - IMGUI_API void          PlotLines(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));
// - IMGUI_API void          PlotHistogram(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
// - IMGUI_API void          PlotHistogram(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));

// @todo(mdodis): Widgets: Value() Helpers.
// - IMGUI_API void          Value(const char* prefix, bool b);
// - IMGUI_API void          Value(const char* prefix, int v);
// - IMGUI_API void          Value(const char* prefix, unsigned int v);
// - IMGUI_API void          Value(const char* prefix, float v, const char* float_format = NULL);

// @todo(mdodis): Widgets: Menus
// - IMGUI_API bool          BeginMenuBar();                                                     // append to menu-bar of current window (requires ImGuiWindowFlags_MenuBar flag set on parent window).
// - IMGUI_API void          EndMenuBar();                                                       // only call EndMenuBar() if BeginMenuBar() returns true!
// - IMGUI_API bool          BeginMainMenuBar();                                                 // create and append to a full screen menu-bar.
// - IMGUI_API void          EndMainMenuBar();                                                   // only call EndMainMenuBar() if BeginMainMenuBar() returns true!
// - IMGUI_API bool          BeginMenu(const char* label, bool enabled = true);                  // create a sub-menu entry. only call EndMenu() if this returns true!
// - IMGUI_API void          EndMenu();                                                          // only call EndMenu() if BeginMenu() returns true!
// - IMGUI_API bool          MenuItem(const char* label, const char* shortcut = NULL, bool selected = false, bool enabled = true);  // return true when activated.
// - IMGUI_API bool          MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true);              // return true when activated + toggle (*p_selected) if p_selected != NULL

// @todo(mdodis): Tooltips
// - IMGUI_API bool          BeginTooltip();                                                     // begin/append a tooltip window.
// - IMGUI_API void          EndTooltip();                                                       // only call EndTooltip() if BeginTooltip()/BeginItemTooltip() returns true!
// - IMGUI_API void          SetTooltip(const char* fmt, ...) IM_FMTARGS(1);                     // set a text-only tooltip. Often used after a ImGui::IsItemHovered() check. Override any previous call to SetTooltip().
// - IMGUI_API void          SetTooltipV(const char* fmt, va_list args) IM_FMTLIST(1);

// @todo(mdodis): Tooltips: helpers for showing a tooltip when hovering an item
// - IMGUI_API bool          BeginItemTooltip();                                                 // begin/append a tooltip window if preceding item was hovered.
// - IMGUI_API void          SetItemTooltip(const char* fmt, ...) IM_FMTARGS(1);                 // set a text-only tooltip if preceding item was hovered. override any previous call to SetTooltip().
// - IMGUI_API void          SetItemTooltipV(const char* fmt, va_list args) IM_FMTLIST(1);

// @todo(mdodis): Popups, Modals
// - IMGUI_API bool          BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0);                         // return true if the popup is open, and you can start outputting to it.
// - IMGUI_API bool          BeginPopupModal(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0); // return true if the modal is open, and you can start outputting to it.
// - IMGUI_API void          EndPopup();                                                                         // only call EndPopup() if BeginPopupXXX() returns true!

// @todo(mdodis): Popups: open/close functions
// - IMGUI_API void          OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags = 0);                     // call to mark popup as open (don't call every frame!).
// - IMGUI_API void          OpenPopup(ImGuiID id, ImGuiPopupFlags popup_flags = 0);                             // id overload to facilitate calling from nested stacks
// - IMGUI_API void          OpenPopupOnItemClick(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);   // helper to open popup when clicked on last item. Default to ImGuiPopupFlags_MouseButtonRight == 1. (note: actually triggers on the mouse _released_ event to be consistent with popup behaviors)
// - IMGUI_API void          CloseCurrentPopup();                                                                // manually close the popup we have begin-ed into.

// @todo(mdodis): Popups: open+begin combined functions helpers
// - IMGUI_API bool          BeginPopupContextItem(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);  // open+begin popup when clicked on last item. Use str_id==NULL to associate the popup to previous item. If you want to use that on a non-interactive item such as Text() you need to pass in an explicit ID here. read comments in .cpp!
// - IMGUI_API bool          BeginPopupContextWindow(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);// open+begin popup when clicked on current window.
// - IMGUI_API bool          BeginPopupContextVoid(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);  // open+begin popup when clicked in void (where there are no windows).

// @todo(mdodis): Popups: query functions
// - IMGUI_API bool          IsPopupOpen(const char* str_id, ImGuiPopupFlags flags = 0);                         // return true if the popup is open.

// @todo(mdodis): Tables
// - IMGUI_API bool          BeginTable(const char* str_id, int columns, ImGuiTableFlags flags = 0, const ImVec2& outer_size = ImVec2(0.0f, 0.0f), float inner_width = 0.0f);
// - IMGUI_API void          EndTable();                                         // only call EndTable() if BeginTable() returns true!
// - IMGUI_API void          TableNextRow(ImGuiTableRowFlags row_flags = 0, float min_row_height = 0.0f); // append into the first cell of a new row.
// - IMGUI_API bool          TableNextColumn();                                  // append into the next column (or first column of next row if currently in last column). Return true when column is visible.
// - IMGUI_API bool          TableSetColumnIndex(int column_n);                  // append into the specified column. Return true when column is visible.

// @todo(mdodis): Tables: Headers & Columns declaration
// - IMGUI_API void          TableSetupColumn(const char* label, ImGuiTableColumnFlags flags = 0, float init_width_or_weight = 0.0f, ImGuiID user_id = 0);
// - IMGUI_API void          TableSetupScrollFreeze(int cols, int rows);         // lock columns/rows so they stay visible when scrolled.
// - IMGUI_API void          TableHeader(const char* label);                     // submit one header cell manually (rarely used)
// - IMGUI_API void          TableHeadersRow();                                  // submit a row with headers cells based on data provided to TableSetupColumn() + submit context menu
// - IMGUI_API void          TableAngledHeadersRow();                            // submit a row with angled headers for every column with the ImGuiTableColumnFlags_AngledHeader flag. MUST BE FIRST ROW.

// @todo(mdodis): Tables: Sorting & Miscellaneous functions
// - IMGUI_API ImGuiTableSortSpecs*  TableGetSortSpecs();                        // get latest sort specs for the table (NULL if not sorting).  Lifetime: don't hold on this pointer over multiple frames or past any subsequent call to BeginTable().
// - IMGUI_API int                   TableGetColumnCount();                      // return number of columns (value passed to BeginTable)
// - IMGUI_API int                   TableGetColumnIndex();                      // return current column index.
// - IMGUI_API int                   TableGetRowIndex();                         // return current row index (header rows are accounted for)
// - IMGUI_API const char*           TableGetColumnName(int column_n = -1);      // return "" if column didn't have a name declared by TableSetupColumn(). Pass -1 to use current column.
// - IMGUI_API ImGuiTableColumnFlags TableGetColumnFlags(int column_n = -1);     // return column flags so you can query their Enabled/Visible/Sorted/Hovered status flags. Pass -1 to use current column.
// - IMGUI_API void                  TableSetColumnEnabled(int column_n, bool v);// change user accessible enabled/disabled state of a column. Set to false to hide the column. User can use the context menu to change this themselves (right-click in headers, or right-click in columns body with ImGuiTableFlags_ContextMenuInBody)
// - IMGUI_API int                   TableGetHoveredColumn();                    // return hovered column. return -1 when table is not hovered. return columns_count if the unused space at the right of visible columns is hovered. Can also use (TableGetColumnFlags() & ImGuiTableColumnFlags_IsHovered) instead.
// - IMGUI_API void                  TableSetBgColor(ImGuiTableBgTarget target, ImU32 color, int column_n = -1);  // change the color of a cell, row, or column. See ImGuiTableBgTarget_ flags for details.

// @todo(mdodis): Tab Bars, Tabs
// - IMGUI_API bool          BeginTabBar(const char* str_id, ImGuiTabBarFlags flags = 0);        // create and append into a TabBar
// - IMGUI_API void          EndTabBar();                                                        // only call EndTabBar() if BeginTabBar() returns true!
// - IMGUI_API bool          BeginTabItem(const char* label, bool* p_open = NULL, ImGuiTabItemFlags flags = 0); // create a Tab. Returns true if the Tab is selected.
// - IMGUI_API void          EndTabItem();                                                       // only call EndTabItem() if BeginTabItem() returns true!
// - IMGUI_API bool          TabItemButton(const char* label, ImGuiTabItemFlags flags = 0);      // create a Tab behaving like a button. return true when clicked. cannot be selected in the tab bar.
// - IMGUI_API void          SetTabItemClosed(const char* tab_or_docked_window_label);           // notify TabBar or Docking system of a closed tab/window ahead (useful to reduce visual flicker on reorderable tab bars). For tab-bar: call after BeginTabBar() and before Tab submissions. Otherwise call with a window name.

// @todo(mdodis): Logging/Capture
// - IMGUI_API void          LogToTTY(int auto_open_depth = -1);                                 // start logging to tty (stdout)
// - IMGUI_API void          LogToFile(int auto_open_depth = -1, const char* filename = NULL);   // start logging to file
// - IMGUI_API void          LogToClipboard(int auto_open_depth = -1);                           // start logging to OS clipboard
// - IMGUI_API void          LogFinish();                                                        // stop logging (close file, etc.)
// - IMGUI_API void          LogButtons();                                                       // helper to display buttons for logging to tty/file/clipboard
// - IMGUI_API void          LogText(const char* fmt, ...) IM_FMTARGS(1);                        // pass text data straight to log (without being displayed)
// - IMGUI_API void          LogTextV(const char* fmt, va_list args) IM_FMTLIST(1);

// @todo(mdodis): Drag and Drop
// - IMGUI_API bool          BeginDragDropSource(ImGuiDragDropFlags flags = 0);                                      // call after submitting an item which may be dragged. when this return true, you can call SetDragDropPayload() + EndDragDropSource()
// - IMGUI_API bool          SetDragDropPayload(const char* type, const void* data, size_t sz, ImGuiCond cond = 0);  // type is a user defined string of maximum 32 characters. Strings starting with '_' are reserved for dear imgui internal types. Data is copied and held by imgui. Return true when payload has been accepted.
// - IMGUI_API void          EndDragDropSource();                                                                    // only call EndDragDropSource() if BeginDragDropSource() returns true!
// - IMGUI_API bool                  BeginDragDropTarget();                                                          // call after submitting an item that may receive a payload. If this returns true, you can call AcceptDragDropPayload() + EndDragDropTarget()
// - IMGUI_API const ImGuiPayload*   AcceptDragDropPayload(const char* type, ImGuiDragDropFlags flags = 0);          // accept contents of a given type. If ImGuiDragDropFlags_AcceptBeforeDelivery is set you can peek into the payload before the mouse button is released.
// - IMGUI_API void                  EndDragDropTarget();                                                            // only call EndDragDropTarget() if BeginDragDropTarget() returns true!
// - IMGUI_API const ImGuiPayload*   GetDragDropPayload();                                                           // peek directly into the current payload from anywhere. returns NULL when drag and drop is finished or inactive. use ImGuiPayload::IsDataType() to test for the payload type.

/* ----CLIPPING------------------------------------------------------------------------------------------------------ */
#define VD_PROC_IMGUI_PUSH_CLIP_RECT(PNAME)                         void             PNAME(Vdf32 min_x, Vdf32 min_y, Vdf32 max_x, Vdf32 max_y, Vdb32 intersect_with_current_clip_rect)
#define VD_PROC_IMGUI_POP_CLIP_RECT(PNAME)                          void             PNAME(void)

// @todo(mdodis): Focus, Activation
// - IMGUI_API void          SetItemDefaultFocus();                                              // make last item the default focused item of a newly appearing window.
// - IMGUI_API void          SetKeyboardFocusHere(int offset = 0);                               // focus keyboard on the next widget. Use positive 'offset' to access sub components of a multiple component widget. Use -1 to access previous widget.

// @todo(mdodis): Keyboard/Gamepad Navigation
// - IMGUI_API void          SetNavCursorVisible(bool visible);                                  // alter visibility of keyboard/gamepad cursor. by default: show when using an arrow key, hide when clicking with mouse.

// @todo(mdodis): Overlapping mode
// - IMGUI_API void          SetNextItemAllowOverlap();                                          // allow next item to be overlapped by a subsequent item. Useful with invisible buttons, selectable, treenode covering an area where subsequent items may need to be added. Note that both Selectable() and TreeNode() have dedicated flags doing this.

/* ----ITEMS/WIDGETS UTILITIES AND QUERY FUNCTIONS------------------------------------------------------------------- */
#define VD_PROC_IMGUI_IS_ITEM_HOVERED(PNAME)                        Vdb32            PNAME(VdIMGUIHoveredFlags flags /* = 0 */)
#define VD_PROC_IMGUI_IS_ITEM_ACTIVE(PNAME)                         Vdb32            PNAME(void)
#define VD_PROC_IMGUI_IS_ITEM_FOCUSED(PNAME)                        Vdb32            PNAME(void)
#define VD_PROC_IMGUI_IS_ITEM_CLICKED(PNAME)                        Vdb32            PNAME(VdIMGUIMouseButton mouse_button /* = 0 */)
#define VD_PROC_IMGUI_IS_ITEM_VISIBLE(PNAME)                        Vdb32            PNAME(void)
#define VD_PROC_IMGUI_IS_ITEM_EDITED(PNAME)                         Vdb32            PNAME(void)
#define VD_PROC_IMGUI_IS_ITEM_ACTIVATED(PNAME)                      Vdb32            PNAME(void)
#define VD_PROC_IMGUI_IS_ITEM_DEACTIVATED(PNAME)                    Vdb32            PNAME(void)
#define VD_PROC_IMGUI_IS_ITEM_DEACTIVATED_AFTER_EDIT(PNAME)         Vdb32            PNAME(void)
#define VD_PROC_IMGUI_IS_ITEM_TOGGLED_OPEN(PNAME)                   Vdb32            PNAME(void)
#define VD_PROC_IMGUI_IS_ANY_ITEM_HOVERED(PNAME)                    Vdb32            PNAME(void)
#define VD_PROC_IMGUI_IS_ANY_ITEM_ACTIVE(PNAME)                     Vdb32            PNAME(void)
#define VD_PROC_IMGUI_IS_ANY_ITEM_FOCUSED(PNAME)                    Vdb32            PNAME(void)
#define VD_PROC_IMGUI_GET_ITEM_ID(PNAME)                            VdIMGUIID        PNAME(void)
#define VD_PROC_IMGUI_GET_ITEM_RECT_MIN(PNAME)                      void             PNAME(Vdf32 *x, Vdf32 *y)
#define VD_PROC_IMGUI_GET_ITEM_RECT_MAX(PNAME)                      void             PNAME(Vdf32 *x, Vdf32 *y)
#define VD_PROC_IMGUI_GET_ITEM_RECT_SIZE(PNAME)                     void             PNAME(Vdf32 *x, Vdf32 *y)

// @todo(mdodis): Viewports
// - IMGUI_API ImGuiViewport* GetMainViewport();                                                 // return primary/default viewport. This can never be NULL.

// @todo(mdodis): Background/Foreground Draw Lists
// - IMGUI_API ImDrawList*   GetBackgroundDrawList();                                            // this draw list will be the first rendered one. Useful to quickly draw shapes/text behind dear imgui contents.
// - IMGUI_API ImDrawList*   GetForegroundDrawList();                                            // this draw list will be the last rendered one. Useful to quickly draw shapes/text over dear imgui contents.

// @todo(mdodis): Miscellaneous Utilities
// - IMGUI_API bool          IsRectVisible(const ImVec2& size);                                  // test if rectangle (of given size, starting from cursor position) is visible / not clipped.
// - IMGUI_API bool          IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max);      // test if rectangle (in screen space) is visible / not clipped. to perform coarse clipping on user's side.
// - IMGUI_API double        GetTime();                                                          // get global imgui time. incremented by io.DeltaTime every frame.
// - IMGUI_API int           GetFrameCount();                                                    // get global imgui frame count. incremented by 1 every frame.
// - IMGUI_API ImDrawListSharedData* GetDrawListSharedData();                                    // you may use this when creating your own ImDrawList instances.
// - IMGUI_API const char*   GetStyleColorName(ImGuiCol idx);                                    // get a string corresponding to the enum value (for display, saving, etc.).
// - IMGUI_API void          SetStateStorage(ImGuiStorage* storage);                             // replace current window storage with our own (if you want to manipulate it yourself, typically clear subsection of it)
// - IMGUI_API ImGuiStorage* GetStateStorage();

// @todo(mdodis): Text Utilities
// - IMGUI_API ImVec2        CalcTextSize(const char* text, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);

// @todo(mdodis): Color Utilities
// - IMGUI_API ImVec4        ColorConvertU32ToFloat4(ImU32 in);
// - IMGUI_API ImU32         ColorConvertFloat4ToU32(const ImVec4& in);
// - IMGUI_API void          ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
// - IMGUI_API void          ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);

// @todo(mdodis): Inputs Utilities: Keyboard/Mouse/Gamepad
// - IMGUI_API bool          IsKeyDown(ImGuiKey key);                                            // is key being held.
// - IMGUI_API bool          IsKeyPressed(ImGuiKey key, bool repeat = true);                     // was key pressed (went from !Down to Down)? if repeat=true, uses io.KeyRepeatDelay / KeyRepeatRate
// - IMGUI_API bool          IsKeyReleased(ImGuiKey key);                                        // was key released (went from Down to !Down)?
// - IMGUI_API bool          IsKeyChordPressed(ImGuiKeyChord key_chord);                         // was key chord (mods + key) pressed, e.g. you can pass 'ImGuiMod_Ctrl | ImGuiKey_S' as a key-chord. This doesn't do any routing or focus check, please consider using Shortcut() function instead.
// - IMGUI_API int           GetKeyPressedAmount(ImGuiKey key, float repeat_delay, float rate);  // uses provided repeat rate/delay. return a count, most often 0 or 1 but might be >1 if RepeatRate is small enough that DeltaTime > RepeatRate
// - IMGUI_API const char*   GetKeyName(ImGuiKey key);                                           // [DEBUG] returns English name of the key. Those names are provided for debugging purpose and are not meant to be saved persistently nor compared.
// - IMGUI_API void          SetNextFrameWantCaptureKeyboard(bool want_capture_keyboard);        // Override io.WantCaptureKeyboard flag next frame (said flag is left for your application to handle, typically when true it instructs your app to ignore inputs). e.g. force capture keyboard when your widget is being hovered. This is equivalent to setting "io.WantCaptureKeyboard = want_capture_keyboard"; after the next NewFrame() call.

/* ----INPUTS UTILITIES: MOUSE--------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_IS_MOUSE_DOWN(PNAME)                          Vdb32            PNAME(VdIMGUIMouseButton button)
#define VD_PROC_IMGUI_IS_MOUSE_CLICKED(PNAME)                       Vdb32            PNAME(VdIMGUIMouseButton button,           Vdb32 *repeat)
#define VD_PROC_IMGUI_IS_MOUSE_RELEASED(PNAME)                      Vdb32            PNAME(VdIMGUIMouseButton button)
#define VD_PROC_IMGUI_IS_MOUSE_DOUBLE_CLICKED(PNAME)                Vdb32            PNAME(VdIMGUIMouseButton button)
#define VD_PROC_IMGUI_IS_MOUSE_RELEASED_WITH_DELAY(PNAME)           Vdb32            PNAME(VdIMGUIMouseButton button,           Vdf32 delay)
#define VD_PROC_IMGUI_GET_MOUSE_CLICKED_COUNT(PNAME)                Vdi32            PNAME(VdIMGUIMouseButton button)
#define VD_PROC_IMGUI_IS_MOUSE_HOVERING_RECT(PNAME)                 Vdb32            PNAME(Vdf32              min_x,            Vdf32 min_y,                        Vdf32 max_x, Vdf32 max_y, Vdb32 clip /* = true */)
#define VD_PROC_IMGUI_GET_MOUSE_POS(PNAME)                          void             PNAME(Vdf32              *x,               Vdf32 *y)
#define VD_PROC_IMGUI_GET_MOUSE_POS_ON_OPENING_CURRENT_POPUP(PNAME) void             PNAME(Vdf32              *x,               Vdf32 *y)
#define VD_PROC_IMGUI_IS_MOUSE_DRAGGING(PNAME)                      Vdb32            PNAME(VdIMGUIMouseButton button,           Vdf32 lock_threshold /* = -1.0f */)
#define VD_PROC_IMGUI_GET_MOUSE_DRAG_DELTA(PNAME)                   void             PNAME(VdIMGUIMouseButton button /* = 0 */, Vdf32 lock_threshold /* = -1.0f */, Vdf32 *x,    Vdf32 *y)
#define VD_PROC_IMGUI_RESET_MOUSE_DRAG_DELTA(PNAME)                 void             PNAME(VdIMGUIMouseButton button /* = 0 */)

/* ----IMGUI IO------------------------------------------------------------------------------------------------------ */
#define VD_PROC_IMGUI_IO_ADD_KEY_EVENT(PNAME)                       void             PNAME(VdIMGUIKey key,            Vdb32 down)
#define VD_PROC_IMGUI_IO_ADD_KEY_ANALOG_EVENT(PNAME)                void             PNAME(VdIMGUIKey key,            Vdb32 down, Vdf32 v)
#define VD_PROC_IMGUI_IO_ADD_MOUSE_POS_EVENT(PNAME)                 void             PNAME(Vdf32 x,                   Vdf32 y)
#define VD_PROC_IMGUI_IO_ADD_MOUSE_BUTTON_EVENT(PNAME)              void             PNAME(Vdi32 button,              Vdb32 down)
#define VD_PROC_IMGUI_IO_ADD_MOUSE_WHEEL_EVENT(PNAME)               void             PNAME(Vdf32 x,                   Vdf32 y)
#define VD_PROC_IMGUI_IO_ADD_MOUSE_SOURCE_EVENT(PNAME)              void             PNAME(VdIMGUIMouseSource source)
#define VD_PROC_IMGUI_IO_ADD_FOCUS_EVENT(PNAME)                     void             PNAME(Vdb32 focused)
#define VD_PROC_IMGUI_IO_ADD_INPUT_CHARACTER(PNAME)                 void             PNAME(Vdu32 c)
#define VD_PROC_IMGUI_IO_ADD_INPUT_CHARACTER_UTF16(PNAME)           void             PNAME(wchar_t c)
#define VD_PROC_IMGUI_IO_ADD_INPUT_CHARACTERS_UTF8(PNAME)           void             PNAME(const char *str)

/* ----IMGUI IO ACCESSORS-------------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_IO_GET_MOUSE_DELTA(PNAME)                     void             PNAME(Vdf32 *x,                  Vdf32 *y)
#define VD_PROC_IMGUI_IO_GET_MOUSE_WHEEL(PNAME)                     void             PNAME(Vdf32 *x,                  Vdf32 *y)

// @todo(mdodis): Rest
// - IMGUI_API ImGuiMouseCursor GetMouseCursor();                                                // get desired mouse cursor shape. Important: reset in ImGui::NewFrame(), this is updated during the frame. valid before Render(). If you use software rendering by setting io.MouseDrawCursor ImGui will render those for you
// - IMGUI_API void          SetMouseCursor(ImGuiMouseCursor cursor_type);                       // set desired mouse cursor shape
// - IMGUI_API void          SetNextFrameWantCaptureMouse(bool want_capture_mouse);              // Override io.WantCaptureMouse flag next frame (said flag is left for your application to handle, typical when true it instructs your app to ignore inputs). This is equivalent to setting "io.WantCaptureMouse = want_capture_mouse;" after the next NewFrame() call.

/* ----DRAW LIST - PRIMITIVES---------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_DRAW_LIST_ADD_LINE(PNAME)                     void             PNAME(VdIMGUIDrawList draw_list, Vdf32 p1_x,  Vdf32 p1_y,  Vdf32 p2_x,  Vdf32 p2_y,            Vdu32 color,                  Vdf32 thickness /* = 1.f */)
#define VD_PROC_IMGUI_DRAW_LIST_ADD_RECT(PNAME)                     void             PNAME(VdIMGUIDrawList draw_list, Vdf32 min_x, Vdf32 min_y, Vdf32 max_x, Vdf32 max_y,           Vdu32 color,                  Vdf32 rounding  /* = 0.f */, VdIMGUIDrawFlags draw_flags /* = 0.f */, Vdf32 thickness /* = 1.f */)
#define VD_PROC_IMGUI_DRAW_LIST_ADD_RECT_FILLED(PNAME)              void             PNAME(VdIMGUIDrawList draw_list, Vdf32 min_x, Vdf32 min_y, Vdf32 max_x, Vdf32 max_y,           Vdu32 color,                  Vdf32 rounding  /* = 0.f */, VdIMGUIDrawFlags draw_flags /* = 0.f */)
#define VD_PROC_IMGUI_DRAW_LIST_ADD_TEXT(PNAME)                     void             PNAME(VdIMGUIDrawList draw_list, Vdf32 pos_x, Vdf32 pos_y, Vdu32 color, const char * text_begin, const char *text_end /* = 0 */)

/* ----DRAW LIST SPLITTER-------------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_GET_SPLITTER(PNAME)                           VdIMGUISplitter  PNAME(void)
#define VD_PROC_IMGUI_RETURN_SPLITTER(PNAME)                        void             PNAME(VdIMGUISplitter splitter)
#define VD_PROC_IMGUI_SPLITTER_CLEAR(PNAME)                         void             PNAME(VdIMGUISplitter splitter)
#define VD_PROC_IMGUI_SPLITTER_CLEAR_FREE_MEMORY(PNAME)             void             PNAME(VdIMGUISplitter splitter)
#define VD_PROC_IMGUI_SPLITTER_SPLIT(PNAME)                         void             PNAME(VdIMGUISplitter splitter, VdIMGUIDrawList draw_list, Vdi32 count)
#define VD_PROC_IMGUI_SPLITTER_MERGE(PNAME)                         void             PNAME(VdIMGUISplitter splitter, VdIMGUIDrawList draw_list)
#define VD_PROC_IMGUI_SPLITTER_SET_CURRENT_CHANNEL(PNAME)           void             PNAME(VdIMGUISplitter splitter, VdIMGUIDrawList draw_list, Vdi32 channel_idx)


#define VD_IMGUI_ALL_FUNCTION_DEFINITIONS                                                                                                                                \
/* ----WINDOWS------------------------------------------------------------------------------------------------------- */                                                 \
    X_DEFINE(VD_PROC_IMGUI_BEGIN,                                  begin,                                   ProcIMGUIBegin)                                              \
    X_DEFINE(VD_PROC_IMGUI_END,                                    end,                                     ProcIMGUIEnd)                                                \
/* ----CHILD WINDOWS------------------------------------------------------------------------------------------------- */                                                 \
    X_DEFINE(VD_PROC_IMGUI_BEGIN_CHILD,                            begin_child,                             ProcIMGUIBeginChild)                                         \
    X_DEFINE(VD_PROC_IMGUI_BEGIN_CHILD_ID,                         begin_child_id,                          ProcIMGUIBeginChildID)                                       \
    X_DEFINE(VD_PROC_IMGUI_END_CHILD,                              end_child,                               ProcIMGUIEndChild)                                           \
/* ----WINDOWS UTILITIES--------------------------------------------------------------------------------------------- */                                                 \
    X_DEFINE(VD_PROC_IMGUI_IS_WINDOW_APPEARING,                    is_window_appearing,                     ProcIMGUIIsWindowAppearing)                                  \
    X_DEFINE(VD_PROC_IMGUI_IS_WINDOW_COLLAPSED,                    is_window_collapsed,                     ProcIMGUIIsWindowCollapsed)                                  \
    X_DEFINE(VD_PROC_IMGUI_IS_WINDOW_FOCUSED,                      is_window_focused,                       ProcIMGUIIsWindowFocused)                                    \
    X_DEFINE(VD_PROC_IMGUI_IS_WINDOW_HOVERED,                      is_window_hovered,                       ProcIMGUIIsWindowHovered)                                    \
    X_DEFINE(VD_PROC_IMGUI_GET_WINDOW_DRAW_LIST,                   get_window_draw_list,                    ProcIMGUIGetWindowDrawList)                                  \
    X_DEFINE(VD_PROC_IMGUI_GET_WINDOW_POS,                         get_window_pos,                          ProcIMGUIGetWindowPos)                                       \
    X_DEFINE(VD_PROC_IMGUI_GET_WINDOW_SIZE,                        get_window_size,                         ProcIMGUIGetWindowSize)                                      \
/* ----WINDOW MANIPULATION------------------------------------------------------------------------------------------- */                                                 \
    X_DEFINE(VD_PROC_IMGUI_SET_NEXT_WINDOW_POS,                    set_next_window_pos,                     ProcIMGUISetNextWindowPos)                                   \
    X_DEFINE(VD_PROC_IMGUI_SET_NEXT_WINDOW_SIZE,                   set_next_window_size,                    ProcIMGUISetNextWindowSize)                                  \
    X_DEFINE(VD_PROC_IMGUI_SET_NEXT_WINDOW_SIZE_CONSTRAINTS,       set_next_window_size_constraints,        ProcIMGUISetNextWindowSizeConstraints)                       \
    X_DEFINE(VD_PROC_IMGUI_SET_NEXT_WINDOW_CONTENT_SIZE,           set_next_window_content_size,            ProcIMGUISetNextWindowContentSize)                           \
    X_DEFINE(VD_PROC_IMGUI_SET_NEXT_WINDOW_COLLAPSED,              set_next_window_collapsed,               ProcIMGUISetNextWindowCollapsed)                             \
    X_DEFINE(VD_PROC_IMGUI_SET_NEXT_WINDOW_FOCUS,                  set_next_window_focus,                   ProcIMGUISetNextWindowFocus)                                 \
    X_DEFINE(VD_PROC_IMGUI_SET_NEXT_WINDOW_SCROLL,                 set_next_window_scroll,                  ProcIMGUISetNextWindowScroll)                                \
    X_DEFINE(VD_PROC_IMGUI_SET_NEXT_WINDOW_BG_ALPHA,               set_next_window_bg_alpha,                ProcIMGUISetNextWindowBgAlpha)                               \
/* ----WINDOW SCROLLING---------------------------------------------------------------------------------------------- */                                                 \
    X_DEFINE(VD_PROC_IMGUI_GET_SCROLL,                             get_scroll,                              ProcIMGUIGetScroll)                                          \
    X_DEFINE(VD_PROC_IMGUI_SET_SCROLL,                             set_scroll,                              ProcIMGUISetScroll)                                          \
    X_DEFINE(VD_PROC_IMGUI_GET_SCROLL_MAX,                         get_scroll_max,                          ProcIMGUIGetScrollMax)                                       \
    X_DEFINE(VD_PROC_IMGUI_SET_SCROLL_HERE,                        set_scroll_here,                         ProcIMGUISetScrollHere)                                      \
    X_DEFINE(VD_PROC_IMGUI_SET_SCROLL_FROM_POS,                    set_scroll_from_pos,                     ProcIMGUISetScrollFromPos)                                   \
/* ----PARAMETERS STACKS (FONT)-------------------------------------------------------------------------------------- */                                                 \
/* ----PARAMETERS STACKS (SHARED)------------------------------------------------------------------------------------ */                                                 \
    X_DEFINE(VD_PROC_IMGUI_PUSH_STYLE_COLORU32,                    push_style_coloru32,                     ProcIMGUIPushStyleColorU32)                                  \
    X_DEFINE(VD_PROC_IMGUI_PUSH_STYLE_COLORRGBA,                   push_style_colorrgba,                    ProcIMGUIPushStyleColorRGBA)                                 \
    X_DEFINE(VD_PROC_IMGUI_POP_STYLE_COLOR,                        pop_style_color,                         ProcIMGUIPopStyleColor)                                      \
    X_DEFINE(VD_PROC_IMGUI_PUSH_STYLE_VARF,                        push_style_varf,                         ProcIMGUIPushStyleVarF)                                      \
    X_DEFINE(VD_PROC_IMGUI_PUSH_STYLE_VAR2F,                       push_style_var2f,                        ProcIMGUIPushStyleVar2F)                                     \
    X_DEFINE(VD_PROC_IMGUI_POP_STYLE_VAR,                          pop_style_var,                           ProcIMGUIPopStyleVar)                                        \
/* ----PARAMETERS STACKS (CURRENT WINDOW)---------------------------------------------------------------------------- */                                                 \
/* ----STYLE READ ACCESS--------------------------------------------------------------------------------------------- */                                                 \
/* ----LAYOUT CURSOR POSITIONING------------------------------------------------------------------------------------- */                                                 \
    X_DEFINE(VD_PROC_IMGUI_GET_CURSOR_SCREEN_POS,                  get_cursor_screen_pos,                   ProcIMGUIGetCursorScreenPos)                                 \
    X_DEFINE(VD_PROC_IMGUI_SET_CURSOR_SCREEN_POS,                  set_cursor_screen_pos,                   ProcIMGUISetCursorScreenPos)                                 \
    X_DEFINE(VD_PROC_IMGUI_GET_CONTENT_REGION_AVAIL,               get_content_region_avail,                ProcIMGUIGetContentRegionAvail)                              \
    X_DEFINE(VD_PROC_IMGUI_GET_CURSOR_POS,                         get_cursor_pos,                          ProcIMGUIGetCursorPos)                                       \
    X_DEFINE(VD_PROC_IMGUI_SET_CURSOR_POS,                         set_cursor_pos,                          ProcIMGUISetCursorPos)                                       \
    X_DEFINE(VD_PROC_IMGUI_GET_CURSOR_START_POS,                   get_cursor_start_pos,                    ProcIMGUIGetCursorStartPos)                                  \
/* ----OTHER LAYOUT FUNCTIONS---------------------------------------------------------------------------------------- */                                                 \
    X_DEFINE(VD_PROC_IMGUI_SEPARATOR,                              separator,                               ProcIMGUISeparator)                                          \
    X_DEFINE(VD_PROC_IMGUI_SAME_LINE,                              same_line,                               ProcIMGUISameLine)                                           \
    X_DEFINE(VD_PROC_IMGUI_NEW_LINE,                               new_line,                                ProcIMGUINewLine)                                            \
    X_DEFINE(VD_PROC_IMGUI_SPACING,                                spacing,                                 ProcIMGUISpacing)                                            \
    X_DEFINE(VD_PROC_IMGUI_DUMMY,                                  dummy,                                   ProcIMGUIDummy)                                              \
    X_DEFINE(VD_PROC_IMGUI_INDENT,                                 indent,                                  ProcIMGUIIndent)                                             \
    X_DEFINE(VD_PROC_IMGUI_UNINDENT,                               unindent,                                ProcIMGUIUnindent)                                           \
    X_DEFINE(VD_PROC_IMGUI_BEGIN_GROUP,                            begin_group,                             ProcIMGUIBeginGroup)                                         \
    X_DEFINE(VD_PROC_IMGUI_END_GROUP,                              end_group,                               ProcIMGUIEndGroup)                                           \
    X_DEFINE(VD_PROC_IMGUI_ALIGN_TEXT_TO_FRAME_PADDING,            align_text_to_frame_padding,             ProcIMGUIAlignTextToFramePadding)                            \
    X_DEFINE(VD_PROC_IMGUI_GET_TEXT_LINE_HEIGHT,                   get_text_line_height,                    ProcIMGUIGetTextLineHeight)                                  \
    X_DEFINE(VD_PROC_IMGUI_GET_TEXT_LINE_HEIGHT_WITH_SPACING,      get_text_line_height_with_spacing,       ProcIMGUIGetTextLineHeightWithSpacing)                       \
    X_DEFINE(VD_PROC_IMGUI_GET_FRAME_HEIGHT,                       get_frame_height,                        ProcIMGUIGetFrameHeight)                                     \
    X_DEFINE(VD_PROC_IMGUI_GET_FRAME_HEIGHT_WITH_SPACING,          get_frame_height_with_spacing,           ProcIMGUIGetFrameHeightWithSpacing)                          \
/* ----ID STACK/SCOPES----------------------------------------------------------------------------------------------- */                                                 \
/* ----WIDGETS: TEXT------------------------------------------------------------------------------------------------- */                                                 \
    X_DEFINE(VD_PROC_IMGUI_TEXT_UNFORMATTED,                       text_unformatted,                        ProcIMGUITextUnformatted)                                    \
    X_DEFINE(VD_PROC_IMGUI_TEXTV,                                  textv,                                   ProcIMGUITextV)                                              \
    X_DEFINE(VD_PROC_IMGUI_TEXT_COLOREDV,                          text_coloredv,                           ProcIMGUITextColoredV)                                       \
    X_DEFINE(VD_PROC_IMGUI_TEXT_DISABLEDV,                         text_disabledv,                          ProcIMGUITextDisabledV)                                      \
    X_DEFINE(VD_PROC_IMGUI_TEXT_WRAPPEDV,                          text_wrappedv,                           ProcIMGUITextWrappedV)                                       \
    X_DEFINE(VD_PROC_IMGUI_LABEL_TEXTV,                            label_textv,                             ProcIMGUILabelTextV)                                         \
    X_DEFINE(VD_PROC_IMGUI_BULLET_TEXTV,                           bullet_textv,                            ProcIMGUIBulletTextV)                                        \
    X_DEFINE(VD_PROC_IMGUI_SEPARATOR_TEXT,                         separator_text,                          ProcIMGUISeparatorText)                                      \
/* ----WIDGETS: MAIN------------------------------------------------------------------------------------------------- */                                                 \
    X_DEFINE(VD_PROC_IMGUI_BUTTON,                                 button,                                  ProcIMGUIButton)                                             \
    X_DEFINE(VD_PROC_IMGUI_SMALL_BUTTON,                           small_button,                            ProcIMGUISmallButton)                                        \
    X_DEFINE(VD_PROC_IMGUI_INVISIBLE_BUTTON,                       invisible_button,                        ProcIMGUIInvisibleButton)                                    \
    X_DEFINE(VD_PROC_IMGUI_ARROW_BUTTON,                           arrow_button,                            ProcIMGUIArrowButton)                                        \
    X_DEFINE(VD_PROC_IMGUI_CHECKBOX,                               checkbox,                                ProcIMGUICheckbox)                                           \
    X_DEFINE(VD_PROC_IMGUI_CHECKBOX_FLAGSI,                        checkbox_flagsi,                         ProcIMGUICheckboxFlagsI)                                     \
    X_DEFINE(VD_PROC_IMGUI_CHECKBOX_FLAGSU,                        checkbox_flagsu,                         ProcIMGUICheckboxFlagsU)                                     \
    X_DEFINE(VD_PROC_IMGUI_RADIO_BUTTON,                           radio_button,                            ProcIMGUIRadioButton)                                        \
    X_DEFINE(VD_PROC_IMGUI_RADIO_BUTTONI,                          radio_buttoni,                           ProcIMGUIRadioButtonI)                                       \
    X_DEFINE(VD_PROC_IMGUI_PROGRESS_BAR,                           progress_bar,                            ProcIMGUIProgressBar)                                        \
    X_DEFINE(VD_PROC_IMGUI_BULLET,                                 bullet,                                  ProcIMGUIBullet)                                             \
    X_DEFINE(VD_PROC_IMGUI_TEXT_LINK,                              text_link,                               ProcIMGUITextLink)                                           \
/* ----CLIPPING------------------------------------------------------------------------------------------------------ */                                                 \
    X_DEFINE(VD_PROC_IMGUI_PUSH_CLIP_RECT,                         push_clip_rect,                          ProcIMGUIPushClipRect)                                       \
    X_DEFINE(VD_PROC_IMGUI_POP_CLIP_RECT,                          pop_clip_rect,                           ProcIMGUIPopClipRect)                                        \
/* ----ITEMS/WIDGETS UTILITIES AND QUERY FUNCTIONS------------------------------------------------------------------- */                                                 \
    X_DEFINE(VD_PROC_IMGUI_IS_ITEM_HOVERED,                        is_item_hovered,                         ProcIMGUIIsItemHovered)                                      \
    X_DEFINE(VD_PROC_IMGUI_IS_ITEM_ACTIVE,                         is_item_active,                          ProcIMGUIIsItemActive)                                       \
    X_DEFINE(VD_PROC_IMGUI_IS_ITEM_FOCUSED,                        is_item_focused,                         ProcIMGUIIsItemFocused)                                      \
    X_DEFINE(VD_PROC_IMGUI_IS_ITEM_CLICKED,                        is_item_clicked,                         ProcIMGUIIsItemClicked)                                      \
    X_DEFINE(VD_PROC_IMGUI_IS_ITEM_VISIBLE,                        is_item_visible,                         ProcIMGUIIsItemVisible)                                      \
    X_DEFINE(VD_PROC_IMGUI_IS_ITEM_EDITED,                         is_item_edited,                          ProcIMGUIIsItemEdited)                                       \
    X_DEFINE(VD_PROC_IMGUI_IS_ITEM_ACTIVATED,                      is_item_activated,                       ProcIMGUIIsItemActivated)                                    \
    X_DEFINE(VD_PROC_IMGUI_IS_ITEM_DEACTIVATED,                    is_item_deactivated,                     ProcIMGUIIsItemDeactivated)                                  \
    X_DEFINE(VD_PROC_IMGUI_IS_ITEM_DEACTIVATED_AFTER_EDIT,         is_item_deactivated_after_edit,          ProcIMGUIIsItemDeactivatedAfterEdit)                         \
    X_DEFINE(VD_PROC_IMGUI_IS_ITEM_TOGGLED_OPEN,                   is_item_toggled_open,                    ProcIMGUIIsItemToggledOpen)                                  \
    X_DEFINE(VD_PROC_IMGUI_IS_ANY_ITEM_HOVERED,                    is_any_item_hovered,                     ProcIMGUIIsAnyItemHovered)                                   \
    X_DEFINE(VD_PROC_IMGUI_IS_ANY_ITEM_ACTIVE,                     is_any_item_active,                      ProcIMGUIIsAnyItemActive)                                    \
    X_DEFINE(VD_PROC_IMGUI_IS_ANY_ITEM_FOCUSED,                    is_any_item_focused,                     ProcIMGUIIsAnyItemFocused)                                   \
    X_DEFINE(VD_PROC_IMGUI_GET_ITEM_ID,                            get_item_id,                             ProcIMGUIGetItemId)                                          \
    X_DEFINE(VD_PROC_IMGUI_GET_ITEM_RECT_MIN,                      get_item_rect_min,                       ProcIMGUIGetItemRectMin)                                     \
    X_DEFINE(VD_PROC_IMGUI_GET_ITEM_RECT_MAX,                      get_item_rect_max,                       ProcIMGUIGetItemRectMax)                                     \
    X_DEFINE(VD_PROC_IMGUI_GET_ITEM_RECT_SIZE,                     get_item_rect_size,                      ProcIMGUIGetItemRectSize)                                    \
/* ----INPUTS UTILITIES: MOUSE--------------------------------------------------------------------------------------- */                                                 \
    X_DEFINE(VD_PROC_IMGUI_IS_MOUSE_DOWN,                          is_mouse_down,                           ProcIMGUIIsMouseDown)                                        \
    X_DEFINE(VD_PROC_IMGUI_IS_MOUSE_CLICKED,                       is_mouse_clicked,                        ProcIMGUIIsMouseClicked)                                     \
    X_DEFINE(VD_PROC_IMGUI_IS_MOUSE_RELEASED,                      is_mouse_released,                       ProcIMGUIIsMouseReleased)                                    \
    X_DEFINE(VD_PROC_IMGUI_IS_MOUSE_DOUBLE_CLICKED,                is_mouse_double_clicked,                 ProcIMGUIIsMouseDoubleClicked)                               \
    X_DEFINE(VD_PROC_IMGUI_IS_MOUSE_RELEASED_WITH_DELAY,           is_mouse_released_with_delay,            ProcIMGUIIsMouseReleasedWithDelay)                           \
    X_DEFINE(VD_PROC_IMGUI_GET_MOUSE_CLICKED_COUNT,                get_mouse_clicked_count,                 ProcIMGUIGetMouseClickedCount)                               \
    X_DEFINE(VD_PROC_IMGUI_IS_MOUSE_HOVERING_RECT,                 is_mouse_hovering_rect,                  ProcIMGUIIsMouseHoveringRect)                                \
    X_DEFINE(VD_PROC_IMGUI_GET_MOUSE_POS,                          get_mouse_pos,                           ProcIMGUIGetMousePos)                                        \
    X_DEFINE(VD_PROC_IMGUI_GET_MOUSE_POS_ON_OPENING_CURRENT_POPUP, get_mouse_pos_on_opening_current_popup,  ProcIMGUIGetMousePosOnOpeningCurrentPopup)                   \
    X_DEFINE(VD_PROC_IMGUI_IS_MOUSE_DRAGGING,                      is_mouse_dragging,                       ProcIMGUIIsMouseDragging)                                    \
    X_DEFINE(VD_PROC_IMGUI_GET_MOUSE_DRAG_DELTA,                   get_mouse_drag_delta,                    ProcIMGUIGetMouseDragDelta)                                  \
    X_DEFINE(VD_PROC_IMGUI_RESET_MOUSE_DRAG_DELTA,                 reset_mouse_drag_delta,                  ProcIMGUIResetMouseDragDelta)                                \
/* ----IMGUI IO------------------------------------------------------------------------------------------------------ */                                                 \
    X_DEFINE(VD_PROC_IMGUI_IO_ADD_KEY_EVENT,                       io_add_key_event,                   ProcIMGUIIOAddKeyEvent)                                           \
    X_DEFINE(VD_PROC_IMGUI_IO_ADD_KEY_ANALOG_EVENT,                io_add_key_analog_event,            ProcIMGUIIOAddKeyAnalogEvent)                                     \
    X_DEFINE(VD_PROC_IMGUI_IO_ADD_MOUSE_POS_EVENT,                 io_add_mouse_pos_event,             ProcIMGUIIOAddMousePosEvent)                                      \
    X_DEFINE(VD_PROC_IMGUI_IO_ADD_MOUSE_BUTTON_EVENT,              io_add_mouse_button_event,          ProcIMGUIIOAddMouseButtonEvent)                                   \
    X_DEFINE(VD_PROC_IMGUI_IO_ADD_MOUSE_WHEEL_EVENT,               io_add_mouse_wheel_event,           ProcIMGUIIOAddMouseWheelEvent)                                    \
    X_DEFINE(VD_PROC_IMGUI_IO_ADD_MOUSE_SOURCE_EVENT,              io_add_mouse_source_event,          ProcIMGUIIOAddMouseSourceEvent)                                   \
    X_DEFINE(VD_PROC_IMGUI_IO_ADD_FOCUS_EVENT,                     io_add_focus_event,                 ProcIMGUIIOAddFocusEvent)                                         \
    X_DEFINE(VD_PROC_IMGUI_IO_ADD_INPUT_CHARACTER,                 io_add_input_character,             ProcIMGUIIOAddInputCharacter)                                     \
    X_DEFINE(VD_PROC_IMGUI_IO_ADD_INPUT_CHARACTER_UTF16,           io_add_input_character_utf16,       ProcIMGUIIOAddInputCharacterUTF16)                                \
    X_DEFINE(VD_PROC_IMGUI_IO_ADD_INPUT_CHARACTERS_UTF8,           io_add_input_characters_utf8,       ProcIMGUIIOAddInputCharactersUTF8)                                \
/* ----IMGUI IO ACCESSORS-------------------------------------------------------------------------------------------- */                                                 \
    X_DEFINE(VD_PROC_IMGUI_IO_GET_MOUSE_DELTA,                     io_get_mouse_delta,                 ProcIMGUIIOGetMouseDelta)                                         \
    X_DEFINE(VD_PROC_IMGUI_IO_GET_MOUSE_WHEEL,                     io_get_mouse_wheel,                 ProcIMGUIIOGetMouseWheel)                                         \
/* ----DRAW LIST - PRIMITIVES---------------------------------------------------------------------------------------- */                                                 \
    X_DEFINE(VD_PROC_IMGUI_DRAW_LIST_ADD_LINE,                     draw_list_add_line,                 ProcIMGUIDrawListAddLine)                                         \
    X_DEFINE(VD_PROC_IMGUI_DRAW_LIST_ADD_RECT,                     draw_list_add_rect,                 ProcIMGUIDrawListAddRect)                                         \
    X_DEFINE(VD_PROC_IMGUI_DRAW_LIST_ADD_RECT_FILLED,              draw_list_add_rect_filled,          ProcIMGUIDrawListAddRectFilled)                                   \
    X_DEFINE(VD_PROC_IMGUI_DRAW_LIST_ADD_TEXT,                     draw_list_add_text,                 ProcIMGUIDrawListAddText)                                         \
/* ----DRAW LIST SPLITTER-------------------------------------------------------------------------------------------- */                                                 \
    X_DEFINE(VD_PROC_IMGUI_GET_SPLITTER,                           get_splitter,                       ProcIMGUIGetSplitter)                                             \
    X_DEFINE(VD_PROC_IMGUI_RETURN_SPLITTER,                        return_splitter,                    ProcIMGUIReturnSplitter)                                          \
    X_DEFINE(VD_PROC_IMGUI_SPLITTER_CLEAR,                         splitter_clear,                     ProcIMGUISplitterClear)                                           \
    X_DEFINE(VD_PROC_IMGUI_SPLITTER_CLEAR_FREE_MEMORY,             splitter_clear_free_memory,         ProcIMGUISplitterClearFreeMemory)                                 \
    X_DEFINE(VD_PROC_IMGUI_SPLITTER_SPLIT,                         splitter_split,                     ProcIMGUISplitterSplit)                                           \
    X_DEFINE(VD_PROC_IMGUI_SPLITTER_MERGE,                         splitter_merge,                     ProcIMGUISplitterMerge)                                           \
    X_DEFINE(VD_PROC_IMGUI_SPLITTER_SET_CURRENT_CHANNEL,           splitter_set_current_channel,       ProcIMGUISplitterSetCurrentChannel)                               \

/* ----TYPEDEFS------------------------------------------------------------------------------------------------------ */
#define X_DEFINE(MACRO, FNAME, TNAME) typedef MACRO(VD_STRING_JOIN2(Vd,TNAME));
VD_IMGUI_ALL_FUNCTION_DEFINITIONS
#undef X_DEFINE

/* ----IMGUI PROCS STRUCT-------------------------------------------------------------------------------------------- */
typedef struct {
#define X_DEFINE(MACRO, FNAME, TNAME) VD_STRING_JOIN2(Vd,TNAME) *FNAME;
VD_IMGUI_ALL_FUNCTION_DEFINITIONS
#undef X_DEFINE
} VdIMGUIProcs;

/* ----MACROS - WINDOWS---------------------------------------------------------------------------------------------- */
#define VD_IMGUI_BEGIN(gui,                            ...) (gui)->begin(__VA_ARGS__)
#define VD_IMGUI_END(gui,                              ...) (gui)->end(__VA_ARGS__)

/* ----MACROS - CHILD WINDOWS---------------------------------------------------------------------------------------- */
#define VD_IMGUI_BEGIN_CHILD(gui,                      ...) (gui)->begin_child(__VA_ARGS__)
#define VD_IMGUI_BEGIN_CHILD_ID(gui,                   ...) (gui)->begin_child_id(__VA_ARGS__)
#define VD_IMGUI_END_CHILD(gui,                       ...) (gui)->end_child(__VA_ARGS__)

/* ----MACROS - WINDOW UTILITIES------------------------------------------------------------------------------------- */
#define VD_IMGUI_IS_WINDOW_APPEARING(gui,              ...) (gui)->is_window_appearing(__VA_ARGS__)
#define VD_IMGUI_IS_WINDOW_COLLAPSED(gui,              ...) (gui)->is_window_collapsed(__VA_ARGS__)
#define VD_IMGUI_IS_WINDOW_FOCUSED(gui,                ...) (gui)->is_window_focused(__VA_ARGS__)
#define VD_IMGUI_IS_WINDOW_HOVERED(gui,                ...) (gui)->is_window_hovered(__VA_ARGS__)
#define VD_IMGUI_GET_WINDOW_DRAW_LIST(gui,             ...) (gui)->get_window_draw_list(__VA_ARGS__)
#define VD_IMGUI_GET_WINDOW_POS(gui,                   ...) (gui)->get_window_pos(__VA_ARGS__)
#define VD_IMGUI_GET_WINDOW_SIZE(gui,                  ...) (gui)->get_window_size(__VA_ARGS__)

/* ----MACROS - WINDOW MANIPULATION---------------------------------------------------------------------------------- */
#define VD_IMGUI_SET_NEXT_WINDOW_POS(gui,              ...) (gui)->set_next_window_pos(__VA_ARGS__)
#define VD_IMGUI_SET_NEXT_WINDOW_SIZE(gui,             ...) (gui)->set_next_window_size(__VA_ARGS__)
#define VD_IMGUI_SET_NEXT_WINDOW_SIZE_CONSTRAINTS(gui, ...) (gui)->set_next_window_size_constraints(__VA_ARGS__)
#define VD_IMGUI_SET_NEXT_WINDOW_CONTENT_SIZE(gui,     ...) (gui)->set_next_window_content_size(__VA_ARGS__)
#define VD_IMGUI_SET_NEXT_WINDOW_COLLAPSED(gui,        ...) (gui)->set_next_window_collapsed(__VA_ARGS__)
#define VD_IMGUI_SET_NEXT_WINDOW_FOCUS(gui,            ...) (gui)->set_next_window_focus(__VA_ARGS__)
#define VD_IMGUI_SET_NEXT_WINDOW_SCROLL(gui,           ...) (gui)->set_next_window_scroll(__VA_ARGS__)
#define VD_IMGUI_SET_NEXT_WINDOW_BG_ALPHA(gui,         ...) (gui)->set_next_window_bg_alpha(__VA_ARGS__)

/* ----MACROS - WINDOW SCROLLING------------------------------------------------------------------------------------- */
#define VD_IMGUI_GET_SCROLL(gui,                       ...) (gui)->get_scroll(__VA_ARGS__)
#define VD_IMGUI_SET_SCROLL(gui,                       ...) (gui)->set_scroll(__VA_ARGS__)
#define VD_IMGUI_GET_SCROLL_MAX(gui,                   ...) (gui)->get_scroll_max(__VA_ARGS__)
#define VD_IMGUI_SET_SCROLL_HERE(gui,                  ...) (gui)->set_scroll_here(__VA_ARGS__)
#define VD_IMGUI_SET_SCROLL_FROM_POS(gui,              ...) (gui)->set_scroll_from_pos(__VA_ARGS__)

/* ----MACROS - PARAMETERS STACKS (FONT)----------------------------------------------------------------------------- */
/* ----MACROS - PARAMETERS STACKS (SHARED)--------------------------------------------------------------------------- */
#define VD_IMGUI_PUSH_STYLE_COLORU32(gui,              ...) (gui)->push_style_coloru32(__VA_ARGS__)
#define VD_IMGUI_PUSH_STYLE_COLORRGBA(gui,             ...) (gui)->push_style_colorrgba(__VA_ARGS__)
#define VD_IMGUI_POP_STYLE_COLOR(gui,                  ...) (gui)->pop_style_color(__VA_ARGS__)
#define VD_IMGUI_PUSH_STYLE_VARF(gui,                  ...) (gui)->push_style_varf(__VA_ARGS__)
#define VD_IMGUI_PUSH_STYLE_VAR2F(gui,                 ...) (gui)->push_style_var2f(__VA_ARGS__)
#define VD_IMGUI_POP_STYLE_VAR(gui,                    ...) (gui)->pop_style_var(__VA_ARGS__)

/* ----MACROS - PARAMETERS STACKS (CURRENT WINDOW)------------------------------------------------------------------- */
/* ----MACROS - STYLE READ ACCESS------------------------------------------------------------------------------------ */
/* ----MACROS - LAYOUT CURSOR POSITIONING---------------------------------------------------------------------------- */
#define VD_IMGUI_GET_CURSOR_SCREEN_POS(gui,    ...) (gui)->get_cursor_screen_pos(__VA_ARGS__)
#define VD_IMGUI_SET_CURSOR_SCREEN_POS(gui,    ...) (gui)->set_cursor_screen_pos(__VA_ARGS__)
#define VD_IMGUI_GET_CONTENT_REGION_AVAIL(gui, ...) (gui)->get_content_region_avail(__VA_ARGS__)
#define VD_IMGUI_GET_CURSOR_POS(gui,           ...) (gui)->get_cursor_pos(__VA_ARGS__)
#define VD_IMGUI_SET_CURSOR_POS(gui,           ...) (gui)->set_cursor_pos(__VA_ARGS__)
#define VD_IMGUI_GET_CURSOR_START_POS(gui,     ...) (gui)->get_cursor_start_pos(__VA_ARGS__)

/* ----MACROS - OTHER LAYOUT FUNCTIONS------------------------------------------------------------------------------- */
#define VD_IMGUI_SEPARATOR(gui,                         ...) (gui)->separator(__VA_ARGS__)
#define VD_IMGUI_SAME_LINE(gui,                         ...) (gui)->same_line(__VA_ARGS__)
#define VD_IMGUI_SAME_LINE_DEFAULT(gui,                 ...) (gui)->same_line(/* Vdf32 offset_from_start_xa */ 0.f, /* Vdf32 spacing */ -1.f)
#define VD_IMGUI_NEW_LINE(gui,                          ...) (gui)->new_line(__VA_ARGS__)
#define VD_IMGUI_SPACING(gui,                           ...) (gui)->spacing(__VA_ARGS__)
#define VD_IMGUI_DUMMY(gui,                             ...) (gui)->dummy(__VA_ARGS__)
#define VD_IMGUI_INDENT(gui,                            ...) (gui)->indent(__VA_ARGS__)
#define VD_IMGUI_UNINDENT(gui,                          ...) (gui)->unindent(__VA_ARGS__)
#define VD_IMGUI_BEGIN_GROUP(gui,                       ...) (gui)->begin_group(__VA_ARGS__)
#define VD_IMGUI_END_GROUP(gui,                         ...) (gui)->end_group(__VA_ARGS__)
#define VD_IMGUI_ALIGN_TEXT_TO_FRAME_PADDING(gui,       ...) (gui)->align_text_to_frame_padding(__VA_ARGS__)
#define VD_IMGUI_GET_TEXT_LINE_HEIGHT(gui,              ...) (gui)->get_text_line_height(__VA_ARGS__)
#define VD_IMGUI_GET_TEXT_LINE_HEIGHT_WITH_SPACING(gui, ...) (gui)->get_text_line_height_with_spacing(__VA_ARGS__)
#define VD_IMGUI_GET_FRAME_HEIGHT(gui,                  ...) (gui)->get_frame_height(__VA_ARGS__)
#define VD_IMGUI_GET_FRAME_HEIGHT_WITH_SPACING(gui,     ...) (gui)->get_frame_height_with_spacing(__VA_ARGS__)

/* ----MACROS - ID STACK/SCOPES-------------------------------------------------------------------------------------- */
/* ----MACROS - WIDGETS: TEXT-----------------------------------------------------------------------------------------*/
#define VD_IMGUI_TEXT_UNFORMATTED(gui, ...) (gui)->text_unformatted(__VA_ARGS__)
#define VD_IMGUI_TEXTV(gui,                            ...) (gui)->textv(__VA_ARGS__)
#define VD_IMGUI_TEXT_COLOREDV(gui,    ...) (gui)->text_coloredv(__VA_ARGS__)
#define VD_IMGUI_TEXT_DISABLEDV(gui,   ...) (gui)->text_disabledv(__VA_ARGS__)
#define VD_IMGUI_TEXT_WRAPPEDV(gui,    ...) (gui)->text_wrappedv(__VA_ARGS__)
#define VD_IMGUI_LABEL_TEXTV(gui,      ...) (gui)->label_textv(__VA_ARGS__)
#define VD_IMGUI_BULLET_TEXTV(gui,     ...) (gui)->bullet_textv(__VA_ARGS__)
#define VD_IMGUI_SEPARATOR_TEXT(gui,   ...) (gui)->separator_text(__VA_ARGS__)

#define VD_IMGUI_TEXT(gui,                             ...) Vd__imgui_text((gui), 0,     0, 0, 0, 0, 0, __VA_ARGS__)
#define VD_IMGUI_TEXT_COLORED(gui, r, g, b, a,         ...) Vd__imgui_text((gui), 0,     r, g, b, a, 1, __VA_ARGS__)
#define VD_IMGUI_TEXT_DISABLED(gui,                    ...) Vd__imgui_text((gui), 0,     0, 0, 0, 0, 2, __VA_ARGS__)
#define VD_IMGUI_TEXT_WRAPPED(gui,                     ...) Vd__imgui_text((gui), 0,     0, 0, 0, 0, 3, __VA_ARGS__)
#define VD_IMGUI_LABEL_TEXT(gui, label,                ...) Vd__imgui_text((gui), label, 0, 0, 0, 0, 4, __VA_ARGS__)
#define VD_IMGUI_BULLET_TEXT(gui, label,               ...) Vd__imgui_text((gui), label, 0, 0, 0, 0, 5, __VA_ARGS__)
static void Vd__imgui_text(VdIMGUIProcs *procs, const char *label, Vdf32 r, Vdf32 g, Vdf32 b, Vdf32 a, int index, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    switch (index) {
        case 0:  procs->textv(fmt, args);                      break;
        case 1:  procs->text_coloredv(r, g, b, a, fmt, args);  break;
        case 2:  procs->text_disabledv(fmt, args);             break;
        case 3:  procs->text_wrappedv(fmt, args);              break;
        case 4:  procs->label_textv(label, fmt, args);         break;
        case 5:  procs->bullet_textv(label, fmt, args);        break;
        default: VD_IMPOSSIBLE();
    }
    va_end(args);
}

/* ----MACROS - WIDGETS: MAIN---------------------------------------------------------------------------------------- */
#define VD_IMGUI_BUTTON(gui,           ...) (gui)->button(__VA_ARGS__)
#define VD_IMGUI_SMALL_BUTTON(gui,     ...) (gui)->small_button(__VA_ARGS__)
#define VD_IMGUI_INVISIBLE_BUTTON(gui, ...) (gui)->invisible_button(__VA_ARGS__)
#define VD_IMGUI_ARROW_BUTTON(gui,     ...) (gui)->arrow_button(__VA_ARGS__)
#define VD_IMGUI_CHECKBOX(gui,         ...) (gui)->checkbox(__VA_ARGS__)
#define VD_IMGUI_CHECKBOX_FLAGSI(gui,  ...) (gui)->checkbox_flagsi(__VA_ARGS__)
#define VD_IMGUI_CHECKBOX_FLAGSU(gui,  ...) (gui)->checkbox_flagsu(__VA_ARGS__)
#define VD_IMGUI_RADIO_BUTTON(gui,     ...) (gui)->radio_button(__VA_ARGS__)
#define VD_IMGUI_RADIO_BUTTONI(gui,    ...) (gui)->radio_buttoni(__VA_ARGS__)
#define VD_IMGUI_PROGRESS_BAR(gui,     ...) (gui)->progress_bar(__VA_ARGS__)
#define VD_IMGUI_BULLET(gui,           ...) (gui)->bullet(__VA_ARGS__)
#define VD_IMGUI_TEXT_LINK(gui,        ...) (gui)->text_link(__VA_ARGS__)

/* ----MACROS - CLIPPING---------------------------------------------------------------------------------------------- */
#define VD_IMGUI_PUSH_CLIP_RECT(gui,   ...) (gui)->push_clip_rect(__VA_ARGS__)
#define VD_IMGUI_POP_CLIP_RECT(gui,    ...) (gui)->pop_clip_rect(__VA_ARGS__)

/* ----MACROS - ITEMS/WIDGETS UTILITIES AND QUERY FUNCTIONS---------------------------------------------------------- */
#define VD_IMGUI_IS_ITEM_HOVERED(gui,                ...) (gui)->is_item_hovered(__VA_ARGS__)
#define VD_IMGUI_IS_ITEM_ACTIVE(gui,                 ...) (gui)->is_item_active(__VA_ARGS__)
#define VD_IMGUI_IS_ITEM_FOCUSED(gui,                ...) (gui)->is_item_focused(__VA_ARGS__)
#define VD_IMGUI_IS_ITEM_CLICKED(gui,                ...) (gui)->is_item_clicked(__VA_ARGS__)
#define VD_IMGUI_IS_ITEM_VISIBLE(gui,                ...) (gui)->is_item_visible(__VA_ARGS__)
#define VD_IMGUI_IS_ITEM_EDITED(gui,                 ...) (gui)->is_item_edited(__VA_ARGS__)
#define VD_IMGUI_IS_ITEM_ACTIVATED(gui,              ...) (gui)->is_item_activated(__VA_ARGS__)
#define VD_IMGUI_IS_ITEM_DEACTIVATED(gui,            ...) (gui)->is_item_deactivated(__VA_ARGS__)
#define VD_IMGUI_IS_ITEM_DEACTIVATED_AFTER_EDIT(gui, ...) (gui)->is_item_deactivated_after_edit(__VA_ARGS__)
#define VD_IMGUI_IS_ITEM_TOGGLED_OPEN(gui,           ...) (gui)->is_item_toggled_open(__VA_ARGS__)
#define VD_IMGUI_IS_ANY_ITEM_HOVERED(gui,            ...) (gui)->is_any_item_hovered(__VA_ARGS__)
#define VD_IMGUI_IS_ANY_ITEM_ACTIVE(gui,             ...) (gui)->is_any_item_active(__VA_ARGS__)
#define VD_IMGUI_IS_ANY_ITEM_FOCUSED(gui,            ...) (gui)->is_any_item_focused(__VA_ARGS__)
#define VD_IMGUI_GET_ITEM_ID(gui,                    ...) (gui)->get_item_id(__VA_ARGS__)
#define VD_IMGUI_GET_ITEM_RECT_MIN(gui,              ...) (gui)->get_item_rect_min(__VA_ARGS__)
#define VD_IMGUI_GET_ITEM_RECT_MAX(gui,              ...) (gui)->get_item_rect_max(__VA_ARGS__)
#define VD_IMGUI_GET_ITEM_RECT_SIZE(gui,             ...) (gui)->get_item_rect_size(__VA_ARGS__)

/* ----MACROS - INPUTS UTILITIES: MOUSE------------------------------------------------------------------------------ */
#define VD_IMGUI_IS_MOUSE_DOWN(gui,                          ...) (gui)->is_mouse_down(__VA_ARGS__)
#define VD_IMGUI_IS_MOUSE_CLICKED(gui,                       ...) (gui)->is_mouse_clicked(__VA_ARGS__)
#define VD_IMGUI_IS_MOUSE_RELEASED(gui,                      ...) (gui)->is_mouse_released(__VA_ARGS__)
#define VD_IMGUI_IS_MOUSE_DOUBLE_CLICKED(gui,                ...) (gui)->is_mouse_double_clicked(__VA_ARGS__)
#define VD_IMGUI_IS_MOUSE_RELEASED_WITH_DELAY(gui,           ...) (gui)->is_mouse_released_with_delay(__VA_ARGS__)
#define VD_IMGUI_GET_MOUSE_CLICKED_COUNT(gui,                ...) (gui)->get_mouse_clicked_count(__VA_ARGS__)
#define VD_IMGUI_IS_MOUSE_HOVERING_RECT(gui,                 ...) (gui)->is_mouse_hovering_rect(__VA_ARGS__)
#define VD_IMGUI_GET_MOUSE_POS(gui,                          ...) (gui)->get_mouse_pos(__VA_ARGS__)
#define VD_IMGUI_GET_MOUSE_POS_ON_OPENING_CURRENT_POPUP(gui, ...) (gui)->get_mouse_pos_on_opening_current_popup(__VA_ARGS__)
#define VD_IMGUI_IS_MOUSE_DRAGGING(gui,                      ...) (gui)->is_mouse_dragging(__VA_ARGS__)
#define VD_IMGUI_GET_MOUSE_DRAG_DELTA(gui,                   ...) (gui)->get_mouse_drag_delta(__VA_ARGS__)
#define VD_IMGUI_RESET_MOUSE_DRAG_DELTA(gui,                 ...) (gui)->reset_mouse_drag_delta(__VA_ARGS__)

/* ----MACROS - IMGUI IO--------------------------------------------------------------------------------------------- */
#define VD_IMGUI_IO_ADD_KEY_EVENT(gui,             ...)  (gui)->io_add_key_event(__VA_ARGS__)
#define VD_IMGUI_IO_ADD_KEY_ANALOG_EVENT(gui,      ...)  (gui)->io_add_key_analog_event(__VA_ARGS__)
#define VD_IMGUI_IO_ADD_MOUSE_POS_EVENT(gui,       ...)  (gui)->io_add_mouse_pos_event(__VA_ARGS__)
#define VD_IMGUI_IO_ADD_MOUSE_BUTTON_EVENT(gui,    ...)  (gui)->io_add_mouse_button_event(__VA_ARGS__)
#define VD_IMGUI_IO_ADD_MOUSE_WHEEL_EVENT(gui,     ...)  (gui)->io_add_mouse_wheel_event(__VA_ARGS__)
#define VD_IMGUI_IO_ADD_MOUSE_SOURCE_EVENT(gui,    ...)  (gui)->io_add_mouse_source_event(__VA_ARGS__)
#define VD_IMGUI_IO_ADD_FOCUS_EVENT(gui,           ...)  (gui)->io_add_focus_event(__VA_ARGS__)
#define VD_IMGUI_IO_ADD_INPUT_CHARACTER(gui,       ...)  (gui)->io_add_input_character(__VA_ARGS__)
#define VD_IMGUI_IO_ADD_INPUT_CHARACTER_UTF16(gui, ...)  (gui)->io_add_input_character_utf16(__VA_ARGS__)
#define VD_IMGUI_IO_ADD_INPUT_CHARACTERS_UTF8(gui, ...)  (gui)->io_add_input_characters_utf8(__VA_ARGS__)

/* ----MACROS - IMGUI IO ACCESSORS----------------------------------------------------------------------------------- */
#define VD_IMGUI_IO_GET_MOUSE_DELTA(gui,           ...) (gui)->io_get_mouse_delta(__VA_ARGS__)
#define VD_IMGUI_IO_GET_MOUSE_WHEEL(gui,           ...) (gui)->io_get_mouse_wheel(__VA_ARGS__)

/* ----MACROS - DRAW LIST - PRIMITIVES------------------------------------------------------------------------------- */
#define VD_IMGUI_DRAW_LIST_ADD_LINE(gui,           ...) (gui)->draw_list_add_line(__VA_ARGS__)
#define VD_IMGUI_DRAW_LIST_ADD_RECT(gui,           ...) (gui)->draw_list_add_rect(__VA_ARGS__)
#define VD_IMGUI_DRAW_LIST_ADD_RECT_FILLED(gui,    ...) (gui)->draw_list_add_rect_filled(__VA_ARGS__)
#define VD_IMGUI_DRAW_LIST_ADD_TEXT(gui,           ...) (gui)->draw_list_add_text(__VA_ARGS__)

/* ----MACROS - DRAW LIST SPLITTER----------------------------------------------------------------------------------- */
#define VD_IMGUI_GET_SPLITTER(gui,                 ...) (gui)->get_splitter(__VA_ARGS__)
#define VD_IMGUI_RETURN_SPLITTER(gui,              ...) (gui)->return_splitter(__VA_ARGS__)
#define VD_IMGUI_SPLITTER_CLEAR(gui,               ...) (gui)->splitter_clear(__VA_ARGS__)
#define VD_IMGUI_SPLITTER_CLEAR_FREE_MEMORY(gui,   ...) (gui)->splitter_clear_free_memory(__VA_ARGS__)
#define VD_IMGUI_SPLITTER_SPLIT(gui,               ...) (gui)->splitter_split(__VA_ARGS__)
#define VD_IMGUI_SPLITTER_MERGE(gui,               ...) (gui)->splitter_merge(__VA_ARGS__)
#define VD_IMGUI_SPLITTER_SET_CURRENT_CHANNEL(gui, ...) (gui)->splitter_set_current_channel(__VA_ARGS__)

/* ----MACROS VD_CG.H------------------------------------------------------------------------------------------------ */
#ifdef VD_CG_H
#define VD_IMGUI_GET_CURSOR_SCREEN_POS_F2(gui) vd__imgui_get_cursor_screen_pos_f2((gui))
static VD_INLINE Vdf2 vd__imgui_get_cursor_screen_pos_f2(VdIMGUIProcs *procs)
{
    f2 result;
    procs->get_cursor_screen_pos(&result.x, &result.y);
    return result;
}

#define VD_IMGUI_SET_CURSOR_SCREEN_POS_F2(gui, f2val) vd__imgui_set_cursor_screen_pos_f2((gui), (f2val))
static VD_INLINE void vd__imgui_set_cursor_screen_pos_f2(VdIMGUIProcs *procs, f2 val)
{
    procs->set_cursor_screen_pos(val.x, val.y);
}

#define VD_IMGUI_IO_GET_MOUSE_DELTA_F2(gui) vd__imgui_io_get_mouse_delta_f2((gui))
static VD_INLINE Vdf2 vd__imgui_io_get_mouse_delta_f2(VdIMGUIProcs *procs)
{
    f2 result;
    procs->io_get_mouse_delta(&result.x, &result.y);
    return result;
}

#define VD_IMGUI_IO_GET_MOUSE_WHEEL_F2(gui) vd__imgui_io_get_mouse_wheel_f2((gui))
static VD_INLINE Vdf2 vd__imgui_io_get_mouse_wheel_f2(VdIMGUIProcs *procs)
{
    f2 result;
    procs->io_get_mouse_wheel(&result.x, &result.y);
    return result;
}

#define VD_IMGUI_DRAW_LIST_ADD_LINE_F2(gui, ...) vd__imgui_draw_list_add_line_f2((gui), __VA_ARGS__)
static VD_INLINE void vd__imgui_draw_list_add_line_f2(VdIMGUIProcs *procs, VdIMGUIDrawList draw_list, Vdf2 p1, Vdf2 p2, Vdu32 color, Vdf32 thickness)
{
    procs->draw_list_add_line(draw_list, p1.x, p1.y, p2.x, p2.y, color, thickness);
}

#endif // VD_CG_H

#if VD_MACRO_ABBREVIATIONS
#define IMGUIDrawList     VdIMGUIDrawList
#define IMGUISplitter     VdIMGUISplitter
#define IMGUIID           VdIMGUIID
#define IMGUIWindowFlags  VdIMGUIWindowFlags
#define IMGUIChildFlags   VdIMGUIChildFlags
#define IMGUIStyleVar     VdIMGUIStyleVar
#define IMGUICol          VdIMGUICol
#define IMGUIDrawFlags    VdIMGUIDrawFlags
#define IMGUISliderFlags  VdIMGUISliderFlags
#define IMGUIHoveredFlags VdIMGUIHoveredFlags
#define IMGUIMouseButton  VdIMGUIMouseButton
#define IMGUIFocusedFlags VdIMGUIFocusedFlags
#define IMGUICond         VdIMGUICond
#define IMGUIKey          VdIMGUIKey
#define IMGUIMouseSource  VdIMGUIMouseSource
#define IMGUIButtonFlags  VdIMGUIButtonFlags
#define IMGUIProcs        VdIMGUIProcs
#define IMGUIDir          VdIMGUIDir
#endif // VD_MACRO_ABBREVIATIONS

#endif // !VD_IMGUI_H

#ifdef VD_IMGUI_IMPL

#ifndef IMGUI_VERSION
#error "You must include imgui.h when defining VD_IMGUI_IMPL!"
#endif // !IMGUI_VERSION

#if IMGUI_VERSION_NUM < 19222
#error "Dear ImGui Version must be 1.92.2 or higher"
#endif // IMGUI_VERSION < 19222

#define VD_IMGUI_IMPL_FUNC(name) VD_STRING_JOIN2(name,_impl)

/* ----WINDOWS------------------------------------------------------------------------------------------------------- */
static VD_PROC_IMGUI_BEGIN(VD_IMGUI_IMPL_FUNC(begin))
{
    return ImGui::Begin(name, (bool*)p_open, (ImGuiWindowFlags)flags);
}

static VD_PROC_IMGUI_END(VD_IMGUI_IMPL_FUNC(end))
{
    ImGui::End();
}

/* ----CHILD WINDOWS------------------------------------------------------------------------------------------------- */
static VD_PROC_IMGUI_BEGIN_CHILD(VD_IMGUI_IMPL_FUNC(begin_child))
{
    return ImGui::BeginChild(str_id, ImVec2(size_x, size_y), child_flags, window_flags);
}

static VD_PROC_IMGUI_BEGIN_CHILD_ID(VD_IMGUI_IMPL_FUNC(begin_child_id))
{
    return ImGui::BeginChild(id, ImVec2(size_x, size_y), child_flags, window_flags);
}

static VD_PROC_IMGUI_END_CHILD(VD_IMGUI_IMPL_FUNC(end_child))
{
    ImGui::EndChild();
}

/* ----WINDOWS UTILITIES--------------------------------------------------------------------------------------------- */
static VD_PROC_IMGUI_IS_WINDOW_APPEARING(VD_IMGUI_IMPL_FUNC(is_window_appearing))
{
    return ImGui::IsWindowAppearing();
}

static VD_PROC_IMGUI_IS_WINDOW_COLLAPSED(VD_IMGUI_IMPL_FUNC(is_window_collapsed))
{
    return ImGui::IsWindowCollapsed();
}

static VD_PROC_IMGUI_IS_WINDOW_FOCUSED(VD_IMGUI_IMPL_FUNC(is_window_focused))
{
    return ImGui::IsWindowFocused(flags);
}

static VD_PROC_IMGUI_IS_WINDOW_HOVERED(VD_IMGUI_IMPL_FUNC(is_window_hovered))
{
    return ImGui::IsWindowHovered(flags);
}

static VD_PROC_IMGUI_GET_WINDOW_DRAW_LIST(VD_IMGUI_IMPL_FUNC(get_window_draw_list))
{
    return ImGui::GetWindowDrawList();
}

static VD_PROC_IMGUI_GET_WINDOW_POS(VD_IMGUI_IMPL_FUNC(get_window_pos))
{
    ImVec2 result = ImGui::GetWindowPos();
    *x = result.x;
    *y = result.y;
}

static VD_PROC_IMGUI_GET_WINDOW_SIZE(VD_IMGUI_IMPL_FUNC(get_window_size))
{
    ImVec2 result = ImGui::GetWindowSize();
    *x = result.x;
    *y = result.y;
}

/* ----WINDOW MANIPULATION------------------------------------------------------------------------------------------- */
static VD_PROC_IMGUI_SET_NEXT_WINDOW_POS(VD_IMGUI_IMPL_FUNC(set_next_window_pos))
{
    ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), cond, ImVec2(pivot_x, pivot_y));
}

static VD_PROC_IMGUI_SET_NEXT_WINDOW_SIZE(VD_IMGUI_IMPL_FUNC(set_next_window_size))
{
    ImGui::SetNextWindowSize(ImVec2(size_x, size_y), cond);
}

static VD_PROC_IMGUI_SET_NEXT_WINDOW_SIZE_CONSTRAINTS(VD_IMGUI_IMPL_FUNC(set_next_window_size_constraints))
{
    VD_TODO();
}

static VD_PROC_IMGUI_SET_NEXT_WINDOW_CONTENT_SIZE(VD_IMGUI_IMPL_FUNC(set_next_window_content_size))
{
    VD_TODO();
}

static VD_PROC_IMGUI_SET_NEXT_WINDOW_COLLAPSED(VD_IMGUI_IMPL_FUNC(set_next_window_collapsed))
{
    VD_TODO();
}

static VD_PROC_IMGUI_SET_NEXT_WINDOW_FOCUS(VD_IMGUI_IMPL_FUNC(set_next_window_focus))
{
    VD_TODO();
}

static VD_PROC_IMGUI_SET_NEXT_WINDOW_SCROLL(VD_IMGUI_IMPL_FUNC(set_next_window_scroll))
{
    VD_TODO();
}

static VD_PROC_IMGUI_SET_NEXT_WINDOW_BG_ALPHA(VD_IMGUI_IMPL_FUNC(set_next_window_bg_alpha))
{
    VD_TODO();
}

/* ----WINDOW SCROLLING---------------------------------------------------------------------------------------------- */
static VD_PROC_IMGUI_GET_SCROLL(VD_IMGUI_IMPL_FUNC(get_scroll))
{
    VD_TODO();
}

static VD_PROC_IMGUI_SET_SCROLL(VD_IMGUI_IMPL_FUNC(set_scroll))
{
    VD_TODO();
}

static VD_PROC_IMGUI_GET_SCROLL_MAX(VD_IMGUI_IMPL_FUNC(get_scroll_max))
{
    VD_TODO();
}

static VD_PROC_IMGUI_SET_SCROLL_HERE(VD_IMGUI_IMPL_FUNC(set_scroll_here))
{
    VD_TODO();
}

static VD_PROC_IMGUI_SET_SCROLL_FROM_POS(VD_IMGUI_IMPL_FUNC(set_scroll_from_pos))
{
    VD_TODO();
}

/* ----PARAMETERS STACKS (SHARED)------------------------------------------------------------------------------------ */
static VD_PROC_IMGUI_PUSH_STYLE_COLORU32(VD_IMGUI_IMPL_FUNC(push_style_coloru32))
{
    ImGui::PushStyleColor(idx, col);
}

static VD_PROC_IMGUI_PUSH_STYLE_COLORRGBA(VD_IMGUI_IMPL_FUNC(push_style_colorrgba))
{
    ImGui::PushStyleColor(idx, ImVec4(r, g, b, a));
}

static VD_PROC_IMGUI_POP_STYLE_COLOR(VD_IMGUI_IMPL_FUNC(pop_style_color))
{
    ImGui::PopStyleColor();
}

static VD_PROC_IMGUI_PUSH_STYLE_VARF(VD_IMGUI_IMPL_FUNC(push_style_varf))
{
    ImGui::PushStyleVar(idx, val);
}

static VD_PROC_IMGUI_PUSH_STYLE_VAR2F(VD_IMGUI_IMPL_FUNC(push_style_var2f))
{
    ImGui::PushStyleVar(idx, ImVec2(val_x, val_y));
}

static VD_PROC_IMGUI_POP_STYLE_VAR(VD_IMGUI_IMPL_FUNC(pop_style_var))
{
    ImGui::PopStyleVar();
}

/* ----PARAMETERS STACKS (CURRENT WINDOW)---------------------------------------------------------------------------- */
/* ----STYLE READ ACCESS--------------------------------------------------------------------------------------------- */
/* ----LAYOUT CURSOR POSITIONING------------------------------------------------------------------------------------- */
static VD_PROC_IMGUI_GET_CURSOR_SCREEN_POS(VD_IMGUI_IMPL_FUNC(get_cursor_screen_pos))
{
    ImVec2 result = ImGui::GetCursorScreenPos();
    *x = result.x;
    *y = result.y;
}

static VD_PROC_IMGUI_SET_CURSOR_SCREEN_POS(VD_IMGUI_IMPL_FUNC(set_cursor_screen_pos))
{
    ImGui::SetCursorScreenPos(ImVec2(x, y));
}

static VD_PROC_IMGUI_GET_CONTENT_REGION_AVAIL(VD_IMGUI_IMPL_FUNC(get_content_region_avail))
{
    ImVec2 result = ImGui::GetContentRegionAvail();
    *x = result.x;
    *y = result.y;
}

static VD_PROC_IMGUI_GET_CURSOR_POS(VD_IMGUI_IMPL_FUNC(get_cursor_pos))
{
    ImVec2 result = ImGui::GetCursorPos();
    *x = result.x;
    *y = result.y;
}

static VD_PROC_IMGUI_SET_CURSOR_POS(VD_IMGUI_IMPL_FUNC(set_cursor_pos))
{
    ImGui::SetCursorPos(ImVec2(x, y));
}

static VD_PROC_IMGUI_GET_CURSOR_START_POS(VD_IMGUI_IMPL_FUNC(get_cursor_start_pos))
{
    ImVec2 result = ImGui::GetCursorStartPos();
    *x = result.x;
    *y = result.y;
}

/* ----OTHER LAYOUT FUNCTIONS---------------------------------------------------------------------------------------- */
static VD_PROC_IMGUI_SEPARATOR(VD_IMGUI_IMPL_FUNC(separator))
{
    ImGui::Separator();
}

static VD_PROC_IMGUI_SAME_LINE(VD_IMGUI_IMPL_FUNC(same_line))
{
    ImGui::SameLine(offset_from_start_x, spacing);
}

static VD_PROC_IMGUI_NEW_LINE(VD_IMGUI_IMPL_FUNC(new_line))
{
    ImGui::NewLine();
}

static VD_PROC_IMGUI_SPACING(VD_IMGUI_IMPL_FUNC(spacing))
{
    ImGui::Spacing();
}

static VD_PROC_IMGUI_DUMMY(VD_IMGUI_IMPL_FUNC(dummy))
{
    ImGui::Dummy(ImVec2(size_x, size_y));
}

static VD_PROC_IMGUI_INDENT(VD_IMGUI_IMPL_FUNC(indent))
{
    ImGui::Indent(indent_w);
}

static VD_PROC_IMGUI_UNINDENT(VD_IMGUI_IMPL_FUNC(unindent))
{
    ImGui::Unindent(indent_w);
}

static VD_PROC_IMGUI_BEGIN_GROUP(VD_IMGUI_IMPL_FUNC(begin_group))
{
    ImGui::BeginGroup();
}

static VD_PROC_IMGUI_END_GROUP(VD_IMGUI_IMPL_FUNC(end_group))
{
    ImGui::EndGroup();
}

static VD_PROC_IMGUI_ALIGN_TEXT_TO_FRAME_PADDING(VD_IMGUI_IMPL_FUNC(align_text_to_frame_padding))
{
    ImGui::AlignTextToFramePadding();
}

static VD_PROC_IMGUI_GET_TEXT_LINE_HEIGHT(VD_IMGUI_IMPL_FUNC(get_text_line_height))
{
    return ImGui::GetTextLineHeight();
}

static VD_PROC_IMGUI_GET_TEXT_LINE_HEIGHT_WITH_SPACING(VD_IMGUI_IMPL_FUNC(get_text_line_height_with_spacing))
{
    return ImGui::GetTextLineHeightWithSpacing();
}

static VD_PROC_IMGUI_GET_FRAME_HEIGHT(VD_IMGUI_IMPL_FUNC(get_frame_height))
{
    return ImGui::GetFrameHeight();
}

static VD_PROC_IMGUI_GET_FRAME_HEIGHT_WITH_SPACING(VD_IMGUI_IMPL_FUNC(get_frame_height_with_spacing))
{
    return ImGui::GetFrameHeightWithSpacing();
}

/* ----ID STACK/SCOPES----------------------------------------------------------------------------------------------- */
/* ----WIDGETS: TEXT------------------------------------------------------------------------------------------------- */
static VD_PROC_IMGUI_TEXT_UNFORMATTED(VD_IMGUI_IMPL_FUNC(text_unformatted))
{
    ImGui::TextUnformatted(text, text_end);
}

static VD_PROC_IMGUI_TEXTV(VD_IMGUI_IMPL_FUNC(textv))
{
    ImGui::TextV(fmt, args);
}

static VD_PROC_IMGUI_TEXT_COLOREDV(VD_IMGUI_IMPL_FUNC(text_coloredv))
{
    ImGui::TextColoredV(ImVec4(r, g, b, a), fmt, args);
}

static VD_PROC_IMGUI_TEXT_DISABLEDV(VD_IMGUI_IMPL_FUNC(text_disabledv))
{
    ImGui::TextDisabledV(fmt, args);
}

static VD_PROC_IMGUI_TEXT_WRAPPEDV(VD_IMGUI_IMPL_FUNC(text_wrappedv))
{
    ImGui::TextWrappedV(fmt, args);
}

static VD_PROC_IMGUI_LABEL_TEXTV(VD_IMGUI_IMPL_FUNC(label_textv))
{
    ImGui::LabelTextV(label, fmt, args);
}

static VD_PROC_IMGUI_BULLET_TEXTV(VD_IMGUI_IMPL_FUNC(bullet_textv))
{
    ImGui::BulletTextV(fmt, args);
}

static VD_PROC_IMGUI_SEPARATOR_TEXT(VD_IMGUI_IMPL_FUNC(separator_text))
{
    ImGui::SeparatorText(label);
}

/* ----WIDGETS: MAIN------------------------------------------------------------------------------------------------- */
static VD_PROC_IMGUI_BUTTON(VD_IMGUI_IMPL_FUNC(button))
{
    return ImGui::Button(label, ImVec2(size_x, size_y));
}

static VD_PROC_IMGUI_SMALL_BUTTON(VD_IMGUI_IMPL_FUNC(small_button))
{
    return ImGui::SmallButton(label);
}

static VD_PROC_IMGUI_INVISIBLE_BUTTON(VD_IMGUI_IMPL_FUNC(invisible_button))
{
    return ImGui::InvisibleButton(str_id, ImVec2(size_x, size_y), flags);
}

static VD_PROC_IMGUI_ARROW_BUTTON(VD_IMGUI_IMPL_FUNC(arrow_button))
{
    return ImGui::ArrowButton(str_id, (ImGuiDir)dir);
}

static VD_PROC_IMGUI_CHECKBOX(VD_IMGUI_IMPL_FUNC(checkbox))
{
    return ImGui::Checkbox(label, (bool*)v);
}

static VD_PROC_IMGUI_CHECKBOX_FLAGSI(VD_IMGUI_IMPL_FUNC(checkbox_flagsi))
{
    return ImGui::CheckboxFlags(label, flags, flags_value);
}

static VD_PROC_IMGUI_CHECKBOX_FLAGSU(VD_IMGUI_IMPL_FUNC(checkbox_flagsu))
{
    return ImGui::CheckboxFlags(label, flags, flags_value);
}

static VD_PROC_IMGUI_RADIO_BUTTON(VD_IMGUI_IMPL_FUNC(radio_button))
{
    return ImGui::RadioButton(label, active);
}

static VD_PROC_IMGUI_RADIO_BUTTONI(VD_IMGUI_IMPL_FUNC(radio_buttoni))
{
    return ImGui::RadioButton(label, v, v_button);
}

static VD_PROC_IMGUI_PROGRESS_BAR(VD_IMGUI_IMPL_FUNC(progress_bar))
{
    return ImGui::ProgressBar(fraction);
}

static VD_PROC_IMGUI_BULLET(VD_IMGUI_IMPL_FUNC(bullet))
{
    return ImGui::Bullet();
}

static VD_PROC_IMGUI_TEXT_LINK(VD_IMGUI_IMPL_FUNC(text_link))
{
    return ImGui::TextLink(label);
}

/* ----CLIPPING------------------------------------------------------------------------------------------------------ */
static VD_PROC_IMGUI_PUSH_CLIP_RECT(VD_IMGUI_IMPL_FUNC(push_clip_rect))
{
    ImGui::PushClipRect(ImVec2(min_x, min_y), ImVec2(max_x, max_y), intersect_with_current_clip_rect);
}

static VD_PROC_IMGUI_POP_CLIP_RECT(VD_IMGUI_IMPL_FUNC(pop_clip_rect))
{
    ImGui::PopClipRect();
}

/* ----ITEMS/WIDGETS UTILITIES AND QUERY FUNCTIONS------------------------------------------------------------------- */
static VD_PROC_IMGUI_IS_ITEM_HOVERED(VD_IMGUI_IMPL_FUNC(is_item_hovered))
{
    return ImGui::IsItemHovered((ImGuiHoveredFlags)flags);
}

static VD_PROC_IMGUI_IS_ITEM_ACTIVE(VD_IMGUI_IMPL_FUNC(is_item_active))
{
    return ImGui::IsItemActive();
}

static VD_PROC_IMGUI_IS_ITEM_FOCUSED(VD_IMGUI_IMPL_FUNC(is_item_focused))
{
    return ImGui::IsItemFocused();
}

static VD_PROC_IMGUI_IS_ITEM_CLICKED(VD_IMGUI_IMPL_FUNC(is_item_clicked))
{
    return ImGui::IsItemClicked((ImGuiMouseButton)mouse_button);
}

static VD_PROC_IMGUI_IS_ITEM_VISIBLE(VD_IMGUI_IMPL_FUNC(is_item_visible))
{
    return ImGui::IsItemVisible();
}

static VD_PROC_IMGUI_IS_ITEM_EDITED(VD_IMGUI_IMPL_FUNC(is_item_edited))
{

    return ImGui::IsItemEdited();
}

static VD_PROC_IMGUI_IS_ITEM_ACTIVATED(VD_IMGUI_IMPL_FUNC(is_item_activated))
{
    return ImGui::IsItemActivated();
}

static VD_PROC_IMGUI_IS_ITEM_DEACTIVATED(VD_IMGUI_IMPL_FUNC(is_item_deactivated))
{
    return ImGui::IsItemDeactivated();
}

static VD_PROC_IMGUI_IS_ITEM_DEACTIVATED_AFTER_EDIT(VD_IMGUI_IMPL_FUNC(is_item_deactivated_after_edit))
{
    return ImGui::IsItemDeactivatedAfterEdit();
}

static VD_PROC_IMGUI_IS_ITEM_TOGGLED_OPEN(VD_IMGUI_IMPL_FUNC(is_item_toggled_open))
{
    return ImGui::IsItemToggledOpen();
}

static VD_PROC_IMGUI_IS_ANY_ITEM_HOVERED(VD_IMGUI_IMPL_FUNC(is_any_item_hovered))
{
    return ImGui::IsAnyItemHovered();
}

static VD_PROC_IMGUI_IS_ANY_ITEM_ACTIVE(VD_IMGUI_IMPL_FUNC(is_any_item_active))
{
    return ImGui::IsAnyItemActive();
}

static VD_PROC_IMGUI_IS_ANY_ITEM_FOCUSED(VD_IMGUI_IMPL_FUNC(is_any_item_focused))
{
    return ImGui::IsAnyItemFocused();
}

static VD_PROC_IMGUI_GET_ITEM_ID(VD_IMGUI_IMPL_FUNC(get_item_id))
{
    return (VdIMGUIID)ImGui::GetItemID();

}

static VD_PROC_IMGUI_GET_ITEM_RECT_MIN(VD_IMGUI_IMPL_FUNC(get_item_rect_min))
{
    ImVec2 result = ImGui::GetItemRectMin();
    *x = result.x;
    *y = result.y;
}

static VD_PROC_IMGUI_GET_ITEM_RECT_MAX(VD_IMGUI_IMPL_FUNC(get_item_rect_max))
{
    ImVec2 result = ImGui::GetItemRectMax();
    *x = result.x;
    *y = result.y;
}

static VD_PROC_IMGUI_GET_ITEM_RECT_SIZE(VD_IMGUI_IMPL_FUNC(get_item_rect_size))
{
    ImVec2 result = ImGui::GetItemRectSize();
    *x = result.x;
    *y = result.y;
}


/* ----INPUTS UTILITIES: MOUSE--------------------------------------------------------------------------------------- */
static VD_PROC_IMGUI_IS_MOUSE_DOWN(VD_IMGUI_IMPL_FUNC(is_mouse_down))
{
    return ImGui::IsMouseDown(button);
}

static VD_PROC_IMGUI_IS_MOUSE_CLICKED(VD_IMGUI_IMPL_FUNC(is_mouse_clicked))
{
    return ImGui::IsMouseClicked(button, repeat);
}

static VD_PROC_IMGUI_IS_MOUSE_RELEASED(VD_IMGUI_IMPL_FUNC(is_mouse_released))
{
    return ImGui::IsMouseReleased(button);
}

static VD_PROC_IMGUI_IS_MOUSE_DOUBLE_CLICKED(VD_IMGUI_IMPL_FUNC(is_mouse_double_clicked))
{
    return ImGui::IsMouseDoubleClicked(button);
}

static VD_PROC_IMGUI_IS_MOUSE_RELEASED_WITH_DELAY(VD_IMGUI_IMPL_FUNC(is_mouse_released_with_delay))
{
    return ImGui::IsMouseReleasedWithDelay(button, delay);
}

static VD_PROC_IMGUI_GET_MOUSE_CLICKED_COUNT(VD_IMGUI_IMPL_FUNC(get_mouse_clicked_count))
{
    return ImGui::GetMouseClickedCount(button);
}

static VD_PROC_IMGUI_IS_MOUSE_HOVERING_RECT(VD_IMGUI_IMPL_FUNC(is_mouse_hovering_rect))
{
    return ImGui::IsMouseHoveringRect(ImVec2(min_x, min_y), ImVec2(max_x, max_y), clip);
}

static VD_PROC_IMGUI_GET_MOUSE_POS(VD_IMGUI_IMPL_FUNC(get_mouse_pos))
{
    ImVec2 result = ImGui::GetMousePos();
    *x = result.x;
    *y = result.y;
}

static VD_PROC_IMGUI_GET_MOUSE_POS_ON_OPENING_CURRENT_POPUP(VD_IMGUI_IMPL_FUNC(get_mouse_pos_on_opening_current_popup))
{
    ImVec2 result = ImGui::GetMousePosOnOpeningCurrentPopup();
    *x = result.x;
    *y = result.y;
}

static VD_PROC_IMGUI_IS_MOUSE_DRAGGING(VD_IMGUI_IMPL_FUNC(is_mouse_dragging))
{
    return ImGui::IsMouseDragging(button, lock_threshold);
}

static VD_PROC_IMGUI_GET_MOUSE_DRAG_DELTA(VD_IMGUI_IMPL_FUNC(get_mouse_drag_delta))
{
    ImVec2 result = ImGui::GetMouseDragDelta(button, lock_threshold);
    *x = result.x;
    *y = result.y;
}

static VD_PROC_IMGUI_RESET_MOUSE_DRAG_DELTA(VD_IMGUI_IMPL_FUNC(reset_mouse_drag_delta))
{
    ImGui::ResetMouseDragDelta(button);
}

/* ----IMGUI IO------------------------------------------------------------------------------------------------------ */
static VD_PROC_IMGUI_IO_ADD_KEY_EVENT(VD_IMGUI_IMPL_FUNC(io_add_key_event))
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddKeyEvent((ImGuiKey)key, down);
}

static VD_PROC_IMGUI_IO_ADD_KEY_ANALOG_EVENT(VD_IMGUI_IMPL_FUNC(io_add_key_analog_event))
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddKeyAnalogEvent((ImGuiKey)key, down, v);
}

static VD_PROC_IMGUI_IO_ADD_MOUSE_POS_EVENT(VD_IMGUI_IMPL_FUNC(io_add_mouse_pos_event))
{
    ImGuiIO &io = ImGui::GetIO();
#if VD_PLATFORM_MACOS
    // @note(mdodis): While mouse reporting seems correct on MacOSX, the ImGui mouse cursor indicates otherwise
    //                This kind of fixes it
    io.AddMousePosEvent(x, y * 1.025f);
#else
    io.AddMousePosEvent(x, y);
#endif
}

static VD_PROC_IMGUI_IO_ADD_MOUSE_BUTTON_EVENT(VD_IMGUI_IMPL_FUNC(io_add_mouse_button_event))
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddMouseButtonEvent(button, down);
}

static VD_PROC_IMGUI_IO_ADD_MOUSE_WHEEL_EVENT(VD_IMGUI_IMPL_FUNC(io_add_mouse_wheel_event))
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddMouseWheelEvent(x, y);
}

static VD_PROC_IMGUI_IO_ADD_MOUSE_SOURCE_EVENT(VD_IMGUI_IMPL_FUNC(io_add_mouse_source_event))
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddMouseSourceEvent((ImGuiMouseSource)source);
}

static VD_PROC_IMGUI_IO_ADD_FOCUS_EVENT(VD_IMGUI_IMPL_FUNC(io_add_focus_event))
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddFocusEvent(focused);
}

static VD_PROC_IMGUI_IO_ADD_INPUT_CHARACTER(VD_IMGUI_IMPL_FUNC(io_add_input_character))
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddInputCharacter(c);
}

static VD_PROC_IMGUI_IO_ADD_INPUT_CHARACTER_UTF16(VD_IMGUI_IMPL_FUNC(io_add_input_character_utf16))
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddInputCharacterUTF16(c);
}

static VD_PROC_IMGUI_IO_ADD_INPUT_CHARACTERS_UTF8(VD_IMGUI_IMPL_FUNC(io_add_input_characters_utf8))
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddInputCharactersUTF8(str);
}

/* ----IMGUI IO ACCESSORS-------------------------------------------------------------------------------------------- */
static VD_PROC_IMGUI_IO_GET_MOUSE_DELTA(VD_IMGUI_IMPL_FUNC(io_get_mouse_delta))
{
    ImGuiIO &io = ImGui::GetIO();
    *x = io.MouseDelta.x;
    *y = io.MouseDelta.y;
}

static VD_PROC_IMGUI_IO_GET_MOUSE_WHEEL(VD_IMGUI_IMPL_FUNC(io_get_mouse_wheel))
{
    ImGuiIO &io = ImGui::GetIO();
    *x = io.MouseWheelH;
    *y = io.MouseWheel;
}

/* ----DRAW LIST - PRIMITIVES---------------------------------------------------------------------------------------- */
static VD_PROC_IMGUI_DRAW_LIST_ADD_LINE(VD_IMGUI_IMPL_FUNC(draw_list_add_line))
{
    ImDrawList *l = (ImDrawList*)draw_list;
    l->AddLine(ImVec2(p1_x, p1_y), ImVec2(p2_x, p2_y), color, thickness);
}

static VD_PROC_IMGUI_DRAW_LIST_ADD_RECT(VD_IMGUI_IMPL_FUNC(draw_list_add_rect))
{
    ImDrawList *l = (ImDrawList*)draw_list;
    l->AddRect(ImVec2(min_x, min_y), ImVec2(max_x, max_y), color, rounding, draw_flags, thickness);
}

static VD_PROC_IMGUI_DRAW_LIST_ADD_RECT_FILLED(VD_IMGUI_IMPL_FUNC(draw_list_add_rect_filled))
{
    ImDrawList *l = (ImDrawList*)draw_list;
    l->AddRectFilled(ImVec2(min_x, min_y), ImVec2(max_x, max_y), color, rounding, draw_flags);
}

static VD_PROC_IMGUI_DRAW_LIST_ADD_TEXT(VD_IMGUI_IMPL_FUNC(draw_list_add_text))
{
    ImDrawList *l = (ImDrawList*)draw_list;
    l->AddText(ImVec2(pos_x, pos_y), color, text_begin, text_end);
}

/* ----DRAW LIST SPLITTER-------------------------------------------------------------------------------------------- */
static ImDrawListSplitter *Vd__IMGUI_Splitters = 0;
static Vdu32             Vd__IMGUI_Current_Splitter = 0;

static VD_PROC_IMGUI_GET_SPLITTER(VD_IMGUI_IMPL_FUNC(get_splitter))
{
    if (Vd__IMGUI_Splitters == 0) {
        Vd__IMGUI_Splitters = (ImDrawListSplitter*)VD_MALLOC(sizeof(ImDrawListSplitter) * VD_IMGUI_MAX_DRAW_LIST_SPLIITERS);
    }

    VD_ASSERT(Vd__IMGUI_Current_Splitter < VD_IMGUI_MAX_DRAW_LIST_SPLIITERS);
    void *spl_memory = &Vd__IMGUI_Splitters[Vd__IMGUI_Current_Splitter++];
    ImDrawListSplitter *spl = new (spl_memory) ImDrawListSplitter();
    return spl;
}

static VD_PROC_IMGUI_RETURN_SPLITTER(VD_IMGUI_IMPL_FUNC(return_splitter))
{
    ImDrawListSplitter *last_spl = &Vd__IMGUI_Splitters[Vd__IMGUI_Current_Splitter - 1];
    VD_ASSERT(last_spl == splitter && "GET SPLITTER must be matched with RETURN SPLITTER");

    last_spl->~ImDrawListSplitter();

    VD_MEMSET(last_spl, 0, sizeof(*last_spl));
    Vd__IMGUI_Current_Splitter--;
}

static VD_PROC_IMGUI_SPLITTER_CLEAR(VD_IMGUI_IMPL_FUNC(splitter_clear))
{
    ImDrawListSplitter *spl = (ImDrawListSplitter*)splitter;
    spl->Clear();
}

static VD_PROC_IMGUI_SPLITTER_CLEAR_FREE_MEMORY(VD_IMGUI_IMPL_FUNC(splitter_clear_free_memory))
{
    ImDrawListSplitter *spl = (ImDrawListSplitter*)splitter;
    spl->ClearFreeMemory();
}

static VD_PROC_IMGUI_SPLITTER_SPLIT(VD_IMGUI_IMPL_FUNC(splitter_split))
{
    ImDrawListSplitter *spl = (ImDrawListSplitter*)splitter;
    spl->Split((ImDrawList*)draw_list, count);
}

static VD_PROC_IMGUI_SPLITTER_MERGE(VD_IMGUI_IMPL_FUNC(splitter_merge))
{
    ImDrawListSplitter *spl = (ImDrawListSplitter*)splitter;
    spl->Merge((ImDrawList*)draw_list);
}

static VD_PROC_IMGUI_SPLITTER_SET_CURRENT_CHANNEL(VD_IMGUI_IMPL_FUNC(splitter_set_current_channel))
{
    ImDrawListSplitter *spl = (ImDrawListSplitter*)splitter;
    spl->SetCurrentChannel((ImDrawList*)draw_list, channel_idx);
}

static void vd_imgui_procs_populate(VdIMGUIProcs *procs)
{
#define X_DEFINE(MACRO, FNAME, TNAME) procs->FNAME = VD_IMGUI_IMPL_FUNC(FNAME);
VD_IMGUI_ALL_FUNCTION_DEFINITIONS
#undef X_DEFINE
}

#if VD_MACRO_ABBREVIATIONS
#define imgui_procs_populate(procs) vd_imgui_procs_populate(procs)
#endif // VD_MACRO_ABBREVIATIONS

#endif // VD_IMGUI_IMPL