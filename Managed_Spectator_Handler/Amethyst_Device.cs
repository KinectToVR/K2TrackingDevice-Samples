using Managed_Spectator_Handler.API_Projection_Files;

namespace Managed_Spectator_Handler;

public class AmethystDevice : AmethystManagedDevice_Spectator
{
    public override void OnLoad()
    {
        // You can use everything from the managed API
        // only after this function has been called

        Log("[Managed Spectator] Loading...", 0);
    }
}