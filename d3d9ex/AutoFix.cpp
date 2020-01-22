#include "stdafx.h"

#include "Context.h"
#include "IDirect3DVertexBuffer9.h"

void MainContext::EnableAutoFix()
{
	std::string exe_name = ModuleNameA(NULL);
	std::transform(exe_name.begin(), exe_name.end(), exe_name.begin(), std::tolower);

	if (exe_name == "game.exe" || exe_name == "game.dat")
	{
		autofix = RESIDENT_EVIL_4;
		PrintLog("AutoFix for \"Resident Evil 4\" enabled");
	}

	if (exe_name == "kb.exe")
	{
		autofix = KINGS_BOUNTY_LEGEND;
		PrintLog("AutoFix for \"Kings Bounty: Legend\" enabled");
	}

	if (exe_name == "ffxiiiimg.exe")
	{
		autofix = FINAL_FANTASY_XIII;
		PrintLog("AutoFix for \"Final Fantasy XIII\" enabled");
		FF13_InitializeGameAddresses();
	}

	if (exe_name == "ffxiii2img.exe")
	{
		autofix = FINAL_FANTASY_XIII2;
		PrintLog("AutoFix for \"Final Fantasy XIII-2\" enabled");
		FF13_2_InitializeGameAddresses();
		FF13_2_CreateSetFrameRateCodeBlock();
	}
}

