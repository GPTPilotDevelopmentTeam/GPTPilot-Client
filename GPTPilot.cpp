#include <string.h>
#include <string>
#include <XPLMProcessing.h>
#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>

#include "Connector.h"
#include "Decoder.h"
#include "Plane.h"
#include "Window.h"

Connector* server;
Plane* planeRef;
XPLMWindowID gSwitchWindow;

void callback(const char* info) {
	std::string jsonStr = std::string(info);
	planeRef->executeAircraftCommands(jsonStr);
}

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc) {
	strcpy(outName, "GPT Pilot");
	strcpy(outSig, "wallace611.gptpilot");
	strcpy(outDesc, "fly as an AI");

	gSwitchWindow = buildSwitchWindow();

	return 1;
}

PLUGIN_API void	XPluginStop(void) { }

PLUGIN_API void XPluginDisable(void) {
	XPLMDebugString("GPTPilot: You've disabled GPTPilot.\n");
	server->Close();
	XPLMDebugString("GPTPilot: Connection closed.\n");
	free(server);
}

PLUGIN_API int XPluginEnable(void) {
	XPLMDebugString("GPTPilot: You've enabled GPTPilot, Have fun!\n");
	server = new Connector();
	if (!server) {
		XPLMDebugString("GPTPilot: Failed to start");
		return 0;
	}
	server->SetRecvCallback(callback);
	XPLMDebugString("GPTPilot: Connecting server...\n");
	if (!server->Connect()) {
		XPLMDebugString("GPTPilot: Connection failed.\n");
		return 0;
	}
	XPLMDebugString("GPTPilot: Connection established!\n");

	planeRef = new Plane();
	if (!planeRef) {
		delete server;
		XPLMDebugString("GPTPilot: Connection failed.\n");
		return 0;
	}
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFromWho, int inMessage, void* inParam) { }

