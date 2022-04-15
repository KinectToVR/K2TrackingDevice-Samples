#include "pch.h"
#include "DeviceHandler.h"

#include <iostream>
#include <chrono>
#include <ppl.h>

HRESULT DeviceHandler::getStatusResult()
{
	return S_OK;
}

std::string DeviceHandler::statusResultString(HRESULT stat)
{
	// Parse your device's status into some nice text here,
	// it has to be formatted like [HEADER]\n[TYPE]\n[MESSAGE]

	switch (stat)
	{
	case S_OK: return "Success!\nS_OK\nEverything's good!";
	default: return "Undefined: " + std::to_string(stat) +
		"\nE_UNDEFINED\nSomething weird has happened, though we can't tell what.";
	}
}

void DeviceHandler::initialize()
{
	// Initialize your device here

	// Mark the device as initialized
	initialized = true;
}

void DeviceHandler::update()
{
	// Update joints' positions here
	// Note: this is fired up every loop

	if (isInitialized())
	{
		// For the plugin's sake, we'll update joints with
		// the user's head position + 1m in z (front) axis
		// & various in x and the user's head orientation

		for (uint32_t i = 0; i < ktvr::Joint_Total; i++)
		{
			jointPositions[i] = getHMDPosition() + Eigen::Vector3f(i, 0, 1);
			jointOrientations[i] = getHMDOrientation();

			trackingStates[i] = ktvr::State_Tracked;
		}

		// Mark that we see the user
		skeletonTracked = true;
	}
}

void DeviceHandler::shutdown()
{
	// Turn your device off here
}
