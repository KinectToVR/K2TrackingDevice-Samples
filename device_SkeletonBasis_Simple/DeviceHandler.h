#pragma once
#include "Amethyst_API_Devices.h"

/* Not exported */

class DeviceHandler : public ktvr::K2TrackingDeviceBase_SkeletonBasis
{
public:
	/* K2API's things, which KTVR will make use of */

	DeviceHandler()
	{
		deviceName = L"SkeletonBasis (Simple)";

		deviceCharacteristics = ktvr::K2_Character_Simple;
		flipSupported = true;
		appOrientationSupported = true;
	}

	virtual ~DeviceHandler()
	{
	}

	HRESULT getStatusResult() override;
	std::wstring statusResultWString(HRESULT stat) override;

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
	if (0 == strcmp(ktvr::IAME_API_Devices_Version, pVersionName))
	{
		static DeviceHandler TrackingHandler; // Create a new device handler -> SkeletonBasis_Simple

		*pReturnCode = ktvr::K2InitError_None;
		return &TrackingHandler;
	}

	// Return code for initialization
	*pReturnCode = ktvr::K2InitError_BadInterface;
}
