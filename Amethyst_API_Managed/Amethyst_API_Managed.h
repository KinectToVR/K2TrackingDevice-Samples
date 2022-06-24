#pragma once
#include <Amethyst_API_Devices.h>

inline void MarshalString(System::String^ s, std::string& os)
{
	auto chars =
		(const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}

inline void MarshalString(System::String^ s, std::wstring& os)
{
	auto chars =
		(const wchar_t*)(System::Runtime::InteropServices::Marshal::StringToHGlobalUni(s)).ToPointer();
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
		Amethyst_Device_Managed::AmethystDevice device_handler_;
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

	__declspec(dllexport) std::array<ktvr::JointTrackingState, ktvr::ITrackedJointType::Joint_Total> GetJointTrackingStates()
	{
		std::array<ktvr::JointTrackingState, ktvr::Joint_Total> cluster_states;
		const auto& states = IAmethyst_API_Managed::wrapper->GetJointTrackingStates();

		for (size_t i = 0; i < ktvr::Joint_Total; i++)
			cluster_states[i] = states[i];

		return cluster_states;
	}
}
