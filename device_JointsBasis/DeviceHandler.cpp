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

	// For the sample's sake, we'll add one dummy joint
	trackedJoints.push_back(ktvr::K2TrackedJoint("Default Joint"));

	// Mark the device as initialized
	initialized = true;
}

void DeviceHandler::update()
{
	// Update joints' positions here
	// Note: this is fired up every loop

	if (isInitialized())
	{
		// For the plugin's sake, we'll update the joint with
		// the user's head position + 1m in z (front) axis
		// and the user's head orientation

		trackedJoints.at(0).update(
			getHMDPosition() + Eigen::Vector3f(0, 0, 1),
			getHMDOrientation(),
			ktvr::State_Tracked);

		// Mark that we see the user
		skeletonTracked = true;
	}
}

void DeviceHandler::shutdown()
{
	// Turn your device off here
}
