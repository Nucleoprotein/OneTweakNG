// wrapper for IDirect3DDevice9 in d3d9.h
// generated using wrapper_gen.rb

#pragma once
#include "stdafx.h"

#include "Logger.h"
#include "Context.h"

#include "IDirect3DDevice9.h"

#include <intrin.h>

#define IDirect3DDevice9_PrintLog(...) //PrintLog("%s", __VA_ARGS__);

HRESULT APIENTRY hkIDirect3DDevice9::QueryInterface(REFIID riid, void** ppvObj) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	if (ppvObj == nullptr) return E_POINTER;

	if (riid == __uuidof(IUnknown) ||
		riid == __uuidof(IDirect3DDevice9))
	{
		*ppvObj = static_cast<IDirect3DDevice9*>(this);
		return S_OK;
	}

	*ppvObj = nullptr;
	return E_NOINTERFACE;
}

HRESULT APIENTRY hkIDirect3DDevice9::TestCooperativeLevel() {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->TestCooperativeLevel();
}

UINT APIENTRY hkIDirect3DDevice9::GetAvailableTextureMem() {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetAvailableTextureMem();
}

HRESULT APIENTRY hkIDirect3DDevice9::EvictManagedResources() {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->EvictManagedResources();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDirect3D(IDirect3D9** ppD3D9) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetDirect3D(ppD3D9);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDeviceCaps(D3DCAPS9* pCaps) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetDeviceCaps(pCaps);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetDisplayMode(iSwapChain, pMode);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetCreationParameters(pParameters);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

void APIENTRY hkIDirect3DDevice9::SetCursorPosition(int X, int Y, DWORD Flags) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetCursorPosition(X, Y, Flags);
}

BOOL APIENTRY hkIDirect3DDevice9::ShowCursor(BOOL bShow) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->ShowCursor(bShow);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain) {
	PrintLog(__FUNCTION__);
	context.ApplyPresentationParameters(pPresentationParameters);
	return m_pWrapped->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);

	// Steam Overlay Fix
	// Add some space, 16bytes should be enough
	__nop();	__nop();	__nop();	__nop();	
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();
	__nop();	__nop();	__nop();	__nop();

	return m_pWrapped->GetSwapChain(iSwapChain, pSwapChain);
}

UINT APIENTRY hkIDirect3DDevice9::GetNumberOfSwapChains() {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetNumberOfSwapChains();
}

HRESULT APIENTRY hkIDirect3DDevice9::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters) {
	PrintLog("hkIDirect3DDevice9::Reset");

	context.ApplyPresentationParameters(pPresentationParameters);
	return m_pWrapped->Reset(pPresentationParameters);
}

HRESULT APIENTRY hkIDirect3DDevice9::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetRasterStatus(iSwapChain, pRasterStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetDialogBoxMode(bEnableDialogs);
}

void APIENTRY hkIDirect3DDevice9::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetGammaRamp(iSwapChain, Flags, pRamp);
}

