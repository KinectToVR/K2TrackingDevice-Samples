#include "pch.h"
#include "KinectV2Handler.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <Ole2.h>

#include <ppl.h>
#include <thread>
#include <chrono>

HRESULT KinectV2Handler::getStatusResult()
{
	BOOLEAN avail;
	if (kinectSensor) // Protect from null call
	{
		kinectSensor->get_IsAvailable(&avail);
		if (avail)
			return S_OK;
	}
	return S_FALSE;
}

std::string KinectV2Handler::statusResultString(HRESULT stat)
{
	switch (stat)
	{
	case S_OK: return "Success!\nS_OK\nEverything's good!";
	case S_FALSE: return
			"Sensor Unavailable!\nE_NOTAVAILABLE\nCheck if the Kinect plugged in to your PC's USB and power plugs.";
	default: return "Undefined: " + std::to_string(stat) +
			"\nE_UNDEFINED\nSomething weird has happened, though we can't tell what.";
	}
}

void KinectV2Handler::initialize()
{
	try
	{
		initialized = initKinect();

		initializeSkeleton();
		if (!initialized) throw FailedJointsInitialization;
	}
	catch (std::exception& e)
	{
	}
}

void KinectV2Handler::initializeSkeleton()
{
	if (bodyFrameReader)
		bodyFrameReader->Release();
	IBodyFrameSource* bodyFrameSource;
	kinectSensor->get_BodyFrameSource(&bodyFrameSource);
	bodyFrameSource->OpenReader(&bodyFrameReader);

	// Newfangled event based frame capture
	// https://github.com/StevenHickson/PCL_Kinect2SDK/blob/master/src/Microsoft_grabber2.cpp
	h_bodyFrameEvent = (WAITABLE_HANDLE)CreateEvent(nullptr, FALSE, FALSE, nullptr);
	HRESULT hr = bodyFrameReader->SubscribeFrameArrived(&h_bodyFrameEvent);
	if (bodyFrameSource) bodyFrameSource->Release();

	// Rebuild the K2App's joint vector too
	trackedJoints.clear();

	using namespace std::literals;
	trackedJoints.push_back(ktvr::K2TrackedJoint("Joint 16 (W)")); // Add the joint to K2App's vector, Waist
	trackedJoints.push_back(ktvr::K2TrackedJoint("Joint 19 (L)")); // Add the joint to K2App's vector, Left Foot
	trackedJoints.push_back(ktvr::K2TrackedJoint("Joint 23 (R)")); // Add the joint to K2App's vector, Right Foot
}

void KinectV2Handler::terminateSkeleton()
{
	if (bodyFrameReader)
	{
		HRESULT hr = bodyFrameReader->UnsubscribeFrameArrived(h_bodyFrameEvent);
		if (FAILED(hr))
		{
			throw std::exception("Couldn't unsubscribe frame!");
		}
		__try
		{
			CloseHandle((HANDLE)h_bodyFrameEvent);
			bodyFrameReader->Release();
		}
		__except (EXCEPTION_INVALID_HANDLE)
		{
		}
		h_bodyFrameEvent = NULL;
		bodyFrameReader = nullptr;
	}
}

void KinectV2Handler::update()
{
	if (isInitialized())
	{
		BOOLEAN isAvailable = false;
		HRESULT kinectStatus = kinectSensor->get_IsAvailable(&isAvailable);
		if (kinectStatus == S_OK)
		{
			// NEW ARRIVED FRAMES ------------------------
			MSG msg;
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // Unneccesary?
			{
				DispatchMessage(&msg);
			}

			if (h_bodyFrameEvent)
			{
				//printf("Kinect Event ID: %d\n" ,(int)h_bodyFrameEvent);

				//now check for IR Events
				HANDLE handles[] = {reinterpret_cast<HANDLE>(h_bodyFrameEvent)};

				switch (MsgWaitForMultipleObjects(_countof(handles), handles, false, 0, QS_ALLINPUT))
				{
				case WAIT_OBJECT_0:
					{
						IBodyFrameArrivedEventArgs* pArgs = nullptr;
						//printf("Body Frame Event Signaled.\n");

						if (bodyFrameReader)
						{
							HRESULT hr = bodyFrameReader->GetFrameArrivedEventData(h_bodyFrameEvent, &pArgs);
							//printf("Retreive Frame Arrive Event Data -HR: %d\n", hr);

							if (SUCCEEDED(hr))
							{
								//printf("Retreived Frame Arrived Event Data\n");
								onBodyFrameArrived(*bodyFrameReader, *pArgs);
								pArgs->Release();
								//printf("Frame Arrived Event Data Released\n");
							}
						}
					}
					break;
				}
			}
		}
	}
}

