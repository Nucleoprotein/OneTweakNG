#include "stdafx.h"

#include "Logger.h"
#include "MemPatch.h"

#include "Context.h"
//#include "IDirect3DVertexBuffer9.h"

void MainContext::EnableAutoFix()
{
	std::string exe_name = ModuleNameA(NULL);
	std::transform(exe_name.begin(), exe_name.end(), exe_name.begin(), std::tolower);

	if (exe_name == "ffxiiiimg.exe")
	{
		autofix = AutoFixes::FINAL_FANTASY_XIII;
		PrintLog("AutoFix for \"Final Fantasy XIII\" enabled");
		FF13_InitializeGameAddresses();
	}

	if (exe_name == "ffxiii2img.exe")
	{
		autofix = AutoFixes::FINAL_FANTASY_XIII2;
		PrintLog("AutoFix for \"Final Fantasy XIII-2\" enabled");
		FF13_2_InitializeGameAddresses();
		FF13_2_CreateSetFrameRateCodeBlock();
	}
}

const std::map<const MainContext::AutoFixes, const uint32_t> MainContext::behaviorflags_add =
{

};

const std::map<const MainContext::AutoFixes, const uint32_t> MainContext::behaviorflags_sub =
{
	//{ AutoFixes::FINAL_FANTASY_XIII, D3DCREATE_PUREDEVICE },
	//{ AutoFixes::FINAL_FANTASY_XIII2, D3DCREATE_PUREDEVICE }
};

void MainContext::FixBehaviorFlagConflict(const DWORD flags_in, DWORD* flags_out)
{
	if (flags_in & D3DCREATE_SOFTWARE_VERTEXPROCESSING)
	{
		*flags_out &= ~(D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING);
		*flags_out |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else if (flags_in & D3DCREATE_MIXED_VERTEXPROCESSING)
	{
		*flags_out &= ~(D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_SOFTWARE_VERTEXPROCESSING);
		*flags_out |= D3DCREATE_MIXED_VERTEXPROCESSING;
	}
	else if (flags_in & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		*flags_out &= ~(D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_SOFTWARE_VERTEXPROCESSING);
		*flags_out |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
}

void MainContext::ApplyBehaviorFlagsFix(DWORD* flags)
{
	if (autofix == AutoFixes::NONE) return;

	auto&& add = behaviorflags_add.find(autofix);
	if (add != behaviorflags_add.end())
		FixBehaviorFlagConflict(add->second, flags);

	auto&& sub = behaviorflags_sub.find(autofix);
	if (sub != behaviorflags_sub.end())
	{
		*flags = *flags & ~sub->second;
		FixBehaviorFlagConflict(sub->second, flags);
	}
}

HRESULT MainContext::SetScissorRect(IDirect3DDevice9* pIDirect3DDevice9, CONST RECT* rect)
{
	if (rect)
	{
		RECT* r = const_cast<RECT*>(rect);
		r->left = (LONG)(r->left * scissor_scaling_factor_w);
		r->top = (LONG)(r->top * scissor_scaling_factor_h);
		r->right = (LONG)(r->right * scissor_scaling_factor_w);
		r->bottom = (LONG)(r->bottom * scissor_scaling_factor_h);
		return pIDirect3DDevice9->SetScissorRect(r);
	}
	return pIDirect3DDevice9->SetScissorRect(rect);
}

// hate this workaround but we cant directly mix d3d9 include with and without defined CINTERFACE
namespace cinterface {
	void VertexBufferFix(IDirect3DVertexBuffer9* pVertexBuffer);
}

HRESULT MainContext::CreateVertexBuffer(IDirect3DDevice9* pIDirect3DDevice9, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
	if (autofix == AutoFixes::FINAL_FANTASY_XIII ||
		autofix == AutoFixes::FINAL_FANTASY_XIII2)
	{
		// Both games lock a vertex buffer 358400 before drawing any 2D element on screen (sometimes multiple times per frame)
		if (Length == 358400 && Pool == D3DPOOL_MANAGED) {
			Usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
			Pool = D3DPOOL_SYSTEMMEM;
			HRESULT hr = pIDirect3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);

			if (config.GetFFXIIIDiscardUIVertexBuffer() && ppVertexBuffer && *ppVertexBuffer)
			{
				//Pool = D3DPOOL_DEFAULT;
				cinterface::VertexBufferFix(*ppVertexBuffer);
			}
			return hr;
		}
	}

	return pIDirect3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}
HRESULT MainContext::DrawPrimitiveUP(IDirect3DDevice9* pIDirect3DDevice9, D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	if (PrimitiveType == D3DPT_TRIANGLEFAN && PrimitiveCount == 2 && VertexStreamZeroStride == 20 && MatchesExpectedVertexStream((const float*)pVertexStreamZeroData)) {
		return pIDirect3DDevice9->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, fixedDrawPrimitiveUpVertexData, VertexStreamZeroStride);
	}
	else {
		return pIDirect3DDevice9->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
	}
}

