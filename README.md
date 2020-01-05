# FF13Fix
Performance and bug fixes for the PC versions of FF13 and FF13-2

# Notes
This is a fork of [OneTweakNG](https://github.com/Nucleoprotein/OneTweakNG), containing additional fixes for FF13.
Thanks Nucleoprotein for starting this!

# Installation
Download the latest [release](https://github.com/rebtd7/FF13Fix/releases) and add both ```dinput8.dll``` and ```FF13Fix.ini``` to the folder ```FINAL FANTASY XIII\white_data\prog\win\bin``` for FF13 and to the folder ```FINAL FANTASY XIII-2\alba_data\prog\win\bin``` for FF13-2.

The frame rate is uncapped by default, but as VSync is enabled it may be limited by your monitor refresh rate.

You can to change ```FF13Fix.ini``` if you want to use the ingame limiter to cap the framerate.

# What this does

## Removes the awful frame pacer
I don't see the point in it. It is overly aggressive. Disabling this greatly improves the frame rate in certain situations.

## Removes stuttering caused by new controllers being scanned every second
This caused stuttering specially if you didn't have a connected controller after the last opening of Steam.

Note that if you want to use a controller you need to connect it before opening the game (i.e. this patch removes the hotplugging support)

## Uncaps the frame rate
I did not see any negatives on unlocking the frame rate, but your mileage may very. If you are having issues, edit ```FF13Fix.ini``` to cap the frame rate.

## Enables Triple Buffering
This may make the frame rate more consistent.

## Uses your desktop monitor refresh rate in full screen mode
By default the game forced a 60Hz refresh rate in full screen mode. With this mod the game uses the same refresh rate that you are using in your operating system.

## Changes to where the memory is allocated on certain vertex buffers (FF13 only)
This considerably improves the frame rate when 2D elements are being disabled on the screen (i.e. minimap or battle menu HUD). This fix is not new, it is from [OneTweakNG](https://github.com/Nucleoprotein/OneTweakNG).

## Fix the enemy scan text on resolutions over 720p (FF13 only)
The game calls [SetScissorRect](https://docs.microsoft.com/en-us/windows/win32/api/d3d9helper/nf-d3d9helper-idirect3ddevice9-setscissorrect) using a rectangle hardcoded with the 720p coordenates. This correct the coordenates and rectangle size in order to fix it.

## Other notes
* This is currently not compatible with GeDoSaTo. 
* I strongly recommend forcing anisotropic filtering on your GPU driver to improve the quality of the textures.
* Using "Maximum Performance" power management in the GPU driver can also help keeping the frame rate smooth. 
