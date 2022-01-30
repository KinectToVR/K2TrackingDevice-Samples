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
of the Microsoft Kinect V2 as a tracking device. (Later referred to as K2TrackingDevice)<br>
<br>
All ```device_KinectV2_*``` projects represent a ```K2TrackingDevice``` Kinect implementation,<br>
(later referred to as ```K2TrackingDevice_KinectBasis```),<br> 
except the ```device_KinectV2_Joints``` which<br> represents a ```K2TrackingDevice``` Joints implementation,<br> later referred to as ```K2TrackingDevice_JointsBasis```.<br>
<br>
The first possible device type is ```K2TrackingDevice_KinectBasis```.<br>
These devices will have to provide selected joints and always the head position.<br>
Such behaviour will allow us to use in-app automatic calibration, so-called ```flip```<br>
(which is a big deal for Kinects) and sometimes custom-calculated orientation.<br>
<br>
```K2TrackingDevice_KinectBasis``` must update a provided, fixed array of joints (```ITrackedJoint```s)<br>
with its own positions. How many joints will the device provide is a matter of its ```Characteristics```.<br>
And so, ```K2_Character_Full``` has to provide every possible joint, making them count 25.<br>
```K2_Character_Simple``` has to update head, waist, knees, ankles and foot (tip) joints.<br>
The last characteristics, ```K2_Character_Basic``` only has to provide head, waist and ankles;<br>
devices using the ```Basic``` character also won't be allowed to use ```AppOrientation```, or so-called math-based.<br>
<br>
Second, but not any worse is the ```K2TrackingDeviceBase_JointsBasis``` device.<br>
This one doesn't have so many different characteristics, and is built on top of how<br>
PSMoveService was working in 0.8~9 KinectToVR versions.<br>
Thses devices have to provide (push) joints, or ```K2TrackedJoint```s to the internal joints' vector.<br>
On each ```update()``` call, the joints have to be updated (but hopefully not rebuilt).<br>
<br>
Each joint can be updated with position, rotation and state with its public ```update()``` function<br>
and signaled (optionally) for differentiating joint devices (like PSMoves), with ```signalJoint()```.<br>
Joints should also be named on creation / pushing by setting their ```jointName```<br>
for the app to be accessed by calling ```getJointName()``` before adding it to the UI.<br>
<br>
Devices in Amethyst are like drivers in OpenVR. They are going to be scanned and constructed at startup.<br>
Therefore, each device must have a ```void* TrackingDeviceBaseFactory``` function, you'll find it in samples too.<br>
<br>
After playing with samples you should have enough knowledge to write a device plugin yourself.<br>
Please remember that ```initialize()```, ```update()``` and ```shutdown()``` are the most ever important functions.

## OK, but what's actually in ```external/vendor```?
Ah yes. For the sake of K2API's simplicity in its current state, it's been split into 3 parts.<br>
The currently used part, devices part allows us to make device plugins and provide the whole tracking.<br>
It's been grabbed from the newest ```KinectToVR API``` project files and put here for simplicity reasons.<br>
If this part will ever get an interface update, you're going to see in logs that your device<br>
has provided an incorrect ```IK2API_Devices_Version``` of the interface, and you'll be able to update it.

## **Build with Docker**
You'll need:
 - The ```K2TrackingDevice_Samples``` repo cloned somewhere and ```cd```'d into
 - Docker Desktop set to use Windows containers

Notes:
 - The build process contains no caching, is one-run, **and will take some time**.<br>
   Everything that's downloaded, set up, and not saved will be gone.

Follow these steps:
  ```powershell
  # Build the whole thing
  > docker build -t samples .

  # Create a dummy container from saved image
  > $id = $(docker create samples)
  # Copy Release files to the host machine
  > mkdir ./x64/Release
  > docker cp ${id}:/x64/Release ./x64/Release

  # Release (remove) the dummy container
  > docker rm -v $id
  # Release (remove) the base samples container
  > docker rmi --force $(docker images -q 'samples')
  ```
  Where ```samples``` may be your very own super cool container tag name.<br>
  Artifacts (```x64/Release``` directory) will be saved to ```x64/Release``` inside the repository root.<br>

  
## **Build manually**
You'll need:
 - The ```K2TrackingDevice-Samples``` repo cloned somewhere and ```cd```'d into
 - Kinect SDK 2.0 installed and visible in PATH
 - Working installation of SteamVR for testing

Follow these steps:

- Install Visual Studio 2022 Build Tools with needed addons<br>
  (If you already have VS2022 or BuildTools set up, **please skip this step**)<br>
  ```powershell
  > Set-ExecutionPolicy Bypass -Scope Process -Force
  > Invoke-Expression (Get-Content .dockerprepvc -Raw)
  ```
  This will install Microsoft Visual Studio Build Tools 2022 in the default location.<br>
  Nothing will be displayed during the process (just the PID & stats), just wait for it.

- Clone latest Eigen3 to ```external/```<br>
  ```powershell
  > git clone https://gitlab.com/libeigen/eigen ./external/eigen
  ```

- Build Samples:<br>
  ```powershell
  > Set-ExecutionPolicy Bypass -Scope Process -Force
  > Invoke-Expression (Get-Content .dockerbuild -Raw)
  ```
  Alternatively you can open the developer powershell for Visual Studio (or BuildTools) 2022 and:
  ```powershell
  > msbuild K2TrackingDevice_Samples.sln "/p:Configuration=Release;Platform=x64"
  ```

## **Deploy**
The whole output can be found in ```x64/Release``` directory<br>
(or ```x64/Debug``` if you're building for ```Debug```, naturally) and:
 - Devices (plugins) are inside ```devices/``` folder