void KinectV2Handler::onBodyFrameArrived(IBodyFrameReader& sender, IBodyFrameArrivedEventArgs& eventArgs)
{
	updateSkeletalData();
}

void KinectV2Handler::updateSkeletalData()
{
	if (bodyFrameReader)
	{
		IBodyFrame* bodyFrame = nullptr;
		HRESULT frameReceived = bodyFrameReader->AcquireLatestFrame(&bodyFrame);
		if (FAILED(frameReceived))
		{
			if (frameReceived == E_PENDING)
			{
				// LOG(INFO) << "Could not retrieve skeleton frame, stuck pending...";
			}
		}
		if (!bodyFrame) return;

		bodyFrame->GetAndRefreshBodyData(BODY_COUNT, kinectBodies);
		newBodyFrameArrived = true;
		if (bodyFrame) bodyFrame->Release();

		// We have the frame, now parse it
		updateParseFrame();
	}
}

void KinectV2Handler::updateParseFrame()
{
	for (int i = 0; i < BODY_COUNT; i++)
	{
		if (kinectBodies[i])
			kinectBodies[i]->get_IsTracked(&isTracking);
		if (isTracking)
		{
			skeletonTracked = true;
			kinectBodies[i]->GetJoints(JointType_Count, joints);
			kinectBodies[i]->GetJointOrientations(JointType_Count, boneOrientations);

			/* Create a helper list with _Joints only used joints */
			const std::array<uint32_t, 8>
				_JointsJointsArray{
					ktvr::Joint_Head,
					ktvr::Joint_SpineWaist,
					ktvr::Joint_AnkleLeft,
					ktvr::Joint_AnkleRight
				};

			/* Copy joint positions */
			trackedJoints.at(0).update( // Waist
				Eigen::Vector3f( // Position
					joints[JointType_SpineBase].Position.X,
					joints[JointType_SpineBase].Position.Y,
					joints[JointType_SpineBase].Position.Z),
				Eigen::Quaternionf( // Rotation
					boneOrientations[JointType_SpineBase].Orientation.w,
					boneOrientations[JointType_SpineBase].Orientation.x,
					boneOrientations[JointType_SpineBase].Orientation.y,
					boneOrientations[JointType_SpineBase].Orientation.z),
				joints[JointType_SpineBase].TrackingState // State
			);

			trackedJoints.at(0).update( // Left Foot
				Eigen::Vector3f( // Position
					joints[JointType_AnkleLeft].Position.X,
					joints[JointType_AnkleLeft].Position.Y,
					joints[JointType_AnkleLeft].Position.Z),
				Eigen::Quaternionf( // Rotation
					boneOrientations[JointType_AnkleLeft].Orientation.w,
					boneOrientations[JointType_AnkleLeft].Orientation.x,
					boneOrientations[JointType_AnkleLeft].Orientation.y,
					boneOrientations[JointType_AnkleLeft].Orientation.z),
				joints[JointType_AnkleLeft].TrackingState // State
			);

			trackedJoints.at(0).update( // Right Foot
				Eigen::Vector3f( // Position
					joints[JointType_AnkleRight].Position.X,
					joints[JointType_AnkleRight].Position.Y,
					joints[JointType_AnkleRight].Position.Z),
				Eigen::Quaternionf( // Rotation
					boneOrientations[JointType_AnkleRight].Orientation.w,
					boneOrientations[JointType_AnkleRight].Orientation.x,
					boneOrientations[JointType_AnkleRight].Orientation.y,
					boneOrientations[JointType_AnkleRight].Orientation.z),
				joints[JointType_AnkleRight].TrackingState // State
			);
			
			newBodyFrameArrived = false;
			break; // Only first skeleton
		}
		skeletonTracked = false;
	}
}

bool KinectV2Handler::initKinect()
{
	if (FAILED(GetDefaultKinectSensor(&kinectSensor)))
	{
		return false;
	}
	if (kinectSensor)
	{
		kinectSensor->get_CoordinateMapper(&coordMapper);

		HRESULT hr_open = kinectSensor->Open();
		std::this_thread::sleep_for(std::chrono::seconds(2));
		// Necessary to allow kinect to become available behind the scenes

		BOOLEAN available = false;
		kinectSensor->get_IsAvailable(&available);

		if (FAILED(hr_open) || !available)
		{
			return false;
		}
		return true;
	}
	return false;
}

void KinectV2Handler::shutdown()
{
	try
	{
		// Release the Kinect sensor, called form k2vr
		// OR from the crash handler
		if (kinectSensor)
		{
			// Protect from null call
			terminateSkeleton();

			kinectSensor->Close();
			kinectSensor->Release();
		}
	}
	catch (std::exception& e)
	{
	}
}
