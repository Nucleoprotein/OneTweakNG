#include "stdafx.h"

#include "Logger.h"
#include "Context.h"

#include "IDirect3D9.h"
#include "IDirect3DDevice9.h"

#define IDirect3D9_PrintLog(format, ...) //PrintLog(format, __VA_ARGS__);

HRESULT APIENTRY hkIDirect3D9::QueryInterface(REFIID riid, void** ppvObj) {
	PrintLog(__FUNCTION__);
	if (ppvObj == nullptr)
		return E_POINTER;

	if (riid == __uuidof(this) ||
		riid == __uuidof(IUnknown) ||
		riid == __uuidof(IDirect3D9) ||
		riid == __uuidof(IDirect3D9Ex))
	{
		if (!m_is_ex && riid == __uuidof(IDirect3D9Ex))
		{
			//we are queried for IDirect3D9Ex but we hold IDirect3D9
			//upgrade wrapped interface, query it
			IDirect3D9Ex* pIDirect3D9Ex = nullptr;
			HRESULT hr = m_pWrapped->QueryInterface(riid, reinterpret_cast<void**>(&pIDirect3D9Ex));
			if (FAILED(hr))
				return hr;

			// release one reference from old one and take new IDirect3DDevice9Ex pointer 
			m_pWrapped->Release();
			m_pWrapped = pIDirect3D9Ex;
			m_is_ex = true;
		}

		AddRef();
		*ppvObj = this;
		return S_OK;
	}

	return m_pWrapped->QueryInterface(riid, ppvObj);
}

ULONG STDMETHODCALLTYPE hkIDirect3D9::AddRef() {
	m_pWrapped->AddRef();
	return InterlockedIncrement(&m_ref);
}

ULONG STDMETHODCALLTYPE hkIDirect3D9::Release() {
	const ULONG ref = InterlockedDecrement(&m_ref);
	if (ref != 0) {
		m_pWrapped->Release();
		return ref;
	}
	const auto pWrapped = m_pWrapped;
	m_pWrapped = nullptr;
	delete this;

	const ULONG ref_last = pWrapped->Release();
	if (ref_last != 0)
		PrintLog("WARNING: Reference count for IDirect3D9 is wrong: %p %u %u", this, ref, ref_last);

	return 0;
}

HRESULT APIENTRY hkIDirect3D9::RegisterSoftwareDevice(void* pInitializeFunction) {
	IDirect3D9_PrintLog(__FUNCTION__);
	return m_pWrapped->RegisterSoftwareDevice(pInitializeFunction);
}

UINT APIENTRY hkIDirect3D9::GetAdapterCount() {
	IDirect3D9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetAdapterCount();
}

HRESULT APIENTRY hkIDirect3D9::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier) {
	PrintLog(__FUNCTION__);
	HRESULT rt = m_pWrapped->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
	if (context.config.GetAdapterAdapter() && SUCCEEDED(rt) && pIdentifier)
	{
		pIdentifier->VendorId = context.config.GetAdapterVendorId();
		pIdentifier->DeviceId = context.config.GetAdapterDeviceId();
	}
	return rt;
}

UINT APIENTRY hkIDirect3D9::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format) {
	IDirect3D9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetAdapterModeCount(Adapter, Format);
}

HRESULT APIENTRY hkIDirect3D9::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode) {
	IDirect3D9_PrintLog(__FUNCTION__);
	return m_pWrapped->EnumAdapterModes(Adapter, Format, Mode, pMode);
}

HRESULT APIENTRY hkIDirect3D9::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode) {
	IDirect3D9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetAdapterDisplayMode(Adapter, pMode);
}

HRESULT APIENTRY hkIDirect3D9::CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed) {
	IDirect3D9_PrintLog(__FUNCTION__);
	if (context.config.GetBorderlessForceWindowedMode()) bWindowed = TRUE;
	return m_pWrapped->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
}

HRESULT APIENTRY hkIDirect3D9::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) {
	IDirect3D9_PrintLog(__FUNCTION__);
	return m_pWrapped->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT APIENTRY hkIDirect3D9::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels) {
	IDirect3D9_PrintLog(__FUNCTION__);
	return m_pWrapped->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
}

HRESULT APIENTRY hkIDirect3D9::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) {
	IDirect3D9_PrintLog(__FUNCTION__);
	return m_pWrapped->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT APIENTRY hkIDirect3D9::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat) {
	IDirect3D9_PrintLog(__FUNCTION__);
	return m_pWrapped->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
}

