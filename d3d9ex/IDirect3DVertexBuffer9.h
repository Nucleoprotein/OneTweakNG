// wrapper for IDirect3DVertexBuffer9 in d3d9.h
// generated using wrapper_gen.rb

#pragma once
#include "d3d9.h"

interface hkIDirect3DVertexBuffer9 final : public IDirect3DVertexBuffer9 {
public:   
    // original interface
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();
    STDMETHOD(GetDevice)(IDirect3DDevice9** ppDevice);
    STDMETHOD(SetPrivateData)(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
    STDMETHOD(GetPrivateData)(REFGUID refguid, void* pData, DWORD* pSizeOfData);
    STDMETHOD(FreePrivateData)(REFGUID refguid);
    STDMETHOD_(DWORD, SetPriority)(DWORD PriorityNew);
    STDMETHOD_(DWORD, GetPriority)();
    STDMETHOD_(void, PreLoad)();
    STDMETHOD_(D3DRESOURCETYPE, GetType)();
    STDMETHOD(Lock)(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags);
    STDMETHOD(Unlock)();
    STDMETHOD(GetDesc)(D3DVERTEXBUFFER_DESC *pDesc);

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

