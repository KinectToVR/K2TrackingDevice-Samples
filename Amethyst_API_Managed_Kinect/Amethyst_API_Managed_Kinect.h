#pragma once
#include <Amethyst_API_Devices.h>

inline void MarshalString(System::String^ s, std::wstring& os)
{
	auto chars =
		(const wchar_t*)(System::Runtime::InteropServices::Marshal::StringToHGlobalUni(s)).ToPointer();
	os = chars;
	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}

// Logging handler (native)
inline std::function<void(std::wstring, unsigned)> LogToAme_Handler;

inline std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> GetHMDPose_Handler;
inline std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> GetHMDPoseCalibrated_Handler;
inline std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> GetLeftControllerPose_Handler;
inline std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> GetLeftControllerPoseCalibrated_Handler;
inline std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> GetRightControllerPose_Handler;
inline std::function<std::pair<Eigen::Vector3f, Eigen::Quaternionf>()> GetRightControllerPoseCalibrated_Handler;
inline std::function<float()> GetHMDOrientationYaw_Handler;
inline std::function<float()> GetHMDOrientationYawCalibrated_Handler;
inline std::function<std::vector<ktvr::K2TrackedJoint>()> GetAppJointPoses_Handler;

// Logging handler (managed)
inline void LogToAme(System::String^ message, unsigned severity)
{
	std::wstring out; // Dummy placeholder
	MarshalString(message, out);

	// Finally call the logger
	LogToAme_Handler(out, severity);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetHMDPose()
{
	const auto& [position, orientation] = GetHMDPose_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3{
			position.x(),
			position.y(),
			position.z()
		},
		System::Numerics::Quaternion{
			orientation.w(),
			orientation.x(),
			orientation.y(),
			orientation.z()
		}
	);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetHMDPoseCalibrated()
{
	const auto& [position, orientation] = GetHMDPoseCalibrated_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3{
			position.x(),
			position.y(),
			position.z()
		},
		System::Numerics::Quaternion{
			orientation.w(),
			orientation.x(),
			orientation.y(),
			orientation.z()
		}
	);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetLeftControllerPose()
{
	const auto& [position, orientation] = GetLeftControllerPose_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3{
			position.x(),
			position.y(),
			position.z()
		},
		System::Numerics::Quaternion{
			orientation.w(),
			orientation.x(),
			orientation.y(),
			orientation.z()
		}
	);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetLeftControllerPoseCalibrated()
{
	const auto& [position, orientation] = GetLeftControllerPoseCalibrated_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3{
			position.x(),
			position.y(),
			position.z()
		},
		System::Numerics::Quaternion{
			orientation.w(),
			orientation.x(),
			orientation.y(),
			orientation.z()
		}
	);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetRightControllerPose()
{
	const auto& [position, orientation] = GetRightControllerPose_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3{
			position.x(),
			position.y(),
			position.z()
		},
		System::Numerics::Quaternion{
			orientation.w(),
			orientation.x(),
			orientation.y(),
			orientation.z()
		}
	);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetRightControllerPoseCalibrated()
{
	const auto& [position, orientation] = GetRightControllerPoseCalibrated_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3{
			position.x(),
			position.y(),
			position.z()
		},
		System::Numerics::Quaternion{
			orientation.w(),
			orientation.x(),
			orientation.y(),
			orientation.z()
		}
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

inline array<Device_Managed_SkeletonBasis::API_Projection_Files::TrackedJoint^>^ GetAppJointPoses()
{
	auto joints = GetAppJointPoses_Handler();
	auto tracker_array = gcnew array<Device_Managed_SkeletonBasis::API_Projection_Files::TrackedJoint^>(joints.size());

	for (size_t i = 0; i < joints.size(); i++)
	{
		const auto& position = joints[i].getJointPosition();
		const auto& orientation = joints[i].getJointOrientation();

		tracker_array[i] = gcnew Device_Managed_SkeletonBasis::API_Projection_Files::TrackedJoint{
			System::Numerics::Vector3{
				position.x(),
				position.y(),
				position.z()
			},
			System::Numerics::Quaternion{
				orientation.w(),
				orientation.x(),
				orientation.y(),
				orientation.z()
			},
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
				Device_Managed_SkeletonBasis::API_Projection_Files::TrackedJoint^>^>(&GetAppJointPoses);
		}

		void OnLoad()
		{
			device_handler_.OnLoad();
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

		unsigned GetDeviceCharacteristics()
		{
			return device_handler_.DeviceCharacteristics;
		}

		bool GetIsFlipSupported()
		{
			return device_handler_.IsFlipSupported;
		}

		bool GetIsAppOrientationSupported()
		{
			return device_handler_.IsAppOrientationSupported;
		}

		bool GetIsSkeletonTracked()
		{
			return device_handler_.IsSkeletonTracked;
		}

		array<System::Numerics::Vector3>^ GetJointPositions()
		{
			return device_handler_.JointPositions;
		}

		array<System::Numerics::Quaternion>^ GetJointOrientations()
		{
			return device_handler_.JointOrientations;
		}

		array<unsigned>^ GetJointTrackingStates()
		{
			return device_handler_.JointTrackingStates;
		}

	private:
		Device_Managed_SkeletonBasis::AmethystDevice device_handler_;
	};
}

ref struct IAmethyst_API_Managed
{
	static Amethyst_API_Managed_Wrapper::Amethyst_API_Managed_Device^ wrapper =
		gcnew Amethyst_API_Managed_Wrapper::Amethyst_API_Managed_Device;
};

namespace Amethyst_API_Managed
{
	__declspec(dllexport) void RegisterLogger(std::function<void(std::wstring, unsigned)> handler)
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

	__declspec(dllexport) void Register_getAppJointPoses(std::function<std::vector<ktvr::K2TrackedJoint>()> handler)
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

	__declspec(dllexport) std::wstring GetDeviceName()
	{
		std::wstring out; // Dummy placeholder
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

	__declspec(dllexport) uint32_t GetDeviceCharacteristics()
	{
		return IAmethyst_API_Managed::wrapper->GetDeviceCharacteristics();
	}

	__declspec(dllexport) bool GetIsFlipSupported()
	{
		return IAmethyst_API_Managed::wrapper->GetIsFlipSupported();
	}

	__declspec(dllexport) bool GetIsAppOrientationSupported()
	{
		return IAmethyst_API_Managed::wrapper->GetIsAppOrientationSupported();
	}

	__declspec(dllexport) bool GetIsSkeletonTracked()
	{
		return IAmethyst_API_Managed::wrapper->GetIsSkeletonTracked();
	}

	__declspec(dllexport) std::array<Eigen::Vector3f, ktvr::ITrackedJointType::Joint_Total> GetJointPositions()
	{
		std::array<Eigen::Vector3f, ktvr::Joint_Total> cluster_positions;
		const auto& positions = IAmethyst_API_Managed::wrapper->GetJointPositions();

		for (size_t i = 0; i < ktvr::Joint_Total; i++)
		{
			cluster_positions[i].x() = positions[i].X;
			cluster_positions[i].y() = positions[i].Y;
			cluster_positions[i].z() = positions[i].Z;
		}

		return cluster_positions;
	}

	__declspec(dllexport) std::array<Eigen::Quaternionf, ktvr::ITrackedJointType::Joint_Total> GetJointOrientations()
	{
		std::array<Eigen::Quaternionf, ktvr::Joint_Total> cluster_orientations;
		const auto& orientations = IAmethyst_API_Managed::wrapper->GetJointOrientations();

		for (size_t i = 0; i < ktvr::Joint_Total; i++)
		{
			cluster_orientations[i].w() = orientations[i].W;
			cluster_orientations[i].x() = orientations[i].X;
			cluster_orientations[i].y() = orientations[i].Y;
			cluster_orientations[i].z() = orientations[i].Z;
		}

		return cluster_orientations;
	}

	__declspec(dllexport) std::array<ktvr::ITrackedJointState, ktvr::ITrackedJointType::Joint_Total>
	GetJointTrackingStates()
	{
		std::array<ktvr::ITrackedJointState, ktvr::Joint_Total> cluster_states;
		const auto& states = IAmethyst_API_Managed::wrapper->GetJointTrackingStates();

		for (size_t i = 0; i < ktvr::Joint_Total; i++)
			cluster_states[i] = static_cast<ktvr::ITrackedJointState>(states[i]);

		return cluster_states;
	}

	__declspec(dllexport) void OnLoad()
	{
		IAmethyst_API_Managed::wrapper->OnLoad();
	}
}
