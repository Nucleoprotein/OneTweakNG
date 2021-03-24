# FF13Fix
Performance and bug fixes for the PC versions of FF13 and FF13-2

https://github.com/rebtd7/FF13Fix

# Notes
This is a fork of [OneTweakNG](https://github.com/Nucleoprotein/OneTweakNG), containing additional fixes for FF13.
Thanks Nucleoprotein for starting this!

# Installation
Download the latest [release](https://github.com/rebtd7/FF13Fix/releases) and add both ```d3d9.dll``` and ```FF13Fix.ini``` to the folder ```FINAL FANTASY XIII\white_data\prog\win\bin``` for FF13 and to the folder ```FINAL FANTASY XIII-2\alba_data\prog\win\bin``` for FF13-2.

You can to change ```FF13Fix.ini``` if you want to change any configuration (e.g. uncap the framerate or disable controller vibration).

# What this does

## Removes the frame pacer
Disabling this greatly improves the frame rate in certain situations.

## Removes stuttering caused by new controllers being scanned every second
This caused stuttering specially if you didn't have a connected controller after the last opening of Steam.

Note that if you want to use a controller you need to connect it before opening the game (i.e. this patch removes the hotplugging support)

## Enables controller vibration
This can be enabled/disabled in the ```FF13Fix.ini``` . 

## Uncaps the frame rate (optional)
Using higher frame rates can cause jankness on facial animations during ingame cutscenes.
See https://github.com/rebtd7/FF13Fix/issues/3

## Enables Triple Buffering
This may make the frame rate more consistent.

## Uses your desktop monitor refresh rate in full screen mode
By default the game forced a 60Hz refresh rate in full screen mode. With this mod the game uses the same refresh rate that you are using in your operating system.

## Changes to where the memory is allocated on certain vertex buffers
This considerably improves the frame rate when 2D elements are being disabled on the screen (i.e. minimap or battle menu HUD). This fix is not new, it is from [OneTweakNG](https://github.com/Nucleoprotein/OneTweakNG).

## Fixes misaligned screen space effects (fixes 1440p resolution)

## Fix the enemy scan text on resolutions over 720p (FFXIII only)
The game calls [SetScissorRect](https://docs.microsoft.com/en-us/windows/win32/api/d3d9helper/nf-d3d9helper-idirect3ddevice9-setscissorrect) using a rectangle hardcoded with the 720p coordenates. This correct the coordenates and rectangle size in order to fix it.

# Usage with DXVK
* Rename DXVK's x86 ```d3d9.dll``` file to ```dxvk.dll```. Keep FF13Fix as ```d3d9.dll```.


# Usage with the 4GB Large Address Aware patch (Recommended)
You may wish to patch the games to allow them to access more than 2GB of RAM. 
This seems to avoid crashes in FF13-2 and FF13 (especially when using triple buffering, playing on 4K+ resolutions, using DXVK and/or Reshade, or using HD texture/model mods).
## FF13:
* Create a copy of the unpatched ```ffxiiiimg.exe``` to the folder ```FINAL FANTASY XIII\white_data\prog\win\bin```. Name it ```untouched.exe```.
* Patch the original ```ffxiiiimg.exe``` (you can use https://ntcore.com/?page_id=371)
## FF13-2: 
* Patch ```ffxiii2img.exe``` (you can use https://ntcore.com/?page_id=371)

# Usage with Reshade

## Regular Direct3D9 users:
* Install ReShade first. Rename ReShade's ```d3d9.dll``` to ```ReShade32.dll```; Install FF13Fix normally, keeping FF13Fix as ```d3d9.dll```
## DXVK users: 
* Install ReShade targeting Vulkan; enable it globally

# Reporting issues
* Please specify what game are you talking about, which mods are you using (dxvk?) post system specs, and post FF13Fix.log
* Add a save file and steps to reproduce the issue if possible

# Other notes
* This is currently not compatible with GeDoSaTo. 
* I strongly recommend forcing anisotropic filtering on your GPU driver to improve the quality of the textures.
* Using "Maximum Performance" power management in the GPU driver can also help keeping the frame rate smooth. 

