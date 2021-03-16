#include "stdafx.h"

#include <thread>
#include <array>


#include "Logger.h"

#include "MinHook.h"
#include "SimpleIni.h"

#include "IDirect3D9.h"
#include "Context.h"
#include "Wrapper.h"

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

#define SETTING(_type, _func, _var, _section, _defaultval, _comment) \
	ini.Set##_func(L#_section, L#_var, _defaultval, _comment)
#include "Settings.h"
#undef SETTING

		ini.SetLongValue(L"Version", L"Config", CONFIG_VERSION);
		ini.SaveFile(inifile.c_str());
		ini.Reset();
		ini.LoadFile(inifile.c_str());
	}

#define SETTING(_type, _func, _var, _section, _defaultval, _comment) \
	_var = ini.Get##_func(L#_section, L#_var)
#include "Settings.h"
#undef SETTING
}

MainContext::MainContext()
{
	LogFile("FF13Fix.log");
	context.PrintVersionInfo();

	if (config.GetOptionsAutoFix()) EnableAutoFix();

	PrintLog("Enabling hooks:");
	const MH_STATUS initializeHooks = MH_Initialize();
	PrintLog("initializeHooks = %d", initializeHooks);

	const MH_STATUS createHookDirect3DCreate9 = MH_CreateHook(D3D9DLL::Get().Direct3DCreate9, HookDirect3DCreate9, reinterpret_cast<void**>(&TrueDirect3DCreate9));
	PrintLog("createHookDirect3DCreate9 = %d", createHookDirect3DCreate9);
	const MH_STATUS enableHookDirect3DCreate9 = MH_EnableHook(D3D9DLL::Get().Direct3DCreate9);
	PrintLog("enableHookDirect3DCreate9 = %d", enableHookDirect3DCreate9);

	const MH_STATUS createHookCreateWindowExA = MH_CreateHook(CreateWindowExA, HookCreateWindowExA, reinterpret_cast<void**>(&TrueCreateWindowExA));
	PrintLog("createHookCreateWindowExA = %d", createHookCreateWindowExA);
	const MH_STATUS enableHookCreateWindowExA = MH_EnableHook(CreateWindowExA);
	PrintLog("enableHookCreateWindowExA = %d", enableHookCreateWindowExA);

	const MH_STATUS createHookCreateWindowExW = MH_CreateHook(CreateWindowExW, HookCreateWindowExW, reinterpret_cast<void**>(&TrueCreateWindowExW));
	PrintLog("createHookCreateWindowExW = %d", createHookCreateWindowExW);
	const MH_STATUS enableHookCreateWindowExW = MH_EnableHook(CreateWindowExW);
	PrintLog("enableHookCreateWindowExW = %d", enableHookCreateWindowExW);

	const MH_STATUS createHookSetWindowLongA = MH_CreateHook(SetWindowLongA, HookSetWindowLongA, reinterpret_cast<void**>(&TrueSetWindowLongA));
	PrintLog("createHookSetWindowLongA = %d", createHookSetWindowLongA);
	const MH_STATUS enableHookSetWindowLongA = MH_EnableHook(SetWindowLongA);
	PrintLog("enableHookSetWindowLongA = %d", enableHookSetWindowLongA);

	const MH_STATUS createHookSetWindowLongW = MH_CreateHook(SetWindowLongW, HookSetWindowLongW, reinterpret_cast<void**>(&TrueSetWindowLongW));
	PrintLog("createHookSetWindowLongW = %d", createHookSetWindowLongW);
	const MH_STATUS enableHookSetWindowLongW = MH_EnableHook(SetWindowLongW);
	PrintLog("enableHookSetWindowLongW = %d", enableHookSetWindowLongW);
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
		// -1 = Auto, enabled when no DXVK is used
		if (config.GetOptionsTripleBuffering() == 1 || (config.GetOptionsTripleBuffering() == -1 && !IsDXVK()))
		{
			pPresentationParameters->BackBufferCount = 3;
			PrintLog("BackBufferCount: BackBufferCount set to %u", pPresentationParameters->BackBufferCount);
		}

		if ((s32)config.GetOptionsFullScreenRefreshRate() >= 0 && pPresentationParameters->FullScreen_RefreshRateInHz != 0)
		{
			PrintLog("Changing refresh rate from %u to %u", pPresentationParameters->FullScreen_RefreshRateInHz, config.GetOptionsFullScreenRefreshRate());
			pPresentationParameters->FullScreen_RefreshRateInHz = config.GetOptionsFullScreenRefreshRate();
		}

		if (config.GetOptionsMultisample() > 0)
		{
			pPresentationParameters->SwapEffect = D3DSWAPEFFECT_DISCARD;
			pPresentationParameters->MultiSampleType = (D3DMULTISAMPLE_TYPE)config.GetOptionsMultisample();
			pPresentationParameters->MultiSampleQuality = 0;

			PrintLog("MultiSampleType %u, MultiSampleQuality %u", pPresentationParameters->MultiSampleType, pPresentationParameters->MultiSampleQuality);
		}

		if (config.GetOptionsPresentationInterval() != 0)
		{
			if (config.GetOptionsPresentationInterval() == -1)
				pPresentationParameters->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
			else if (config.GetOptionsPresentationInterval() > 0)
				pPresentationParameters->PresentationInterval = D3DPRESENT_INTERVAL_ONE;

			PrintLog("PresentationInterval: PresentationInterval set to 0x%x", pPresentationParameters->PresentationInterval);
		}

		if (config.GetOptionsSwapEffect() != -1)
		{
			pPresentationParameters->SwapEffect = (D3DSWAPEFFECT)config.GetOptionsSwapEffect();
			PrintLog("SwapEffect: SwapEffect set to %u", pPresentationParameters->SwapEffect);
		}

		if (config.GetBorderlessBorderless())
		{
			int cx = GetSystemMetrics(SM_CXSCREEN);
			int cy = GetSystemMetrics(SM_CYSCREEN);

			SetWindowPos(pPresentationParameters->hDeviceWindow, HWND_TOP, 0, 0, cx, cy, SWP_SHOWWINDOW | SWP_NOCOPYBITS | SWP_NOSENDCHANGING);

			if (config.GetBorderlessForceWindowedMode())
			{
				pPresentationParameters->SwapEffect = pPresentationParameters->MultiSampleType == D3DMULTISAMPLE_NONE ? D3DSWAPEFFECT_DISCARD : D3DSWAPEFFECT_FLIP;
				pPresentationParameters->Windowed = TRUE;
				pPresentationParameters->FullScreen_RefreshRateInHz = 0;
				
				PrintLog("ForceWindowedMode");
			}
		}

		if (config.GetOptionsHideCursor()) while (::ShowCursor(FALSE) >= 0); // ShowCursor < 0 -> hidden

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

	bool class_found = config.GetWindowWindowClass().compare(className.get()) == 0;
	bool window_found = config.GetWindowWindowName().compare(windowName.get()) == 0;
	bool force = config.GetBorderlessAllWindows();
	bool ff13fix = OneTimeFixInit(className, hWnd);

	return class_found || window_found || force || ff13fix;
}

