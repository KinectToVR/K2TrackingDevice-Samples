#pragma once
#include <Amethyst_API_Devices.h>

namespace Amethyst_API_Managed
{
	__declspec(dllimport) bool Initialize();

	__declspec(dllimport) void Update();

	__declspec(dllimport) bool Shutdown();

	__declspec(dllimport) std::string GetDeviceName();

	__declspec(dllimport) long GetDeviceStatus();

	__declspec(dllimport) std::wstring GetDeviceStatusWString();

	__declspec(dllimport) uint32_t GetDeviceType();

	__declspec(dllimport) uint32_t GetDeviceCharacteristics();

	__declspec(dllimport) bool GetIsFlipSupported();

	__declspec(dllimport) bool GetIsAppOrientationSupported();

	__declspec(dllimport) bool GetIsSkeletonTracked();

	__declspec(dllimport) std::array<Eigen::Vector3f, ktvr::ITrackedJointType::Joint_Total> GetJointPositions();

	__declspec(dllimport) std::array<Eigen::Quaternionf, ktvr::ITrackedJointType::Joint_Total> GetJointOrientations();

	__declspec(dllimport) std::array<ktvr::JointTrackingState, ktvr::ITrackedJointType::Joint_Total> GetJointTrackingStates();
}


/* Not exported */

class DeviceHandler : public ktvr::K2TrackingDeviceBase_KinectBasis
{
public:
	/* K2API's things, which KTVR will make use of */

	DeviceHandler()
	{
		deviceType = Amethyst_API_Managed::GetDeviceType();
		deviceName = Amethyst_API_Managed::GetDeviceName();

		deviceCharacteristics = Amethyst_API_Managed::GetDeviceCharacteristics();
		flipSupported = Amethyst_API_Managed::GetIsFlipSupported();
		appOrientationSupported = Amethyst_API_Managed::GetIsAppOrientationSupported();
	}

	virtual ~DeviceHandler()
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
	if (0 == strcmp(ktvr::IK2API_Devices_Version, pVersionName))
	{
		static DeviceHandler TrackingHandler; // Create a new device handler -> KinectV2

		*pReturnCode = ktvr::K2InitError_None;
		return &TrackingHandler;
	}

	// Return code for initialization
	*pReturnCode = ktvr::K2InitError_BadInterface;
}
