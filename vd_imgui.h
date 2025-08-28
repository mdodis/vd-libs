/**
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
 * for setting up (i.e. using backends, creating context, rendering the draw list).
 * For that, you will have to drop down to C++ land.
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

typedef void*        VD(IMGUIDrawList);
typedef unsigned int VD(IMGUIID);

enum {
    VD_(IMGUI_WINDOW_FLAGS_NONE)                           = 0,
    VD_(IMGUI_WINDOW_FLAGS_NO_TITLEBAR)                    = 1 << 0,   // Disable title-bar
    VD_(IMGUI_WINDOW_FLAGS_NO_RESIZE)                      = 1 << 1,   // Disable user resizing with the lower-right grip
    VD_(IMGUI_WINDOW_FLAGS_NO_MOVE)                        = 1 << 2,   // Disable user moving the window
    VD_(IMGUI_WINDOW_FLAGS_NO_SCROLLBAR)                   = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
    VD_(IMGUI_WINDOW_FLAGS_NO_SCROLL_WITH_MOUSE)           = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
    VD_(IMGUI_WINDOW_FLAGS_NO_COLLAPSE)                    = 1 << 5,   // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
    VD_(IMGUI_WINDOW_FLAGS_ALWAYS_AUTO_RESIZE)             = 1 << 6,   // Resize every window to its content every frame
    VD_(IMGUI_WINDOW_FLAGS_NO_BACKGROUND)                  = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
    VD_(IMGUI_WINDOW_FLAGS_NO_SAVED_SETTINGS)              = 1 << 8,   // Never load/save settings in .ini file
    VD_(IMGUI_WINDOW_FLAGS_NO_MOUSE_INPUTS)                = 1 << 9,   // Disable catching mouse, hovering test with pass through.
    VD_(IMGUI_WINDOW_FLAGS_MENU_BAR)                       = 1 << 10,  // Has a menu-bar
    VD_(IMGUI_WINDOW_FLAGS_HORIZONTAL_SCROLLBAR)           = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
    VD_(IMGUI_WINDOW_FLAGS_NO_FOCUS_ON_APPEARING)          = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
    VD_(IMGUI_WINDOW_FLAGS_NO_BRING_TO_FRONT_ON_FOCUS)     = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
    VD_(IMGUI_WINDOW_FLAGS_ALWAYS_VERTICAL_SCROLLBAR)      = 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
    VD_(IMGUI_WINDOW_FLAGS_ALWAYS_HORIZONTAL_SCROLLBAR)    = 1 << 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
    VD_(IMGUI_WINDOW_FLAGS_NO_NAV_INPUTS)                  = 1 << 16,  // No keyboard/gamepad navigation within the window
    VD_(IMGUI_WINDOW_FLAGS_NO_NAV_FOCUS)                   = 1 << 17,  // No focusing toward this window with keyboard/gamepad navigation (e.g. skipped by CTRL+TAB)
    VD_(IMGUI_WINDOW_FLAGS_UNSAVED_DOCUMENT)               = 1 << 18,  // Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
    VD_(IMGUI_WINDOW_FLAGS_NO_DOCKING)                     = 1 << 19,  // Disable docking of this window
    VD_(IMGUI_WINDOW_FLAGS_NO_NAV)                         = VD_(IMGUI_WINDOW_FLAGS_NO_NAV_INPUTS) | VD_(IMGUI_WINDOW_FLAGS_NO_NAV_FOCUS),
    VD_(IMGUI_WINDOW_FLAGS_NO_DECORATION)                  = VD_(IMGUI_WINDOW_FLAGS_NO_TITLEBAR) | VD_(IMGUI_WINDOW_FLAGS_NO_RESIZE) | VD_(IMGUI_WINDOW_FLAGS_NO_SCROLLBAR) | VD_(IMGUI_WINDOW_FLAGS_NO_COLLAPSE),
    VD_(IMGUI_WINDOW_FLAGS_NO_INPUTS)                      = VD_(IMGUI_WINDOW_FLAGS_NO_MOUSE_INPUTS) | VD_(IMGUI_WINDOW_FLAGS_NO_NAV_INPUTS) | VD_(IMGUI_WINDOW_FLAGS_NO_NAV_FOCUS),
};
typedef int VD(IMGUIWindowFlags);

enum {
    VD_(IMGUI_CHILD_FLAGS_NONE)                       = 0,
    VD_(IMGUI_CHILD_FLAGS_BORDERS)                    = 1 << 0,   // Show an outer border and enable WindowPadding. (IMPORTANT: this is always == 1 == true for legacy reason)
    VD_(IMGUI_CHILD_FLAGS_ALWAYS_USE_WINDOW_PADDING)  = 1 << 1,   // Pad with style.WindowPadding even if no border are drawn (no padding by default for non-bordered child windows because it makes more sense)
    VD_(IMGUI_CHILD_FLAGS_RESIZE_X)                   = 1 << 2,   // Allow resize from right border (layout direction). Enable .ini saving (unless ImGuiWindowFlags_NoSavedSettings passed to window flags)
    VD_(IMGUI_CHILD_FLAGS_RESIZE_Y)                   = 1 << 3,   // Allow resize from bottom border (layout direction). "
    VD_(IMGUI_CHILD_FLAGS_AUTO_RESIZE_X)              = 1 << 4,   // Enable auto-resizing width. Read "IMPORTANT: Size measurement" details above.
    VD_(IMGUI_CHILD_FLAGS_AUTO_RESIZE_Y)              = 1 << 5,   // Enable auto-resizing height. Read "IMPORTANT: Size measurement" details above.
    VD_(IMGUI_CHILD_FLAGS_ALWAYS_AUTO_RESIZE)         = 1 << 6,   // Combined with AutoResizeX/AutoResizeY. Always measure size even when child is hidden, always return true, always disable clipping optimization! NOT RECOMMENDED.
    VD_(IMGUI_CHILD_FLAGS_FRAME_STYLE)                = 1 << 7,   // Style the child window like a framed item: use FrameBg, FrameRounding, FrameBorderSize, FramePadding instead of ChildBg, ChildRounding, ChildBorderSize, WindowPadding.
};
typedef int VD(IMGUIChildFlags);

enum {
    VD_(IMGUI_STYLE_VAR_ALPHA),                                   // float     Alpha
    VD_(IMGUI_STYLE_VAR_DISABLED_ALPHA),                          // float     DisabledAlpha
    VD_(IMGUI_STYLE_VAR_WINDOW_PADDING),                          // ImVec2    WindowPadding
    VD_(IMGUI_STYLE_VAR_WINDOW_ROUNDING),                         // float     WindowRounding
    VD_(IMGUI_STYLE_VAR_WINDOW_BORDER_SIZE),                      // float     WindowBorderSize
    VD_(IMGUI_STYLE_VAR_WINDOW_MIN_SIZE),                         // ImVec2    WindowMinSize
    VD_(IMGUI_STYLE_VAR_WINDOW_TITLE_ALIGN),                      // ImVec2    WindowTitleAlign
    VD_(IMGUI_STYLE_VAR_CHILD_ROUNDING),                          // float     ChildRounding
    VD_(IMGUI_STYLE_VAR_CHILD_BORDER_SIZE),                       // float     ChildBorderSize
    VD_(IMGUI_STYLE_VAR_POPUP_ROUNDING),                          // float     PopupRounding
    VD_(IMGUI_STYLE_VAR_POPUP_BORDER_SIZE),                       // float     PopupBorderSize
    VD_(IMGUI_STYLE_VAR_FRAME_PADDING),                           // ImVec2    FramePadding
    VD_(IMGUI_STYLE_VAR_FRAME_ROUNDING),                          // float     FrameRounding
    VD_(IMGUI_STYLE_VAR_FRAME_BORDER_SIZE),                       // float     FrameBorderSize
    VD_(IMGUI_STYLE_VAR_ITEM_SPACING),                            // ImVec2    ItemSpacing
    VD_(IMGUI_STYLE_VAR_ITEM_INNER_SPACING),                      // ImVec2    ItemInnerSpacing
    VD_(IMGUI_STYLE_VAR_INDENT_SPACING),                          // float     IndentSpacing
    VD_(IMGUI_STYLE_VAR_CELL_PADDING),                            // ImVec2    CellPadding
    VD_(IMGUI_STYLE_VAR_SCROLLBAR_SIZE),                          // float     ScrollbarSize
    VD_(IMGUI_STYLE_VAR_SCROLLBAR_ROUNDING),                      // float     ScrollbarRounding
    VD_(IMGUI_STYLE_VAR_GRAB_MIN_SIZE),                           // float     GrabMinSize
    VD_(IMGUI_STYLE_VAR_GRAB_ROUNDING),                           // float     GrabRounding
    VD_(IMGUI_STYLE_VAR_IMAGE_BORDER_SIZE),                       // float     ImageBorderSize
    VD_(IMGUI_STYLE_VAR_TAB_ROUNDING),                            // float     TabRounding
    VD_(IMGUI_STYLE_VAR_TAB_BORDER_SIZE),                         // float     TabBorderSize
    VD_(IMGUI_STYLE_VAR_TAB_MIN_WIDTH_BASE),                      // float     TabMinWidthBase
    VD_(IMGUI_STYLE_VAR_TAB_MIN_WIDTH_SHRINK),                    // float     TabMinWidthShrink
    VD_(IMGUI_STYLE_VAR_TAB_BAR_BORDER_SIZE),                     // float     TabBarBorderSize
    VD_(IMGUI_STYLE_VAR_TAB_BAR_OVERLINE_SIZE),                   // float     TabBarOverlineSize
    VD_(IMGUI_STYLE_VAR_TABLE_ANGLED_HEADERS_ANGLE),              // float     TableAngledHeadersAngle
    VD_(IMGUI_STYLE_VAR_TABLE_ANGLED_HEADERS_TEXT_ALIGN),         // ImVec2  TableAngledHeadersTextAlign
    VD_(IMGUI_STYLE_VAR_TREE_LINES_SIZE),                         // float     TreeLinesSize
    VD_(IMGUI_STYLE_VAR_TREE_LINES_ROUNDING),                     // float     TreeLinesRounding
    VD_(IMGUI_STYLE_VAR_BUTTON_TEXT_ALIGN),                       // ImVec2    ButtonTextAlign
    VD_(IMGUI_STYLE_VAR_SELECTABLE_TEXT_ALIGN),                   // ImVec2    SelectableTextAlign
    VD_(IMGUI_STYLE_VAR_SEPARATOR_TEXT_BORDER_SIZE),              // float     SeparatorTextBorderSize
    VD_(IMGUI_STYLE_VAR_SEPARATOR_TEXT_ALIGN),                    // ImVec2    SeparatorTextAlign
    VD_(IMGUI_STYLE_VAR_SEPARATOR_TEXT_PADDING),                  // ImVec2    SeparatorTextPadding
    VD_(IMGUI_STYLE_VAR_COUNT)
};
typedef int VD(IMGUIStyleVar);

enum {
    VD_(IMGUI_COL_TEXT),
    VD_(IMGUI_COL_TEXT_DISABLED),
    VD_(IMGUI_COL_WINDOW_BG),                    // Background of normal windows
    VD_(IMGUI_COL_CHILD_BG),                     // Background of child windows
    VD_(IMGUI_COL_POPUP_BG),                     // Background of popups, menus, tooltips windows
    VD_(IMGUI_COL_BORDER),
    VD_(IMGUI_COL_BORDER_SHADOW),
    VD_(IMGUI_COL_FRAME_BG),                     // Background of checkbox, radio button, plot, slider, text input
    VD_(IMGUI_COL_FRAME_BG_HOVERED),
    VD_(IMGUI_COL_FRAME_BG_ACTIVE),
    VD_(IMGUI_COL_TITLE_BG),                     // Title bar
    VD_(IMGUI_COL_TITLE_BG_ACTIVE),              // Title bar when focused
    VD_(IMGUI_COL_TITLE_BG_COLLAPSED),           // Title bar when collapsed
    VD_(IMGUI_COL_MENU_BAR_BG),
    VD_(IMGUI_COL_SCROLLBAR_BG),
    VD_(IMGUI_COL_SCROLLBAR_GRAB),
    VD_(IMGUI_COL_SCROLLBAR_GRAB_HOVERED),
    VD_(IMGUI_COL_SCROLLBAR_GRAB_ACTIVE),
    VD_(IMGUI_COL_CHECK_MARK),                   // Checkbox tick and RadioButton circle
    VD_(IMGUI_COL_SLIDER_GRAB),
    VD_(IMGUI_COL_SLIDER_GRAB_ACTIVE),
    VD_(IMGUI_COL_BUTTON),
    VD_(IMGUI_COL_BUTTON_HOVERED),
    VD_(IMGUI_COL_BUTTON_ACTIVE),
    VD_(IMGUI_COL_HEADER),                       // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
    VD_(IMGUI_COL_HEADER_HOVERED),
    VD_(IMGUI_COL_HEADER_ACTIVE),
    VD_(IMGUI_COL_SEPARATOR),
    VD_(IMGUI_COL_SEPARATOR_HOVERED),
    VD_(IMGUI_COL_SEPARATOR_ACTIVE),
    VD_(IMGUI_COL_RESIZE_GRIP),                  // Resize grip in lower-right and lower-left corners of windows.
    VD_(IMGUI_COL_RESIZE_GRIP_HOVERED),
    VD_(IMGUI_COL_RESIZE_GRIP_ACTIVE),
    VD_(IMGUI_COL_INPUT_TEXT_CURSOR),            // InputText cursor/caret
    VD_(IMGUI_COL_TAB_HOVERED),                  // Tab background, when hovered
    VD_(IMGUI_COL_TAB),                          // Tab background, when tab-bar is focused & tab is unselected
    VD_(IMGUI_COL_TAB_SELECTED),                 // Tab background, when tab-bar is focused & tab is selected
    VD_(IMGUI_COL_TAB_SELECTED_OVERLINE),        // Tab horizontal overline, when tab-bar is focused & tab is selected
    VD_(IMGUI_COL_TAB_DIMMED),                   // Tab background, when tab-bar is unfocused & tab is unselected
    VD_(IMGUI_COL_TAB_DIMMED_SELECTED),          // Tab background, when tab-bar is unfocused & tab is selected
    VD_(IMGUI_COL_TAB_DIMMED_SELECTED_OVERLINE), //..horizontal overline, when tab-bar is unfocused & tab is selected
    VD_(IMGUI_COL_PLOT_LINES),
    VD_(IMGUI_COL_PLOT_LINES_HOVERED),
    VD_(IMGUI_COL_PLOT_HISTOGRAM),
    VD_(IMGUI_COL_PLOT_HISTOGRAM_HOVERED),
    VD_(IMGUI_COL_TABLE_HEADER_BG),              // Table header background
    VD_(IMGUI_COL_TABLE_BORDER_STRONG),          // Table outer and header borders (prefer using Alpha=1.0 here)
    VD_(IMGUI_COL_TABLE_BORDER_LIGHT),           // Table inner borders (prefer using Alpha=1.0 here)
    VD_(IMGUI_COL_TABLE_ROW_BG),                 // Table row background (even rows)
    VD_(IMGUI_COL_TABLE_ROW_BG_ALT),             // Table row background (odd rows)
    VD_(IMGUI_COL_TEXT_LINK),                    // Hyperlink color
    VD_(IMGUI_COL_TEXT_SELECTED_BG),             // Selected text inside an InputText
    VD_(IMGUI_COL_TREE_LINES),                   // Tree node hierarchy outlines when using ImGuiTreeNodeFlags_DrawLines
    VD_(IMGUI_COL_DRAG_DROP_TARGET),             // Rectangle highlighting a drop target
    VD_(IMGUI_COL_NAV_CURSOR),                   // Color of keyboard/gamepad navigation cursor/rectangle, when visible
    VD_(IMGUI_COL_NAV_WINDOWING_HIGHLIGHT),      // Highlight window when using CTRL+TAB
    VD_(IMGUI_COL_NAV_WINDOWING_DIM_BG),         // Darken/colorize entire screen behind the CTRL+TAB window list, when active
    VD_(IMGUI_COL_MODAL_WINDOW_DIM_BG),          // Darken/colorize entire screen behind a modal window, when one is active
    VD_(IMGUI_COL_COUNT),
};
typedef int VD(IMGUICol);

enum {
    VD_(IMGUI_DRAW_FLAGS_NONE)                        = 0,
    VD_(IMGUI_DRAW_FLAGS_CLOSED)                      = 1 << 0, // PathStroke(), AddPolyline(): specify that shape should be closed (Important: this is always == 1 for legacy reason)
    VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_LEFT)      = 1 << 4, // AddRect(), AddRectFilled(), PathRect(): enable rounding top-left corner only (when rounding > 0.0f, we default to all corners). Was 0x01.
    VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_RIGHT)     = 1 << 5, // AddRect(), AddRectFilled(), PathRect(): enable rounding top-right corner only (when rounding > 0.0f, we default to all corners). Was 0x02.
    VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_LEFT)   = 1 << 6, // AddRect(), AddRectFilled(), PathRect(): enable rounding bottom-left corner only (when rounding > 0.0f, we default to all corners). Was 0x04.
    VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_RIGHT)  = 1 << 7, // AddRect(), AddRectFilled(), PathRect(): enable rounding bottom-right corner only (when rounding > 0.0f, we default to all corners). Wax 0x08.
    VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_NONE)          = 1 << 8, // AddRect(), AddRectFilled(), PathRect(): disable rounding on all corners (when rounding > 0.0f). This is NOT zero, NOT an implicit flag!
    VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP)           = VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_LEFT) | VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_RIGHT),
    VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM)        = VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_LEFT) | VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_RIGHT),
    VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_LEFT)          = VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_LEFT) | VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_LEFT),
    VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_RIGHT)         = VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_RIGHT) | VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_RIGHT),
    VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_ALL)           = VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_LEFT) | VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_TOP_RIGHT) | VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_LEFT) | VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_BOTTOM_RIGHT),
    VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_DEFAULT_)      = VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_ALL), // Default to ALL corners if none of the _RoundCornersXX flags are specified.
    VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_MASK_)         = VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_ALL) | VD_(IMGUI_DRAW_FLAGS_ROUND_CORNERS_NONE),
};
typedef int VD(IMGUIDrawFlags);

enum {
    VD_(IMGUI_SLIDER_FLAGS_NONE)                  = 0,
    VD_(IMGUI_SLIDER_FLAGS_LOGARITHMIC)           = 1 << 5,       // Make the widget logarithmic (linear otherwise). Consider using ImGuiSliderFlags_NoRoundToFormat with this if using a format-string with small amount of digits.
    VD_(IMGUI_SLIDER_FLAGS_NO_ROUND_TO_FORMAT)    = 1 << 6,       // Disable rounding underlying value to match precision of the display format string (e.g. %.3f values are rounded to those 3 digits).
    VD_(IMGUI_SLIDER_FLAGS_NO_INPUT)              = 1 << 7,       // Disable CTRL+Click or Enter key allowing to input text directly into the widget.
    VD_(IMGUI_SLIDER_FLAGS_WRAP_AROUND)           = 1 << 8,       // Enable wrapping around from max to min and from min to max. Only supported by DragXXX() functions for now.
    VD_(IMGUI_SLIDER_FLAGS_CLAMP_ON_INPUT)        = 1 << 9,       // Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.
    VD_(IMGUI_SLIDER_FLAGS_CLAMP_ZERO_RANGE)      = 1 << 10,      // Clamp even if min==max==0.0f. Otherwise due to legacy reason DragXXX functions don't clamp with those values. When your clamping limits are dynamic you almost always want to use it.
    VD_(IMGUI_SLIDER_FLAGS_NO_SPEED_TWEAKS)       = 1 << 11,      // Disable keyboard modifiers altering tweak speed. Useful if you want to alter tweak speed yourself based on your own logic.
    VD_(IMGUI_SLIDER_FLAGS_ALWAYS_CLAMP)          = VD_(IMGUI_SLIDER_FLAGS_CLAMP_ON_INPUT) | VD_(IMGUI_SLIDER_FLAGS_CLAMP_ZERO_RANGE),
};
typedef int VD(IMGUISliderFlags);

enum {
    //ImGuiHoveredFlags_DockHierarchy               = 1 << 4,   // IsWindowHovered() only: Consider docking hierarchy (treat dockspace host as parent of docked window) (when used with _ChildWindows or _RootWindow)
    //ImGuiHoveredFlags_AllowWhenBlockedByModal     = 1 << 6,   // Return true even if a modal popup window is normally blocking access to this item/window. FIXME-TODO: Unavailable yet.
    VD_(IMGUI_HOVERED_FLAGS_NONE)                               = 0,        // Return true if directly over the item/window, not obstructed by another window, not obstructed by an active popup or modal blocking inputs under them.
    VD_(IMGUI_HOVERED_FLAGS_CHILD_WINDOWS)                      = 1 << 0,   // IsWindowHovered() only: Return true if any children of the window is hovered
    VD_(IMGUI_HOVERED_FLAGS_ROOT_WINDOW)                        = 1 << 1,   // IsWindowHovered() only: Test from root window (top most parent of the current hierarchy)
    VD_(IMGUI_HOVERED_FLAGS_ANY_WINDOW)                         = 1 << 2,   // IsWindowHovered() only: Return true if any window is hovered
    VD_(IMGUI_HOVERED_FLAGS_NO_POPUP_HIERARCHY)                 = 1 << 3,   // IsWindowHovered() only: Do not consider popup hierarchy (do not treat popup emitter as parent of popup) (when used with _ChildWindows or _RootWindow)
    VD_(IMGUI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_POPUP)        = 1 << 5,   // Return true even if a popup window is normally blocking access to this item/window
    VD_(IMGUI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_ACTIVE_ITEM)  = 1 << 7,   // Return true even if an active item is blocking access to this item/window. Useful for Drag and Drop patterns.
    VD_(IMGUI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED_BY_ITEM)      = 1 << 8,   // IsItemHovered() only: Return true even if the item uses AllowOverlap mode and is overlapped by another hoverable item.
    VD_(IMGUI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED_BY_WINDOW)    = 1 << 9,   // IsItemHovered() only: Return true even if the position is obstructed or overlapped by another window.
    VD_(IMGUI_HOVERED_FLAGS_ALLOW_WHEN_DISABLED)                = 1 << 10,  // IsItemHovered() only: Return true even if the item is disabled
    VD_(IMGUI_HOVERED_FLAGS_NO_NAV_OVERRIDE)                    = 1 << 11,  // IsItemHovered() only: Disable using keyboard/gamepad navigation state when active, always query mouse
    VD_(IMGUI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED)              = VD_(IMGUI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED_BY_ITEM) | VD_(IMGUI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED_BY_WINDOW),
    VD_(IMGUI_HOVERED_FLAGS_RECT_ONLY)                          = VD_(IMGUI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_POPUP) | VD_(IMGUI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_ACTIVE_ITEM) | VD_(IMGUI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED),
    VD_(IMGUI_HOVERED_FLAGS_ROOT_AND_CHILD_WINDOWS)             = VD_(IMGUI_HOVERED_FLAGS_ROOT_WINDOW) | VD_(IMGUI_HOVERED_FLAGS_CHILD_WINDOWS),

    // Tooltips mode
    // - typically used in IsItemHovered() + SetTooltip() sequence.
    // - this is a shortcut to pull flags from 'style.HoverFlagsForTooltipMouse' or 'style.HoverFlagsForTooltipNav' where you can reconfigure desired behavior.
    //   e.g. 'TooltipHoveredFlagsForMouse' defaults to 'ImGuiHoveredFlags_Stationary | ImGuiHoveredFlags_DelayShort'.
    // - for frequently actioned or hovered items providing a tooltip, you want may to use ImGuiHoveredFlags_ForTooltip (stationary + delay) so the tooltip doesn't show too often.
    // - for items which main purpose is to be hovered, or items with low affordance, or in less consistent apps, prefer no delay or shorter delay.
    VD_(IMGUI_HOVERED_FLAGS_FOR_TOOLTIP)                        = 1 << 12,  // Shortcut for standard flags when using IsItemHovered() + SetTooltip() sequence.

    // (Advanced) Mouse Hovering delays.
    // - generally you can use ImGuiHoveredFlags_ForTooltip to use application-standardized flags.
    // - use those if you need specific overrides.
    VD_(IMGUI_HOVERED_FLAGS_STATIONARY)                         = 1 << 13,  // Require mouse to be stationary for style.HoverStationaryDelay (~0.15 sec) _at least one time_. After this, can move on same item/window. Using the stationary test tends to reduces the need for a long delay.
    VD_(IMGUI_HOVERED_FLAGS_DELAY_NONE)                         = 1 << 14,  // IsItemHovered() only: Return true immediately (default). As this is the default you generally ignore this.
    VD_(IMGUI_HOVERED_FLAGS_DELAY_SHORT)                        = 1 << 15,  // IsItemHovered() only: Return true after style.HoverDelayShort elapsed (~0.15 sec) (shared between items) + requires mouse to be stationary for style.HoverStationaryDelay (once per item).
    VD_(IMGUI_HOVERED_FLAGS_DELAY_NORMAL)                       = 1 << 16,  // IsItemHovered() only: Return true after style.HoverDelayNormal elapsed (~0.40 sec) (shared between items) + requires mouse to be stationary for style.HoverStationaryDelay (once per item).
    VD_(IMGUI_HOVERED_FLAGS_NO_SHARED_DELAY)                    = 1 << 17,  // IsItemHovered() only: Disable shared delay system where moving from one item to the next keeps the previous timer for a short time (standard for tooltips with long delays)
};
typedef int VD(IMGUIHoveredFlags);

enum {
    VD_(IMGUI_MOUSE_BUTTON_LEFT)   = 0,
    VD_(IMGUI_MOUSE_BUTTON_RIGHT)  = 1,
    VD_(IMGUI_MOUSE_BUTTON_MIDDLE) = 2,
};
typedef int VD(IMGUIMouseButton);

enum {
    VD_(IMGUI_FOCUSED_FLAGS_NONE)                    = 0,
    VD_(IMGUI_FOCUSED_FLAGS_CHILD_WINDOWS)           = 1 << 0,   // Return true if any children of the window is focused
    VD_(IMGUI_FOCUSED_FLAGS_ROOT_WINDOW)             = 1 << 1,   // Test from root window (top most parent of the current hierarchy)
    VD_(IMGUI_FOCUSED_FLAGS_ANY_WINDOW)              = 1 << 2,   // Return true if any window is focused. Important: If you are trying to tell how to dispatch your low-level inputs, do NOT use this. Use 'io.WantCaptureMouse' instead! Please read the FAQ!
    VD_(IMGUI_FOCUSED_FLAGS_NO_POPUP_HIERARCHY)      = 1 << 3,   // Do not consider popup hierarchy (do not treat popup emitter as parent of popup) (when used with _ChildWindows or _RootWindow)
    //VD_(IMGUI_FOCUSED_FLAGS_DOCK_HIERARCHY)        = 1 << 4,   // Consider docking hierarchy (treat dockspace host as parent of docked window) (when used with _ChildWindows or _RootWindow)
    VD_(IMGUI_FOCUSED_FLAGS_ROOT_AND_CHILD_WINDOWS)  = VD_(IMGUI_FOCUSED_FLAGS_ROOT_WINDOW) | VD_(IMGUI_FOCUSED_FLAGS_CHILD_WINDOWS),
};
typedef int VD(IMGUIFocusedFlags);

enum {
    VD_(IMGUI_COND_NONE)           = 0,        // No condition (always set the variable), same as _Always
    VD_(IMGUI_COND_ALWAYS)         = 1 << 0,   // No condition (always set the variable), same as _None
    VD_(IMGUI_COND_ONCE)           = 1 << 1,   // Set the variable once per runtime session (only the first call will succeed)
    VD_(IMGUI_COND_FIRST_USE_EVER) = 1 << 2,   // Set the variable if the object/window has no persistently saved data (no entry in .ini file)
    VD_(IMGUI_COND_APPEARING)      = 1 << 3,   // Set the variable if the object/window is appearing after being hidden/inactive (or the first time)
};
typedef int VD(IMGUICond);

enum {
    // Keyboard
    VD_(IMGUI_KEY_NONE) = 0,
    VD_(IMGUI_KEY_NAMED_KEY_BEGIN) = 512,  // First valid key value (other than 0)

    VD_(IMGUI_KEY_TAB) = 512,             // == ImGuiKey_NamedKey_BEGIN
    VD_(IMGUI_KEY_LEFT_ARROW),
    VD_(IMGUI_KEY_RIGHT_ARROW),
    VD_(IMGUI_KEY_UP_ARROW),
    VD_(IMGUI_KEY_DOWN_ARROW),
    VD_(IMGUI_KEY_PAGE_UP),
    VD_(IMGUI_KEY_PAGE_DOWN),
    VD_(IMGUI_KEY_HOME),
    VD_(IMGUI_KEY_END),
    VD_(IMGUI_KEY_INSERT),
    VD_(IMGUI_KEY_DELETE),
    VD_(IMGUI_KEY_BACKSPACE),
    VD_(IMGUI_KEY_SPACE),
    VD_(IMGUI_KEY_ENTER),
    VD_(IMGUI_KEY_ESCAPE),
    VD_(IMGUI_KEY_LEFT_CTRL),   VD_(IMGUI_KEY_LEFT_SHIFT),  VD_(IMGUI_KEY_LEFT_ALT),  VD_(IMGUI_KEY_LEFT_SUPER),
    VD_(IMGUI_KEY_RIGHT_CTRL),  VD_(IMGUI_KEY_RIGHT_SHIFT), VD_(IMGUI_KEY_RIGHT_ALT), VD_(IMGUI_KEY_RIGHT_SUPER),
    VD_(IMGUI_KEY_MENU),
    VD_(IMGUI_KEY_0),           VD_(IMGUI_KEY_1),   VD_(IMGUI_KEY_2),   VD_(IMGUI_KEY_3),   VD_(IMGUI_KEY_4),   VD_(IMGUI_KEY_5), VD_(IMGUI_KEY_6), VD_(IMGUI_KEY_7), VD_(IMGUI_KEY_8), VD_(IMGUI_KEY_9),
    VD_(IMGUI_KEY_A),           VD_(IMGUI_KEY_B),   VD_(IMGUI_KEY_C),   VD_(IMGUI_KEY_D),   VD_(IMGUI_KEY_E),   VD_(IMGUI_KEY_F), VD_(IMGUI_KEY_G), VD_(IMGUI_KEY_H), VD_(IMGUI_KEY_I), VD_(IMGUI_KEY_J),
    VD_(IMGUI_KEY_K),           VD_(IMGUI_KEY_L),   VD_(IMGUI_KEY_M),   VD_(IMGUI_KEY_N),   VD_(IMGUI_KEY_O),   VD_(IMGUI_KEY_P), VD_(IMGUI_KEY_Q), VD_(IMGUI_KEY_R), VD_(IMGUI_KEY_S), VD_(IMGUI_KEY_T),
    VD_(IMGUI_KEY_U),           VD_(IMGUI_KEY_V),   VD_(IMGUI_KEY_W),   VD_(IMGUI_KEY_X),   VD_(IMGUI_KEY_Y),   VD_(IMGUI_KEY_Z),
    VD_(IMGUI_KEY_F1),          VD_(IMGUI_KEY_F2),  VD_(IMGUI_KEY_F3),  VD_(IMGUI_KEY_F4),  VD_(IMGUI_KEY_F5),  VD_(IMGUI_KEY_F6),
    VD_(IMGUI_KEY_F7),          VD_(IMGUI_KEY_F8),  VD_(IMGUI_KEY_F9),  VD_(IMGUI_KEY_F10), VD_(IMGUI_KEY_F11), VD_(IMGUI_KEY_F12),
    VD_(IMGUI_KEY_F13),         VD_(IMGUI_KEY_F14), VD_(IMGUI_KEY_F15), VD_(IMGUI_KEY_F16), VD_(IMGUI_KEY_F17), VD_(IMGUI_KEY_F18),
    VD_(IMGUI_KEY_F19),         VD_(IMGUI_KEY_F20), VD_(IMGUI_KEY_F21), VD_(IMGUI_KEY_F22), VD_(IMGUI_KEY_F23), VD_(IMGUI_KEY_F24),
    VD_(IMGUI_KEY_APOSTROPHE),        // '
    VD_(IMGUI_KEY_COMMA),             // ,
    VD_(IMGUI_KEY_MINUS),             // -
    VD_(IMGUI_KEY_PERIOD),            // .
    VD_(IMGUI_KEY_SLASH),             // /
    VD_(IMGUI_KEY_SEMICOLON),         // ;
    VD_(IMGUI_KEY_EQUAL),             // =
    VD_(IMGUI_KEY_LEFT_BRACKET),      // [
    VD_(IMGUI_KEY_BACKSLASH),         // \ (this text inhibit multiline comment caused by backslash)
    VD_(IMGUI_KEY_RIGHT_BRACKET),     // ]
    VD_(IMGUI_KEY_GRAVE_ACCENT),      // `
    VD_(IMGUI_KEY_CAPS_LOCK),
    VD_(IMGUI_KEY_SCROLL_LOCK),
    VD_(IMGUI_KEY_NUM_LOCK),
    VD_(IMGUI_KEY_PRINT_SCREEN),
    VD_(IMGUI_KEY_PAUSE),
    VD_(IMGUI_KEY_KEYPAD0),     VD_(IMGUI_KEY_KEYPAD1), VD_(IMGUI_KEY_KEYPAD2), VD_(IMGUI_KEY_KEYPAD3), VD_(IMGUI_KEY_KEYPAD4),
    VD_(IMGUI_KEY_KEYPAD5),     VD_(IMGUI_KEY_KEYPAD6), VD_(IMGUI_KEY_KEYPAD7), VD_(IMGUI_KEY_KEYPAD8), VD_(IMGUI_KEY_KEYPAD9),
    VD_(IMGUI_KEY_KEYPAD_DECIMAL),
    VD_(IMGUI_KEY_KEYPAD_DIVIDE),
    VD_(IMGUI_KEY_KEYPAD_MULTIPLY),
    VD_(IMGUI_KEY_KEYPAD_SUBTRACT),
    VD_(IMGUI_KEY_KEYPAD_ADD),
    VD_(IMGUI_KEY_KEYPAD_ENTER),
    VD_(IMGUI_KEY_KEYPAD_EQUAL),
    VD_(IMGUI_KEY_APP_BACK),               // Available on some keyboard/mouses. Often referred as "Browser Back"
    VD_(IMGUI_KEY_APP_FORWARD),
    VD_(IMGUI_KEY_OEM102),                // Non-US backslash.

    // Gamepad
    // (analog values are 0.0f to 1.0f)
    // (download controller mapping PNG/PSD at http://dearimgui.com/controls_sheets)
    //                                     // XBOX        | SWITCH  | PLAYSTA. | -> ACTION
    VD_(IMGUI_KEY_GAMEPAD_START),          // Menu        | +       | Options  |
    VD_(IMGUI_KEY_GAMEPAD_BACK),           // View        | -       | Share    |
    VD_(IMGUI_KEY_GAMEPAD_FACE_LEFT),      // X           | Y       | Square   | Tap: Toggle Menu. Hold: Windowing mode (Focus/Move/Resize windows)
    VD_(IMGUI_KEY_GAMEPAD_FACE_RIGHT),     // B           | A       | Circle   | Cancel / Close / Exit
    VD_(IMGUI_KEY_GAMEPAD_FACE_UP),        // Y           | X       | Triangle | Text Input / On-screen Keyboard
    VD_(IMGUI_KEY_GAMEPAD_FACE_DOWN),      // A           | B       | Cross    | Activate / Open / Toggle / Tweak
    VD_(IMGUI_KEY_GAMEPAD_DPAD_LEFT),      // D-pad Left  | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    VD_(IMGUI_KEY_GAMEPAD_DPAD_RIGHT),     // D-pad Right | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    VD_(IMGUI_KEY_GAMEPAD_DPAD_UP),        // D-pad Up    | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    VD_(IMGUI_KEY_GAMEPAD_DPAD_DOWN),      // D-pad Down  | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    VD_(IMGUI_KEY_GAMEPAD_L1),             // L Bumper    | L       | L1       | Tweak Slower / Focus Previous (in Windowing mode)
    VD_(IMGUI_KEY_GAMEPAD_R1),             // R Bumper    | R       | R1       | Tweak Faster / Focus Next (in Windowing mode)
    VD_(IMGUI_KEY_GAMEPAD_L2),             // L Trigger   | ZL      | L2       | [Analog]
    VD_(IMGUI_KEY_GAMEPAD_R2),             // R Trigger   | ZR      | R2       | [Analog]
    VD_(IMGUI_KEY_GAMEPAD_L3),             // L Stick     | L3      | L3       |
    VD_(IMGUI_KEY_GAMEPAD_R3),             // R Stick     | R3      | R3       |
    VD_(IMGUI_KEY_GAMEPAD_L_STICK_LEFT),   //             |         |          | [Analog] Move Window (in Windowing mode)
    VD_(IMGUI_KEY_GAMEPAD_L_STICK_RIGHT),  //             |         |          | [Analog] Move Window (in Windowing mode)
    VD_(IMGUI_KEY_GAMEPAD_L_STICK_UP),     //             |         |          | [Analog] Move Window (in Windowing mode)
    VD_(IMGUI_KEY_GAMEPAD_L_STICK_DOWN),   //             |         |          | [Analog] Move Window (in Windowing mode)
    VD_(IMGUI_KEY_GAMEPAD_R_STICK_LEFT),   //             |         |          | [Analog]
    VD_(IMGUI_KEY_GAMEPAD_R_STICK_RIGHT),  //             |         |          | [Analog]
    VD_(IMGUI_KEY_GAMEPAD_R_STICK_UP),     //             |         |          | [Analog]
    VD_(IMGUI_KEY_GAMEPAD_R_STICK_DOWN),   //             |         |          | [Analog]

    // Aliases: Mouse Buttons (auto-submitted from AddMouseButtonEvent() calls)
    // - This is mirroring the data also written to io.MouseDown[], io.MouseWheel, in a format allowing them to be accessed via standard key API.
    VD_(IMGUI_KEY_MOUSE_LEFT), VD_(IMGUI_KEY_MOUSE_RIGHT), VD_(IMGUI_KEY_MOUSE_MIDDLE), VD_(IMGUI_KEY_MOUSE_X1), VD_(IMGUI_KEY_MOUSE_X2), VD_(IMGUI_KEY_MOUSE_WHEEL_X), VD_(IMGUI_KEY_MOUSE_WHEEL_Y),

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
    VD_(IMGUI_MOD_NONE)                   = 0,
    VD_(IMGUI_MOD_CTRL)                   = 1 << 12, // Ctrl (non-macOS), Cmd (macOS)
    VD_(IMGUI_MOD_SHIFT)                  = 1 << 13, // Shift
    VD_(IMGUI_MOD_ALT)                    = 1 << 14, // Option/Menu
    VD_(IMGUI_MOD_SUPER)                  = 1 << 15, // Windows/Super (non-macOS), Ctrl (macOS)
    VD_(IMGUI_MOD_MASK_)                  = 0xF000,  // 4-bits
};
typedef int VD(IMGUIKey);

enum {
    VD_(IMGUI_MOUSE_SOURCE_MOUSE) = 0,         // Input is coming from an actual mouse.
    VD_(IMGUI_MOUSE_SOURCE_TOUCH_SCREEN),      // Input is coming from a touch screen (no hovering prior to initial press, less precise initial press aiming, dual-axis wheeling possible).
    VD_(IMGUI_MOUSE_SOURCE_PEN),               // Input is coming from a pressure/magnetic pen (often used in conjunction with high-sampling rates).
};
typedef int VD(IMGUIMouseSource);

/* ----WINDOWS------------------------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_BEGIN(PNAME)                                  VD(b32)            PNAME(const char *name, VD(b32) *p_open /* = NULL */, VD(IMGUIWindowFlags) flags /* = 0 */)
#define VD_PROC_IMGUI_END(PNAME)                                    void               PNAME(void)

