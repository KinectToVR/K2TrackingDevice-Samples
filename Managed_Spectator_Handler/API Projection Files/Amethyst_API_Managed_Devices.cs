using System.Diagnostics;
using System.Numerics;

namespace Device_Managed_Spectator.API_Projection_Files;

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

public abstract class AmethystManagedDevice_Spectator
{
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
}

public enum TrackedJointState
{
    State_NotTracked,
    State_Inferred,
    State_Tracked
};

public enum LogSeverity
{
    INFO, // 0
    WARN, // 1
    ERROR // 2
}