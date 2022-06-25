using System.Diagnostics;
using System.Numerics;

namespace Device_Managed_JointsBasis.API_Projection_Files;

public abstract class AmethystManagedDevice_Joints
{
    public string DeviceName = "[NAME NOT SET]"; // Make sure you set this!
    public const uint DeviceType = (uint)TrackingDeviceType.K2_Joints;

    public bool IsSkeletonTracked = false;

    public class TrackedJoint
    {
        public Vector3 Position = new Vector3();
        public Quaternion Orientation = new Quaternion();
        public uint TrackingState = 0; // State_NotTracked
        public string JointName = "[NAME NOT SET]";
    }

    public List<TrackedJoint> JointsList = new();

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