/* ----CHILD WINDOWS------------------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_BEGIN_CHILD(PNAME)                            VD(b32)            PNAME(const char *str_id, VD(f32) size_x /* = 0.f */, VD(f32) size_y /* = 0.f */, VD(IMGUIChildFlags) child_flags /* = 0 */, VD(IMGUIWindowFlags) window_flags /* = 0 */)
#define VD_PROC_IMGUI_BEGIN_CHILD_ID(PNAME)                         VD(b32)            PNAME(VD(IMGUIID) id,     VD(f32) size_x /* = 0.f */, VD(f32) size_y /* = 0.f */, VD(IMGUIChildFlags) child_flags /* = 0 */, VD(IMGUIWindowFlags) window_flags /* = 0 */)
#define VD_PROC_IMGUI_END_CHILD(PNAME)                              void               PNAME(void)

/* ----WINDOWS UTILITIES--------------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_IS_WINDOW_APPEARING(PNAME)                    VD(b32)            PNAME(void)
#define VD_PROC_IMGUI_IS_WINDOW_COLLAPSED(PNAME)                    VD(b32)            PNAME(void)
#define VD_PROC_IMGUI_IS_WINDOW_FOCUSED(PNAME)                      VD(b32)            PNAME(VD(IMGUIFocusedFlags) flags /* = 0 */)
#define VD_PROC_IMGUI_IS_WINDOW_HOVERED(PNAME)                      VD(b32)            PNAME(VD(IMGUIHoveredFlags) flags /* = 0 */)
#define VD_PROC_IMGUI_GET_WINDOW_DRAW_LIST(PNAME)                   VD(IMGUIDrawList)  PNAME(void)
#define VD_PROC_IMGUI_GET_WINDOW_POS(PNAME)                         void               PNAME(VD(f32) *x, VD(f32) *y)
#define VD_PROC_IMGUI_GET_WINDOW_SIZE(PNAME)                        void               PNAME(VD(f32) *x, VD(f32) *y)

