#pragma once
#include <Amethyst_API_Devices.h>

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>

namespace Amethyst_API_Managed
{
	__declspec(dllimport) void RegisterLogger(std::function<void(std::wstring, unsigned)> handler);

	__declspec(dllimport) void Register_getHMDPose(std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> handler);
	__declspec(dllimport) void Register_getHMDPoseCalibrated(std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> handler);
	__declspec(dllimport) void Register_getLeftControllerPose(std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> handler);
	__declspec(dllimport) void Register_getLeftControllerPoseCalibrated(std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> handler);
	__declspec(dllimport) void Register_getRightControllerPose(std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> handler);
	__declspec(dllimport) void Register_getRightControllerPoseCalibrated(std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> handler);
	__declspec(dllimport) void Register_getHMDOrientationYaw(std::function<double()> handler);
	__declspec(dllimport) void Register_getHMDOrientationYawCalibrated(std::function<double()> handler);
	__declspec(dllimport) void Register_getAppJointPoses(std::function<std::vector<ktvr::K2TrackedJoint>()> handler);

	__declspec(dllimport) void OnLoad();
}

inline void log_to_ame(const std::wstring& message, unsigned severity)
{
	// GLOG_INFO = 0, GLOG_WARNING = 1, GLOG_ERROR = 2
	switch (severity)
	{
	case 0:
		LOG(INFO) << WStringToString(message);
		return;
	case 1:
		LOG(WARNING) << WStringToString(message);
		return;
	case 2:
		LOG(ERROR) << WStringToString(message);
		return;
	}
}

/* Not exported */

class DeviceHandler : public ktvr::K2TrackingDeviceBase_Spectator
{
public:
	/* K2API's things, which Amethyst will make use of */

	DeviceHandler()
	{
		Amethyst_API_Managed::RegisterLogger(log_to_ame);
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
