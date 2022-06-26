#pragma once
#include <Amethyst_API_Devices.h>

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>

namespace Amethyst_API_Managed
{
	__declspec(dllimport) void RegisterLogger(std::function<void(std::string, unsigned)> handler);

	__declspec(dllimport) void Register_getHMDPose(std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> handler);
	__declspec(dllimport) void Register_getHMDPoseCalibrated(std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> handler);
	__declspec(dllimport) void Register_getLeftControllerPose(std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> handler);
	__declspec(dllimport) void Register_getLeftControllerPoseCalibrated(std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> handler);
	__declspec(dllimport) void Register_getRightControllerPose(std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> handler);
	__declspec(dllimport) void Register_getRightControllerPoseCalibrated(std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> handler);
	__declspec(dllimport) void Register_getHMDOrientationYaw(std::function<float()> handler);
	__declspec(dllimport) void Register_getHMDOrientationYawCalibrated(std::function<float()> handler);
	__declspec(dllimport) void Register_getAppJointPoses(std::function<std::array<ktvr::K2TrackedJoint, 7>()> handler);

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

inline void log_to_ame(const std::string& message, unsigned severity)
{
	// GLOG_INFO = 0, GLOG_WARNING = 1, GLOG_ERROR = 2
	switch (severity)
	{
	case 0:
		LOG(INFO) << message;
		return;
	case 1:
		LOG(WARNING) << message;
		return;
	case 2:
		LOG(ERROR) << message;
		return;
	}
}

/* Not exported */

class DeviceHandler : public ktvr::K2TrackingDeviceBase_KinectBasis
{
public:
	/* K2API's things, which KTVR will make use of */

	DeviceHandler()
	{
		Amethyst_API_Managed::RegisterLogger(log_to_ame);
		
		deviceType = Amethyst_API_Managed::GetDeviceType();
		deviceName = Amethyst_API_Managed::GetDeviceName();

		deviceCharacteristics = Amethyst_API_Managed::GetDeviceCharacteristics();
		flipSupported = Amethyst_API_Managed::GetIsFlipSupported();
		appOrientationSupported = Amethyst_API_Managed::GetIsAppOrientationSupported();
	}

	void onLoad() override
	{
		Amethyst_API_Managed::Register_getHMDPose(getHMDPose);
		Amethyst_API_Managed::Register_getHMDPoseCalibrated(getHMDPoseCalibrated);
		Amethyst_API_Managed::Register_getLeftControllerPose(getLeftControllerPose);
		Amethyst_API_Managed::Register_getLeftControllerPoseCalibrated(getLeftControllerPoseCalibrated);
		Amethyst_API_Managed::Register_getRightControllerPose(getRightControllerPose);
		Amethyst_API_Managed::Register_getRightControllerPoseCalibrated(getRightControllerPoseCalibrated);
		Amethyst_API_Managed::Register_getHMDOrientationYaw(getHMDOrientationYaw);
		Amethyst_API_Managed::Register_getHMDOrientationYawCalibrated(getHMDOrientationYawCalibrated);
		Amethyst_API_Managed::Register_getAppJointPoses(getAppJointPoses);
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
	if (0 == strcmp(ktvr::IK2API_Devices_Version, pVersionName))
	{
		static DeviceHandler TrackingHandler; // Create a new device handler -> KinectV2

		*pReturnCode = ktvr::K2InitError_None;
		return &TrackingHandler;
	}

	// Return code for initialization
	*pReturnCode = ktvr::K2InitError_BadInterface;
}
