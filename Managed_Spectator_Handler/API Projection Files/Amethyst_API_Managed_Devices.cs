using System.Numerics;

namespace Managed_Spectator_Handler.API_Projection_Files;

public class TrackedJoint
{
    public string JointName = "[NAME NOT SET]";

    public Quaternion Orientation;

    public Vector3 Position,
        Velocity,
        Acceleration,
        AngularVelocity,
        AngularAcceleration;

    public uint TrackingState =
        (uint)TrackedJointState.State_NotTracked;

    public TrackedJoint()
    {
    }

    public TrackedJoint(
        Vector3 position,
        Quaternion orientation,
        string name)
    {
        Position = position;
        Orientation = orientation;
        JointName = name;
    }
}

public abstract class AmethystManagedDevice_Spectator
{
    public Func<TrackedJoint[]>? GetAppJointPoses;
    public Func<double>? GetHMDOrientationYaw;
    public Func<double>? GetHMDOrientationYawCalibrated;

    public Func<Tuple<Vector3, Quaternion>>? GetHMDPose;
    public Func<Tuple<Vector3, Quaternion>>? GetHMDPoseCalibrated;
    public Func<Tuple<Vector3, Quaternion>>? GetLeftControllerPose;
    public Func<Tuple<Vector3, Quaternion>>? GetLeftControllerPoseCalibrated;
    public Func<Tuple<Vector3, Quaternion>>? GetRightControllerPose;

    public Func<Tuple<Vector3, Quaternion>>? GetRightControllerPoseCalibrated;
    
    public abstract void OnLoad();
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