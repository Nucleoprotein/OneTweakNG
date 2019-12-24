# FF13Fix
Fixes for the PC version of FF13

# Notes
This is a fork of [OneTweakNG](https://github.com/Nucleoprotein/OneTweakNG), containing additional fixes for FF13.
Thanks Nucleoprotein for starting this!

# Installation
Download the latest [release](https://github.com/rebtd7/FF13Fix/releases) and add both ```dinput8.dll``` and ```FF13Fix.ini``` to the folder ```FINAL FANTASY XIII\white_data\prog\win\bin```.

You may want to change ```FF13Fix.ini``` if you want to cap the frame rate.

# What this does

## Removes the awful frame pacer
I don't see the point in it. It is overly aggressive. I did not see any n

## Removes stuttering caused by new controllers being scanned every second
This caused stuttering specially if you didn't have a connected controller after the last opening of Steam.

Note that if you want to use a controller you need to connect it before opening the game (i.e. this patch removes the hotplugging support)

## Fix the enemy scan text on resolutions over 720p
The game calls [SetScissorRect](https://docs.microsoft.com/en-us/windows/win32/api/d3d9helper/nf-d3d9helper-idirect3ddevice9-setscissorrect) using a rectangle hardcoded with the 720p coordenates. This patches fixes it.

## Uncaps the frame rate
I did not see any negatives on unlocking the frame rate, but your mileage may very. If you are having issues, edit ```FF13Fix.ini``` to cap the frame rate.

## Enables Triple Buffering
This seems to make the frame rate more consistent.

## Changes to where the memory is allocated on certain vertex buffers
This seems to improve the frame rate. This fix is not new, it is from [OneTweakNG](https://github.com/Nucleoprotein/OneTweakNG).
