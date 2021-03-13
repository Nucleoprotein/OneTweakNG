// wrapper for IDirect3D9 in d3d9.h
// generated using wrapper_gen.rb

#include <d3d9.h>

#include "IDirect3DDevice9.h"

interface hkIDirect3D9 final : public IDirect3D9 {
public:
	// original interface
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj);
    ULONG STDMETHODCALLTYPE AddRef() {
        uint32_t refCount = m_refCount++;
        if (!refCount)
            AddRefPrivate();
        return refCount + 1;
    }

    ULONG STDMETHODCALLTYPE Release() {
        ULONG refCount = this->m_refCount;
        if (refCount != 0ul) {
            this->m_refCount--;
            refCount--;

            if (refCount == 0ul)
                this->ReleasePrivate();
        }

        return refCount;
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
		m_pWrapped->AddRef();
	}

	virtual ~hkIDirect3D9(){while (m_pWrapped->Release());}


private:
	IDirect3D9* m_pWrapped;
    std::atomic<uint32_t> m_refCount = { 0ul };
    std::atomic<uint32_t> m_refPrivate = { 1ul };

	void AddRefPrivate() {
		++m_refPrivate;
	}

	void ReleasePrivate() {
		uint32_t refPrivate = --m_refPrivate;
		if (!refPrivate) {
			m_refPrivate += 0x80000000;
			delete this;
		}
	}
};