void MainContext::ApplyWindow(HWND hWnd)
{
	if (config.GetOptionsAlwaysActive() || config.GetOptionsHideCursor())
	{
		context.oldWndProc = (WNDPROC)context.TrueSetWindowLongA(hWnd, GWLP_WNDPROC, (LONG_PTR)context.WindowProc);
	}
}

void MainContext::ApplyBorderless(HWND hWnd)
{
	if (config.GetBorderlessBorderless())
	{
		LONG_PTR dwStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
		LONG_PTR dwExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);

		DWORD new_dwStyle = dwStyle & ~WS_OVERLAPPEDWINDOW;
		DWORD new_dwExStyle = dwExStyle & ~(WS_EX_OVERLAPPEDWINDOW);

		context.TrueSetWindowLongW(hWnd, GWL_STYLE, new_dwStyle);
		context.TrueSetWindowLongW(hWnd, GWL_EXSTYLE, new_dwExStyle);

		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);

		HWND insertAfter = HWND_TOP;

		if (config.GetWindowTopMost())
			insertAfter = HWND_TOPMOST;

		SetWindowPos(hWnd, insertAfter, 0, 0, cx, cy, SWP_SHOWWINDOW | SWP_NOCOPYBITS | SWP_NOSENDCHANGING);
		SetFocus(hWnd);

		PrintLog("HWND 0x%p: Borderless dwStyle: %lX->%lX", hWnd, dwStyle, new_dwStyle);
		PrintLog("HWND 0x%p: Borderless dwExStyle: %lX->%lX", hWnd, dwExStyle, new_dwExStyle);
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
			if (context.config.GetOptionsAlwaysActive())
				return TRUE;

			if (!context.config.GetOptionsForceHideCursor())
				while (::ShowCursor(TRUE) < 0);
			break;
		}

	case WM_ACTIVATEAPP:
		if (context.config.GetOptionsAlwaysActive())
			return TRUE;
	}

	if (context.config.GetOptionsForceHideCursor())
		while (::ShowCursor(FALSE) >= 0);

	return CallWindowProc(context.oldWndProc, hWnd, uMsg, wParam, lParam);
}

LONG WINAPI MainContext::HookSetWindowLongA(HWND hWnd, int nIndex, LONG dwNewLong)
{
	if (context.config.GetBorderlessBorderless())
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
	if (context.config.GetBorderlessBorderless())
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

	if (context.CheckWindow(hWnd))
	{
		context.ApplyWindow(hWnd);
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

	if (context.CheckWindow(hWnd))
	{
		context.ApplyWindow(hWnd);
		context.ApplyBorderless(hWnd);
	}

	return hWnd;
}

bool MainContext::IsDXVK() { return D3D9DLL::Get().IsDXVK(); }