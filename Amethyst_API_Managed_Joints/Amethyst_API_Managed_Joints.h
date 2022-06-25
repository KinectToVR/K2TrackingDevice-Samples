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

namespace Amethyst_API_Managed_Wrapper
{
	public ref class Amethyst_API_Managed_Device
	{
	public:
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