bool MainContext::MatchesExpectedVertexStream(const float* pVertexStreamZeroData) {
	for (int i = 0; i < 4 * 5; i++) {
		if (fabs(expectedDrawPrimitiveUpVertexData[i] - pVertexStreamZeroData[i]) > 0.000001f) {
			return false;
		}
	}
	return true;
}

bool MainContext::OneTimeFixInit(std::unique_ptr<wchar_t[]>& className, HWND hWnd)
{
	if (wcscmp(className.get(), L"SQEX.CDev.Engine.Framework.MainWindow") == 0) {
		otf_init = true;
		hWndFF13 = hWnd;
		return true;
	}
	return false;
}

void MainContext::OneTimeFix()
{
	if (otf_init)
	{
		otf_init = false;
		std::thread fix(&context.Fix_Thread);
		fix.detach();
	}
}

void MainContext::Fix_Thread()
{
	auto start = std::chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;
	PrintLog("Waited %f ms", elapsed.count());

	std::lock_guard<std::mutex> lock(context.fix_mutex);
	if (context.autofix == AutoFixes::FINAL_FANTASY_XIII) {
		PrintLog("Starting FFXIII one time RAM patches.");
		context.FF13_OneTimeFixes();
	}
	else if (context.autofix == AutoFixes::FINAL_FANTASY_XIII2) {
		PrintLog("Starting FFXIII-2 one time RAM patches.");
		context.FF13_2_OneTimeFixes();
	}
	MessageBeep(MB_ICONASTERISK);
}

void MainContext::FF13_InitializeGameAddresses()
{
	// FF13 always seem to use the same addresses (even if you force ASLR on the OS), but we are calculating the addresses just in case...
	uint8_t* baseAddr = (uint8_t*)GetModuleHandle(NULL); // Should be 400000
	PrintLog("Base Addr = %x", baseAddr);

	ff13_frame_pacer_ptr = (float**)(baseAddr + 0x243E34C);
	ff13_set_framerate_ingame_instruction_address = baseAddr + 0xA8D65F;
	ff13_continuous_scan_instruction_address = baseAddr + 0x420868;
	ff13_enemy_scan_box_code_address = baseAddr + 0x54C920;
	ff13_base_controller_input_address_ptr = (uint8_t**)(baseAddr + 0x02411220);
	ff13_vibration_low_set_zero_address = baseAddr + 0x4210DF;
	ff13_vibration_high_set_zero_address = baseAddr + 0x4210F3;
	ff13_internal_res_w = (uint32_t*)(baseAddr + 0x22E5168);
	ff13_internal_res_h = ff13_internal_res_w + 1;
	ff13_loading_screen_scissor_scaling_factor_1 = baseAddr + 0x616596;
	ff13_loading_screen_scissor_scaling_factor_2 = baseAddr + 0x6165BB;
	ff13_loading_screen_scissor_scaling_factor_3 = baseAddr + 0x61654C;
	ff13_loading_screen_scissor_scaling_factor_4 = baseAddr + 0x616571;
	ff13_settings_screen_scissor_scaling_factor = baseAddr + 0x572B26;
	ff13_party_screen_scissor_scaling_factor_1 = baseAddr + 0x668DE9;
	ff13_party_screen_scissor_scaling_factor_2 = baseAddr + 0x668E1E;
	ff13_party_screen_scissor_scaling_factor_3 = baseAddr + 0x668E56;
	ff13_party_screen_scissor_scaling_factor_4 = baseAddr + 0x668E91;
}

