<h1 dir=auto>
<text>(</text>
<text style="color:#9966cc;">Amethyst</text>
<text>)</text>
<b>Amethyst API Devices</b>
</h1>
Before we start: <ins>please make sure to read this all, it's really important.</ins><br>
Every secion is equally important, so make sure you don't skip any.
<br> <br>

## **Device types**
As introduced [here](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/README.md#overview-ame-api-devices-docs), there are 2 base tracking provider types and a spectator type.

All `device_SkeletonBasis_*` projects represent a `K2TrackingDevice_SkeletonBasis` implementation,  
except the `device_JointsBasis` which represents a `K2TrackingDevice_JointsBasis`,  
and the `device_Spectator`, which shows how to use the `K2TrackingDevice_Spectator` class.  
<br>

### `K2TrackingDevice_SkeletonBasis`
These devices will have to provide selected joints and always the head position.  
Such behaviour will allow us to use in-app automatic calibration, so-called `flip`  
(which is a big deal for Kinects) and sometimes custom-calculated orientation.  
<br>
`K2TrackingDevice_SkeletonBasis` must update a provided, fixed array of joints (`ITrackedJoint`s)  
with its own positions. How many joints will the device provide is a matter of its `Characteristics`.  
And so, `K2_Character_Full` ([sample](https://github.com/KinectToVR/K2TrackingDevice-Samples/tree/main/device_SkeletonBasis_Full)) has to provide every possible joint, count `Joint_Total` (25).  
`K2_Character_Simple` ([sample](https://github.com/KinectToVR/K2TrackingDevice-Samples/tree/main/device_SkeletonBasis_Simple)) has to update head, elbows, waist, knees, ankles and foot (tip) joints.  
The last characteristics, `K2_Character_Basic` ([sample](https://github.com/KinectToVR/K2TrackingDevice-Samples/tree/main/device_SkeletonBasis_Basic)) only has to provide head, waist and ankles.  
Devices using the `Basic` character also won't be allowed to use `AppOrientation`, or so-called math-based.  
<br>

### `K2TrackingDeviceBase_JointsBasis` ([sample](https://github.com/KinectToVR/K2TrackingDevice-Samples/tree/main/device_JointsBasis))
This device class is built on top of how PSMoveService was working in legacy KinectToVR versions.  
Thses devices have first to provide (push) joints, (`K2TrackedJoint`s) to the internal joints' vector.  
On each `update()` call, the joints have to be updated (hopefully vector doesn't need to be rebuilt).  
<br>
Each joint can be updated with position, rotation and state with its public `update()` function,  
and signaled (optionally) for differentiating joint devices (like PSMoves), with `signalJoint()`.  
Joints should also be named on creation / pushing by setting their internal `jointName` string,  
for the app to be accessed by calling `getJointName()` before adding it the joint to the joint picker,  
<br>

### `K2TrackingDevice_Spectator` ([sample](https://github.com/KinectToVR/K2TrackingDevice-Samples/tree/main/device_Spectator))
When a device reports itself as a spectator, it states that it won't be providing any tracking.  
Spectator devices, and generally all device types can poll tracking data from the host (`Amethyst`).  
There are several helper functions to make that easier:
- [`getHMDPose()`](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/external/vendor/Amethyst_API_Devices.h#L747) - grab the VR Headset pose (position in meters and orientation as a quaternion)  
  Access pos via `.first` and ori via `.second`. The `Calibrated` variant makes use of your room setup.
- [`getHMDOrientationYaw()`](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/external/vendor/Amethyst_API_Devices.h#L762) - get VR Headset's extracted orientation yaw (in radians, +y)  
  The `Calibrated` variant makes use of your room setup, so it's generally better to use it.
- [`getAppJointPoses()`](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/external/vendor/Amethyst_API_Devices.h#L771) - the most useful one, returns an array of joints from `Amethyst`,  
  with their final (composed) names, roles, positions & orientations and states.
<br>
<br>

## **Loading the devices**
Devices in Amethyst are like drivers in OpenVR. They are scanned and constructed at the startup.  
Each device must have a `void* TrackingDeviceBaseFactory` function, [you'll find it in samples too](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/device_JointsBasis/DeviceHandler.h#L30).  
In this function, the device handler will be built and returned as a reference to the API.  
The function will also check the used API version, make sure to keep it the latest!
<br>
<br>

## **Device handlers**
After playing with [the samples](https://github.com/KinectToVR/K2TrackingDevice-Samples) you should have enough knowledge to write a device plugin yourself.  
Remember that `initialize()`, `update()` and `shutdown()` are the most ever important functions.  
(At least for the devices which actually provide tracking and not only grab it from the app)  
<br>

## **Device settings**
Since `K2API` v7 you also can add custom, device-provided settings to each device plugin.  
Currently, the whole idea of using the `ktvr::Interface::` works similar to how `ImGUI` works.  
You create elements (element pointers), set them up, and push to the UI via selected functions.  
Of course you can manage them later, either via their pointers or inside handler functions.  

When Amethyst is ready for plugin-wise UI construction, it's gonna call the `OnLoad()` function,  
[you can override it to set your UI up](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/device_SkeletonBasis_Full_Settings/DeviceHandler.h#L28). Remember your device must report `settingsSupported` as true!  
<br>
### `AppendSingleElement`
This function will append a single `UIElement` to the UI layout. It will appear on a single line,  
either snapped to the left, right or centered - it's to your liking. (Though default is snap to the left)
<br> <br>
### `AppendElementPair`
If you want to append two elements to a single row, use this function. The column space for each  
appended `UIElement` will be the same - 50% (or one star unit) Elements will be centered in their columns.
<br> <br>
### `AppendElementVector`
When there's a need to append even more elements to one row, you should use this function.  
It will work exactly the same as `AppendElementPair`, but of course will create more columns.  
You don't need to create an additional vector to apply the elements, you can just go like this:
```c++
// Let's say you have a label (TextBlock) and two buttons already created
AppendElementVector({ myTextBlock, myButton_1, myButton_2 }); // Notice the braces
```
<br>

### Handlers
You can register handlers for buttons, number boxes, check boxes, etc. as you would normally do.  
Let's say that you have a button `myButton` and want to change its text (content) on click.
```c++
// Let's say that you created your button like this:
Button* myButton = CreateButton(L"Click Me!");
// And then added it to the device settings like this:
this->layoutRoot->AppendSingleElement(myButton);
```

Now that you've created your button, you want to register a handler for it.
```c++
// You can either create a functions for it, like this:
void myButton_OnClick(Button* sender) {
  // Change the content to "Clicked!"
  sender->Content(L"Clicked!");
}
// And then register it at Onload() via:
myButton->OnClick = myButton_OnClick;
```

If you don't want to define and create additional  functions for it, you can just:
```c++
// Or you can dynamically create a handler function:
myButton->OnClick = [] (Button* sender) {
  // Change the content to "Clicked!"
  sender->Content(L"Clicked!");
};
```

Both scenarios will result in the same action - the button will change its content on click.  
Of course you don't need to access the `sender` to access the button. If you've defined it  
in some outer scope, you can *just* access it. Though, remember that you can use `Create` functions  
only after `OnLoad()` was called, never before it. You'd need to define a `Button*` and assign it later.  

Now, device handlers may involve additional function parameters, like `int const& new_value`.  
Just make sure to follow the function's template, it's always named like `On[Something]`.
<br> <br>

## OK, but what's actually in `external/vendor`?
Ah yes. For the sake of K2API's simplicity in its current state, it's been split into 3 parts.  
The currently used part, `devices` part, allows us to make device plugins and provide the whole tracking.  
It's been grabbed from the newest `Amethyst API` project files and put here for simplicity reasons.  
If this part will ever get an interface update, you're going to see in logs that your device  
has provided an incorrect `IAME_API_Devices_Version` of the interface, and you'll be able to update it.
<br> <br>

## I've read it all, now what?
It seems that you're really motivated for this, nice! [Please have a thorough look at the repo](https://github.com/KinectToVR/K2TrackingDevice-Samples).  
You're going to find samples for all currently supported device types & characteristics,  
including a device with custom settings, and a spectator (data-poller) device.

Also a fun thing, we're using `std::wstring`s for pretty much everything,  
so almost any string literal you're gonna use will surely start with an `L`.  
(Like `L"Hello there!"`, this is to support UTF-16 paths and characters!)