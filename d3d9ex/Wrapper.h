#pragma once

#include "comdef.h"

#include "d3d9.h"
#include "dinput.h"

template <class T>
class WrapperBase : NonCopyable
{
public:
	WrapperBase() {}

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
