using System.Numerics;

namespace Device_Managed_KinectBasis.API_Projection_Files;

public class TrackedJoint
{
    public TrackedJoint()
    {
    }

    public TrackedJoint(Vector3 position, Quaternion orientation, string name)
    {
        Position = position;
        Orientation = orientation;
        JointName = name;
    }

    public Vector3 Position = new Vector3();
    public Quaternion Orientation = new Quaternion();
    public uint TrackingState = 0; // State_NotTracked
    public string JointName = "[NAME NOT SET]";
}

public abstract class AmethystManagedDevice_Kinect
{
    public const uint DeviceType = (uint)TrackingDeviceType.K2_Kinect;
    public uint DeviceCharacteristics = (uint)TrackingDeviceCharacteristics.K2_Character_Unknown;
    public string DeviceName = "[NAME NOT SET]"; // Make sure you set this!
    public bool IsAppOrientationSupported = false;

    public bool IsFlipSupported = false;
    public bool IsSkeletonTracked = false;
    public Quaternion[] JointOrientations = new Quaternion[25]; // Quaternion<float>

    public Vector3[] JointPositions = new Vector3[25]; // Vector3<float>
    public uint[] JointTrackingStates = new uint[25]; // TrackedJointState

    // Log a message to Amethyst logs : handler
    public Action<string, uint>? LoggerAction;

    public Func<Tuple<Vector3, Quaternion>>? GetHMDPose;
    public Func<Tuple<Vector3, Quaternion>>? GetHMDPoseCalibrated;
    public Func<Tuple<Vector3, Quaternion>>? GetLeftControllerPose;
    public Func<Tuple<Vector3, Quaternion>>? GetLeftControllerPoseCalibrated;
    public Func<Tuple<Vector3, Quaternion>>? GetRightControllerPose;
    public Func<Tuple<Vector3, Quaternion>>? GetRightControllerPoseCalibrated;
    public Func<float>? GetHMDOrientationYaw;
    public Func<float>? GetHMDOrientationYawCalibrated;
    public Func<TrackedJoint[]>? GetAppJointPoses;

    // Log a message to Amethyst logs : wrapper
    public void Log(string msg, LogSeverity sev)
    {
        LoggerAction?.Invoke(msg, (uint)sev);
    }

    public abstract void OnLoad();

    public abstract bool Initialize();

    public abstract void Update();

    public abstract bool Shutdown();

    public abstract long GetDeviceStatus();

    public abstract string GetDeviceStatusString();
}

public enum TrackingDeviceCharacteristics
{
    // Not set???
    K2_Character_Unknown,

    // NO mathbased, only [ head, waist, ankles ]
    K2_Character_Basic,

    // SUP mathbased, only [ head, elbows, waist, knees, ankles, foot_tips ]
    K2_Character_Simple,

    // SUP mathbased, [ everything ]
    K2_Character_Full
}

public enum TrackedJointState
{
    State_NotTracked,
    State_Inferred,
    State_Tracked
}

public enum TrackingDeviceType
{
    K2_Unknown,
    K2_Kinect,
    K2_Joints
}

public enum LogSeverity
{
    INFO, // 0
    WARN, // 1
    ERROR // 2
}