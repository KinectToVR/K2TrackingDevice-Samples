﻿using Managed_SkeletonBasis_Handler.API_Projection_Files;

namespace Managed_SkeletonBasis_Handler;

public class AmethystDevice : AmethystManagedDevice_Kinect
{
    public AmethystDevice()
    {
        // Set up the device name
        DeviceName = "SkeletonBasis (Managed)";
        DeviceGUID = "KSAMPLES-VEND-API1-DVCE-DVCESKELTMGD";

        // Provide every joint possible in Amethyst API
        DeviceCharacteristics = (uint)TrackingDeviceCharacteristics.K2_Character_Full;

        IsFlipSupported = false; // No flip for us :(
        IsAppOrientationSupported = false; // Neither MathBased
    }

    public override void OnLoad()
    {
        // You can use everything from the managed API
        // only after this function has been called

        Log("[Managed Kinect] Loading...", 0);
    }

    public override bool Initialize()
    {
        // Set up or refresh your device here,
        // I suggest you use additional BOOLs
        // to check if you've already done that
        Log("[Managed Kinect] Initalizing...", 0);

        return true;
    }

    public override void Update()
    {
        // Update your tracking here,
        // to mark if the user is tracked
        // use the IsSkeletonTracked BOOL
        IsSkeletonTracked = true;
    }

    public override bool Shutdown()
    {
        // Shut your device down here
        // (This will likely be called only once)
        Log("[Managed Kinect] Shutting down...", 0);

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