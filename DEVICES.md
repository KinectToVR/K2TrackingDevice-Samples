<h1 dir=auto>
<text>(</text>
<text style="color:#9966cc;">Amethyst</text>
<text>)</text>
<b>KinectToVR API Devices</b>
</h1>
Before we start: <ins>please make sure to read this all, it's really important.</ins><br>
Every secion is equally important, so make sure you don't skip any.
<br> <br>

## **Device types**
As introduced [here](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/README.md#overview-k2api-devices-docs), there are 2 base tracking provider types and a spectator type.

All `device_KinectBasis_*` projects represent a `K2TrackingDevice_KinectBasis` implementation,<br>
except the `device_JointsBasis` which represents a `K2TrackingDevice_JointsBasis`,<br>
and the `device_Spectator`, which shows how to use the `K2TrackingDevice_Spectator` class.<br>
<br>

### `K2TrackingDevice_KinectBasis`
These devices will have to provide selected joints and always the head position.<br>
Such behaviour will allow us to use in-app automatic calibration, so-called `flip`<br>
(which is a big deal for Kinects) and sometimes custom-calculated orientation.<br>
<br>
`K2TrackingDevice_KinectBasis` must update a provided, fixed array of joints (`ITrackedJoint`s)<br>
with its own positions. How many joints will the device provide is a matter of its `Characteristics`.<br>
And so, `K2_Character_Full` ([sample](https://github.com/KinectToVR/K2TrackingDevice-Samples/tree/main/device_KinectBasis_Full)) has to provide every possible joint, count `Joint_Total` (25).<br>
`K2_Character_Simple` ([sample](https://github.com/KinectToVR/K2TrackingDevice-Samples/tree/main/device_KinectBasis_Simple)) has to update head, elbows, waist, knees, ankles and foot (tip) joints.<br>
The last characteristics, `K2_Character_Basic` ([sample](https://github.com/KinectToVR/K2TrackingDevice-Samples/tree/main/device_KinectBasis_Basic)) only has to provide head, waist and ankles.<br>
Devices using the `Basic` character also won't be allowed to use `AppOrientation`, or so-called math-based.<br>
<br>

### `K2TrackingDeviceBase_JointsBasis` ([sample](https://github.com/KinectToVR/K2TrackingDevice-Samples/tree/main/device_JointsBasis))
This device class is built on top of how PSMoveService was working in legacy KinectToVR versions.<br>
Thses devices have first to provide (push) joints, (`K2TrackedJoint`s) to the internal joints' vector.<br>
On each `update()` call, the joints have to be updated (hopefully vector doesn't need to be rebuilt).<br>
<br>
Each joint can be updated with position, rotation and state with its public `update()` function,<br>
and signaled (optionally) for differentiating joint devices (like PSMoves), with `signalJoint()`.<br>
Joints should also be named on creation / pushing by setting their internal `jointName` string,<br>
for the app to be accessed by calling `getJointName()` before adding it the joint to the joint picker,<br>
<br>

### `K2TrackingDevice_Spectator` ([sample](https://github.com/KinectToVR/K2TrackingDevice-Samples/tree/main/device_Spectator))
When a device reports itself as a spectator, it states that it won't be providing any tracking.<br>
Spectator devices, and generally all device types can poll tracking data from the host (`Amethyst`).<br>
There are several helper functions to make that easier:
- [`getHMDPosition()`](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/external/vendor/KinectToVR_API_Devices.h#L428) - grab the VR Headset position (in meters, xyz)
- [`getHMDOrientation()`](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/external/vendor/KinectToVR_API_Devices.h#L429) - get the VR Headset orientation (quaternion)
- [`getHMDOrientationYaw()`](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/external/vendor/KinectToVR_API_Devices.h#L430) - get VR Headset's extracted orientation yaw (in radians, +y)
- [`getAppJointPoses()`](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/external/vendor/KinectToVR_API_Devices.h#L438) - the most useful one, returns an array of joints from `Amethyst`,<br>
  with their final (composed) names, roles, positions & orientations and states.
<br>
<br>

## **Loading the devices**
Devices in Amethyst are like drivers in OpenVR. They are scanned and constructed at the startup.<br>
Each device must have a `void* TrackingDeviceBaseFactory` function, [you'll find it in samples too](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/device_JointsBasis/DeviceHandler.h#L30).<br>
In this function, the device handler will be built and returned as a reference to the API.<br>
The function will also check the used API version, make sure to keep it the latest!
<br>
<br>

## **Device handlers**
After playing with [the samples](https://github.com/KinectToVR/K2TrackingDevice-Samples) you should have enough knowledge to write a device plugin yourself.<br>
Remember that `initialize()`, `update()` and `shutdown()` are the most ever important functions.<br>
(At least for the devices which actually provide tracking and not only grab it from the app)<br>
<br>

## **Device settings**
Since `K2API` v7 you also can add custom, device-provided settings to each device plugin.<br>
Currently, the whole idea of using the `ktvr::Interface::` works similar to how `ImGUI` works.<br>
You create elements (element pointers), set them up, and push to the UI via selected functions.<br>
Of course you can manage them later, either via their pointers or inside handler functions.<br>

When Amethyst is ready for plugin-wise UI construction, it's gonna call the `OnLoad()` function,<br>
[you can override it to set your UI up](https://github.com/KinectToVR/K2TrackingDevice-Samples/blob/main/device_KinectBasis_Full_Settings/DeviceHandler.h#L28). Remember your device must report `settingsSupported` as true!<br>
<br>
### `AppendSingleElement`
This function will append a single `UIElement` to the UI layout. It will appear on a single line,<br>
either snapped to the left, right or centered - it's to your liking. (Though default is snap to the left)
<br> <br>
### `AppendElementPair`
If you want to append two elements to a single row, use this function. The column space for each<br>
appended `UIElement` will be the same - 50% (or one star unit) Elements will be centered in their columns.
<br> <br>
### `AppendElementVector`
When there's a need to append even more elements to one row, you should use this function.<br>
It will work exactly the same as `AppendElementPair`, but of course will create more columns.<br>
You don't need to create an additional vector to apply the elements, you can just go like this:
```c++
// Let's say you have a label (TextBlock) and two buttons already created
AppendElementVector({ myTextBlock, myButton_1, myButton_2 }); // Notice the braces
```
<br>

### Handlers
You can register handlers for buttons, number boxes, check boxes, etc. as you would normally do.<br>
Let's say that you have a button `myButton` and want to change its text (content) on click.
```c++
// Let's say that you created your button like this:
Button* myButton = CreateButton("Click Me!");
// And then added it to the device settings like this:
this->layoutRoot->AppendSingleElement(myButton);
```

Now that you've created your button, you want to register a handler for it.
```c++
// You can either create a functions for it, like this:
void myButton_OnClick(Button* sender) {
  // Change the content to "Clicked!"
  sender->Content("Clicked!");
}
// And then register it at Onload() via:
myButton->OnClick = myButton_OnClick;
```

If you don't want to define and create additional  functions for it, you can just:
```c++
// Or you can dynamically create a handler function:
myButton->OnClick = [] (Button* sender) {
  // Change the content to "Clicked!"
  sender->Content("Clicked!");
};
```

Both scenarios will result in the same action - the button will change its content on click.<br>
Of course you don't need to access the `sender` to access the button. If you've defined it<br>
in some outer scope, you can *just* access it. Though, remember that you can use `Create` functions<br>
only after `OnLoad()` was called, never before it. You'd need to define a `Button*` and assign it later.<br>

Now, device handlers may involve additional function parameters, like `int const& new_value`.<br>
Just make sure to follow the function's template, it's always named like `On[Something]`.
<br> <br>

## OK, but what's actually in `external/vendor`?
Ah yes. For the sake of K2API's simplicity in its current state, it's been split into 3 parts.<br>
The currently used part, `devices` part, allows us to make device plugins and provide the whole tracking.<br>
It's been grabbed from the newest `KinectToVR API` project files and put here for simplicity reasons.<br>
If this part will ever get an interface update, you're going to see in logs that your device<br>
has provided an incorrect `IK2API_Devices_Version` of the interface, and you'll be able to update it.
<br> <br>

## I've read it all, now what?
It seems that you're really motivated for this, nice! [Please have a thorough look at the repo](https://github.com/KinectToVR/K2TrackingDevice-Samples).<br>
You're going to find samples for all currently supported device types & characteristics,<br>
including a device with custom settings, and a spectator (data-poller) device.
