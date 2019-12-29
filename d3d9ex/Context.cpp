#include "stdafx.h"

#include "Wrapper.h"

#include "Context.h"
#include "IDirect3D9.h"

MainContext context;

Config::Config()
{
	std::string inifile = FullPathFromPath(inifilename);

	CSimpleIniW ini;
	ini.LoadFile(inifile.c_str());

	u32 config_version = ini.GetLongValue(L"Version", L"Config");
	if (config_version != CONFIG_VERSION)
	{
		// save file and reload
		ini.Reset();

#define SETTING(_type, _func, _var, _section, _defaultval) \
	ini.Set##_func(L#_section, L#_var, _defaultval)
#include "Settings.h"
#undef SETTING

		ini.SetLongValue(L"Version", L"Config", CONFIG_VERSION);
		ini.SaveFile(inifile.c_str());
		ini.Reset();
		ini.LoadFile(inifile.c_str());
	}

#define SETTING(_type, _func, _var, _section, _defaultval) \
	_var = ini.Get##_func(L#_section, L#_var)
#include "Settings.h"
#undef SETTING
}

MainContext::MainContext() : oldWndProc(nullptr)
{
	LogFile("FF13Fix.log");

	if (config.GetAutoFix()) EnableAutoFix();

	MH_Initialize();

	MH_CreateHook(D3D9DLL::Get().Direct3DCreate9, HookDirect3DCreate9, reinterpret_cast<void**>(&TrueDirect3DCreate9));
	MH_EnableHook(D3D9DLL::Get().Direct3DCreate9);

	MH_CreateHook(CreateWindowExA, HookCreateWindowExA, reinterpret_cast<void**>(&TrueCreateWindowExA));
	MH_EnableHook(CreateWindowExA);

	MH_CreateHook(CreateWindowExW, HookCreateWindowExW, reinterpret_cast<void**>(&TrueCreateWindowExW));
	MH_EnableHook(CreateWindowExW);

	MH_CreateHook(SetWindowLongA, HookSetWindowLongA, reinterpret_cast<void**>(&TrueSetWindowLongA));
	MH_EnableHook(SetWindowLongA);

	MH_CreateHook(SetWindowLongW, HookSetWindowLongW, reinterpret_cast<void**>(&TrueSetWindowLongW));
	MH_EnableHook(SetWindowLongW);
}

MainContext::~MainContext()
{
	while (::ShowCursor(TRUE) <= 0);
}

IDirect3D9* WINAPI MainContext::HookDirect3DCreate9(UINT SDKVersion)
{
	IDirect3D9* d3d9 = context.TrueDirect3DCreate9(SDKVersion);
	if (d3d9)
	{
		return new hkIDirect3D9(d3d9);
	}

	return d3d9;
}

bool MainContext::BehaviorFlagsToString(DWORD BehaviorFlags, std::string* BehaviorFlagsString)
{
#define BF2STR(x) if (BehaviorFlags & x) BehaviorFlagsString->append(#x" ");

	BF2STR(D3DCREATE_ADAPTERGROUP_DEVICE);
	BF2STR(D3DCREATE_DISABLE_DRIVER_MANAGEMENT);
	BF2STR(D3DCREATE_DISABLE_DRIVER_MANAGEMENT_EX);
	BF2STR(D3DCREATE_DISABLE_PRINTSCREEN);
	BF2STR(D3DCREATE_DISABLE_PSGP_THREADING);
	BF2STR(D3DCREATE_ENABLE_PRESENTSTATS);
	BF2STR(D3DCREATE_FPU_PRESERVE);
	BF2STR(D3DCREATE_HARDWARE_VERTEXPROCESSING);
	BF2STR(D3DCREATE_MIXED_VERTEXPROCESSING);
	BF2STR(D3DCREATE_SOFTWARE_VERTEXPROCESSING);
	BF2STR(D3DCREATE_MULTITHREADED);
	BF2STR(D3DCREATE_NOWINDOWCHANGES);
	BF2STR(D3DCREATE_PUREDEVICE);
	BF2STR(D3DCREATE_SCREENSAVER);

#undef BF2STR

	if (BehaviorFlagsString->back() == ' ')
		BehaviorFlagsString->pop_back();

	return false;
}