/* ----WINDOW MANIPULATION------------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_POS(PNAME)                    void               PNAME(VD(f32) pos_x,      VD(f32) pos_y,      VD(IMGUICond) cond /* = 0 */, VD(f32) pivot_x /* = 0.f */, VD(f32) pivot_y /* = 0.f */)
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_SIZE(PNAME)                   void               PNAME(VD(f32) size_x,     VD(f32) size_y,     VD(IMGUICond) cond /* = 0 */)
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_SIZE_CONSTRAINTS(PNAME)       void               PNAME(VD(f32) size_min_x, VD(f32) size_min_y, VD(f32) size_max_x,           VD(f32) size_max_y) // @todo(mdodis): custom callback
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_CONTENT_SIZE(PNAME)           void               PNAME(VD(f32) size_x,     VD(f32) size_y)
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_COLLAPSED(PNAME)              void               PNAME(VD(b32) collapsed,  VD(IMGUICond) cond /* = 0 */)
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_FOCUS(PNAME)                  void               PNAME(void)
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_SCROLL(PNAME)                 void               PNAME(VD(f32) scroll_x,   VD(f32) scroll_y)
#define VD_PROC_IMGUI_SET_NEXT_WINDOW_BG_ALPHA(PNAME)               void               PNAME(VD(f32) alpha)
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
#define VD_PROC_IMGUI_GET_SCROLL(PNAME)                             void               PNAME(VD(f32) *x, VD(f32) *y)
#define VD_PROC_IMGUI_SET_SCROLL(PNAME)                             void               PNAME(VD(f32) *x, VD(f32) *y)
#define VD_PROC_IMGUI_GET_SCROLL_MAX(PNAME)                         void               PNAME(VD(f32) *x, VD(f32) *y)
#define VD_PROC_IMGUI_SET_SCROLL_HERE(PNAME)                        void               PNAME(VD(f32) *x, VD(f32) center_x_ratio /* = 0.5f */, VD(f32) *y, VD(f32) center_y_ratio /* = 0.5f */)
#define VD_PROC_IMGUI_SET_SCROLL_FROM_POS(PNAME)                    void               PNAME(VD(f32) *x, VD(f32) center_x_ratio /* = 0.5f */, VD(f32) *y, VD(f32) center_y_ratio /* = 0.5f */)

