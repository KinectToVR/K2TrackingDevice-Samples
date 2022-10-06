using Managed_SkeletonBasis_Handler.API_Projection_Files;

namespace Managed_SkeletonBasis_Handler;

public class AmethystDevice : AmethystManagedDevice_Skeleton
{
    // Has this plugin been loaded?
    private bool _loaded;

    // Status preset
    private string _status = "Success!";

    // Don't construct it yet!
    private TextBox _statusTextBox;

    public AmethystDevice()
    {
        // Set up the device name
        DeviceName = "SkeletonBasis (Managed)";
        DeviceGUID = "KSAMPLES-VEND-API1-DVCE-DVCESKELTMGD";

        // Provide every joint possible in Amethyst API
        DeviceCharacteristics = (uint)TrackingDeviceCharacteristics.K2_Character_Full;

        // Settings are supported
        Flags_SettingsSupported = true;

        // For all the remaining flags, please see
        // the AmethystManagedDevice_Skeleton class
        // and Amethyst Dev docs / plugin docs
    }

    public override void OnLoad()
    {
        // You can use everything from the managed API
        // only after this function has been called

        Log("[Managed Kinect] Loading...", 0);

        // Create a new text input
        _statusTextBox = new TextBox
        {
            Text = _status, // Pre-apply text
            OnEnterKeyDown = // Handler
                text =>
                {
                    // Give up if pre-init
                    if (!_loaded) return;

                    // Overwrite status
                    _status = text;
                    // Force ame to refresh
                    RefreshStatusUI();
                }
        };

        // Add the created elements to the UI panel
        AppendElementPairStack(
            // The label
            new TextBlock("Change status:") { IsPrimary = false },
            // The text box
            _statusTextBox);

        // Mark our plugin as loaded
        _loaded = true;
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

        // Return our custom status and the rest (r not shown)
        return _status + "\nS_OK\nEverything's good!";
    }
}