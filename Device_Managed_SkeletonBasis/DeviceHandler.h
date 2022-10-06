#pragma once
#include <Amethyst_API_Devices.h>

namespace Amethyst_API_Managed
{
	__declspec(dllimport) void RegisterDevice(ktvr::K2TrackingDeviceBase_SkeletonBasis* handler);
	
	__declspec(dllimport) void OnLoad();

	__declspec(dllimport) bool Initialize();
	__declspec(dllimport) void Update();
	__declspec(dllimport) bool Shutdown();

	__declspec(dllimport) std::wstring GetDeviceName();
	__declspec(dllimport) std::wstring GetDeviceGUID();

	__declspec(dllimport) long GetDeviceStatus();
	__declspec(dllimport) std::wstring GetDeviceStatusWString();
	
	__declspec(dllimport) uint32_t GetDeviceCharacteristics();

	__declspec(dllimport) bool GetIsSkeletonTracked();
	__declspec(dllimport) bool GetIsFlipSupported();
	__declspec(dllimport) bool GetIsAppOrientationSupported();
	__declspec(dllimport) bool GetIsPositionFilterBlockingEnabled();
	__declspec(dllimport) bool GetIsPhysicsOverrideEnabled();
	__declspec(dllimport) bool GetIsSelfUpdateEnabled();
	__declspec(dllimport) bool GetIsSettingsDaemonSupported();

	__declspec(dllimport) std::array<ktvr::K2TrackedBaseJoint, 25> GetTrackedJoints();
}

/* Not exported */

class DeviceHandler : public ktvr::K2TrackingDeviceBase_SkeletonBasis
{
public:
	/* K2API's things, which Amethyst will make use of */

	DeviceHandler()
	{
		Amethyst_API_Managed::RegisterDevice(this);

		deviceName = Amethyst_API_Managed::GetDeviceName();
		deviceCharacteristics = 
			static_cast<ktvr::ITrackingDeviceCharacteristics>(
				Amethyst_API_Managed::GetDeviceCharacteristics());

		skeletonTracked =
			Amethyst_API_Managed::GetIsSkeletonTracked();
		Flags_FlipSupported = 
			Amethyst_API_Managed::GetIsFlipSupported();
		Flags_AppOrientationSupported = 
			Amethyst_API_Managed::GetIsAppOrientationSupported();
		Flags_BlocksPositionFiltering =
			Amethyst_API_Managed::GetIsPositionFilterBlockingEnabled();
		Flags_OverridesJointPhysics =
			Amethyst_API_Managed::GetIsPhysicsOverrideEnabled();
		Flags_ForceSelfUpdate =
			Amethyst_API_Managed::GetIsSelfUpdateEnabled();
		Flags_SettingsSupported =
			Amethyst_API_Managed::GetIsSettingsDaemonSupported();
	}

	std::wstring getDeviceGUID() override
	{
		// This ID is unique to this plugin!
		return Amethyst_API_Managed::GetDeviceGUID();
	}

	void onLoad() override
	{
		Amethyst_API_Managed::OnLoad();
	}

	~DeviceHandler() override
	{
	}

	HRESULT getStatusResult() override;
	std::wstring statusResultWString(HRESULT) override;

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
