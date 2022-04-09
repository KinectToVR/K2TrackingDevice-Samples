<h1 dir=auto>
<b>K2TrackingDevice Samples</b>
<text>(</text>
<text style="color:#9966cc;">Amethyst</text>
<text>'s plugins )</text>
</h1>

## <ins>__[Discord server](https://discord.gg/YBQCRDG)__</ins> and I'm **公彦赤屋先#5023**

## **License**
This project is licensed under the GNU GPL v3 License 

## **Overview**
This repository contains sample projects of devices / plugins in terms of Amethyst project.<br>
Each project in the solution is an example of a different possible implementation<br>
of a tracking device (Later referred to as K2TrackingDevice), and an additional one with settings.<br>
<br>
All `device_KinectV2_*` projects represent a `K2TrackingDevice` Kinect implementation,<br>
(later referred to as `K2TrackingDevice_KinectBasis`),<br> 
except the `device_KinectV2_Joints` which<br> represents a `K2TrackingDevice` Joints implementation,<br> later referred to as `K2TrackingDevice_JointsBasis`.<br>
<br>
The first possible device type is `K2TrackingDevice_KinectBasis`.<br>
These devices will have to provide selected joints and always the head position.<br>
Such behaviour will allow us to use in-app automatic calibration, so-called `flip`<br>
(which is a big deal for Kinects) and sometimes custom-calculated orientation.<br>
<br>
`K2TrackingDevice_KinectBasis` must update a provided, fixed array of joints (`ITrackedJoint`s)<br>
with its own positions. How many joints will the device provide is a matter of its `Characteristics`.<br>
And so, `K2_Character_Full` has to provide every possible joint, making them count 25.<br>
`K2_Character_Simple` has to update head, elbows, waist, knees, ankles and foot (tip) joints.<br>
The last characteristics, `K2_Character_Basic` only has to provide head, waist and ankles;<br>
devices using the `Basic` character also won't be allowed to use `AppOrientation`, or so-called math-based.<br>
<br>
Second, but not any worse is the `K2TrackingDeviceBase_JointsBasis` device.<br>
This one doesn't have so many different characteristics, and is built on top of how<br>
PSMoveService was working in 0.8~9 KinectToVR versions.<br>
Thses devices have to provide (push) joints, or `K2TrackedJoint`s to the internal joints' vector.<br>
On each `update()` call, the joints have to be updated (but hopefully not rebuilt).<br>
<br>
Each joint can be updated with position, rotation and state with its public `update()` function<br>
and signaled (optionally) for differentiating joint devices (like PSMoves), with `signalJoint()`.<br>
Joints should also be named on creation / pushing by setting their `jointName`<br>
for the app to be accessed by calling `getJointName()` before adding it to the UI.<br>
<br>
Devices in Amethyst are like drivers in OpenVR. They are going to be scanned and constructed at startup.<br>
Therefore, each device must have a `void* TrackingDeviceBaseFactory` function, you'll find it in samples too.<br>
<br>
Additional device type which may be used for third-party plugins is the `K2TrackingDevice_Spectator`.<br>
As the name suggests, it's gonna be used only for grabbing data from Amethyst, and nothing else.<br>
You can pull states, names, roles and positions of all 7 trackers (as of now).<br>
To save time on interfacing with OpenVR, you can pull HMD's pose & additional, extracted yaw.<br>
<br>
After playing with samples you should have enough knowledge to write a device plugin yourself.<br>
Please remember that `initialize()`, `update()` and `shutdown()` are the most ever important functions.<br>
(At least for the devices which actually provide tracking and not only grab it from the app)<br>
<br>
Since K2API v7 you also can add custom settings to each device plugin.<br>
Currently, the whole idea of using the `ktvr::Interface::` works similar to how ImGUI works.<br>
You create elements (element pointers), set them up, and push to the UI via selected functions.<br>
Of course you can manage them later, either via their pointers or inside handler functions.<br>
When Amethyst is ready for plugin-wise UI construction, it's gonna call the `OnLoad()` function,<br>
which you can override to set your UI up. Just remember your device must report `settingsSupported` as true.
<br>
<br>
**Note:** every device has various helper internal functions, e.g. for getting the HMD position in VRSystem,<br>
that may be called if it was initialized properly (loaded by Amethyst).<br>
These functions may help in plugin development, as for example loading the whole OpenVR API<br>
only to get the head position may be skipped by calling an appropriate function.

## OK, but what's actually in `external/vendor`?
Ah yes. For the sake of K2API's simplicity in its current state, it's been split into 3 parts.<br>
The currently used part, devices part allows us to make device plugins and provide the whole tracking.<br>
It's been grabbed from the newest `KinectToVR API` project files and put here for simplicity reasons.<br>
If this part will ever get an interface update, you're going to see in logs that your device<br>
has provided an incorrect `IK2API_Devices_Version` of the interface, and you'll be able to update it.

## I've read your damn essay, now what?
It seems that you're really motivated for this, nice! Please have a thorough look at the repo.<br>
You're going to find samples for all device types & characteristics, including a device with<br>
custom settings, and a spectator (data-poller) device.
  
## **Build instructions**
You'll need:
 - The `K2TrackingDevice-Samples` repo cloned somewhere and `cd`'d into
 - (For testing purpose) Working installation of Amethyst and SteamVR

Follow these steps:

- [Install tools for the Windows App development](https://docs.microsoft.com/en-us/windows/apps/windows-app-sdk/set-up-your-development-environment?tabs=vs-2022-17-1-a%2Cvs-2022-17-1-b).<br>
  You'll have to install Visual Studio 2022 or its Build Tools.

- Clone `Eigen3` to `external/`<br>
  ```powershell
  > git clone https://gitlab.com/libeigen/eigen ./external/eigen
  # Reset Eigen to the latest OK state
  > cd ./external/eigen
  > git reset --hard 1fd5ce1002a6f30e1169b529b291216a18be2f7e
  # Go back
  > cd ../..
  ```

- Build Samples:<br>
  ```powershell
  # Download the vswhere tool to find msbuild without additional interactions
  > Invoke-WebRequest -Uri 'https://github.com/microsoft/vswhere/releases/latest/download/vswhere.exe' -OutFile './vswhere.exe'
  # Use the downloaded vswhere tool to find msbuild. Skip this step if you use the Dev Powershell
  > $msbuild = "$("$(.\vswhere.exe -legacy -prerelease -products * -format json | Select-String -Pattern "2022" | `
        Select-String -Pattern "Studio" | Select-Object -First 1 | Select-String -Pattern "installationPath")" `
        -replace('"installationPath": "','') -replace('",',''))".Trim() + "\\MSBuild\\Current\\Bin\\MSBuild.exe"

  # Restore NuGet packages and build everything
  > &"$msbuild" K2TrackingDevice_Samples.sln "/p:Configuration=Release;Platform=x64"
  ```

## **Deployment**
The whole output can be found in `x64/Release` directory<br>
(or `x64/Debug` if you're building for `Debug`, naturally) and:
 - Devices (plugins) are inside `devices/` folder

Note: Debug builds will only work with a debug host,<br>
the same schema applies to OpenVR Driver, the API and Amethyst.