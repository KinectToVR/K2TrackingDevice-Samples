using Managed_JointsBasis_Handler.API_Projection_Files;

namespace Managed_JointsBasis_Handler;

public class AmethystDevice : AmethystManagedDevice_Joints
{
    public AmethystDevice()
    {
        // Set up the device name
        DeviceName = "JointsBasis (Managed)";
        DeviceGUID = "KSAMPLES-VEND-API1-DVCE-DVCEJOINTMGD";
    }

    public override void OnLoad()
    {
        // You can use everything from the managed API
        // only after this function has been called

        Log("[Managed Joints] Loading...", 0);
    }

    public override bool Initialize()
    {
        // Set up or refresh your device here,
        // I suggest you use additional BOOLs
        // to check if you've already done that
        Log("[Managed Joints] Initalizing...", 0);

        // Rebuild the joint list
        JointsList.Clear(); // Erase all

        // Add 5 dummy joints
        for (uint i = 0; i < 5; i++)
            JointsList.Add(new TrackedJoint { JointName = $"Joint {i}" });

        return true; // We're done
    }

    public override void Update()
    {
        // Update your tracking here,
        // to mark if the user is tracked
        // use the IsSkeletonTracked BOOL

        // Use a dummy random position for each joint
        var random = new Random();

        foreach (var joint in JointsList)
        {
            // Random position (max 3sqrt2m from 0,0,0)
            joint.Position.X = random.Next(-300, 300) / 100f;
            joint.Position.Y = random.Next(-300, 300) / 100f;
            joint.Position.Z = random.Next(-300, 300) / 100f;

            // This joint is being tracked atm
            joint.TrackingState = (uint)TrackedJointState.State_Tracked;
        }

        // Mark skeleton as tracked
        IsSkeletonTracked = true;
    }

    public override bool Shutdown()
    {
        // Shut your device down here
        // (This will likely be called only once)
        Log("[Managed Joints] Shutting down...", 0);

        return true; // Return the result (not checked)
    }

    public override long GetDeviceStatus()
    {
        // Check and return your device's status here,
        // S_OK or 0 is equal to "Success"

        return 0; // S_OK
    }

    public override string GetDeviceStatusString()
    {
        // Either GetDeviceStatus() here or something else,
        // the goal is to string-ize the status result
        // Format: "[SHORT STATUS]\n[ABBREVIATION]\n[MESSAGE]"
        // Note: A success status abbreviation MUST be "S_OK"

        return "Success!\nS_OK\nEverything's good!";
    }
}