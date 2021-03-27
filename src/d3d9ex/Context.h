#pragma once

#include <atomic>
#include <d3d9.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "WinUtil.h"

#include "Types.h"
#include "XInputManager.h"
#include "scmversion.h"

static const char* inifilename = "FF13Fix.ini";
#define CONFIG_VERSION 7

class Config
{
public:
	Config(const Config&) = delete;
	const Config& operator=(Config& other) = delete;

	Config();

#define SETTING(_type, _func, _var, _section, _defaultval, _comment) \
	private: _type _var; \
	public: const _type& Get##_section##_var() const { return _var; };
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
	DECLARE_HOOK(HRESULT, WINAPI, Direct3DCreate9Ex, UINT SDKVersion, IDirect3D9Ex** ppIDirect3D9Ex);
	DECLARE_HOOK(LONG, WINAPI, SetWindowLongA, HWND hWnd, int nIndex, LONG dwNewLong);
	DECLARE_HOOK(LONG, WINAPI, SetWindowLongW, HWND hWnd, int nIndex, LONG dwNewLong);
	DECLARE_HOOK(HWND, WINAPI, CreateWindowExA, DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
	DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	DECLARE_HOOK(HWND, WINAPI, CreateWindowExW, DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName,
	DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

	DECLARE_HOOK(HANDLE, WINAPI, CreateFileA, LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
	DECLARE_HOOK(HANDLE, WINAPI, CreateFileW, LPCWSTR lpFileName,DWORD dwDesiredAccess,DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes,DWORD dwCreationDisposition,DWORD dwFlagsAndAttributes,HANDLE hTemplateFile);
public:
	MainContext();
	virtual ~MainContext();

	bool ApplyPresentationParameters(D3DPRESENT_PARAMETERS* pPresentationParameters);
	void ApplyBehaviorFlagsFix(DWORD* flags);
	HRESULT SetScissorRect(IDirect3DDevice9* pIDirect3DDevice9, CONST RECT* rect);
	HRESULT CreateVertexBuffer(IDirect3DDevice9* pIDirect3DDevice9, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle);
	HRESULT DrawPrimitiveUP(IDirect3DDevice9* pIDirect3DDevice9, D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
	bool BehaviorFlagsToString(DWORD BehaviorFlags, std::string* BehaviorFlagsString);

	bool CheckWindow(HWND hWnd);

	void ApplyWindow(HWND hWnd);
	void ApplyBorderless(HWND hWnd);

	Config config;

	void OneTimeFix();
	bool IsDXVK();

private:
	enum class AutoFixes : u32
	{
		NONE = 0,
		FINAL_FANTASY_XIII,
		FINAL_FANTASY_XIII2,
	};

	void EnableAutoFix();

	AutoFixes autofix = AutoFixes::NONE;
	HWND hWndFF13 = 0;

	std::mutex fix_mutex;

	//MainContext is global!
	//TODO this class is cluttered, separate fixes class from MainContext

	const float MAX_FRAME_RATE_LIMIT = 250000.0F;
	float** ff13_frame_pacer_ptr = NULL;
	uint8_t* ff13_set_framerate_ingame_instruction_address = NULL;
	uint8_t* ff13_continuous_scan_instruction_address = NULL;
	uint8_t* ff13_enemy_scan_box_code_address = NULL;
	uint8_t** ff13_base_controller_input_address_ptr = NULL;
	uint8_t* ff13_vibration_high_set_zero_address = NULL;
	uint8_t* ff13_vibration_low_set_zero_address = NULL;
	uint8_t* ff13_loading_screen_scissor_scaling_factor_1 = NULL;
	uint8_t* ff13_loading_screen_scissor_scaling_factor_2 = NULL;
	uint8_t* ff13_loading_screen_scissor_scaling_factor_3 = NULL;
	uint8_t* ff13_loading_screen_scissor_scaling_factor_4 = NULL;
	uint8_t* ff13_settings_screen_scissor_scaling_factor = NULL;
	uint8_t* ff13_party_screen_scissor_scaling_factor_1 = NULL;
	uint8_t* ff13_party_screen_scissor_scaling_factor_2 = NULL;
	uint8_t* ff13_party_screen_scissor_scaling_factor_3 = NULL;
	uint8_t* ff13_party_screen_scissor_scaling_factor_4 = NULL;
	uint8_t* ff13_message_box_stack_push_address = NULL;
	uint8_t* ff13_message_box_call_address = NULL;
	uint8_t* ff13_exe_large_address_aware_flag_address = NULL;
	uint32_t* ff13_exe_checksum_address = NULL;
	uint32_t* ff13_internal_res_w;
	uint32_t* ff13_internal_res_h;

	uint8_t* FF13_2_SET_FRAME_RATE_INJECTED_CODE = NULL;
	uint8_t* ff13_2_continuous_scan_instruction_address;
	uint8_t* ff13_2_set_frame_rate_address;
	float** ff13_2_frame_pacer_ptr_address;
	float ff13_2_targetFrameRate;
	uint8_t** ff13_2_base_controller_input_address_ptr = NULL;
	uint8_t* ff13_2_vibration_high_set_zero_address = NULL;
	uint8_t* ff13_2_vibration_low_set_zero_address = NULL;
	uint8_t* ff13_2_message_box_stack_push_address = NULL;
	uint8_t* ff13_2_message_box_call_address = NULL;
	uint32_t* ff13_2_internal_res_w;
	uint32_t* ff13_2_internal_res_h;

	float expectedDrawPrimitiveUpVertexData[5 * 4]
	{ -1.00f - 1.0f / 1280.0f,       1.00f + 1.0f / 720.0f,    0.0f,    0.0f,    0.0f,
	   1.00f - 1.0f / 1280.0f,       1.00f + 1.0f / 720.0f,    0.0f,    1.0f,    0.0f,
	   1.00f - 1.0f / 1280.0f,      -1.00f + 1.0f / 720.0f,    0.0f,    1.0f,    1.0f,
	  -1.00f - 1.0f / 1280.0f,      -1.00f + 1.0f / 720.0f,    0.0f,    0.0f,    1.0f
	};

	float fixedDrawPrimitiveUpVertexData[5 * 4]
	{ -1.00f,       1.00f,    0.0f,    0.0f,    0.0f,
	   1.00f,       1.00f,    0.0f,    1.0f,    0.0f,
	   1.00f,      -1.00f,    0.0f,    1.0f,    1.0f,
	  -1.00f,      -1.00f,    0.0f,    0.0f,    1.0f
	};

	const float FF13_2_30_FPS = 30.0F;
	const float FF13_2_MAX_FRAME_CAP = 1000.0F;

	float scissor_scaling_factor_w = 1.0f;
	float scissor_scaling_factor_h = 1.0f;

	XInputManager* xinputManager;

	void FixBehaviorFlagConflict(const DWORD flags_in, DWORD* flags_out);
	static const std::map<const AutoFixes, const uint32_t> behaviorflags_add;
	static const std::map<const AutoFixes, const uint32_t> behaviorflags_sub;

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	WNDPROC oldWndProc = nullptr;

	void PrintVersionInfo();

	void FF13_InitializeGameAddresses();
	
	void FF13_OneTimeFixes();
	void FF13_PatchMessageBox();
	void FF13_EnableControllerVibration();
	void FF13_NOPIngameFrameRateLimitSetter();
	void FF13_SetFrameRateVariables();
	void FF13_FixScissorRect();
	void FF13_RemoveContinuousControllerScan();
	void FF13_HandleLargeAddressAwarePatch();

	void FF13_2_CreateSetFrameRateCodeBlock();
	void FF13_2_InitializeGameAddresses();
	void FF13_2_RemoveContinuousControllerScan();
	void FF13_2_AddHookIngameFrameRateLimitSetter();
	void FF13_2_OneTimeFixes();
	void FF13_2_PatchMessageBox();
	void FF13_2_EnableControllerVibration();

	void AdjustVertexData(const uint32_t width, const uint32_t height);
	bool MatchesExpectedVertexStream(const float* pVertexStreamZeroData);
	void ForceWindowActivate(const HWND hWnd);
	void PatchMessageBoxCall(uint8_t* callInstructionAddress);

	bool OneTimeFixInit(std::unique_ptr<wchar_t[]>& className, HWND hWnd);
	std::atomic_bool otf_init = false;

	static void Fix_Thread();
};

extern MainContext context;
