using System.Diagnostics;
using System.Numerics;

namespace Device_Managed_KinectBasis.API_Projection_Files;

public abstract class AmethystManagedDevice_Kinect
{
    public string DeviceName = "[NAME NOT SET]"; // Make sure you set this!
    public const uint DeviceType = (uint)TrackingDeviceType.K2_Kinect;
    public uint DeviceCharacteristics = (uint)TrackingDeviceCharacteristics.K2_Character_Unknown;

    public bool IsFlipSupported = false;
    public bool IsAppOrientationSupported = false;
    public bool IsSkeletonTracked = false;

    public Vector3[] JointPositions = new Vector3[25]; // Vector3<float>
    public Quaternion[] JointOrientations = new Quaternion[25]; // Quaternion<float>
    public uint[] JointTrackingStates = new uint[25]; // TrackedJointState

    public abstract bool Initialize();

    public abstract void Update();

    public abstract bool Shutdown();

    public abstract long GetDeviceStatus();

    public abstract string GetDeviceStatusString();
}

internal enum TrackingDeviceCharacteristics
{
    // Not set???
    K2_Character_Unknown,

    // NO mathbased, only [ head, waist, ankles ]
    K2_Character_Basic,

    // SUP mathbased, only [ head, elbows, waist, knees, ankles, foot_tips ]
    K2_Character_Simple,

    // SUP mathbased, [ everything ]
    K2_Character_Full
};

internal enum TrackedJointState
{
    State_NotTracked,
    State_Inferred,
    State_Tracked
};

internal enum TrackingDeviceType
{
    K2_Unknown,
    K2_Kinect,
    K2_Joints
};