/* ----PARAMETERS STACKS (FONT)-------------------------------------------------------------------------------------- */
// @todo(mdodis): Parameters stacks (font)
// - IMGUI_API void          PushFont(ImFont* font, float font_size_base_unscaled);          // Use NULL as a shortcut to keep current font. Use 0.0f to keep current size.
// - IMGUI_API void          PopFont();
// - IMGUI_API ImFont*       GetFont();                                                      // get current font
// - IMGUI_API float         GetFontSize();                                                  // get current scaled font size (= height in pixels). AFTER global scale factors applied. *IMPORTANT* DO NOT PASS THIS VALUE TO PushFont()! Use ImGui::GetStyle().FontSizeBase to get value before global scale factors.
// - IMGUI_API ImFontBaked*  GetFontBaked();                                                 // get current font bound at current size // == GetFont()->GetFontBaked(GetFontSize())

/* ----PARAMETERS STACKS (SHARED)------------------------------------------------------------------------------------ */
#define VD_PROC_IMGUI_PUSH_STYLE_COLORU32(PNAME)                    void               PNAME(VD(IMGUICol) idx,      VD(u32) col)
#define VD_PROC_IMGUI_PUSH_STYLE_COLORRGBA(PNAME)                   void               PNAME(VD(IMGUICol) idx,      VD(f32) r,     VD(f32) g, VD(f32) b, VD(f32) a)
#define VD_PROC_IMGUI_POP_STYLE_COLOR(PNAME)                        void               PNAME(VD(i32) count /* = 1 */)
#define VD_PROC_IMGUI_PUSH_STYLE_VARF(PNAME)                        void               PNAME(VD(IMGUIStyleVar) idx, VD(f32) val)
#define VD_PROC_IMGUI_PUSH_STYLE_VAR2F(PNAME)                       void               PNAME(VD(IMGUIStyleVar) idx, VD(f32) val_x, VD(f32) val_y)
#define VD_PROC_IMGUI_POP_STYLE_VAR(PNAME)                          void               PNAME(VD(i32) count /* = 1 */)
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
#define VD_PROC_IMGUI_GET_CURSOR_SCREEN_POS(PNAME)                  void               PNAME(VD(f32) *x, VD(f32) *y)
#define VD_PROC_IMGUI_SET_CURSOR_SCREEN_POS(PNAME)                  void               PNAME(VD(f32) x,  VD(f32) y)
#define VD_PROC_IMGUI_GET_CONTENT_REGION_AVAIL(PNAME)               void               PNAME(VD(f32) *x, VD(f32) *y)
#define VD_PROC_IMGUI_GET_CURSOR_POS(PNAME)                         void               PNAME(VD(f32) *x, VD(f32) *y)
#define VD_PROC_IMGUI_SET_CURSOR_POS(PNAME)                         void               PNAME(VD(f32) x,  VD(f32) y)
#define VD_PROC_IMGUI_GET_CURSOR_START_POS(PNAME)                   void               PNAME(VD(f32) *x, VD(f32) *y)