bool MainContext::ApplyPresentationParameters(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (pPresentationParameters)
	{
		context.backbufferWidth = pPresentationParameters->BackBufferWidth;
		if (config.GetTripleBuffering())
		{
			pPresentationParameters->BackBufferCount = 3;
			PrintLog("BackBufferCount: BackBufferCount set to %u", pPresentationParameters->BackBufferCount);
		}

		if ((s32)config.GetFullScreenRefreshRate() >= 0 && pPresentationParameters->FullScreen_RefreshRateInHz != 0)
		{
			PrintLog("Changing refresh rate from %u to %u", pPresentationParameters->FullScreen_RefreshRateInHz, config.GetFullScreenRefreshRate());
			pPresentationParameters->FullScreen_RefreshRateInHz = config.GetFullScreenRefreshRate();
		}

		if (config.GetMultisample() > 0)
		{
			pPresentationParameters->SwapEffect = D3DSWAPEFFECT_DISCARD;
			pPresentationParameters->MultiSampleType = (D3DMULTISAMPLE_TYPE)config.GetMultisample();
			pPresentationParameters->MultiSampleQuality = 0;

			PrintLog("MultiSampleType %u, MultiSampleQuality %u", pPresentationParameters->MultiSampleType, pPresentationParameters->MultiSampleQuality);
		}

		if (config.GetPresentationInterval() != -1)
		{
			pPresentationParameters->PresentationInterval = config.GetPresentationInterval();
			PrintLog("PresentationInterval: PresentationInterval set to %u", pPresentationParameters->PresentationInterval);
		}

		if ((s32)config.GetSwapEffect() != -1)
		{
			pPresentationParameters->SwapEffect = (D3DSWAPEFFECT)config.GetSwapEffect();
			PrintLog("SwapEffect: SwapEffect set to %u", pPresentationParameters->SwapEffect);
		}

		if (config.GetBorderless())
		{
			int cx = GetSystemMetrics(SM_CXSCREEN);
			int cy = GetSystemMetrics(SM_CYSCREEN);

			SetWindowPos(pPresentationParameters->hDeviceWindow, HWND_TOP, 0, 0, cx, cy, SWP_SHOWWINDOW | SWP_NOCOPYBITS | SWP_NOSENDCHANGING);

			if (config.GetForceWindowedMode())
			{
				pPresentationParameters->SwapEffect = pPresentationParameters->MultiSampleType == D3DMULTISAMPLE_NONE ? D3DSWAPEFFECT_DISCARD : D3DSWAPEFFECT_FLIP;
				pPresentationParameters->Windowed = TRUE;
				pPresentationParameters->FullScreen_RefreshRateInHz = 0;
				PrintLog("ForceWindowedMode");
			}
		}

		if (config.GetHideCursor()) while (::ShowCursor(FALSE) >= 0); // ShowCursor < 0 -> hidden

		return true;
	}
	return false;
}

bool MainContext::CheckWindow(HWND hWnd)
{
	std::unique_ptr<wchar_t[]> className(new wchar_t[MAX_PATH]);
	std::unique_ptr<wchar_t[]> windowName(new wchar_t[MAX_PATH]);

	GetClassNameW(hWnd, className.get(), MAX_PATH);
	GetWindowTextW(hWnd, windowName.get(), MAX_PATH);

	PrintLog("HWND 0x%p: ClassName \"%ls\", WindowName: \"%ls\"", hWnd, className.get(), windowName.get());

	bool class_found = config.GetWindowClass().compare(className.get()) == 0;
	bool window_found = config.GetWindowName().compare(windowName.get()) == 0;
	bool force = config.GetAllWindows();

	return class_found || window_found || force;
}

void MainContext::ApplyWndProc(HWND hWnd)
{
	if (config.GetAlwaysActive() || config.GetHideCursor())
	{
		context.oldWndProc = (WNDPROC)context.TrueSetWindowLongA(hWnd, GWLP_WNDPROC, (LONG_PTR)context.WindowProc);
	}
}

void MainContext::ApplyBorderless(HWND hWnd)
{
	if (config.GetBorderless())
	{
		LONG_PTR dwStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
		LONG_PTR dwExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);

		DWORD new_dwStyle = dwStyle & ~WS_OVERLAPPEDWINDOW;
		DWORD new_dwExStyle = dwExStyle & ~(WS_EX_OVERLAPPEDWINDOW);

		context.TrueSetWindowLongW(hWnd, GWL_STYLE, new_dwStyle);
		context.TrueSetWindowLongW(hWnd, GWL_EXSTYLE, new_dwExStyle);

		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);

		SetWindowPos(hWnd, HWND_TOP, 0, 0, cx, cy, SWP_SHOWWINDOW | SWP_NOCOPYBITS | SWP_NOSENDCHANGING);
		SetFocus(hWnd);

		PrintLog("HWND 0x%p: Borderless dwStyle: %lX->%lX", hWnd, dwStyle, new_dwStyle);
		PrintLog("HWND 0x%p: Borderless dwExStyle: %lX->%lX", hWnd, dwExStyle, new_dwExStyle);
		MessageBeep(MB_ICONASTERISK);
	}
}