HRESULT APIENTRY hkIDirect3D9::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps) {
	IDirect3D9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

HMONITOR APIENTRY hkIDirect3D9::GetAdapterMonitor(UINT Adapter) {
	IDirect3D9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetAdapterMonitor(Adapter);
}

HRESULT APIENTRY hkIDirect3D9::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface) {
	PrintLog(__FUNCTION__);
	return ApplyCreateDeviceFix<IDirect3DDevice9, false>(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, nullptr, ppReturnedDeviceInterface);
}

UINT STDMETHODCALLTYPE hkIDirect3D9::GetAdapterModeCountEx(UINT Adapter, CONST D3DDISPLAYMODEFILTER* pFilter) {
	IDirect3D9_PrintLog(__FUNCTION__);

	return static_cast<IDirect3D9Ex*>(m_pWrapped)->GetAdapterModeCountEx(Adapter, pFilter);
}

HRESULT STDMETHODCALLTYPE hkIDirect3D9::EnumAdapterModesEx(UINT Adapter, CONST D3DDISPLAYMODEFILTER* pFilter, UINT Mode, D3DDISPLAYMODEEX* pMode) {
	IDirect3D9_PrintLog(__FUNCTION__);
	return static_cast<IDirect3D9Ex*>(m_pWrapped)->EnumAdapterModesEx(Adapter, pFilter, Mode, pMode);
}

HRESULT STDMETHODCALLTYPE hkIDirect3D9::GetAdapterDisplayModeEx(UINT Adapter, D3DDISPLAYMODEEX* pMode, D3DDISPLAYROTATION* pRotation) {
	IDirect3D9_PrintLog(__FUNCTION__);
	return static_cast<IDirect3D9Ex*>(m_pWrapped)->GetAdapterDisplayModeEx(Adapter, pMode, pRotation);
}

HRESULT STDMETHODCALLTYPE hkIDirect3D9::CreateDeviceEx(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface) {
	PrintLog(__FUNCTION__);
	return ApplyCreateDeviceFix<IDirect3DDevice9Ex, true>(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, pFullscreenDisplayMode, ppReturnedDeviceInterface);
}

HRESULT STDMETHODCALLTYPE hkIDirect3D9::GetAdapterLUID(UINT Adapter, LUID* pLUID) {
	IDirect3D9_PrintLog(__FUNCTION__);
	return static_cast<IDirect3D9Ex*>(m_pWrapped)->GetAdapterLUID(Adapter, pLUID);
}

//TODO move this to other file
template <typename T, bool ex>
HRESULT hkIDirect3D9::ApplyCreateDeviceFix(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, T** ppReturnedDeviceInterface)
{
	PrintLog(__FUNCTION__);

	DWORD OriginalBehaviorFlags = BehaviorFlags;
	std::string BehaviorFlagsString;
	context.BehaviorFlagsToString(BehaviorFlags, &BehaviorFlagsString);

	PrintLog("BehaviorFlags: %08X %s", BehaviorFlags, BehaviorFlagsString.c_str());

	if (context.config.GetOptionsBehaviorFlags() > 0) {
		BehaviorFlags = context.config.GetOptionsBehaviorFlags();
		PrintLog("Advanced Mode: BehaviorFlags set");
	}
	else {
		context.ApplyBehaviorFlagsFix(&BehaviorFlags);
	}

	if (hFocusWindow == NULL)
		hFocusWindow = pPresentationParameters->hDeviceWindow;

	context.ApplyPresentationParameters(pPresentationParameters);

	if (OriginalBehaviorFlags != BehaviorFlags) {
		std::string BehaviorFlagsString;
		context.BehaviorFlagsToString(BehaviorFlags, &BehaviorFlagsString);
		PrintLog("BehaviorFlags changed: %08X %s", BehaviorFlags, BehaviorFlagsString.c_str());
	}

	T* device = nullptr;
	HRESULT hr = S_FALSE;
	bool is_ex;

	if (ex) {
		hr = m_pWrapped->CreateDeviceEx(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, pFullscreenDisplayMode, reinterpret_cast<IDirect3DDevice9Ex**>(&device));
		is_ex = true;
	}
	else {
		hr = m_pWrapped->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, reinterpret_cast<IDirect3DDevice9**>(&device));
		is_ex = false;
	}

	if (FAILED(hr)) {
		PrintLog("CreateDevice fail with HRESULT: %08X", hr);
		*ppReturnedDeviceInterface = nullptr;
		return hr;
	}
	else {
		*ppReturnedDeviceInterface = new hkIDirect3DDevice9(static_cast<IDirect3DDevice9Ex*>(device), is_ex);
		context.OneTimeFix();
	}

	return hr;
}