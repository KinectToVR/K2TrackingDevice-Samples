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

	return Amethyst_API_Managed::GetDeviceStatusWString();
}

void DeviceHandler::initialize()
{
	// Initialize your device here
	initialized = Amethyst_API_Managed::Initialize();
}

void DeviceHandler::update()
{
	// Update joints' positions here
	// Note: this is fired up every loop

	if (isInitialized())
	{
		// Update the managed tracking device
		Amethyst_API_Managed::Update();

		// Copy positions and states
		jointPositions = Amethyst_API_Managed::GetJointPositions();
		jointOrientations = Amethyst_API_Managed::GetJointOrientations();
		trackingStates = Amethyst_API_Managed::GetJointTrackingStates();

		// Mark if we see the user
		skeletonTracked = Amethyst_API_Managed::GetIsSkeletonTracked();
	}
}

void DeviceHandler::shutdown()
{
	// Turn your device off here
	Amethyst_API_Managed::Shutdown();
}