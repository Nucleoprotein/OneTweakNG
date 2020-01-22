#pragma once

#include <array>
#include "d3d9.h"
#include <MinHook.h>
#include "SimpleIni.h"
#include "XInputManager.h"

struct hkIDirect3D9;

static const char* inifilename = "FF13Fix.ini";
#define CONFIG_VERSION 4

class Config
{
public:
	Config(const Config&) = delete;
	const Config& operator=(Config& other) = delete;

	Config();

#define SETTING(_type, _func, _var, _section, _defaultval) \
	private: _type _var; \
	public: const _type& Get##_var() const { return _var; };
#include "Settings.h"
#undef SETTING
};

#define DECLARE_HOOK(type, callconv, name, ...) \
	public: type(callconv* True##name)(__VA_ARGS__) = name; \
	private: static type callconv Hook##name(__VA_ARGS__);

class MainContext
{
	MainContext(const MainContext&) = delete;
	const MainContext& operator=(MainContext& other) = delete;

	DECLARE_HOOK(IDirect3D9*, WINAPI, Direct3DCreate9, UINT SDKVersion);
	DECLARE_HOOK(LONG, WINAPI, SetWindowLongA, HWND hWnd, int nIndex, LONG dwNewLong);
	DECLARE_HOOK(LONG, WINAPI, SetWindowLongW, HWND hWnd, int nIndex, LONG dwNewLong);
	DECLARE_HOOK(HWND, WINAPI, CreateWindowExA, DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
		DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	DECLARE_HOOK(HWND, WINAPI, CreateWindowExW, DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName,
		DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

public:
	MainContext();
	void Foo();
	~MainContext();

	bool ApplyPresentationParameters(D3DPRESENT_PARAMETERS* pPresentationParameters);
	bool ApplyBehaviorFlagsFix(DWORD* flags);
	HRESULT APIENTRY ApplyVertexBufferFix(IDirect3DDevice9* pIDirect3DDevice9, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle);
	bool BehaviorFlagsToString(DWORD BehaviorFlags, std::string* BehaviorFlagsString);

	bool CheckWindow(HWND hWnd);

	void ApplyWndProc(HWND hWnd);
	void ApplyBorderless(HWND hWnd);

	Config config;

private:
	enum AutoFixes : u32
	{
		NONE = 0,
		RESIDENT_EVIL_4,
		KINGS_BOUNTY_LEGEND,
		FINAL_FANTASY_XIII,
		FINAL_FANTASY_XIII2,
	};

	void EnableAutoFix();

	AutoFixes autofix = AutoFixes::NONE;

	std::mutex oneTimeFixesMutex;
	bool didOneTimeFixes = false;

	const float MAX_FRAME_RATE_LIMIT = 250000.0F;
	float** ff13_frame_pacer_ptr = NULL;
	byte* ff13_set_framerate_ingame_instruction_address = NULL;
	byte* ff13_continuous_scan_instruction_address = NULL;
	byte* ff13_enemy_scan_box_code_address = NULL;
	byte** ff13_base_controller_input_address_ptr = NULL;
	byte* ff13_vibration_high_set_zero_address = NULL;
	byte* ff13_vibration_low_set_zero_address = NULL;

	byte* FF13_2_SET_FRAME_RATE_INJECTED_CODE = NULL;
	byte* ff13_2_continuous_scan_instruction_address;
	byte* ff13_2_set_frame_rate_address;
	float** ff13_2_frame_pacer_ptr_address;
	float ff13_2_targetFrameRate;
	byte** ff13_2_base_controller_input_address_ptr = NULL;
	byte* ff13_2_vibration_high_set_zero_address = NULL;
	byte* ff13_2_vibration_low_set_zero_address = NULL;

	const float FF13_2_30_FPS = 30.0F;
	const float FF13_2_MAX_FRAME_CAP = 1000.0F;
	XInputManager* xinputManager;
	
	UINT backbufferWidth = 0;
	
	void FixBehaviorFlagConflict(const DWORD flags_in, DWORD* flags_out);
	static const std::map<const AutoFixes, const uint32_t> behaviorflags_fixes;

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	WNDPROC oldWndProc;

	void ChangeMemoryProtectionToReadWriteExecute(void* address, const int size);
	bool AreAlmostTheSame(float a, float b);
	void PrintVersionInfo();

	void FF13_InitializeGameAddresses();
	void FF13_OneTimeFixes();
	void FF13_EnableControllerVibration();
	void FF13_NOPIngameFrameRateLimitSetter();
	void FF13_SetFrameRateVariables();
	void FF13_FixMissingEnemyScan();
	void FF13_RemoveContinuousControllerScan();

	void FF13_2_CreateSetFrameRateCodeBlock();
	void FF13_2_InitializeGameAddresses();
	void FF13_2_RemoveContinuousControllerScan();
	void FF13_2_AddHookIngameFrameRateLimitSetter();
	void FF13_2_OneTimeFixes();
	void FF13_2_EnableControllerVibration();
};

extern MainContext context;