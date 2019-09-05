// wrapper for IDirect3DVertexBuffer9 in d3d9.h
// generated using wrapper_gen.rb

#include "stdafx.h"
#include "Context.h"

#include "IDirect3D9.h"
#include "IDirect3DDevice9.h"
#include "IDirect3DVertexBuffer9.h"

#define IDirect3DVertexBuffer9_PrintLog(format, ...) PrintLog(format, __VA_ARGS__);

HRESULT APIENTRY hkIDirect3DVertexBuffer9::QueryInterface(REFIID riid, void** ppvObj) {
	IDirect3DVertexBuffer9_PrintLog(__FUNCTION__);
	if (ppvObj == nullptr) return E_POINTER;

	if (riid == __uuidof(IUnknown) ||
		riid == __uuidof(IDirect3DResource9) ||
		riid == __uuidof(IDirect3DVertexBuffer9))
	{
		*ppvObj = static_cast<IDirect3DVertexBuffer9*>(this);
		AddRef();
		return S_OK;
	}

	*ppvObj = nullptr;
	return E_NOINTERFACE;
}

ULONG APIENTRY hkIDirect3DVertexBuffer9::AddRef() {
	IDirect3DVertexBuffer9_PrintLog(__FUNCTION__);
	return _InterlockedIncrement(&m_refCount);
}

ULONG APIENTRY hkIDirect3DVertexBuffer9::Release() {
	IDirect3DVertexBuffer9_PrintLog(__FUNCTION__);
	const LONG ref = _InterlockedDecrement(&m_refCount);
	if (ref == 0)
	{
		delete this;
	}
	return ref;
}

HRESULT APIENTRY hkIDirect3DVertexBuffer9::GetDevice(IDirect3DDevice9** ppDevice) {
	IDirect3DVertexBuffer9_PrintLog(__FUNCTION__);

	if (ppDevice == nullptr)
		return D3DERR_INVALIDCALL;
	
	m_pIDirect3DDevice9->AddRef();
	*ppDevice = m_pIDirect3DDevice9;
	return D3D_OK;
}

HRESULT APIENTRY hkIDirect3DVertexBuffer9::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) {
    IDirect3DVertexBuffer9_PrintLog(__FUNCTION__ " %s", GUIDtoStringA(refguid));
    return m_pWrapped->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

HRESULT APIENTRY hkIDirect3DVertexBuffer9::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData) {
	IDirect3DVertexBuffer9_PrintLog(__FUNCTION__ " %s",  GUIDtoStringA(refguid));
    return m_pWrapped->GetPrivateData(refguid, pData, pSizeOfData);
}

HRESULT APIENTRY hkIDirect3DVertexBuffer9::FreePrivateData(REFGUID refguid) {
    IDirect3DVertexBuffer9_PrintLog(__FUNCTION__);
    return m_pWrapped->FreePrivateData(refguid);
}

DWORD APIENTRY hkIDirect3DVertexBuffer9::SetPriority(DWORD PriorityNew) {
    IDirect3DVertexBuffer9_PrintLog(__FUNCTION__);
    return m_pWrapped->SetPriority(PriorityNew);
}

DWORD APIENTRY hkIDirect3DVertexBuffer9::GetPriority() {
    IDirect3DVertexBuffer9_PrintLog(__FUNCTION__);
    return m_pWrapped->GetPriority();
}

void APIENTRY hkIDirect3DVertexBuffer9::PreLoad() {
    IDirect3DVertexBuffer9_PrintLog(__FUNCTION__);
    return m_pWrapped->PreLoad();
}

D3DRESOURCETYPE APIENTRY hkIDirect3DVertexBuffer9::GetType() {
    IDirect3DVertexBuffer9_PrintLog(__FUNCTION__);
    return m_pWrapped->GetType();
}

HRESULT APIENTRY hkIDirect3DVertexBuffer9::Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags) {
    IDirect3DVertexBuffer9_PrintLog(__FUNCTION__);
    return m_pWrapped->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
}

HRESULT APIENTRY hkIDirect3DVertexBuffer9::Unlock() {
    IDirect3DVertexBuffer9_PrintLog(__FUNCTION__);
    return m_pWrapped->Unlock();
}

HRESULT APIENTRY hkIDirect3DVertexBuffer9::GetDesc(D3DVERTEXBUFFER_DESC *pDesc) {
    IDirect3DVertexBuffer9_PrintLog(__FUNCTION__);
    return m_pWrapped->GetDesc(pDesc);
}