void MainContext::FF13_OneTimeFixes() {

	if (IsDXVK())
		SetForegroundWindow(hWndFF13);

	FF13_NOPIngameFrameRateLimitSetter();
	FF13_RemoveContinuousControllerScan();
	FF13_FixScissorRect();
	FF13_EnableControllerVibration();
	FF13_SetFrameRateVariables();
	AdjustVertexData(*ff13_internal_res_w, *ff13_internal_res_h);

	PrintLog("Finished FF13 One Time Fixes");
}

void MainContext::FF13_EnableControllerVibration()
{
	if (!config.GetFFXIIIEnableControllerVibration()) {
		PrintLog("Vibration should not be enabled (config file)");
		return;
	}
	if (!config.GetFFXIIIDisableIngameControllerHotSwapping()) {
		PrintLog("Vibration disabled because FFXIIIDisableIngameControllerHotSwapping is set to false (config file)");
		return;
	}
	PrintLog("Enabling controller vibration...");
	MemPatch::Nop(ff13_vibration_low_set_zero_address, 5);
	MemPatch::Nop(ff13_vibration_high_set_zero_address, 5);

	xinputManager = new XInputManager(ff13_base_controller_input_address_ptr, config.GetFFXIIIVibrationStrengthFactor());
}

void MainContext::FF13_RemoveContinuousControllerScan()
{
	if (!config.GetFFXIIIDisableIngameControllerHotSwapping()) {
		PrintLog("Continuous controller scanning not disabled (config)");
		return;
	}
	// Disable continuous controller scanning.

	PrintLog("Removing game slow and synchronous controller continuous controller scanning...");
	// change a jne to jmp
	MemPatch::Fill(ff13_continuous_scan_instruction_address, 0xEB, 1);
}

void MainContext::FF13_FixScissorRect()
{
	PrintLog("Fixing ScissorRect...");
	const float originalWidth = 1280.0F;
	const float resolutionFactorW = (float)*ff13_internal_res_w / originalWidth;
	scissor_scaling_factor_w = resolutionFactorW;

	const float originalHeight = 720.0F;
	const float resolutionFactorH = (float)*ff13_internal_res_h / originalHeight;
	scissor_scaling_factor_h = resolutionFactorH;

	// The game scales some scissor rects, but not all of them.
	// It seems easier to neuter its internal scaling process and scale everything on our own...

	MemPatch::Nop(ff13_loading_screen_scissor_scaling_factor_1, 3);
	MemPatch::Nop(ff13_loading_screen_scissor_scaling_factor_2, 3);
	MemPatch::Nop(ff13_loading_screen_scissor_scaling_factor_3, 3);
	MemPatch::Nop(ff13_loading_screen_scissor_scaling_factor_4, 3);
	MemPatch::Nop(ff13_settings_screen_scissor_scaling_factor, 5);
	MemPatch::Nop(ff13_party_screen_scissor_scaling_factor_1, 4);
	MemPatch::Nop(ff13_party_screen_scissor_scaling_factor_2, 7);
	MemPatch::Nop(ff13_party_screen_scissor_scaling_factor_3, 7);
	MemPatch::Nop(ff13_party_screen_scissor_scaling_factor_4, 7);
}

void MainContext::FF13_NOPIngameFrameRateLimitSetter()
{
	PrintLog("NOPing the in-game instruction that sets the frame rate.");
	MemPatch::Nop(ff13_set_framerate_ingame_instruction_address, 5);
}

