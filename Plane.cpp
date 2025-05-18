#include "Plane.h"

Plane::Plane() {
    throttleRef = XPLMFindDataRef("sim/cockpit2/engine/actuators/throttle_ratio_all");
    flapRef = XPLMFindDataRef("sim/cockpit2/controls/flap_handle_request_ratio");
    gearRef = XPLMFindDataRef("sim/cockpit2/controls/gear_handle_request");
    spdbkRef = XPLMFindDataRef("sim/cockpit2/controls/speedbrake_ratio");
    handbkRef = XPLMFindDataRef("sim/cockpit2/controls/parking_brake_ratio");

    apHeadingRef = XPLMFindDataRef("sim/cockpit/autopilot/heading_mag");
    apAltitudeRef = XPLMFindDataRef("sim/cockpit/autopilot/altitude");
    apAirspeedRef = XPLMFindDataRef("sim/cockpit/autopilot/airspeed");
    apAirspeedUnitRef = XPLMFindDataRef("sim/cockpit/autopilot/airspeed_is_mach");
    apVSRef = XPLMFindDataRef("sim/cockpit/autopilot/vertical_velocity");
}

void Plane::setThrottle(float ratio) {
	if (throttleRef != nullptr) {
		XPLMSetDataf(throttleRef, ratio);
	}
}

void Plane::setFlap(float ratio) {
	if (flapRef != nullptr) {
		XPLMSetDataf(flapRef, ratio);
	}
}

void Plane::setGear(int ratio) {
    if (gearRef) {
        XPLMSetDatai(gearRef, ratio);
    }
}

void Plane::setSpeedbrake(float ratio) {
    if (spdbkRef) {
        XPLMSetDataf(spdbkRef, ratio);
    }
}

void Plane::setHandbrake(float ratio) {
    if (handbkRef) {
        XPLMSetDataf(handbkRef, ratio);
    }
}

void Plane::setAutopilotHeading(float heading) {
    if (apHeadingRef) XPLMSetDataf(apHeadingRef, heading);
}

void Plane::setAutopilotAltitude(float alt) {
    if (apAltitudeRef) XPLMSetDataf(apAltitudeRef, alt);
}

void Plane::setAutopilotAirspeed(float speed) {
    if (apAirspeedUnitRef && apAirspeedRef) {
        XPLMSetDatai(apAirspeedUnitRef, 0);
        XPLMSetDataf(apAirspeedRef, speed);
    }
}

void Plane::setAutopilotVerticalSpeed(float fpm) {
    if (apVSRef) XPLMSetDataf(apVSRef, fpm);
}

void Plane::executeAircraftCommands(const std::string& jsonStr) {
    try {
        auto data = json::parse(jsonStr);

        for (const auto& command : data) {
            std::string action = command.value("action", "");
            std::string operation = command.value("operation", "");
            float target = command.value("target_value", 0.0f);

            if (action == "operate_aircraft") {
                std::ostringstream msg;

                if (operation == "throttle") {
                    setThrottle(target);
                    msg << "[XPLM] Set throttle to " << target << "\n";
                }
                else if (operation == "flap") {
                    setFlap(target);
                    msg << "[XPLM] Set flap to " << target << "\n";
                }
                else if (operation == "gear") {
                    setGear(target);
                    msg << "[XPLM] Set gear " << target << "\n";
                }
                else if (operation == "speedbrake") {
                    setSpeedbrake(target);
                    msg << "[XPLM] Set speedbrake to " << target << "\n";
                }
                else if (operation == "handbrake") {
                    setHandbrake(target);
                    msg << "[XPLM] Set handbrake to " << target << "\n";
                }
                else if (operation == "ap_heading") {
                    setAutopilotHeading(target);
                    msg << "[XPLM] Set AP heading to " << target << "\n";
                }
                else if (operation == "ap_altitude") {
                    setAutopilotAltitude(target);
                    msg << "[XPLM] Set AP altitude to " << target << "\n";
                }
                else if (operation == "ap_airspeed") {
                    setAutopilotAirspeed(target);
                    msg << "[XPLM] Set AP airspeed to " << target << "\n";
                }
                else if (operation == "ap_vs") {
                    setAutopilotVerticalSpeed(target);
                    msg << "[XPLM] Set AP vertical speed to " << target << "\n";
                }
                else {
                    msg << "[XPLM] Unknown operation: " << operation << "\n";
                }

                XPLMDebugString(msg.str().c_str());
            }
        }

    }
    catch (json::parse_error& e) {
        std::string err = "[XPLM] JSON Parse Error: ";
        err += e.what();
        err += "\n";
        XPLMDebugString(err.c_str());
    }
    catch (std::exception& e) {
        std::string err = "[XPLM] Execution Error: ";
        err += e.what();
        err += "\n";
        XPLMDebugString(err.c_str());
    }
}