/* ----OTHER LAYOUT FUNCTIONS---------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_SEPARATOR(PNAME)                              void               PNAME(void)
#define VD_PROC_IMGUI_SAME_LINE(PNAME)                              void               PNAME(VD(f32) offset_from_start_x /* = 0.f */, VD(f32) spacing /* = -1.f */)
#define VD_PROC_IMGUI_NEW_LINE(PNAME)                               void               PNAME(void)
#define VD_PROC_IMGUI_SPACING(PNAME)                                void               PNAME(void)
#define VD_PROC_IMGUI_DUMMY(PNAME)                                  void               PNAME(VD(f32) size_x,                          VD(f32) size_y)
#define VD_PROC_IMGUI_INDENT(PNAME)                                 void               PNAME(VD(f32) indent_w /* = 0.f */)
#define VD_PROC_IMGUI_UNINDENT(PNAME)                               void               PNAME(VD(f32) indent_w /* = 0.f */)
#define VD_PROC_IMGUI_BEGIN_GROUP(PNAME)                            void               PNAME(void)
#define VD_PROC_IMGUI_END_GROUP(PNAME)                              void               PNAME(void)
#define VD_PROC_IMGUI_ALIGN_TEXT_TO_FRAME_PADDING(PNAME)            void               PNAME(void)
#define VD_PROC_IMGUI_GET_TEXT_LINE_HEIGHT(PNAME)                   VD(f32)            PNAME(void)
#define VD_PROC_IMGUI_GET_TEXT_LINE_HEIGHT_WITH_SPACING(PNAME)      VD(f32)            PNAME(void)
#define VD_PROC_IMGUI_GET_FRAME_HEIGHT(PNAME)                       VD(f32)            PNAME(void)
#define VD_PROC_IMGUI_GET_FRAME_HEIGHT_WITH_SPACING(PNAME)          VD(f32)            PNAME(void)

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
#define VD_PROC_IMGUI_TEXTV(PNAME)                                  void               PNAME(const char *fmt, va_list args)

