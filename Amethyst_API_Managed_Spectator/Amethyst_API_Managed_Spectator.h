#pragma once
#include <Amethyst_API_Devices.h>

inline void MarshalString(System::String^ s, std::wstring& os)
{
	auto chars =
		static_cast<const wchar_t*>((System::Runtime::InteropServices::Marshal::StringToHGlobalUni(s)).ToPointer());
	os = chars;
	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}

// Logging handler (native)
inline std::function<void(std::wstring, unsigned)> LogToAme_Handler;

inline std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> GetHMDPose_Handler;
inline std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> GetHMDPoseCalibrated_Handler;
inline std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> GetLeftControllerPose_Handler;
inline std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> GetLeftControllerPoseCalibrated_Handler;
inline std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> GetRightControllerPose_Handler;
inline std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> GetRightControllerPoseCalibrated_Handler;
inline std::function<double()> GetHMDOrientationYaw_Handler;
inline std::function<double()> GetHMDOrientationYawCalibrated_Handler;
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
			static_cast<float>(position.x()),
			static_cast<float>(position.y()),
			static_cast<float>(position.z())
		},
		System::Numerics::Quaternion{
			static_cast<float>(orientation.w()),
			static_cast<float>(orientation.x()),
			static_cast<float>(orientation.y()),
			static_cast<float>(orientation.z())
		}
	);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetHMDPoseCalibrated()
{
	const auto& [position, orientation] = GetHMDPoseCalibrated_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3{
			static_cast<float>(position.x()),
			static_cast<float>(position.y()),
			static_cast<float>(position.z())
		},
		System::Numerics::Quaternion{
			static_cast<float>(orientation.w()),
			static_cast<float>(orientation.x()),
			static_cast<float>(orientation.y()),
			static_cast<float>(orientation.z())
		}
	);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetLeftControllerPose()
{
	const auto& [position, orientation] = GetLeftControllerPose_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3{
			static_cast<float>(position.x()),
			static_cast<float>(position.y()),
			static_cast<float>(position.z())
		},
		System::Numerics::Quaternion{
			static_cast<float>(orientation.w()),
			static_cast<float>(orientation.x()),
			static_cast<float>(orientation.y()),
			static_cast<float>(orientation.z())
		}
	);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetLeftControllerPoseCalibrated()
{
	const auto& [position, orientation] = GetLeftControllerPoseCalibrated_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3{
			static_cast<float>(position.x()),
			static_cast<float>(position.y()),
			static_cast<float>(position.z())
		},
		System::Numerics::Quaternion{
			static_cast<float>(orientation.w()),
			static_cast<float>(orientation.x()),
			static_cast<float>(orientation.y()),
			static_cast<float>(orientation.z())
		}
	);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetRightControllerPose()
{
	const auto& [position, orientation] = GetRightControllerPose_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3{
			static_cast<float>(position.x()),
			static_cast<float>(position.y()),
			static_cast<float>(position.z())
		},
		System::Numerics::Quaternion{
			static_cast<float>(orientation.w()),
			static_cast<float>(orientation.x()),
			static_cast<float>(orientation.y()),
			static_cast<float>(orientation.z())
		}
	);
}

inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetRightControllerPoseCalibrated()
{
	const auto& [position, orientation] = GetRightControllerPoseCalibrated_Handler();
	return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
		System::Numerics::Vector3{
			static_cast<float>(position.x()),
			static_cast<float>(position.y()),
			static_cast<float>(position.z())
		},
		System::Numerics::Quaternion{
			static_cast<float>(orientation.w()),
			static_cast<float>(orientation.x()),
			static_cast<float>(orientation.y()),
			static_cast<float>(orientation.z())
		}
	);
}

inline double GetHMDOrientationYaw()
{
	return GetHMDOrientationYaw_Handler();
}

inline double GetHMDOrientationYawCalibrated()
{
	return GetHMDOrientationYawCalibrated_Handler();
}

inline array<Managed_Spectator_Handler::API_Projection_Files::TrackedJoint^>^ GetAppJointPoses()
{
	auto joints = GetAppJointPoses_Handler();
	auto tracker_array = gcnew array<Managed_Spectator_Handler::API_Projection_Files::TrackedJoint^>(joints.size());

	for (size_t i = 0; i < joints.size(); i++)
	{
		const auto& position = joints[i].getJointPosition();
		const auto& orientation = joints[i].getJointOrientation();

		tracker_array[i] = gcnew Managed_Spectator_Handler::API_Projection_Files::TrackedJoint{
			System::Numerics::Vector3{
				static_cast<float>(position.x()),
				static_cast<float>(position.y()),
				static_cast<float>(position.z())
			},
			System::Numerics::Quaternion{
				static_cast<float>(orientation.w()),
				static_cast<float>(orientation.x()),
				static_cast<float>(orientation.y()),
				static_cast<float>(orientation.z())
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
			device_handler_.GetHMDOrientationYaw = gcnew System::Func<double>(&GetHMDOrientationYaw);
			device_handler_.GetHMDOrientationYawCalibrated = gcnew System::Func<
				double>(&GetHMDOrientationYawCalibrated);
			device_handler_.GetAppJointPoses = gcnew System::Func<array<
				Managed_Spectator_Handler::API_Projection_Files::TrackedJoint^>^>(&GetAppJointPoses);
		}

		void OnLoad()
		{
			device_handler_.OnLoad();
		}

	private:
		Managed_Spectator_Handler::AmethystDevice device_handler_;
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
		std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> handler)
	{
		GetHMDPose_Handler = handler;
	}

	__declspec(dllexport) void Register_getHMDPoseCalibrated(
		std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> handler)
	{
		GetHMDPoseCalibrated_Handler = handler;
	}

	__declspec(dllexport) void Register_getLeftControllerPose(
		std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> handler)
	{
		GetLeftControllerPose_Handler = handler;
	}

	__declspec(dllexport) void Register_getLeftControllerPoseCalibrated(
		std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> handler)
	{
		GetLeftControllerPoseCalibrated_Handler = handler;
	}

	__declspec(dllexport) void Register_getRightControllerPose(
		std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> handler)
	{
		GetRightControllerPose_Handler = handler;
	}

	__declspec(dllexport) void Register_getRightControllerPoseCalibrated(
		std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> handler)
	{
		GetRightControllerPoseCalibrated_Handler = handler;
	}

	__declspec(dllexport) void Register_getHMDOrientationYaw(std::function<double()> handler)
	{
		GetHMDOrientationYaw_Handler = handler;
	}

	__declspec(dllexport) void Register_getHMDOrientationYawCalibrated(std::function<double()> handler)
	{
		GetHMDOrientationYawCalibrated_Handler = handler;
	}

	__declspec(dllexport) void Register_getAppJointPoses(std::function<std::vector<ktvr::K2TrackedJoint>()> handler)
	{
		GetAppJointPoses_Handler = handler;
	}

	__declspec(dllexport) void OnLoad()
	{
		IAmethyst_API_Managed::wrapper->OnLoad();
	}
}
