// wrapper for IDirect3D9 in d3d9.h
// generated using wrapper_gen.rb

#include <d3d9.h>

#include "IDirect3DDevice9.h"

interface hkIDirect3D9 final : public IDirect3D9 {
public:
	// original interface
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj);
	ULONG STDMETHODCALLTYPE AddRef() {
		m_refPrivate++;
		m_refCount = m_pWrapped->AddRef();
		if (m_refCount > m_refPrivate) {
			// you should not see this ever
			PrintLog("WARNING: Internal reference counting to low, adjusting %u->%u", m_refPrivate.load(), m_refCount.load());
			m_refPrivate = m_refCount + 1;
		}

		return m_refCount + 1;
	}

	ULONG STDMETHODCALLTYPE Release() {
		if (m_refPrivate == 0ul)
			delete this;

		if (m_refPrivate != 0ul) {
			m_refPrivate--;

			m_refCount = m_pWrapped->Release();
		}
		return m_refCount + 1;
	}
	STDMETHOD(RegisterSoftwareDevice)(void* pInitializeFunction);
	STDMETHOD_(UINT, GetAdapterCount)();
	STDMETHOD(GetAdapterIdentifier)(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier);
	STDMETHOD_(UINT, GetAdapterModeCount)(UINT Adapter, D3DFORMAT Format);
	STDMETHOD(EnumAdapterModes)(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode);
	STDMETHOD(GetAdapterDisplayMode)(UINT Adapter, D3DDISPLAYMODE* pMode);
	STDMETHOD(CheckDeviceType)(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed);
	STDMETHOD(CheckDeviceFormat)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
	STDMETHOD(CheckDeviceMultiSampleType)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels);
	STDMETHOD(CheckDepthStencilMatch)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
	STDMETHOD(CheckDeviceFormatConversion)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat);
	STDMETHOD(GetDeviceCaps)(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps);
	STDMETHOD_(HMONITOR, GetAdapterMonitor)(UINT Adapter);
	STDMETHOD(CreateDevice)(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface);


	hkIDirect3D9(IDirect3D9* pIDirect3D9)
		: m_pWrapped(pIDirect3D9)
	{
	}

	virtual ~hkIDirect3D9(){while (m_pWrapped->Release());}


private:
	IDirect3D9* m_pWrapped;
    std::atomic<uint32_t> m_refCount = { 0ul };
    std::atomic<uint32_t> m_refPrivate = { 1ul };
};