LRESULT CALLBACK MainContext::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	case WM_ACTIVATE:
		switch (LOWORD(wParam))
		{
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			while (::ShowCursor(FALSE) >= 0);
			break;

		case WA_INACTIVE:
			if (context.config.GetAlwaysActive())
				return TRUE;

			if (!context.config.GetForceHideCursor())
				while (::ShowCursor(TRUE) < 0);
			break;
		}

	case WM_ACTIVATEAPP:
		if (context.config.GetAlwaysActive())
			return TRUE;

	}

	if (context.config.GetForceHideCursor())
		while (::ShowCursor(FALSE) >= 0);

	return CallWindowProc(context.oldWndProc, hWnd, uMsg, wParam, lParam);
}

LONG WINAPI MainContext::HookSetWindowLongA(HWND hWnd, int nIndex, LONG dwNewLong)
{
	if (context.config.GetBorderless())
	{
		DWORD olddwNewLong = dwNewLong;
		if (nIndex == GWL_STYLE)
		{
			dwNewLong &= ~WS_OVERLAPPEDWINDOW;
			PrintLog("SetWindowLongA dwStyle: %lX->%lX", olddwNewLong, dwNewLong);
		}

		if (nIndex == GWL_EXSTYLE)
		{
			dwNewLong &= ~(WS_EX_OVERLAPPEDWINDOW);
			PrintLog("SetWindowLongA dwExStyle: %lX->%lX", olddwNewLong, dwNewLong);
		}
	}
	return context.TrueSetWindowLongA(hWnd, nIndex, dwNewLong);
}

LONG WINAPI MainContext::HookSetWindowLongW(HWND hWnd, int nIndex, LONG dwNewLong)
{
	if (context.config.GetBorderless())
	{
		DWORD olddwNewLong = dwNewLong;
		if (nIndex == GWL_STYLE)
		{
			dwNewLong &= ~WS_OVERLAPPEDWINDOW;
			PrintLog("SetWindowLongW dwStyle: %lX->%lX", olddwNewLong, dwNewLong);
		}

		if (nIndex == GWL_EXSTYLE)
		{
			dwNewLong &= ~(WS_EX_OVERLAPPEDWINDOW);
			PrintLog("SetWindowLongW dwExStyle: %lX->%lX", olddwNewLong, dwNewLong);
		}
	}
	return context.TrueSetWindowLongW(hWnd, nIndex, dwNewLong);
}

HWND WINAPI MainContext::HookCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	HWND hWnd = context.TrueCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	if (!hWnd)
	{
		PrintLog("CreateWindowExA failed");
		return hWnd;
	}

	if (context.autofix == FINAL_FANTASY_XIII && !context.didOneTimeFixes) {
		PrintLog("Starting FFXIII one time RAM patches. (HookCreateWindowExA)");
		context.FFXIIIOneTimeFixes();
	}

	if (context.CheckWindow(hWnd))
	{
		context.ApplyWndProc(hWnd);
		context.ApplyBorderless(hWnd);
	}

	return hWnd;
}

HWND WINAPI MainContext::HookCreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	HWND hWnd = context.TrueCreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	if (!hWnd)
	{
		PrintLog("CreateWindowExW failed");
		return hWnd;
	}

	if (context.autofix == FINAL_FANTASY_XIII && !context.didOneTimeFixes) {
		PrintLog("Starting FFXIII one time RAM patches. (HookCreateWindowExW)");
		context.FFXIIIOneTimeFixes();
	}

	if (context.CheckWindow(hWnd))
	{
		context.ApplyWndProc(hWnd);
		context.ApplyBorderless(hWnd);
	}

	return hWnd;
}

void MainContext::FFXIIIOneTimeFixes() {

	bool successSoFar = true;
	// The game repeatedly sets the frame rate limit. Disable the instruction that does it.
	successSoFar &= MainContext::FFXIIINOPIngameFrameRateLimitSetter();

	successSoFar &= MainContext::FFXIIISetFrameRateVariables();

	if (successSoFar) {
		context.RemoveContinuousControllerScan();
		context.FixMissingEnemyScan();
		context.didOneTimeFixes = true;
	}
}

void MainContext::ChangeMemoryProtectionToReadWriteExecute(void* address, const int size) {
	DWORD oldProtection;
	VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtection);
}

