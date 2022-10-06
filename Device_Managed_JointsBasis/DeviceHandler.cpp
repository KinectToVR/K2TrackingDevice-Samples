#include "pch.h"
#include "DeviceHandler.h"

HRESULT DeviceHandler::getStatusResult()
{
	return Amethyst_API_Managed::GetDeviceStatus();
}

std::wstring DeviceHandler::statusResultWString(HRESULT stat)
{
	// Parse your device's status into some nice text here,
	// it has to be formatted like [HEADER]\n[TYPE]\n[MESSAGE]

	skeletonTracked =
		Amethyst_API_Managed::GetIsSkeletonTracked();
	Flags_BlocksPositionFiltering =
		Amethyst_API_Managed::GetIsPositionFilterBlockingEnabled();
	Flags_OverridesJointPhysics =
		Amethyst_API_Managed::GetIsPhysicsOverrideEnabled();
	Flags_ForceSelfUpdate =
		Amethyst_API_Managed::GetIsSelfUpdateEnabled();
	Flags_SettingsSupported =
		Amethyst_API_Managed::GetIsSettingsDaemonSupported();

	return Amethyst_API_Managed::GetDeviceStatusWString();
}

void DeviceHandler::initialize()
{
	// Initialize your device here

	initialized = Amethyst_API_Managed::Initialize();
	trackedJoints = Amethyst_API_Managed::GetTrackedJoints();

	skeletonTracked =
		Amethyst_API_Managed::GetIsSkeletonTracked();
	Flags_BlocksPositionFiltering =
		Amethyst_API_Managed::GetIsPositionFilterBlockingEnabled();
	Flags_OverridesJointPhysics =
		Amethyst_API_Managed::GetIsPhysicsOverrideEnabled();
	Flags_ForceSelfUpdate =
		Amethyst_API_Managed::GetIsSelfUpdateEnabled();
	Flags_SettingsSupported =
		Amethyst_API_Managed::GetIsSettingsDaemonSupported();
}

void DeviceHandler::update()
{
	// Update joints' positions here
	// Note: this is fired up every loop

	if (isInitialized())
	{
		// Update the managed tracking device
		Amethyst_API_Managed::Update();

		// Copy poses and states
		// (Assignment is fine since joints don't have GUIDs)
		trackedJoints = Amethyst_API_Managed::GetTrackedJoints();

		// Mark if we see the user
		skeletonTracked = Amethyst_API_Managed::GetIsSkeletonTracked();
	}
}

void DeviceHandler::shutdown()
{
	// Turn your device off here
	Amethyst_API_Managed::Shutdown();
}
