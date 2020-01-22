
SETTING(u32, LongValue, PresentationInterval, Options, 1);
SETTING(bool, BoolValue, TripleBuffering, Options, true);
SETTING(u32, LongValue, FullScreenRefreshRate, Options, 0);
SETTING(u32, LongValue, SwapEffect, Options, -1);
SETTING(bool, BoolValue, AlwaysActive, Options, false);
SETTING(bool, BoolValue, AutoFix, Options, true);
SETTING(u32, LongValue, Multisample, Options, 0);
SETTING(bool, BoolValue, HideCursor, Options, false);
SETTING(bool, BoolValue, ForceHideCursor, Options, false);
SETTING(u32, LongValue, BehaviorFlags, Options, 0);

SETTING(double, DoubleValue, FFXIIIIngameFrameRateLimit, Options, 0.0);
SETTING(bool, BoolValue, FFXIIIEnableControllerVibration, Options, true);

SETTING(bool, BoolValue, Adapter, Adapter, false);
SETTING(u32, LongValue, VendorId, Adapter, 0);
SETTING(u32, LongValue, DeviceId, Adapter, 0);

SETTING(bool, BoolValue, Borderless, Borderless, false);
SETTING(bool, BoolValue, ForceWindowedMode, Borderless, false);
SETTING(bool, BoolValue, AllWindows, Borderless, false);
SETTING(bool, BoolValue, TopMost, Borderless, false);
SETTING(std::wstring, StringValue, WindowClass, Borderless, L"");
SETTING(std::wstring, StringValue, WindowName, Borderless, L"");

