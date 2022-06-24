<h1 dir=auto>
<b>K2TrackingDevice Samples</b>
<text>(</text>
<text style="color:#9966cc;">Amethyst</text>
<text>'s plugins )</text>
</h1>

## **License**
This project is licensed under the GNU GPL v3 License 

## **Downloads**
You're going to find built plugins in [repo Actions](https://github.com/KinectToVR/K2TrackingDevice-Samples/actions), in the run artifact's section.

## **Build & Deploy**
Both build and deployment instructions [are available here](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/managed/BUILD_AND_DEPLOY.md).

## **Overview (Managed Amethyst API)**
This repository contains a sample projection of Amethyst API to Managed (CLR and C#)  
If you wanna use C# for an Amethyst plugin, I just thought it may help a bit.  
(Note: API functions are highly limited currently, this applies to device types too)  

You're the best off just cloning this branch and trying things out yourself.  

<ins>[You can find a detailed description about every single plugin and device type here.](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/DEVICES.md)</ins><br>
[As for a real-world example, please check out this. (JointsBasis w/ custom settings)](https://github.com/KimihikoAkayasaki/device_owoTrackVR)

Briefly, currently supported device types are:
- Tracking providers:
  - `JointsBasis` - Provide a vector of named, tracked joints
  - `KinectBasis` - Provide a fixed array of enumerated, tracked joints
    + `Full` Character - Provide Joint_Total joints (as many as Kinect V2 does)
    + `Simple` Character - Provide [ Head, Elbows, Waist, Knees, Ankles, Foot Tips ]
    + `Basic` Character - Provide [ Head, Waist, Ankles ]
- `Spectator` - Only poll data from the app

Please note that `KinectBasis` types can use automatic calibration, whereas `JointsBasis` cannot.<br>
Additionally, plugins (devices) can provide their own settings, as [described here](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/DEVICES.md#device-settings) and [shown here](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/device_Managed_Full_Settings/DeviceHandler.h#L28).