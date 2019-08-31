#pragma once

#include "comdef.h"

#include "d3d9.h"
#include "dinput.h"
#include "xinput.h"

template <class T>
class WrapperBase 
{
public:
	WrapperBase(const WrapperBase&) = delete;
	const WrapperBase& operator=(WrapperBase& other) = delete;

	WrapperBase(): m_module(nullptr) {}

	~WrapperBase()
	{
		if (m_module)
		{
			FreeLibrary(m_module);
			PrintLog("Unloaded %s", module_path.c_str());
		}
	}

	static T& Get()
	{
		static T instance;
		return instance;
	}

protected:
	void WrapperLoad(const char* module_name)
	{
		module_path = CreateSystemModulePath(module_name);
		m_module = LoadLibraryA(module_path.c_str());

		if (!m_module)
		{
			HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
			_com_error err(hr);

			std::string msg = StringFromFormat("Cannot load %s\nHRESULT 0x%08X: \"%s\"", module_path.c_str(), err.Error(), err.ErrorMessage());

			PrintLog(msg.c_str());
			MessageBoxA(NULL, msg.c_str(), "Error", MB_ICONERROR);
			ExitProcess(hr);
		}

		PrintLog("Loaded %s", module_path.c_str());
	}

	template<typename T>
	void StoreAddress(T* dest, const char* name)
	{
		*dest = reinterpret_cast<T>(::GetProcAddress(m_module, name));
	}

private:
	HMODULE m_module;
	std::string module_path;
};

class D3D9DLL : public WrapperBase<D3D9DLL>
{
public:
	IDirect3D9* (WINAPI* Direct3DCreate9)(UINT SDKVersion);
	HRESULT(WINAPI* Direct3DCreate9Ex)(UINT SDKVersion, IDirect3D9Ex **ppD3D);

	int (WINAPI* D3DPERF_BeginEvent)(D3DCOLOR col, LPCWSTR wszName);
	int (WINAPI* D3DPERF_EndEvent)(void);

	DWORD(WINAPI* D3DPERF_GetStatus)();
	BOOL(WINAPI* D3DPERF_QueryRepeatFrame)();
	void (WINAPI* D3DPERF_SetMarker)(D3DCOLOR col, LPCWSTR wszName);
	void (WINAPI* D3DPERF_SetOptions)(DWORD dwOptions);
	void (WINAPI* D3DPERF_SetRegion)(D3DCOLOR col, LPCWSTR wszName);

	D3D9DLL()
	{
		WrapperLoad("d3d9.dll");

		StoreAddress(&Direct3DCreate9, "Direct3DCreate9");
		StoreAddress(&Direct3DCreate9Ex, "Direct3DCreate9Ex");

		StoreAddress(&D3DPERF_BeginEvent, "D3DPERF_BeginEvent");
		StoreAddress(&D3DPERF_EndEvent, "D3DPERF_EndEvent");

		StoreAddress(&D3DPERF_GetStatus, "D3DPERF_GetStatus");
		StoreAddress(&D3DPERF_QueryRepeatFrame, "D3DPERF_QueryRepeatFrame");
		StoreAddress(&D3DPERF_SetMarker, "D3DPERF_SetMarker");
		StoreAddress(&D3DPERF_SetOptions, "D3DPERF_SetOptions");
		StoreAddress(&D3DPERF_SetRegion, "D3DPERF_SetRegion");
	}
};

extern "C"
{

	IDirect3D9 * WINAPI _Direct3DCreate9(UINT SDKVersion)
	{
		return D3D9DLL::Get().Direct3DCreate9(SDKVersion);
	}

	HRESULT WINAPI _Direct3DCreate9Ex(UINT SDKVersion, IDirect3D9Ex **ppD3D)
	{
		return D3D9DLL::Get().Direct3DCreate9Ex(SDKVersion, ppD3D);
	}

	int WINAPI _D3DPERF_BeginEvent(D3DCOLOR col, LPCWSTR wszName)
	{
		return D3D9DLL::Get().D3DPERF_BeginEvent(col, wszName);
	}

	int WINAPI _D3DPERF_EndEvent()
	{
		return D3D9DLL::Get().D3DPERF_EndEvent();
	}

	DWORD WINAPI _D3DPERF_GetStatus()
	{
		return D3D9DLL::Get().D3DPERF_GetStatus();
	}

	BOOL WINAPI _D3DPERF_QueryRepeatFrame()
	{
		return D3D9DLL::Get().D3DPERF_QueryRepeatFrame();
	}

	void WINAPI _D3DPERF_SetMarker(D3DCOLOR col, LPCWSTR wszName)
	{
		return D3D9DLL::Get().D3DPERF_SetMarker(col, wszName);
	}

	void WINAPI _D3DPERF_SetOptions(DWORD dwOptions)
	{
		return D3D9DLL::Get().D3DPERF_SetOptions(dwOptions);
	}

	void WINAPI _D3DPERF_SetRegion(D3DCOLOR col, LPCWSTR wszName)
	{
		return D3D9DLL::Get().D3DPERF_SetRegion(col, wszName);
	}
}

class DINPUT8DLL : public WrapperBase<DINPUT8DLL>
{
public:

