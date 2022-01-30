#pragma once
#include <Kinect.h>
#include <Windows.h>

#include "KinectToVR_API_Devices.h"

/* Not exported */

class KinectV2Handler : public ktvr::K2TrackingDeviceBase_KinectBasis
{
public:
	/* K2API's things, which KTVR will make use of */

	KinectV2Handler()
	{
		K2TrackingDeviceBase_KinectBasis::deviceType = ktvr::K2_Kinect;
		K2TrackingDeviceBase_KinectBasis::deviceName = "XBONE Kinect (Simple)";

		K2TrackingDeviceBase_KinectBasis::deviceCharacteristics = ktvr::K2_Character_Simple;
		K2TrackingDeviceBase_KinectBasis::flipSupported = true;
		K2TrackingDeviceBase_KinectBasis::appOrientationSupported = true;
	}

	virtual ~KinectV2Handler()
	{
	}

	HRESULT getStatusResult() override;
	std::string statusResultString(HRESULT stat) override;

	void initialize() override;
	void update() override;
	void shutdown() override;

private:
	/* Device's own things, handlers and variables */

	IKinectSensor* kinectSensor = nullptr;
	IBodyFrameReader* bodyFrameReader = nullptr;
	IColorFrameReader* colorFrameReader = nullptr;
	IDepthFrameReader* depthFrameReader = nullptr;
	IMultiSourceFrame* multiFrame = nullptr;
	ICoordinateMapper* coordMapper = nullptr;
	BOOLEAN isTracking = false;

	Joint joints[JointType_Count];
	JointOrientation boneOrientations[JointType_Count];
	IBody* kinectBodies[BODY_COUNT] = {nullptr};

	bool convertColorToDepthResolution = false;
	void onBodyFrameArrived(IBodyFrameReader& sender, IBodyFrameArrivedEventArgs& eventArgs);
	virtual void updateSkeletalData();

	bool initKinect();
	void updateParseFrame();
	void initializeSkeleton();
	void terminateSkeleton();

	WAITABLE_HANDLE h_bodyFrameEvent;
	bool newBodyFrameArrived = false;

	/*
	 * If your device maps joints differently than K2API,
	 * you can create a map like this, and when update()
	 * is called, just copy all joints recursively
	 */

	int globalIndex[25] = {3, 2, 20, 4, 5, 6, 7, 21, 22, 8, 9, 10, 11, 23, 24, 1, 0, 12, 13, 14, 15, 16, 17, 18, 19};
};

/* Exported for dynamic linking */
extern "C" __declspec(dllexport) void* TrackingDeviceBaseFactory(
	const char* pVersionName, int* pReturnCode)
{
	// Return the device handler for tracking
	// but only if interfaces are the same / up-to-date
	if (0 == strcmp(ktvr::IK2API_Devices_Version, pVersionName))
	{
		static KinectV2Handler TrackingHandler; // Create a new device handler -> KinectV2

		*pReturnCode = ktvr::K2InitError_None;
		return &TrackingHandler;
	}

	// Return code for initialization
	*pReturnCode = ktvr::K2InitError_BadInterface;
}
