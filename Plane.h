#pragma once

#include <string>
#include <sstream>
#include <nlohmann/json.hpp>
#include "XPLMUtilities.h"
#include "XPLMDataAccess.h"

using json = nlohmann::json;

class Plane {
public:
	Plane();

	void executeAircraftCommands(const std::string& jsonStr);
	void setThrottle(float ratio);
	void setFlap(float ratio);
	void setGear(int down);
	void setSpeedbrake(float ratio);
	void setHandbrake(float ratio);
	void setAutopilotHeading(float heading);
	void setAutopilotAltitude(float altitude);
	void setAutopilotAirspeed(float speed);
	void setAutopilotVerticalSpeed(float fpm);

private:
	XPLMDataRef throttleRef;
	XPLMDataRef flapRef;
	XPLMDataRef gearRef;
	XPLMDataRef spdbkRef;
	XPLMDataRef handbkRef;

	XPLMDataRef apHeadingRef;
	XPLMDataRef apAltitudeRef;
	XPLMDataRef apAirspeedRef;
	XPLMDataRef apAirspeedUnitRef;
	XPLMDataRef apVSRef;
};