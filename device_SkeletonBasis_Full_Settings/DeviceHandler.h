#pragma once
#include "Amethyst_API_Devices.h"

/* Not exported */

class DeviceHandler : public ktvr::K2TrackingDeviceBase_SkeletonBasis
{
public:
	/* K2API's things, which KTVR will make use of */

	DeviceHandler()
	{
		deviceName = L"SkeletonBasis (Full, Settings)";

		deviceCharacteristics = ktvr::K2_Character_Full;
		Flags_FlipSupported = true;
		Flags_AppOrientationSupported = true;

		// Mark that our device supports settings
		Flags_SettingsSupported = true;
	}

	virtual ~DeviceHandler()
	{
	}

	std::wstring getDeviceGUID() override
	{
		// This ID is unique to this plugin!
		return L"KSAMPLES-VEND-API1-DVCE-DVCESKLTNFST";
	}

	void onLoad() override
	{
		// Construct the device's settings here

		// You can put anything here,
		// from textblocks (labels) to numberboxes
		// Just remember to register handlers
		//   and optionally cache the pointers as shared
		layoutRoot->AppendSingleElement(
			CreateTextBlock(
				L"In the beginning was the Word."));

		layoutRoot->AppendSingleElement(
			CreateTextBlock(
				L"  But this device's settings are something else..."));
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
		static DeviceHandler TrackingHandler; // Create a new device handler -> KinectV2

		*pReturnCode = ktvr::K2InitError_None;
		return &TrackingHandler;
	}

	// Return code for initialization
	*pReturnCode = ktvr::K2InitError_BadInterface;
}
