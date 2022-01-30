#pragma once
#include <Kinect.h>
#include <Windows.h>

#include "KinectToVR_API_Devices.h"

/* Not exported */

class KinectV2Handler : public ktvr::K2TrackingDeviceBase_JointsBasis
{
public:
	/* K2API's things, which KTVR will make use of */

	KinectV2Handler()
	{
		K2TrackingDeviceBase_JointsBasis::deviceType = ktvr::K2_Joints;
		K2TrackingDeviceBase_JointsBasis::deviceName = "XBONE Kinect (Joints)";
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
