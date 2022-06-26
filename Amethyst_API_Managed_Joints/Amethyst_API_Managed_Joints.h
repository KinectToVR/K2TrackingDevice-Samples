#pragma once
#include <Amethyst_API_Devices.h>

inline void MarshalString(System::String^ s, std::string& os)
{
	auto chars =
		static_cast<const char*>((System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(s)).ToPointer());
	os = chars;
	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}

inline void MarshalString(System::String^ s, std::wstring& os)
{
	auto chars =
		static_cast<const wchar_t*>((System::Runtime::InteropServices::Marshal::StringToHGlobalUni(s)).ToPointer());
	os = chars;
	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}

// Logging handler (native)
inline std::function<void(std::string, unsigned)> LogToAme_Handler;

inline std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> GetHMDPose_Handler;
inline std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> GetHMDPoseCalibrated_Handler;
inline std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> GetLeftControllerPose_Handler;
inline std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> GetLeftControllerPoseCalibrated_Handler;
inline std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> GetRightControllerPose_Handler;
inline std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> GetRightControllerPoseCalibrated_Handler;
inline std::function<float()> GetHMDOrientationYaw_Handler;
inline std::function<float()> GetHMDOrientationYawCalibrated_Handler;
inline std::function<std::array<ktvr::K2TrackedJoint, 7>()> GetAppJointPoses_Handler;

// Logging handler (managed)
inline void LogToAme(System::String^ message, unsigned severity)
{
	std::string out; // Dummy placeholder
	MarshalString(message, out);

	// Finally call the logger
	LogToAme_Handler(out, severity);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetHMDPose()
{
	const auto& [position, orientation] = GetHMDPose_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3(
			position.x(),
			position.y(),
			position.z()),
		System::Numerics::Quaternion(
			orientation.x(),
			orientation.y(),
			orientation.z(),
			orientation.w())
		);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetHMDPoseCalibrated()
{
	const auto& [position, orientation] = GetHMDPoseCalibrated_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3(
			position.x(),
			position.y(),
			position.z()),
		System::Numerics::Quaternion(
			orientation.x(),
			orientation.y(),
			orientation.z(),
			orientation.w())
		);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetLeftControllerPose()
{
	const auto& [position, orientation] = GetLeftControllerPose_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3(
			position.x(),
			position.y(),
			position.z()),
		System::Numerics::Quaternion(
			orientation.x(),
			orientation.y(),
			orientation.z(),
			orientation.w())
		);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetLeftControllerPoseCalibrated()
{
	const auto& [position, orientation] = GetLeftControllerPoseCalibrated_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3(
			position.x(),
			position.y(),
			position.z()),
		System::Numerics::Quaternion(
			orientation.x(),
			orientation.y(),
			orientation.z(),
			orientation.w())
		);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetRightControllerPose()
{
	const auto& [position, orientation] = GetRightControllerPose_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3(
			position.x(),
			position.y(),
			position.z()),
		System::Numerics::Quaternion(
			orientation.x(),
			orientation.y(),
			orientation.z(),
			orientation.w())
		);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetRightControllerPoseCalibrated()
{
	const auto& [position, orientation] = GetRightControllerPoseCalibrated_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3(
			position.x(),
			position.y(),
			position.z()),
		System::Numerics::Quaternion(
			orientation.x(),
			orientation.y(),
			orientation.z(),
			orientation.w())
		);
}

inline float GetHMDOrientationYaw()
{
	return GetHMDOrientationYaw();
}

inline float GetHMDOrientationYawCalibrated()
{
	return GetHMDOrientationYawCalibrated();
}

inline array<Device_Managed_JointsBasis::API_Projection_Files::TrackedJoint^>^ GetAppJointPoses()
{
	auto joints = GetAppJointPoses_Handler();
	auto tracker_array = gcnew array<Device_Managed_JointsBasis::API_Projection_Files::TrackedJoint^>(joints.size());

	for (size_t i = 0; i < joints.size(); i++)
	{
		const auto& position = joints[i].getJointPosition();
		const auto& orientation = joints[i].getJointOrientation();

		tracker_array[i] = gcnew Device_Managed_JointsBasis::API_Projection_Files::TrackedJoint{
			System::Numerics::Vector3(
				position.x(),
				position.y(),
				position.z()),
			System::Numerics::Quaternion(
				orientation.x(),
				orientation.y(),
				orientation.z(),
				orientation.w()),
			gcnew System::String(joints[i].getJointName().c_str())
		};
	}

	return tracker_array;
}

namespace Amethyst_API_Managed_Wrapper
{
	public ref class Amethyst_API_Managed_Device
	{
	public:
		Amethyst_API_Managed_Device()
		{
			device_handler_.LoggerAction = gcnew System::Action<System::String^, unsigned>(&LogToAme);

			device_handler_.GetHMDPose = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&GetHMDPose);
			device_handler_.GetHMDPoseCalibrated = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&GetHMDPoseCalibrated);
			device_handler_.GetLeftControllerPose = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&GetLeftControllerPose);
			device_handler_.GetLeftControllerPoseCalibrated = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&GetLeftControllerPoseCalibrated);
			device_handler_.GetRightControllerPose = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&GetRightControllerPose);
			device_handler_.GetRightControllerPoseCalibrated = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&GetRightControllerPoseCalibrated);
			device_handler_.GetHMDOrientationYaw = gcnew System::Func<float>(&GetHMDOrientationYaw);
			device_handler_.GetHMDOrientationYawCalibrated = gcnew System::Func<float>(&GetHMDOrientationYawCalibrated);
			device_handler_.GetAppJointPoses = gcnew System::Func<array<
				Device_Managed_JointsBasis::API_Projection_Files::TrackedJoint^>^>(&GetAppJointPoses);
		}

		bool Initialize()
		{
			return device_handler_.Initialize();
		}

		void Update()
		{
			device_handler_.Update();
		}

		bool Shutdown()
		{
			return device_handler_.Shutdown();
		}

		System::String^ GetDeviceName()
		{
			return device_handler_.DeviceName;
		}

		long GetDeviceStatus()
		{
			return device_handler_.GetDeviceStatus();
		}

		System::String^ GetDeviceStatusString()
		{
			return device_handler_.GetDeviceStatusString();
		}

		unsigned GetDeviceType()
		{
			return device_handler_.DeviceType;
		}

		bool GetIsSkeletonTracked()
		{
			return device_handler_.IsSkeletonTracked;
		}

		auto GetTrackedJoints()
		{
			return device_handler_.JointsList;
		}

	private:
		Device_Managed_JointsBasis::AmethystDevice device_handler_;
	};
}