	HRESULT (WINAPI* DirectInput8Create)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter);

	DINPUT8DLL()
	{
		WrapperLoad("dinput8.dll");

		StoreAddress(&DirectInput8Create, "DirectInput8Create");
	}
};

extern "C"
{
	HRESULT WINAPI _DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter)
	{
		return DINPUT8DLL::Get().DirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
	}
}

class XINPUTDLL : public WrapperBase<XINPUTDLL>
{
public:

	// XInput 1.3 and older functions
	DWORD(WINAPI* XInputGetState)(DWORD dwUserIndex, XINPUT_STATE* pState);
	DWORD(WINAPI* XInputSetState)(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);
	DWORD(WINAPI* XInputGetCapabilities)(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities);
	VOID(WINAPI* XInputEnable)(BOOL enable);
	DWORD(WINAPI* XInputGetDSoundAudioDeviceGuids)(DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid);
	DWORD(WINAPI* XInputGetBatteryInformation)(DWORD  dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation);
	DWORD(WINAPI* XInputGetKeystroke)(DWORD dwUserIndex, DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke);

	// XInput 1.3 undocumented functions
	DWORD(WINAPI* XInputGetStateEx)(DWORD dwUserIndex, XINPUT_STATE *pState); // 100
	DWORD(WINAPI* XInputWaitForGuideButton)(DWORD dwUserIndex, DWORD dwFlag, LPVOID pVoid); // 101
	DWORD(WINAPI* XInputCancelGuideButtonWait)(DWORD dwUserIndex); // 102
	DWORD(WINAPI* XInputPowerOffController)(DWORD dwUserIndex); // 103

	// XInput 1.4 functions
	DWORD(WINAPI* XInputGetAudioDeviceIds)(DWORD dwUserIndex, LPWSTR pRenderDeviceId, UINT* pRenderCount, LPWSTR pCaptureDeviceId, UINT* pCaptureCount);

	// XInput 1.4 undocumented functionss
	DWORD(WINAPI* XInputGetBaseBusInformation)(DWORD dwUserIndex, struct XINPUT_BUSINFO* pBusinfo); // 104
	DWORD(WINAPI* XInputGetCapabilitiesEx)(DWORD unk1, DWORD dwUserIndex, DWORD dwFlags, struct XINPUT_CAPABILITIESEX* pCapabilitiesEx); // 108

	XINPUTDLL()
	{
		WrapperLoad("xinput1_3.dll");

		// XInput 1.3 and older functions
		StoreAddress(&XInputGetState, "XInputGetState");
		StoreAddress(&XInputSetState, "XInputSetState");
		StoreAddress(&XInputGetCapabilities, "XInputGetCapabilities");
		StoreAddress(&XInputEnable, "XInputEnable");
		StoreAddress(&XInputGetDSoundAudioDeviceGuids, "XInputGetDSoundAudioDeviceGuids");
		StoreAddress(&XInputGetBatteryInformation, "XInputGetBatteryInformation");
		StoreAddress(&XInputGetKeystroke, "XInputGetKeystroke");

		// XInput 1.3 undocumented functions
		StoreAddress(&XInputGetStateEx, (const char*)100);
		StoreAddress(&XInputWaitForGuideButton, (const char*)101);
		StoreAddress(&XInputCancelGuideButtonWait, (const char*)102);
		StoreAddress(&XInputPowerOffController, (const char*)103);

		// XInput 1.4 functions
		StoreAddress(&XInputGetAudioDeviceIds, "XInputGetAudioDeviceIds");

		// XInput 1.4 undocumented functionss
		StoreAddress(&XInputGetBaseBusInformation, (const char*)104);
		StoreAddress(&XInputGetCapabilitiesEx, (const char*)108);
	}
};

extern "C"
{
	// XInput 1.3 and older functions
	DWORD WINAPI _XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState)
	{
		return XINPUTDLL::Get().XInputGetState(dwUserIndex, pState);
	}

	DWORD WINAPI _XInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
	{
		return XINPUTDLL::Get().XInputSetState(dwUserIndex, pVibration);
	}

	DWORD WINAPI _XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities)
	{
		return XINPUTDLL::Get().XInputGetCapabilities(dwUserIndex, dwFlags, pCapabilities);
	}

	VOID WINAPI _XInputEnable(BOOL enable)
	{
		return XINPUTDLL::Get().XInputEnable(enable);
	}

	DWORD WINAPI _XInputGetDSoundAudioDeviceGuids(DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid)
	{
		return XINPUTDLL::Get().XInputGetDSoundAudioDeviceGuids(dwUserIndex, pDSoundRenderGuid, pDSoundCaptureGuid);
	}

	DWORD WINAPI _XInputGetBatteryInformation(DWORD  dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation)
	{
		return XINPUTDLL::Get().XInputGetBatteryInformation(dwUserIndex, devType, pBatteryInformation);
	}

	DWORD WINAPI _XInputGetKeystroke(DWORD dwUserIndex, DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke)
	{
		return XINPUTDLL::Get().XInputGetKeystroke(dwUserIndex, dwReserved, pKeystroke);
	}
}
