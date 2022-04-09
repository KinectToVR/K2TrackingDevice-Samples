#pragma once
#include "KinectToVR_API_Devices.h"

/* Not exported */

class DeviceHandler : public ktvr::K2TrackingDeviceBase_KinectBasis
{
public:
	/* K2API's things, which KTVR will make use of */

	DeviceHandler()
	{
		deviceType = ktvr::K2_Kinect;
		deviceName = "KinectBasis (Simple)";

		deviceCharacteristics = ktvr::K2_Character_Simple;
		flipSupported = true;
		appOrientationSupported = true;
	}

	virtual ~DeviceHandler()
	{
	}

	HRESULT getStatusResult() override;
	std::string statusResultString(HRESULT stat) override;

	void initialize() override;
	void update() override;
	void shutdown() override;
};

/* Exported for dynamic linking */
extern "C" __declspec(dllexport) void* TrackingDeviceBaseFactory(
	const char* pVersionName, int* pReturnCode)
{
	// Return the device handler for tracking
	// but only if interfaces are the same / up-to-date
	if (0 == strcmp(ktvr::IK2API_Devices_Version, pVersionName))
	{
		static DeviceHandler TrackingHandler; // Create a new device handler -> KinectBasis_Simple

		*pReturnCode = ktvr::K2InitError_None;
		return &TrackingHandler;
	}

	// Return code for initialization
	*pReturnCode = ktvr::K2InitError_BadInterface;
}
