// wrapper for IDirect3D9 in d3d9.h
// generated using wrapper_gen.rb

#include "stdafx.h"
#include "Context.h"

#include "IDirect3D9.h"
#include "IDirect3DDevice9.h"

#define IDirect3D9_PrintLog(format, ...) //PrintLog(format, __VA_ARGS__);

hkIDirect3D9::hkIDirect3D9(IDirect3D9 *pIDirect3D9) {
	PrintLog(__FUNCTION__);
	m_pWrapped = pIDirect3D9;
}

hkIDirect3D9::~hkIDirect3D9()
{
	PrintLog(__FUNCTION__);
}

HRESULT APIENTRY hkIDirect3D9::QueryInterface(REFIID riid, void** ppvObj) {
	PrintLog(__FUNCTION__);
	return m_pWrapped->QueryInterface(riid, ppvObj);
}

ULONG APIENTRY hkIDirect3D9::AddRef() {
	PrintLog(__FUNCTION__);
	return m_pWrapped->AddRef();
}

ULONG APIENTRY hkIDirect3D9::Release() {
	ULONG ref_count = m_pWrapped->Release();
	PrintLog(__FUNCTION__ " return = %lu", ref_count);
	if (ref_count == 0) delete this;
	return ref_count;
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
	if (context.config.GetAdapter() && SUCCEEDED(rt) && pIdentifier)
	{
		pIdentifier->VendorId = context.config.GetVendorId();
		pIdentifier->DeviceId = context.config.GetDeviceId();
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
	if (context.config.GetForceWindowedMode()) bWindowed = TRUE;
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

	DWORD OriginalBehaviorFlags = BehaviorFlags;
	std::string BehaviorFlagsString;
	context.BehaviorFlagsToString(BehaviorFlags, &BehaviorFlagsString);

	PrintLog("BehaviorFlags: %08X %s", BehaviorFlags, BehaviorFlagsString.c_str());

	if (context.config.GetBehaviorFlags() > 0)
	{
		BehaviorFlags = context.config.GetBehaviorFlags();
		PrintLog("Advanced Mode: BehaviorFlags set");
	}
	else
	{
		context.ApplyBehaviorFlagsFix(&BehaviorFlags);
	}

	if (hFocusWindow == NULL) hFocusWindow = pPresentationParameters->hDeviceWindow;
	context.ApplyPresentationParameters(pPresentationParameters);

	HRESULT hr = m_pWrapped->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
	if (SUCCEEDED(hr) && ppReturnedDeviceInterface && *ppReturnedDeviceInterface)
	{
		hkIDirect3DDevice9* pIDirect3DDevice9 = new hkIDirect3DDevice9(*ppReturnedDeviceInterface);
		*ppReturnedDeviceInterface = pIDirect3DDevice9;
	}

	if (OriginalBehaviorFlags != BehaviorFlags)
	{
		std::string BehaviorFlagsString;
		context.BehaviorFlagsToString(BehaviorFlags, &BehaviorFlagsString);
		PrintLog("BehaviorFlags changed: %08X %s", BehaviorFlags, BehaviorFlagsString.c_str());
	}
	return hr;
}