// @todo(mdodis): Widgets: Text
// - IMGUI_API void          TextUnformatted(const char* text, const char* text_end = NULL); // raw text without formatting. Roughly equivalent to Text("%s", text) but: A) doesn't require null terminated string if 'text_end' is specified, B) it's faster, no memory copy is done, no buffer size limits, recommended for long chunks of text.
// - IMGUI_API void          Text(const char* fmt, ...)                                      IM_FMTARGS(1); // formatted text
// - IMGUI_API void          TextColored(const ImVec4& col, const char* fmt, ...)            IM_FMTARGS(2); // shortcut for PushStyleColor(ImGuiCol_Text, col); Text(fmt, ...); PopStyleColor();
// - IMGUI_API void          TextColoredV(const ImVec4& col, const char* fmt, va_list args)  IM_FMTLIST(2);
// - IMGUI_API void          TextDisabled(const char* fmt, ...)                              IM_FMTARGS(1); // shortcut for PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]); Text(fmt, ...); PopStyleColor();
// - IMGUI_API void          TextDisabledV(const char* fmt, va_list args)                    IM_FMTLIST(1);
// - IMGUI_API void          TextWrapped(const char* fmt, ...)                               IM_FMTARGS(1); // shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();. Note that this won't work on an auto-resizing window if there's no other widgets to extend the window width, yoy may need to set a size using SetNextWindowSize().
// - IMGUI_API void          TextWrappedV(const char* fmt, va_list args)                     IM_FMTLIST(1);
// - IMGUI_API void          LabelText(const char* label, const char* fmt, ...)              IM_FMTARGS(2); // display text+label aligned the same way as value+label widgets
// - IMGUI_API void          LabelTextV(const char* label, const char* fmt, va_list args)    IM_FMTLIST(2);
// - IMGUI_API void          BulletText(const char* fmt, ...)                                IM_FMTARGS(1); // shortcut for Bullet()+Text()
// - IMGUI_API void          BulletTextV(const char* fmt, va_list args)                      IM_FMTLIST(1);
// - IMGUI_API void          SeparatorText(const char* label);                               // currently: formatted text with a horizontal line

