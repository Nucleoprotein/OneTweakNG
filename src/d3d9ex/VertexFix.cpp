#define CINTERFACE
#include <d3d9.h>

#include "spdlog/spdlog.h"
#include "MinHook.h"

namespace cinterface
{
	HRESULT(STDMETHODCALLTYPE* TrueLock)(IDirect3DVertexBuffer9* This, UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags) = nullptr;

	HRESULT STDMETHODCALLTYPE HookLock(IDirect3DVertexBuffer9* This, UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)
	{
		spdlog::trace(__FUNCTION__" {}", SizeToLock);
		if (SizeToLock == 358400)
			Flags = D3DLOCK_DISCARD;
		return TrueLock(This, OffsetToLock, SizeToLock, ppbData, Flags);
	}

	void VertexBufferFix(IDirect3DVertexBuffer9* pVertexBuffer)
	{
		if (pVertexBuffer->lpVtbl->Lock && TrueLock == nullptr)
		{
			spdlog::warn("Experimental DiscardUIVertexBuffer enabled!");
			const MH_STATUS createHookLock = MH_CreateHook(pVertexBuffer->lpVtbl->Lock, HookLock, reinterpret_cast<void**>(&TrueLock));
			spdlog::info("CreateHookLock = {}", createHookLock);
			const MH_STATUS enableHookLock = MH_EnableHook(pVertexBuffer->lpVtbl->Lock);
			spdlog::info("EnableHookLock = {}", enableHookLock);
		}
	}
}


