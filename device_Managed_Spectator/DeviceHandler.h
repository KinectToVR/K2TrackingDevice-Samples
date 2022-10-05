#pragma once
#include <Amethyst_API_Devices.h>

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>

namespace Amethyst_API_Managed
{
	__declspec(dllimport) void RegisterDevice(ktvr::K2TrackingDeviceBase_Spectator* handler);

	__declspec(dllimport) void OnLoad();
}

/* Not exported */

class DeviceHandler : public ktvr::K2TrackingDeviceBase_Spectator
{
public:
	/* K2API's things, which Amethyst will make use of */

	DeviceHandler()
	{
		Amethyst_API_Managed::RegisterDevice(this);
	}

	void onLoad() override
	{
		Amethyst_API_Managed::OnLoad();
	}

	virtual ~DeviceHandler()
	{
	}
};

/* Exported for dynamic linking */
extern "C" __declspec(dllexport) void* TrackingDeviceBaseFactory(
	const char* pVersionName, int* pReturnCode)
{
	// Return the device handler for tracking
	// but only if interfaces are the same / up-to-date
	if (0 == strcmp(ktvr::IAME_API_Devices_Version, pVersionName))
	{
		static DeviceHandler TrackingHandler; // Create a new device handler -> KinectV2

		*pReturnCode = ktvr::K2InitError_None;
		return &TrackingHandler;
	}

	// Return code for initialization
	*pReturnCode = ktvr::K2InitError_BadInterface;
}