void MainContext::FF13_SetFrameRateVariables()
{
	float* framePacerTargetPtr = *ff13_frame_pacer_ptr;
	if (framePacerTargetPtr) {
		PrintLog("Frame pacer target frame rate is at address %x", framePacerTargetPtr);

		float* ingameFrameRateFramePacerTarget = framePacerTargetPtr;
		*ingameFrameRateFramePacerTarget = MAX_FRAME_RATE_LIMIT;
		PrintLog("Frame pacer disabled.");

		if (config.GetFFXIIIIngameFrameRateLimit() != 0)
		{
			const s32 frameRateConfig = config.GetFFXIIIIngameFrameRateLimit();
			float frameRateLimit = 0;

			if (frameRateConfig == -1) {
				frameRateLimit = MAX_FRAME_RATE_LIMIT;
			}
			else if (frameRateConfig > 0) {
				frameRateLimit = (float)std::min(frameRateConfig, (s32)MAX_FRAME_RATE_LIMIT);
			}

			float* ingameFrameRateLimitPtr = framePacerTargetPtr + 1;
			*ingameFrameRateLimitPtr = frameRateLimit;
			PrintLog("Target frame rate set to %f", frameRateLimit);
		}
	}
	else {
		PrintLog("Unable to find frame pacer / frame rate address. This shouldn't happen! Report this.");
	}
}

void MainContext::FF13_2_OneTimeFixes()
{
	if (IsDXVK())
		SetForegroundWindow(hWndFF13);

	if (*ff13_2_frame_pacer_ptr_address) {
		**ff13_2_frame_pacer_ptr_address = MAX_FRAME_RATE_LIMIT;
		PrintLog("Frame pacer disabled");

		FF13_2_AddHookIngameFrameRateLimitSetter();
		FF13_2_RemoveContinuousControllerScan();
		FF13_2_EnableControllerVibration();
		AdjustVertexData(*ff13_2_internal_res_w, *ff13_2_internal_res_h);
		PrintLog("Finished FF13-2 One Time Fixes");
	}
	else {
		PrintLog("Unable to apply FF13-2 One Time Fixes. Report this!");
	}
}

void MainContext::FF13_2_EnableControllerVibration()
{
	if (!config.GetFFXIIIEnableControllerVibration()) {
		PrintLog("Vibration should not be enabled (config file)");
		return;
	}
	PrintLog("Enabling controller vibration...");

	MemPatch::Nop(ff13_2_vibration_low_set_zero_address, 5);
	MemPatch::Nop(ff13_2_vibration_high_set_zero_address, 5);

	xinputManager = new XInputManager(ff13_2_base_controller_input_address_ptr, config.GetFFXIIIVibrationStrengthFactor());
}

void MainContext::AdjustVertexData(const uint32_t width, const uint32_t height)
{
	const float widthHalfPixelSize = 1.0f / width;
	const float heightHalfPixelSize = 1.0f / height;
	for (int i = 0; i < 4; i++) {
		const unsigned int rowBaseIndex = i * 5;
		context.fixedDrawPrimitiveUpVertexData[rowBaseIndex] -= widthHalfPixelSize;
		context.fixedDrawPrimitiveUpVertexData[1 + rowBaseIndex] += heightHalfPixelSize;
	}
}

void MainContext::FF13_2_InitializeGameAddresses()
{
	// FF13-2 uses address space layout randomization (ASLR) so we can't rely on fixed addresses without considering the base address
	uint8_t* baseAddr = (uint8_t*)GetModuleHandle(NULL);
	PrintLog("Base Addr = %x", baseAddr);

	ff13_2_continuous_scan_instruction_address = baseAddr + 0x2A6E7F;
	ff13_2_set_frame_rate_address = baseAddr + 0x802616;
	ff13_2_frame_pacer_ptr_address = (float**)(baseAddr + 0x4D67208);
	ff13_2_base_controller_input_address_ptr = (uint8_t**)(baseAddr + 0x212A164);
	ff13_2_vibration_low_set_zero_address = baseAddr + 0x2A7221;
	ff13_2_vibration_high_set_zero_address = baseAddr + 0x2A7226;
	ff13_2_internal_res_w = (uint32_t*)(baseAddr + 0x1FA864C);
	ff13_2_internal_res_h = ff13_2_internal_res_w + 1;
}

