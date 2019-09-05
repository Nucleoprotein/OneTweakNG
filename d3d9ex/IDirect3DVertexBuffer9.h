// wrapper for IDirect3DVertexBuffer9 in d3d9.h
// generated using wrapper_gen.rb

#pragma once
#include "d3d9.h"

interface hkIDirect3DVertexBuffer9 final : public IDirect3DVertexBuffer9 {
public:   
	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	/*** IDirect3DResource9 methods ***/
	STDMETHOD(GetDevice)(THIS_ IDirect3DDevice9** ppDevice);
	STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
	STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData);
	STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid);
	STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD PriorityNew);
	STDMETHOD_(DWORD, GetPriority)(THIS);
	STDMETHOD_(void, PreLoad)(THIS);
	STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS);
	STDMETHOD(Lock)(THIS_ UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags);
	STDMETHOD(Unlock)(THIS);
	STDMETHOD(GetDesc)(THIS_ D3DVERTEXBUFFER_DESC* pDesc);

public:
	hkIDirect3DVertexBuffer9(IDirect3DDevice9* pIDirect3DDevice9, IDirect3DVertexBuffer9* pIDirect3DVertexBuffer9)
		: m_pWrapped(pIDirect3DVertexBuffer9), m_pIDirect3DDevice9(pIDirect3DDevice9)
	{}

private:
	~hkIDirect3DVertexBuffer9()
	{
		m_pWrapped->Release();
	}

	LONG m_refCount = 1;
	IDirect3DVertexBuffer9 *m_pWrapped;
	IDirect3DDevice9* m_pIDirect3DDevice9;
};