/* ----INPUTS UTILITIES: MOUSE--------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_IS_MOUSE_DOWN(PNAME)                          VD(b32)            PNAME(VD(IMGUIMouseButton) button)
#define VD_PROC_IMGUI_IS_MOUSE_CLICKED(PNAME)                       VD(b32)            PNAME(VD(IMGUIMouseButton) button,           VD(b32) *repeat)
#define VD_PROC_IMGUI_IS_MOUSE_RELEASED(PNAME)                      VD(b32)            PNAME(VD(IMGUIMouseButton) button)
#define VD_PROC_IMGUI_IS_MOUSE_DOUBLE_CLICKED(PNAME)                VD(b32)            PNAME(VD(IMGUIMouseButton) button)
#define VD_PROC_IMGUI_IS_MOUSE_RELEASED_WITH_DELAY(PNAME)           VD(b32)            PNAME(VD(IMGUIMouseButton) button,           VD(f32) delay)
#define VD_PROC_IMGUI_GET_MOUSE_CLICKED_COUNT(PNAME)                VD(i32)            PNAME(VD(IMGUIMouseButton) button)
#define VD_PROC_IMGUI_IS_MOUSE_HOVERING_RECT(PNAME)                 VD(b32)            PNAME(VD(f32)              min_x,            VD(f32) min_y,                        VD(f32) max_x, VD(f32) max_y, VD(b32) clip /* = true */)
#define VD_PROC_IMGUI_GET_MOUSE_POS(PNAME)                          void               PNAME(VD(f32)              *x,               VD(f32) *y)
#define VD_PROC_IMGUI_GET_MOUSE_POS_ON_OPENING_CURRENT_POPUP(PNAME) void               PNAME(VD(f32)              *x,               VD(f32) *y)
#define VD_PROC_IMGUI_IS_MOUSE_DRAGGING(PNAME)                      VD(b32)            PNAME(VD(IMGUIMouseButton) button,           VD(f32) lock_threshold /* = -1.0f */)
#define VD_PROC_IMGUI_GET_MOUSE_DRAG_DELTA(PNAME)                   void               PNAME(VD(IMGUIMouseButton) button /* = 0 */, VD(f32) lock_threshold /* = -1.0f */, VD(f32) *x,    VD(f32) *y)
#define VD_PROC_IMGUI_RESET_MOUSE_DRAG_DELTA(PNAME)                 void               PNAME(VD(IMGUIMouseButton) button /* = 0 */)