const std::map<const MainContext::AutoFixes, const uint32_t> MainContext::behaviorflags_fixes =
{
	{ RESIDENT_EVIL_4, D3DCREATE_SOFTWARE_VERTEXPROCESSING },
	{ KINGS_BOUNTY_LEGEND, D3DCREATE_MIXED_VERTEXPROCESSING }
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

bool MainContext::ApplyBehaviorFlagsFix(DWORD* flags)
{
	if (autofix == AutoFixes::NONE) return false;

	auto&& fix = behaviorflags_fixes.find(autofix);
	if (fix != behaviorflags_fixes.end())
	{
		FixBehaviorFlagConflict(fix->second, flags);
		return true;
	}

	return false;
}

HRESULT APIENTRY MainContext::ApplyVertexBufferFix(IDirect3DDevice9* pIDirect3DDevice9, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
	switch (autofix) {
	case AutoFixes::NONE:
		return pIDirect3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
	case FINAL_FANTASY_XIII:
	case FINAL_FANTASY_XIII2:
		// Both games lock a vertex buffer 358400 before drawing any 2D element on screen (sometimes multiple times per frame)
		if (Length == 358400 && Pool == D3DPOOL_MANAGED) {
			Usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
			Pool = D3DPOOL_SYSTEMMEM;

			//IDirect3DVertexBuffer9* buffer = nullptr;
			//HRESULT hr = pIDirect3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, &buffer, NULL);
			//if (FAILED(hr))
			//{
			//	return pIDirect3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
			//}

			//if(ppVertexBuffer) *ppVertexBuffer = new hkIDirect3DVertexBuffer9(pIDirect3DDevice9, buffer);
			//return hr;
		}
		break;
	}

	return pIDirect3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

void MainContext::FF13_InitializeGameAddresses()
{
	// FF13 always seem to use the same addresses (even if you force ASLR on the OS), but we are calculating the addresses just in case...
	byte* baseAddr = (byte*)GetModuleHandle(NULL); // Should be 400000
	PrintLog("Base Addr = %x", baseAddr);

	ff13_frame_pacer_ptr = (float**)(baseAddr + 0x243E34C);
	ff13_set_framerate_ingame_instruction_address = baseAddr + 0xA8D65F;
	ff13_continuous_scan_instruction_address = baseAddr + 0x420868;
	ff13_enemy_scan_box_code_address = baseAddr + 0x54C920;
	ff13_base_controller_input_address_ptr = (byte**)(baseAddr + 0x02411220);
	ff13_vibration_low_set_zero_address = baseAddr + 0x4210DF;
	ff13_vibration_high_set_zero_address = baseAddr + 0x4210F3;
}

void MainContext::FF13_OneTimeFixes() {
	MainContext::FF13_NOPIngameFrameRateLimitSetter();
	MainContext::FF13_SetFrameRateVariables();
	MainContext::FF13_RemoveContinuousControllerScan();
	MainContext::FF13_FixMissingEnemyScan();
	MainContext::FF13_EnableControllerVibration();
	
	PrintLog("Finished FF13 One Time Fixes");
	MainContext::didOneTimeFixes = true;
}

void MainContext::FF13_EnableControllerVibration()
{
	if (!config.GetFFXIIIEnableControllerVibration()) {
		PrintLog("Vibration should not be enabled (config file)");
		return;
	}
	PrintLog("Enabling controller vibration...");
	ChangeMemoryProtectionToReadWriteExecute(ff13_vibration_low_set_zero_address, 5);

	*ff13_vibration_low_set_zero_address = 0x90;
	*(ff13_vibration_low_set_zero_address + 1) = 0x90;
	*(ff13_vibration_low_set_zero_address + 2) = 0x90;
	*(ff13_vibration_low_set_zero_address + 3) = 0x90;
	*(ff13_vibration_low_set_zero_address + 4) = 0x90;

	ChangeMemoryProtectionToReadWriteExecute(ff13_vibration_high_set_zero_address, 5);
	*ff13_vibration_high_set_zero_address = 0x90;
	*(ff13_vibration_high_set_zero_address + 1) = 0x90;
	*(ff13_vibration_high_set_zero_address + 2) = 0x90;
	*(ff13_vibration_high_set_zero_address + 3) = 0x90;
	*(ff13_vibration_high_set_zero_address + 4) = 0x90;
	xinputManager = new XInputManager(ff13_base_controller_input_address_ptr);
}

void MainContext::FF13_RemoveContinuousControllerScan() {
	// Disable continuous controller scanning.

	PrintLog("Removing game slow and synchronous controller continuous controller scanning...");
	context.ChangeMemoryProtectionToReadWriteExecute(ff13_continuous_scan_instruction_address, 1);
	// change a jne to jmp
	*(byte*)ff13_continuous_scan_instruction_address = 0xEB;
}

void MainContext::FF13_FixMissingEnemyScan() {
	// This patches the variables that eventually will turn into a RECT to be used in a IDirect3DDevice9::SetScissorRect call. 
	// The game incorrectly uses the same values here regardless of the resolution.

	PrintLog("Patching libra info box instructions to take in account the game resolution...");

	const float resolutionFactor = (float)context.backbufferWidth / 1280.0F;

	const uint32_t rectHeight = (uint32_t)ceil(130.0F * resolutionFactor);
	const uint32_t rectWidth = context.backbufferWidth;
	const uint32_t rectPosY = (uint32_t)(496.0F * resolutionFactor);

	context.ChangeMemoryProtectionToReadWriteExecute(ff13_enemy_scan_box_code_address, 18);

	//push rectHeight
	*(byte*)(ff13_enemy_scan_box_code_address + 0) = 0x68;
	*(uint32_t*)(ff13_enemy_scan_box_code_address + 1) = rectHeight;

	// push rectWidth
	*(byte*)(ff13_enemy_scan_box_code_address + 5) = 0x68;
	*(uint32_t*)(ff13_enemy_scan_box_code_address + 6) = rectWidth;

	// push rectPosY
	*(byte*)(ff13_enemy_scan_box_code_address + 10) = 0x68;
	*(uint32_t*)(ff13_enemy_scan_box_code_address + 11) = rectPosY;

	// NOP NOP NOP
	*(byte*)(ff13_enemy_scan_box_code_address + 15) = 0x90;
	*(byte*)(ff13_enemy_scan_box_code_address + 16) = 0x90;
	*(byte*)(ff13_enemy_scan_box_code_address + 17) = 0x90;
}

void MainContext::FF13_NOPIngameFrameRateLimitSetter() {
	PrintLog("NOPing the in-game instruction that sets the frame rate.");

	context.ChangeMemoryProtectionToReadWriteExecute(ff13_set_framerate_ingame_instruction_address, 5);

	// patching to: NOP NOP NOP NOP
	*ff13_set_framerate_ingame_instruction_address = 0x90;
	*(ff13_set_framerate_ingame_instruction_address + 1) = 0x90;
	*(ff13_set_framerate_ingame_instruction_address + 2) = 0x90;
	*(ff13_set_framerate_ingame_instruction_address + 3) = 0x90;
	*(ff13_set_framerate_ingame_instruction_address + 4) = 0x90;
}

void MainContext::FF13_SetFrameRateVariables() {
	float* framePacerTargetPtr = *ff13_frame_pacer_ptr;
	if (framePacerTargetPtr) {
		PrintLog("Frame pacer target frame rate is at address %x", framePacerTargetPtr);

		float* ingameFrameRateFramePacerTarget = framePacerTargetPtr;
		*ingameFrameRateFramePacerTarget = MAX_FRAME_RATE_LIMIT;
		PrintLog("Frame pacer disabled.");

		const float frameRateConfig = (float)context.config.GetFFXIIIIngameFrameRateLimit();
		const bool unlimitedFrameRate = AreAlmostTheSame(frameRateConfig, -1.0f);
		const bool shouldSetFrameRateLimit = !AreAlmostTheSame(frameRateConfig, 0.0f);

		float frameRateLimit = 0;

		if (unlimitedFrameRate) {
			frameRateLimit = MAX_FRAME_RATE_LIMIT;
		}
		else {
			frameRateLimit = frameRateConfig;
		}

		if (shouldSetFrameRateLimit) {
			float* ingameFrameRateLimitPtr = framePacerTargetPtr + 1;
			*ingameFrameRateLimitPtr = frameRateLimit;
			PrintLog("Target frame rate set to %f", frameRateLimit);
		}
	}
	else {
		PrintLog("Unable to find frame pacer / frame rate address. This shouldn't happen! Report this.");
	}
}

void MainContext::FF13_2_OneTimeFixes() {

	if (*ff13_2_frame_pacer_ptr_address) {
		**ff13_2_frame_pacer_ptr_address = MAX_FRAME_RATE_LIMIT;
		PrintLog("Frame pacer disabled");

		context.FF13_2_AddHookIngameFrameRateLimitSetter();
		context.FF13_2_RemoveContinuousControllerScan();
		context.FF13_2_EnableControllerVibration();
		PrintLog("Finished FF13-2 One Time Fixes");
		context.didOneTimeFixes = true;
	}
}

void MainContext::FF13_2_EnableControllerVibration()
{
	if (!config.GetFFXIIIEnableControllerVibration()) {
		PrintLog("Vibration should not be enabled (config file)");
		return;
	}
	PrintLog("Enabling controller vibration...");
	ChangeMemoryProtectionToReadWriteExecute(ff13_2_vibration_low_set_zero_address, 5);

	*ff13_2_vibration_low_set_zero_address = 0x90;
	*(ff13_2_vibration_low_set_zero_address + 1) = 0x90;
	*(ff13_2_vibration_low_set_zero_address + 2) = 0x90;
	*(ff13_2_vibration_low_set_zero_address + 3) = 0x90;
	*(ff13_2_vibration_low_set_zero_address + 4) = 0x90;

	ChangeMemoryProtectionToReadWriteExecute(ff13_2_vibration_high_set_zero_address, 5);
	*ff13_2_vibration_high_set_zero_address = 0x90;
	*(ff13_2_vibration_high_set_zero_address + 1) = 0x90;
	*(ff13_2_vibration_high_set_zero_address + 2) = 0x90;
	*(ff13_2_vibration_high_set_zero_address + 3) = 0x90;
	*(ff13_2_vibration_high_set_zero_address + 4) = 0x90;

	xinputManager = new XInputManager(ff13_2_base_controller_input_address_ptr);
}

void MainContext::FF13_2_InitializeGameAddresses()
{
	// FF13-2 uses address space layout randomization (ASLR) so we can't rely on fixed addresses without considering the base address
	byte* baseAddr = (byte*)GetModuleHandle(NULL);
	PrintLog("Base Addr = %x", baseAddr);

	ff13_2_continuous_scan_instruction_address = baseAddr + 0x2A6E7F;
	ff13_2_set_frame_rate_address = baseAddr + 0x802616;
	ff13_2_frame_pacer_ptr_address = (float**)(baseAddr + 0x4D67208);
	ff13_2_base_controller_input_address_ptr = (byte**)(baseAddr + 0x212A164);
	ff13_2_vibration_low_set_zero_address = baseAddr + 0x2A7221;
	ff13_2_vibration_high_set_zero_address = baseAddr + 0x2A7226;
}

void MainContext::FF13_2_RemoveContinuousControllerScan() {
	// Disable continuous controller scanning.

	PrintLog("Removing game slow and synchronous controller continuous controller scanning...");
	context.ChangeMemoryProtectionToReadWriteExecute(ff13_2_continuous_scan_instruction_address, 1);
	// change a jne to jmp
	*(byte*)ff13_2_continuous_scan_instruction_address = 0xEB;
}

void MainContext::FF13_2_AddHookIngameFrameRateLimitSetter() {
	if (context.AreAlmostTheSame((float)context.config.GetFFXIIIIngameFrameRateLimit(), 0.0F)) {
		PrintLog("Frame rate should not be changed (config = 0)");
		return;
	}

	PrintLog("Hooking the instruction that sets the frame rate...");

	context.ChangeMemoryProtectionToReadWriteExecute(ff13_2_set_frame_rate_address, 5);

	// patching to: jmp FF13_2_SET_FRAME_RATE_INJECTED_CODE
	*ff13_2_set_frame_rate_address = 0xE9;
	*((uint32_t*)(ff13_2_set_frame_rate_address + 1)) = FF13_2_SET_FRAME_RATE_INJECTED_CODE - ff13_2_set_frame_rate_address - 5;
}

void MainContext::FF13_2_CreateSetFrameRateCodeBlock()
{
	const int blockSize = 31;
	FF13_2_SET_FRAME_RATE_INJECTED_CODE = new byte[blockSize];

	ChangeMemoryProtectionToReadWriteExecute(FF13_2_SET_FRAME_RATE_INJECTED_CODE, blockSize);

	float frameRateConfigValue = (float)context.config.GetFFXIIIIngameFrameRateLimit();
	if (AreAlmostTheSame(frameRateConfigValue, -1.0F) || frameRateConfigValue > FF13_2_MAX_FRAME_CAP) {
		ff13_2_targetFrameRate = FF13_2_MAX_FRAME_CAP;
	}
	else {
		ff13_2_targetFrameRate = frameRateConfigValue;
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


void MainContext::ChangeMemoryProtectionToReadWriteExecute(void* address, const int size) {
	DWORD oldProtection;
	VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtection);
}

void MainContext::PrintVersionInfo() {
	PrintLog("FF13Fix 1.4.0 https://github.com/rebtd7/FF13Fix");
}

bool MainContext::AreAlmostTheSame(float a, float b) {
	return fabs(a - b) < 0.01f;
}