void MainContext::FF13_2_RemoveContinuousControllerScan()
{
	if (!config.GetFFXIIIDisableIngameControllerHotSwapping()) {
		PrintLog("Continuous controller scanning not disabled (config)");
		return;
	}
	// Disable continuous controller scanning.

	PrintLog("Removing game slow and synchronous controller continuous controller scanning...");
	// change a jne to jmp
	MemPatch::Fill(ff13_2_continuous_scan_instruction_address, 0xEB, 1);
}

void MainContext::FF13_2_AddHookIngameFrameRateLimitSetter()
{
	if (config.GetFFXIIIIngameFrameRateLimit() == 0) {
		PrintLog("Frame rate should not be changed (config = 0)");
		return;
	}

	PrintLog("Hooking the instruction that sets the frame rate...");

	MemPatch::CUnprotect unp(ff13_2_set_frame_rate_address, 5);

	// patching to: jmp FF13_2_SET_FRAME_RATE_INJECTED_CODE
	*ff13_2_set_frame_rate_address = 0xE9;
	*((uint32_t*)(ff13_2_set_frame_rate_address + 1)) = FF13_2_SET_FRAME_RATE_INJECTED_CODE - ff13_2_set_frame_rate_address - 5;
}

void MainContext::FF13_2_CreateSetFrameRateCodeBlock()
{
	const int blockSize = 31;
	FF13_2_SET_FRAME_RATE_INJECTED_CODE = new(std::nothrow) uint8_t[blockSize];
	if (!FF13_2_SET_FRAME_RATE_INJECTED_CODE) {
		PrintLog("Failed to initialize FFXIII-2 code block");
		return;
	}
	DWORD oldProtect;
	VirtualProtect(FF13_2_SET_FRAME_RATE_INJECTED_CODE, blockSize, PAGE_EXECUTE_READWRITE, &oldProtect);

	if (config.GetFFXIIIIngameFrameRateLimit() != 0)
	{
		const s32 frameRateConfig = config.GetFFXIIIIngameFrameRateLimit();

		if (frameRateConfig == -1) {
			ff13_2_targetFrameRate = FF13_2_MAX_FRAME_CAP;
		}
		else if (frameRateConfig > 0) {
			ff13_2_targetFrameRate = (float)std::min(frameRateConfig, (s32)FF13_2_MAX_FRAME_CAP);
		}

		PrintLog("Target frame rate set to %f", ff13_2_targetFrameRate);
	}

	// movss xmm1,[&FF13_2_30_FPS]
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 0) = 0xF3;
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 1) = 0x0F;
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 2) = 0x10;
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 3) = 0x0D;
	*(float**)(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 4) = (float*)(&FF13_2_30_FPS);

	// ucomiss xmm0,xmm1
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 8) = 0x0F;
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 9) = 0x2E;
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 10) = 0xC1;

	//jna SetFrameRateVar
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 11) = 0x76;
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 12) = 0x08;

	// movss xmm0,[&FF13_2_30_FPS]
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 13) = 0xF3;
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 14) = 0x0F;
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 15) = 0x10;
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 16) = 0x05;
	*(float**)(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 17) = (float*)(&ff13_2_targetFrameRate);

	// SetFrameRateVar:
	// movss [ecx+04],xmm0
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 21) = 0xF3;
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 22) = 0x0F;
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 23) = 0x11;
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 24) = 0x41;
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 25) = 0x04;

	// jmp ffxiii2img.exe + 80261B
	*(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 26) = 0xE9;
	*(uint32_t*)(FF13_2_SET_FRAME_RATE_INJECTED_CODE + 27) = ff13_2_set_frame_rate_address - FF13_2_SET_FRAME_RATE_INJECTED_CODE - 26;
}

void MainContext::PrintVersionInfo() {
	PrintLog("FF13Fix 1.5.2 https://github.com/rebtd7/FF13Fix");
}