/* ----IMGUI IO------------------------------------------------------------------------------------------------------ */
#define VD_PROC_IMGUI_IO_ADD_KEY_EVENT(PNAME)                       void               PNAME(VD(IMGUIKey) key,            VD(b32) down)
#define VD_PROC_IMGUI_IO_ADD_KEY_ANALOG_EVENT(PNAME)                void               PNAME(VD(IMGUIKey) key,            VD(b32) down, VD(f32) v)
#define VD_PROC_IMGUI_IO_ADD_MOUSE_POS_EVENT(PNAME)                 void               PNAME(VD(f32) x,                   VD(f32) y)
#define VD_PROC_IMGUI_IO_ADD_MOUSE_BUTTON_EVENT(PNAME)              void               PNAME(VD(i32) button,              VD(b32) down)
#define VD_PROC_IMGUI_IO_ADD_MOUSE_WHEEL_EVENT(PNAME)               void               PNAME(VD(f32) x,                   VD(f32) y)
#define VD_PROC_IMGUI_IO_ADD_MOUSE_SOURCE_EVENT(PNAME)              void               PNAME(VD(IMGUIMouseSource) source)
#define VD_PROC_IMGUI_IO_ADD_FOCUS_EVENT(PNAME)                     void               PNAME(VD(b32) focused)
#define VD_PROC_IMGUI_IO_ADD_INPUT_CHARACTER(PNAME)                 void               PNAME(VD(u32) c)
#define VD_PROC_IMGUI_IO_ADD_INPUT_CHARACTER_UTF16(PNAME)           void               PNAME(wchar_t c)
#define VD_PROC_IMGUI_IO_ADD_INPUT_CHARACTERS_UTF8(PNAME)           void               PNAME(const char *str)

/* ----IMGUI IO ACCESSORS-------------------------------------------------------------------------------------------- */
#define VD_PROC_IMGUI_IO_GET_MOUSE_DELTA(PNAME)                     void               PNAME(VD(f32) *x,                  VD(f32) *y)
// @todo(mdodis): Rest
// - IMGUI_API ImGuiMouseCursor GetMouseCursor();                                                // get desired mouse cursor shape. Important: reset in ImGui::NewFrame(), this is updated during the frame. valid before Render(). If you use software rendering by setting io.MouseDrawCursor ImGui will render those for you
// - IMGUI_API void          SetMouseCursor(ImGuiMouseCursor cursor_type);                       // set desired mouse cursor shape
// - IMGUI_API void          SetNextFrameWantCaptureMouse(bool want_capture_mouse);              // Override io.WantCaptureMouse flag next frame (said flag is left for your application to handle, typical when true it instructs your app to ignore inputs). This is equivalent to setting "io.WantCaptureMouse = want_capture_mouse;" after the next NewFrame() call.

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
    X_DEFINE(VD_PROC_IMGUI_TEXTV,                                  textv,                                   ProcIMGUITextV)                                              \
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


/* ----TYPEDEFS------------------------------------------------------------------------------------------------------ */
#define X_DEFINE(MACRO, FNAME, TNAME) typedef MACRO(VD(TNAME));
VD_IMGUI_ALL_FUNCTION_DEFINITIONS
#undef X_DEFINE

/* ----IMGUI PROCS STRUCT-------------------------------------------------------------------------------------------- */
typedef struct {
#define X_DEFINE(MACRO, FNAME, TNAME) VD(TNAME) *FNAME;
VD_IMGUI_ALL_FUNCTION_DEFINITIONS
#undef X_DEFINE
} VD(IMGUIProcs);

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
#define VD_IMGUI_TEXTV(gui,                            ...) (gui)->textv(__VA_ARGS__)
#define VD_IMGUI_TEXT(gui,                             ...) VDI(imgui_text)((gui), __VA_ARGS__)
static void VDI(imgui_text)(VD(IMGUIProcs) *procs, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    procs->textv(fmt, args);
    va_end(args);
}

/* ----INPUTS UTILITIES: MOUSE--------------------------------------------------------------------------------------- */
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

/* ----IMGUI IO------------------------------------------------------------------------------------------------------ */
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

/* ----IMGUI IO ACCESSORS-------------------------------------------------------------------------------------------- */
#define VD_IMGUI_IO_GET_MOUSE_DELTA(gui,           ...) (gui)->io_get_mouse_delta(__VA_ARGS__)

/* ----MACROS VD_CG.H------------------------------------------------------------------------------------------------ */
#ifdef VD_CG_H
#define VD_IMGUI_GET_CURSOR_SCREEN_POS_F2(gui) VDI(imgui_get_cursor_screen_pos_f2)((gui))
static VD_INLINE VD(f2) VDI(imgui_get_cursor_screen_pos_f2)(VD(IMGUIProcs) *procs)
{
    f2 result;
    procs->get_cursor_screen_pos(&result.x, &result.y);
    return result;
}

#define VD_IMGUI_SET_CURSOR_SCREEN_POS_F2(gui, f2val) VDI(imgui_set_cursor_screen_pos_f2)((gui), (f2val))
static VD_INLINE void VDI(imgui_set_cursor_screen_pos_f2)(VD(IMGUIProcs) *procs, f2 val)
{
    procs->set_cursor_screen_pos(val.x, val.y);
}

#define VD_IMGUI_IO_GET_MOUSE_DELTA_F2(gui) VDI(imgui_io_get_mouse_delta_f2)((gui))
static VD_INLINE VD(f2) VDI(imgui_io_get_mouse_delta_f2)(VD(IMGUIProcs) *procs)
{
    f2 result;
    procs->io_get_mouse_delta(&result.x, &result.y);
    return result;
}

#endif // VD_CG_H

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
static VD_PROC_IMGUI_TEXTV(VD_IMGUI_IMPL_FUNC(textv))
{
    ImGui::TextV(fmt, args);
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

static void VDF(imgui_procs_populate)(VD(IMGUIProcs) *procs)
{
#define X_DEFINE(MACRO, FNAME, TNAME) procs->FNAME = VD_IMGUI_IMPL_FUNC(FNAME);
VD_IMGUI_ALL_FUNCTION_DEFINITIONS
#undef X_DEFINE
}

#endif // VD_IMGUI_IMPL