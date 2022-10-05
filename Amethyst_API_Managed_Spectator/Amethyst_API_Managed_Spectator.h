#pragma once
#include <Amethyst_API_Devices.h>

inline ktvr::K2TrackingDeviceBase_Spectator* _this_Device;

// Native functions
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

inline void MarshalString(System::String^ s, std::wstring& os)
{
	const auto chars = static_cast<const wchar_t*>(
		System::Runtime::InteropServices::Marshal::StringToHGlobalUni(s).ToPointer());

	os = chars;
	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}

namespace Helpers
{
	inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetHMDPose()
	{
		const auto& [position, orientation] = _this_Device->getHMDPose();
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
		const auto& [position, orientation] = _this_Device->getHMDPoseCalibrated();
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
		const auto& [position, orientation] = _this_Device->getLeftControllerPose();
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
		const auto& [position, orientation] = _this_Device->getLeftControllerPoseCalibrated();
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
		const auto& [position, orientation] = _this_Device->getRightControllerPose();
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
		const auto& [position, orientation] = _this_Device->getRightControllerPoseCalibrated();
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
		return _this_Device->getHMDOrientationYaw();
	}

	inline double GetHMDOrientationYawCalibrated()
	{
		return _this_Device->getHMDOrientationYawCalibrated();
	}

	inline array<Managed_Spectator_Handler::API_Projection_Files::TrackedJoint^>^ GetAppJointPoses()
	{
		const auto joints = _this_Device->getAppJointPoses();
		auto tracker_array = gcnew array<Managed_Spectator_Handler::API_Projection_Files::TrackedJoint^>(
			joints.size());

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
}

namespace Amethyst_API_Managed_Wrapper
{
	public ref class Amethyst_API_Managed_Device
	{
	public:
		Amethyst_API_Managed_Device()
		{
			device_handler_.GetHMDPose = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&Helpers::GetHMDPose);
			device_handler_.GetHMDPoseCalibrated = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&Helpers::GetHMDPoseCalibrated);
			device_handler_.GetLeftControllerPose = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&Helpers::GetLeftControllerPose);
			device_handler_.GetLeftControllerPoseCalibrated = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&Helpers::GetLeftControllerPoseCalibrated);
			device_handler_.GetRightControllerPose = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&Helpers::GetRightControllerPose);
			device_handler_.GetRightControllerPoseCalibrated = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&Helpers::GetRightControllerPoseCalibrated);
			device_handler_.GetHMDOrientationYaw = gcnew System::Func<double>(&Helpers::GetHMDOrientationYaw);
			device_handler_.GetHMDOrientationYawCalibrated = gcnew System::Func<
				double>(&Helpers::GetHMDOrientationYawCalibrated);
			device_handler_.GetAppJointPoses = gcnew System::Func<array<
				Managed_Spectator_Handler::API_Projection_Files::TrackedJoint^>^>(&Helpers::GetAppJointPoses);
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
	__declspec(dllexport) void RegisterDevice(ktvr::K2TrackingDeviceBase_Spectator* handler)
	{
		_this_Device = handler;
	}

	__declspec(dllexport) void OnLoad()
	{
		IAmethyst_API_Managed::wrapper->OnLoad();
	}
}