void APIENTRY hkIDirect3DDevice9::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetGammaRamp(iSwapChain, pRamp);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle) {
	//PrintLog("hkIDirect3DDevice9::CreateVertexBuffer %08u %x %x %x %p %p", Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
	return context.CreateVertexBuffer(m_pWrapped, Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle) {
	//PrintLog("hkIDirect3DDevice9::CreateIndexBuffer %u %x %x %x %p %p", Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
	return m_pWrapped->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

HRESULT APIENTRY hkIDirect3DDevice9::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetRenderTargetData(pRenderTarget, pDestSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetFrontBufferData(iSwapChain, pDestSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

HRESULT APIENTRY hkIDirect3DDevice9::ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->ColorFill(pSurface, pRect, color);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetDepthStencilSurface(pNewZStencil);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::BeginScene() {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->BeginScene();
}

HRESULT APIENTRY hkIDirect3DDevice9::EndScene() {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->EndScene();
}

HRESULT APIENTRY hkIDirect3DDevice9::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->MultiplyTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetViewport(CONST D3DVIEWPORT9* pViewport) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetViewport(pViewport);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetViewport(D3DVIEWPORT9* pViewport) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetViewport(pViewport);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9* pMaterial) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetMaterial(pMaterial);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetMaterial(D3DMATERIAL9* pMaterial) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetMaterial(pMaterial);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetLight(DWORD Index, CONST D3DLIGHT9* pLight) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetLight(Index, pLight);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetLight(DWORD Index, D3DLIGHT9* pLight) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetLight(Index, pLight);
}

HRESULT APIENTRY hkIDirect3DDevice9::LightEnable(DWORD Index, BOOL Enable) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->LightEnable(Index, Enable);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetLightEnable(DWORD Index, BOOL* pEnable) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetLightEnable(Index, pEnable);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetClipPlane(DWORD Index, CONST float* pPlane) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetClipPlane(Index, pPlane);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetClipPlane(DWORD Index, float* pPlane) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetClipPlane(Index, pPlane);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetRenderState(State, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetRenderState(State, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->CreateStateBlock(Type, ppSB);
}

HRESULT APIENTRY hkIDirect3DDevice9::BeginStateBlock() {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->BeginStateBlock();
}

HRESULT APIENTRY hkIDirect3DDevice9::EndStateBlock(IDirect3DStateBlock9** ppSB) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->EndStateBlock(ppSB);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetClipStatus(pClipStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetClipStatus(D3DCLIPSTATUS9* pClipStatus) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetClipStatus(pClipStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetTexture(Stage, ppTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetTexture(Stage, pTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetTextureStageState(Stage, Type, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetTextureStageState(Stage, Type, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetSamplerState(Sampler, Type, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetSamplerState(Sampler, Type, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::ValidateDevice(DWORD* pNumPasses) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->ValidateDevice(pNumPasses);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetCurrentTexturePalette(PaletteNumber);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetCurrentTexturePalette(UINT *PaletteNumber) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetCurrentTexturePalette(PaletteNumber);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetScissorRect(CONST RECT* pRect) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return context.SetScissorRect(m_pWrapped, pRect);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetScissorRect(RECT* pRect) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetScissorRect(pRect);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetSoftwareVertexProcessing(bSoftware);
}

BOOL APIENTRY hkIDirect3DDevice9::GetSoftwareVertexProcessing() {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetSoftwareVertexProcessing();
}

HRESULT APIENTRY hkIDirect3DDevice9::SetNPatchMode(float nSegments) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetNPatchMode(nSegments);
}

float APIENTRY hkIDirect3DDevice9::GetNPatchMode() {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetNPatchMode();
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return context.DrawPrimitiveUP(m_pWrapped, PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->CreateVertexDeclaration(pVertexElements, ppDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetVertexDeclaration(pDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetVertexDeclaration(ppDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetFVF(DWORD FVF) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetFVF(FVF);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetFVF(DWORD* pFVF) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetFVF(pFVF);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->CreateVertexShader(pFunction, ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShader(IDirect3DVertexShader9* pShader) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetVertexShader(pShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShader(IDirect3DVertexShader9** ppShader) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetVertexShader(ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetStreamSourceFreq(UINT StreamNumber, UINT Setting) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetStreamSourceFreq(StreamNumber, Setting);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetStreamSourceFreq(UINT StreamNumber, UINT* pSetting) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetStreamSourceFreq(StreamNumber, pSetting);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetIndices(IDirect3DIndexBuffer9* pIndexData) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetIndices(pIndexData);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetIndices(IDirect3DIndexBuffer9** ppIndexData) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetIndices(ppIndexData);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->CreatePixelShader(pFunction, ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetPixelShader(pShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShader(IDirect3DPixelShader9** ppShader) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetPixelShader(ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT APIENTRY hkIDirect3DDevice9::DeletePatch(UINT Handle) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->DeletePatch(Handle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery) {
	IDirect3DDevice9_PrintLog(__FUNCTION__);
	return m_pWrapped->CreateQuery(Type, ppQuery);
}
