<h1 dir=auto>
<b>K2TrackingDevice Samples</b>
<text>(</text>
<text style="color:#9966cc;">Amethyst</text>
<text>'s plugins )</text>
</h1>

## <ins>__[Discord server](https://discord.gg/YBQCRDG)__</ins> and I'm **公彦赤屋先#5023**

## **License**
This project is licensed under the GNU GPL v3 License 

## **Downloads**
You're going to find built plugins in [repo Actions](https://github.com/KinectToVR/K2TrackingDevice-Samples/actions), in the run artifact's section.

## **Build & Deploy**
Both build and deployment instructions [are available here](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/BUILD_AND_DEPLOY.md).

## **Overview (AME API Devices Docs)**
This repository contains sample projects of devices / plugins in terms of `Amethyst` project.<br>
Each project in the solution is an example of a different possible implementation<br>
of a tracking device (Later referred to as `K2TrackingDevice`), and an additional one with settings.<br>

<ins>[You can find a detailed description about every single plugin and device type here.](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/DEVICES.md)</ins><br>
[As for a real-world example, please check out this. (JointsBasis w/ custom settings)](https://github.com/KimihikoAkayasaki/device_owoTrackVR)  

BTW: If you prefer C# to C++ for your device/plugin, please check out [this branch](https://github.com/KinectToVR/K2TrackingDevice-Samples/tree/managed) for samples.  
All device docs are still valid for them, you can also find a small note at the end  of the device docs.

Briefly, currently supported device types are:
- Tracking providers:
  - `JointsBasis` - Provide a vector of named, tracked joints
  - `SkeletonBasis` - Provide a fixed array of enumerated, tracked joints
    + `Full` Character - Provide Joint_Total joints (as many as Kinect V2 does)
    + `Simple` Character - Provide [ Head, Elbows, Waist, Knees, Ankles, Foot Tips ]
    + `Basic` Character - Provide [ Head, Waist, Ankles ]
- `Spectator` - Only poll data from the app

Please note that `SkeletonBasis` types can use automatic calibration, whereas `JointsBasis` cannot.<br>
Additionally, plugins (devices) can provide their own settings, as [described here](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/DEVICES.md#device-settings) and [shown here](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/device_SkeletonBasis_Full_Settings/DeviceHandler.h#L28).