#ifdef SETTING

SETTING(s32, LongValue, PresentationInterval, Options, 0, 
	L"# PresentationInterval (V-Sync)\n"
	L"#\n"
	L"# -1 = V-Sync forced off\n"
	L"#  0 = V-Sync not changed (default)\n"
	L"#  1 = V-Sync forced on"
);

SETTING(s32, LongValue, TripleBuffering, Options, -1, 
	L"# TripleBuffering\n"
	L"#\n"
	L"# -1 = Auto (Disabled for dxvk.dll; else, enabled) (default)\n"
	L"#  0 = Off\n"
	L"#  1 = On"
);

SETTING(s32, LongValue, FullScreenRefreshRate, Options, 0, 
	L"# FullScreenRefreshRate\n"
	L"#\n"
	L"# If you are not using any adaptive sync technology (e.g. FreeSync/Gsync) you may want to set this to the highest\n"
	L"#   multiple of your frame rate that your monitor can handle to improve frame pacing.\n"
	L"# i.e. if you are using a 60FPS cap, you can possibly improve the frame pacing when you are reaching the cap\n"
	L"#   by using refresh rates 60, 120, 180, 240...\n"
	L"#\n"
	L"#  0    = Same as configured in the operating system\n"
	L"# other = Use the number provided as the display refresh rate (in Hz)"
);

SETTING(s32, LongValue, SwapEffect, Options, -1, 0);
SETTING(bool, BoolValue, AlwaysActive, Options, false, 0);
SETTING(bool, BoolValue, AutoFix, Options, true, 0);
SETTING(s32, LongValue, Multisample, Options, 0, 0);
SETTING(bool, BoolValue, HideCursor, Options, false, 0);
SETTING(bool, BoolValue, ForceHideCursor, Options, false, 0);
SETTING(u32, LongValue, BehaviorFlags, Options, 0, 0);

SETTING(s32, LongValue, IngameFrameRateLimit, FFXIII, 0, 
	L"# IngameFrameRateLimit\n"
	L"#\n"
	L"# Using higher numbers may cause noticiable facial animation jankness during cutscenes.\n"
	L"# See https://github.com/rebtd7/FF13Fix/issues/3\n"
	L"#\n"
	L"# Note that the frame pacer is always disabled by this mod, regardless of the value this option is set.\n"
	L"#\n"
	L"# -1    = Use the maximum FPS cap possible\n"
	L"#  0    = Use the game's default FPS limit (59.94005966 FPS)\n"
	L"# other = Use the number provided as the FPS limit (e.g. set 30 to limit to 30 FPS)"
);
SETTING(bool, BoolValue, DiscardUIVertexBuffer, FFXIII, false,
	L"# DiscardUIVertexBuffer (EXPERIMENTAL!)\n"
	L"#\n"
	L"# Discards UI vertex buffer to improve performance\n"
	L"#\n"
	L"# false = default, do not hook and discard vertex buffer\n"
	L"# true  = hook and discard vertex buffer"
);
SETTING(bool, BoolValue, DisableIngameControllerHotSwapping, FFXIII, true,
	L"# DisableIngameControllerHotSwapping\n"
	L"#\n"
	L"# By default FF13Fix disables the game's continuous controller scanning that causes stuttering (especially if you do not have any controller connected)\n"
	L"# If you with you can enable it again (by setting the config to 'false', so you can re-connect your controller while playing.\n"
	L"# Note that EnableControllerVibration is incompatible with the controller hotswapping, \n"
	L"#  so it is automatically disabled if DisableIngameControllerHotSwapping is set to 'false'"
);
SETTING(bool, BoolValue, EnableControllerVibration, FFXIII, true, 
	L"# EnableControllerVibration\n"
	L"#\n"
	L"# Enables controller vibration on the first connected XInput device."
);
SETTING(bool, DoubleValue, VibrationStrengthFactor, FFXIII, 2.0, 
	L"# VibrationStrengthFactor\n"
	L"#\n"
	L"# Higher numbers = stronger vibration"
);

SETTING(bool, BoolValue, Adapter, Adapter, false, 0);
SETTING(u32, LongValue, VendorId, Adapter, 0, 0);
SETTING(u32, LongValue, DeviceId, Adapter, 0, 0);

SETTING(bool, BoolValue, TopMost, Window, false, 0);
SETTING(std::wstring, StringValue, WindowClass, Window, L"", 0);
SETTING(std::wstring, StringValue, WindowName, Window, L"", 0);

SETTING(bool, BoolValue, Borderless, Borderless, false, 0);
SETTING(bool, BoolValue, ForceWindowedMode, Borderless, false, 0);
SETTING(bool, BoolValue, AllWindows, Borderless, false, 0);

SETTING(s32, LongValue, LogLevel, Log, 2,
	L"# LogLevel\n"
	L"#\n"
	L"#  0 = trace\n"
	L"#  1 = debug\n"
	L"#  2 = info (default)\n"
	L"#  3 = warning\n"
	L"#  4 = error\n"
	L"#  5 = critical"
);

SETTING(bool, BoolValue, LogFlush, Log, false,
	L"# LogFlush\n"
	L"#\n"
	L"#  true = flush log file every line (very slow!)\n"
	L"#  false = flush log file only when error level message is logged (default)"
);


#endif
