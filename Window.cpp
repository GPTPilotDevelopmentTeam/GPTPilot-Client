#include "Window.h"

#include <XPLMUtilities.h>

bool switchState = false;

int switchMouseHandler(XPLMWindowID in_window_id, int x, int y, int is_down, void* in_refcon) {
	if (!is_down) return 0;

	int left, top, right, bottom;
	XPLMGetWindowGeometry(in_window_id, &left, &top, &right, &bottom);

	int buttonLeft = left + 20;
	int buttonRight = left + 120;
	int buttonTop = top - 50;
	int buttonBottom = top - 80;

	if (x >= buttonLeft && x <= buttonRight && y >= buttonBottom && y <= buttonTop) {
		switchState = !switchState; // 切換狀態
		XPLMDebugString(switchState ? "Button toggled ON\n" : "Button toggled OFF\n");
		return 1;
	}

	return 0;
}

void drawSwitchWindow(XPLMWindowID in_window_id, void* in_refcon) {
	XPLMSetGraphicsState(0, 0, 0, 0, 1, 1, 0);

	int left, top, right, bottom;
	XPLMGetWindowGeometry(in_window_id, &left, &top, &right, &bottom);

	float buttonColor[] = { switchState ? 0.0f : 1.0f, switchState ? 1.0f : 0.0f, 0.0f };
	XPLMDrawTranslucentDarkBox(left + 20, top - 50, left + 120, top - 80);

	const char* buttonText = switchState ? "ON" : "OFF";
	XPLMDrawString(buttonColor, left + 50, top - 65, const_cast<char*>(buttonText), nullptr, xplmFont_Proportional);
}

XPLMWindowID buildSwitchWindow() {
	XPLMWindowID gWindow;
	XPLMCreateWindow_t params;
	params.structSize = sizeof(params);
	params.visible = 1;
	params.drawWindowFunc = drawSwitchWindow;
	params.handleMouseClickFunc = switchMouseHandler;
	params.handleRightClickFunc = dummy_mouse_handler;
	params.handleMouseWheelFunc = dummy_wheel_handler;
	params.handleKeyFunc = dummy_key_handler;
	params.handleCursorFunc = dummy_cursor_status_handler;
	params.refcon = NULL;
	params.layer = xplm_WindowLayerFloatingWindows;
	params.decorateAsFloatingWindow = xplm_WindowDecorationRoundRectangle;

	int left, bottom, right, top;
	XPLMGetScreenBoundsGlobal(&left, &top, &right, &bottom);
	params.left = left + 50;
	params.bottom = bottom + 150;
	params.right = params.left + 200;
	params.top = params.bottom + 200;

	gWindow = XPLMCreateWindowEx(&params);

	XPLMSetWindowPositioningMode(gWindow, xplm_WindowPositionFree, -1);
	XPLMSetWindowResizingLimits(gWindow, 200, 200, 300, 300);
	XPLMSetWindowTitle(gWindow, "Debug window");

	return gWindow;
}
