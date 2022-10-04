using System.Numerics;

namespace Managed_SkeletonBasis_Handler.API_Projection_Files;

public class TrackedJoint
{
    public string JointName = "[NAME NOT SET]";
    public Quaternion Orientation;

    public Vector3 Position;
    public uint TrackingState = 0; // State_NotTracked

    public TrackedJoint()
    {
    }

    public TrackedJoint(Vector3 position, Quaternion orientation, string name)
    {
        Position = position;
        Orientation = orientation;
        JointName = name;
    }
}

public abstract class AmethystManagedDevice_Kinect
{
    public uint DeviceCharacteristics = (uint)TrackingDeviceCharacteristics.K2_Character_Unknown;
    public string DeviceGUID = "INVALID"; // Make sure you set this!
    public string DeviceName = "[NAME NOT SET]"; // Make sure you set this!
    public Func<TrackedJoint[]>? GetAppJointPoses;
    public Func<double>? GetHMDOrientationYaw;
    public Func<double>? GetHMDOrientationYawCalibrated;

    public Func<Tuple<Vector3, Quaternion>>? GetHMDPose;
    public Func<Tuple<Vector3, Quaternion>>? GetHMDPoseCalibrated;
    public Func<Tuple<Vector3, Quaternion>>? GetLeftControllerPose;
    public Func<Tuple<Vector3, Quaternion>>? GetLeftControllerPoseCalibrated;
    public Func<Tuple<Vector3, Quaternion>>? GetRightControllerPose;
    public Func<Tuple<Vector3, Quaternion>>? GetRightControllerPoseCalibrated;
    public bool IsAppOrientationSupported = false;

    public bool IsFlipSupported = false;
    public bool IsSkeletonTracked = false;

    public List<TrackedJoint> JointsList = new();

    // Log a message to Amethyst logs : handler
    public Action<string, uint>? LoggerAction;

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

public enum LogSeverity
{
    INFO, // 0
    WARN, // 1
    ERROR // 2
}