#pragma once

#include <XPLMDisplay.h>
#include <XPLMGraphics.h>

extern bool switchState;

inline int					dummy_mouse_handler(XPLMWindowID in_window_id, int x, int y, int is_down, void * in_refcon) { return 0; }
inline XPLMCursorStatus	dummy_cursor_status_handler(XPLMWindowID in_window_id, int x, int y, void * in_refcon) { return xplm_CursorDefault; }
inline int					dummy_wheel_handler(XPLMWindowID in_window_id, int x, int y, int wheel, int clicks, void * in_refcon) { return 0; }
inline void				dummy_key_handler(XPLMWindowID in_window_id, char key, XPLMKeyFlags flags, char virtual_key, void * in_refcon, int losing_focus) { }

int switchMouseHandler(XPLMWindowID in_window_id, int x, int y, int is_down, void* in_refcon);
void drawSwitchWindow(XPLMWindowID in_window_id, void* in_refcon);

XPLMWindowID buildSwitchWindow();
