#pragma once

#include <array>
#include "d3d9.h"
#include <MinHook.h>
#include "SimpleIni.h"

struct hkIDirect3D9;

static const char* inifilename = "OneTweakNG.ini";
#define CONFIG_VERSION 3

class Config : NonCopyable
{
public:
	Config();

#define SETTING(_type, _func, _var, _section, _defaultval) \
	private: _type _var; \
	public: const _type& Get##_var() const { return _var; };
#include "Settings.h"
#undef SETTING
};

#define DECLARE_HOOK(type, callconv, name, ...) \
	public: type(callconv* True##name)(__VA_ARGS__); \
	private: static type callconv Hook##name(__VA_ARGS__);

class MainContext : NonCopyable
{
	DECLARE_HOOK(IDirect3D9*, WINAPI, Direct3DCreate9, UINT SDKVersion);
	DECLARE_HOOK(LONG, WINAPI, SetWindowLongA, HWND hWnd, int nIndex, LONG dwNewLong);
	DECLARE_HOOK(LONG, WINAPI, SetWindowLongW, HWND hWnd, int nIndex, LONG dwNewLong);
	DECLARE_HOOK(HWND, WINAPI, CreateWindowExA, DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
		DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	DECLARE_HOOK(HWND, WINAPI, CreateWindowExW, DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName,
		DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

public:
	MainContext();
	~MainContext();

	bool ApplyPresentationParameters(D3DPRESENT_PARAMETERS* pPresentationParameters);
	bool ApplyBehaviorFlagsFix(DWORD* flags);
	bool ApplyVertexBufferFix(UINT& Length, DWORD& Usage, DWORD& FVF, D3DPOOL& Pool);
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
		FINAL_FANTASY_XIII
	};

	void EnableAutoFix();
	AutoFixes autofix = AutoFixes::NONE;

	void FixBehaviorFlagConflict(const DWORD flags_in, DWORD* flags_out);
	static const std::map<const AutoFixes, const uint32_t> behaviorflags_fixes;

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	WNDPROC oldWndProc;
};

extern MainContext context;