void MainContext::RemoveContinuousControllerScan() {
	// Disable continuous controller scanning.

	PrintLog("Removing game slow and synchronous controller continuous controller scanning...");
	context.ChangeMemoryProtectionToReadWriteExecute(CONTINUOUS_SCAN_INSTRUCTION_ADDRESS, 1);
	// change a jne to jmp
	*(byte*)CONTINUOUS_SCAN_INSTRUCTION_ADDRESS = 0xEB;
}

void MainContext::FixMissingEnemyScan() {
	// This patches the variables that eventually will turn into a RECT to be used in a IDirect3DDevice9::SetScissorRect call. 
	// The game incorrectly uses the same values here regardless of the resolution.

	PrintLog("Patching libra info box instructions to take in account the game resolution...");

	const float resolutionFactor = (float)context.backbufferWidth / 1280.0F;
	
	const uint32_t rectHeight = (uint32_t)ceil(130.0F * resolutionFactor);
	const uint32_t rectWidth = context.backbufferWidth;
	const uint32_t rectPosY = (uint32_t)(496.0F * resolutionFactor);

	context.ChangeMemoryProtectionToReadWriteExecute(ENEMY_SCAN_BOX_CODE_ADDRESS, 18);

	//push boxHeight
	*(byte*)(ENEMY_SCAN_BOX_CODE_ADDRESS + 0) = 0x68;
	*(uint32_t*)(ENEMY_SCAN_BOX_CODE_ADDRESS + 1) = rectHeight;

	// push boxWidth
	*(byte*)(ENEMY_SCAN_BOX_CODE_ADDRESS + 5) = 0x68;
	*(uint32_t*)(ENEMY_SCAN_BOX_CODE_ADDRESS + 6) = rectWidth;

	// push boxPosY
	*(byte*)(ENEMY_SCAN_BOX_CODE_ADDRESS + 10) = 0x68;
	*(uint32_t*)(ENEMY_SCAN_BOX_CODE_ADDRESS + 11) = rectPosY;

	// NOP NOP NOP
	*(byte*)(ENEMY_SCAN_BOX_CODE_ADDRESS + 15) = 0x90;
	*(byte*)(ENEMY_SCAN_BOX_CODE_ADDRESS + 16) = 0x90;
	*(byte*)(ENEMY_SCAN_BOX_CODE_ADDRESS + 17) = 0x90;
}

bool MainContext::FFXIIINOPIngameFrameRateLimitSetter() {
	PrintLog("Using the ingame the instruction that sets the frame rate to get the frame rate address.");

	context.ChangeMemoryProtectionToReadWriteExecute(SET_FRAMERATE_INGAME_INSTRUCTION_ADDRESS, 5);

	// patching to: mov [framePacerTargetPtr], eax
	*SET_FRAMERATE_INGAME_INSTRUCTION_ADDRESS = 0xA3;
	*((uint32_t*)(SET_FRAMERATE_INGAME_INSTRUCTION_ADDRESS + 1)) = (uint32_t)(&framePacerTargetPtr);

	return true;
}

bool MainContext::FFXIIISetFrameRateVariables() {
	if (framePacerTargetPtr) {
		PrintLog("Frame pacer target frame rate is at address %x", framePacerTargetPtr);

		float* ingameFrameRateFramePacerTarget = framePacerTargetPtr;
		*ingameFrameRateFramePacerTarget = MAX_FRAME_RATE_LIMIT;
		PrintLog("Frame pacer disabled.");

		const float frameRateConfig = (float)context.config.GetFFXIIIIngameFrameRateLimit();
		bool unlimitedFrameRate = areAlmostTheSame(frameRateConfig, -1.0f);
		bool shouldSetFrameRateLimit = !areAlmostTheSame(frameRateConfig, 0.0f);

		float frameRateLimit = 0;

		if(unlimitedFrameRate){
			frameRateLimit = MAX_FRAME_RATE_LIMIT;
		}
		else {
			frameRateLimit = frameRateConfig;
		}

		if (shouldSetFrameRateLimit) {
			float* ingameFrameRateLimitPtr = framePacerTargetPtr + 1;
			*ingameFrameRateLimitPtr = frameRateLimit;
			PrintLog("Target frame rate set to %f", frameRateLimit);
		}
	}
	else {
		PrintLog("Unable to find frame rate pattern. This is normal if the game still hasn't completely started yet.");
	}

	return framePacerTargetPtr;
}

bool MainContext::areAlmostTheSame(float a, float b) {
	return fabs(a - b) < 0.01f;
}