ref struct IAmethyst_API_Managed
{
	static Amethyst_API_Managed_Wrapper::Amethyst_API_Managed_Device^ wrapper =
		gcnew Amethyst_API_Managed_Wrapper::Amethyst_API_Managed_Device;
};

namespace Amethyst_API_Managed
{
	__declspec(dllexport) void RegisterLogger(std::function<void(std::string, unsigned)> handler)
	{
		LogToAme_Handler = handler;
	}

	__declspec(dllexport) void Register_getHMDPose(
		std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> handler)
	{
		GetHMDPose_Handler = handler;
	}

	__declspec(dllexport) void Register_getHMDPoseCalibrated(
		std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> handler)
	{
		GetHMDPoseCalibrated_Handler = handler;
	}

	__declspec(dllexport) void Register_getLeftControllerPose(
		std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> handler)
	{
		GetLeftControllerPose_Handler = handler;
	}

	__declspec(dllexport) void Register_getLeftControllerPoseCalibrated(
		std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> handler)
	{
		GetLeftControllerPoseCalibrated_Handler = handler;
	}

	__declspec(dllexport) void Register_getRightControllerPose(
		std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> handler)
	{
		GetRightControllerPose_Handler = handler;
	}

	__declspec(dllexport) void Register_getRightControllerPoseCalibrated(
		std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> handler)
	{
		GetRightControllerPoseCalibrated_Handler = handler;
	}

	__declspec(dllexport) void Register_getHMDOrientationYaw(std::function<float()> handler)
	{
		GetHMDOrientationYaw_Handler = handler;
	}

	__declspec(dllexport) void Register_getHMDOrientationYawCalibrated(std::function<float()> handler)
	{
		GetHMDOrientationYawCalibrated_Handler = handler;
	}

	__declspec(dllexport) void Register_getAppJointPoses(std::function<std::array<ktvr::K2TrackedJoint, 7>()> handler)
	{
		GetAppJointPoses_Handler = handler;
	}

	__declspec(dllexport) bool Initialize()
	{
		return IAmethyst_API_Managed::wrapper->Initialize();
	}

	__declspec(dllexport) void Update()
	{
		IAmethyst_API_Managed::wrapper->Update();
	}

	__declspec(dllexport) bool Shutdown()
	{
		return IAmethyst_API_Managed::wrapper->Shutdown();
	}

	__declspec(dllexport) std::string GetDeviceName()
	{
		std::string out; // Dummy placeholder
		MarshalString(IAmethyst_API_Managed::wrapper->GetDeviceName(), out);
		return out; // Return what we've got
	}

	__declspec(dllexport) long GetDeviceStatus()
	{
		return IAmethyst_API_Managed::wrapper->GetDeviceStatus();
	}

	__declspec(dllexport) std::wstring GetDeviceStatusWString()
	{
		std::wstring out; // Dummy placeholder
		MarshalString(IAmethyst_API_Managed::wrapper->GetDeviceStatusString(), out);
		return out; // Return what we've got
	}

	__declspec(dllexport) uint32_t GetDeviceType()
	{
		return IAmethyst_API_Managed::wrapper->GetDeviceType();
	}

	__declspec(dllexport) bool GetIsSkeletonTracked()
	{
		return IAmethyst_API_Managed::wrapper->GetIsSkeletonTracked();
	}

	__declspec(dllexport) std::vector<ktvr::K2TrackedJoint> GetTrackedJoints()
	{
		std::vector<ktvr::K2TrackedJoint> cluster_joints;
		const auto& joints = IAmethyst_API_Managed::wrapper->GetTrackedJoints();

		for (size_t i = 0; i < joints->Count; i++)
		{
			std::string joint_name; // Dummy placeholder
			MarshalString(joints[i]->JointName, joint_name);

			Eigen::Vector3f joint_position;
			joint_position.x() = joints[i]->Position.X;
			joint_position.y() = joints[i]->Position.Y;
			joint_position.z() = joints[i]->Position.Z;

			Eigen::Quaternionf joint_orientation;
			joint_orientation.w() = joints[i]->Orientation.W;
			joint_orientation.x() = joints[i]->Orientation.X;
			joint_orientation.y() = joints[i]->Orientation.Y;
			joint_orientation.z() = joints[i]->Orientation.Z;

			cluster_joints.push_back(
				ktvr::K2TrackedJoint(
					joint_position,
					joint_orientation,
					static_cast<ktvr::JointTrackingState>(joints[i]->TrackingState),
					joint_name
				));
		}

		return cluster_joints;
	}
}
