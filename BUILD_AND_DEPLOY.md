## **Build instructions**
You'll need:
 - The `K2TrackingDevice-Samples` repo cloned somewhere and `cd`'d into
 - (For testing purpose) Working installation of Amethyst and SteamVR

Follow these steps:

- [Install tools for the Windows App development](https://docs.microsoft.com/en-us/windows/apps/windows-app-sdk/set-up-your-development-environment?tabs=vs-2022-17-1-a%2Cvs-2022-17-1-b).<br>
  You'll have to install Visual Studio 2022 or its Build Tools.

- Set up external dependencies
  ```powershell
  git submodule update --init
  ```

- Build Samples:<br>
  ```powershell
  # Download the vswhere tool to find msbuild without additional interactions
  > Invoke-WebRequest -Uri 'https://github.com/microsoft/vswhere/releases/latest/download/vswhere.exe' -OutFile './vswhere.exe'
  # Use the downloaded vswhere tool to find msbuild. Skip this step if you use the Dev Powershell
  > $msbuild = "$("$(.\vswhere.exe -legacy -prerelease -products * -format json | Select-String -Pattern "2022" | `
        Select-String -Pattern "Studio" | Select-Object -First 1 | Select-String -Pattern "installationPath")" `
        -replace('"installationPath": "','') -replace('",',''))".Trim() + "\\MSBuild\\Current\\Bin\\MSBuild.exe"

  # Restore NuGet packages and build everything
  > &"$msbuild" K2TrackingDevice_Samples.sln "/p:Configuration=Release;Platform=x64"
  ```

## **Deployment**
The whole output can be found in `x64/Release` directory<br>
(or `x64/Debug` if you're building for `Debug`, naturally) and:
 - Devices (plugins) are inside `devices/` folder

Note: Debug builds will only work with a debug host,<br>
the same schema applies to OpenVR Driver, the API and Amethyst.