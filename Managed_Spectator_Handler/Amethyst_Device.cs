using System;
using System.Numerics;
using System.Runtime.InteropServices;
using Device_Managed_Spectator.API_Projection_Files;

namespace Device_Managed_Spectator;

public class AmethystDevice : AmethystManagedDevice_Spectator
{
    public AmethystDevice()
    {
    }

    public override void OnLoad()
    {
        // You can use everything from the managed API
        // only after this function has been called

        Log("[Managed Spectator] Loading...", 0);
    }
}