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
	}
}

const std::map<const MainContext::AutoFixes, const uint32_t> MainContext::behaviorflags_fixes =
{
	{ RESIDENT_EVIL_4, D3DCREATE_SOFTWARE_VERTEXPROCESSING },
	{ KINGS_BOUNTY_LEGEND, D3DCREATE_MIXED_VERTEXPROCESSING },
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

	auto && fix = behaviorflags_fixes.find(autofix);
	if (fix != behaviorflags_fixes.end())
	{
		FixBehaviorFlagConflict(fix->second, flags);
		return true;
	}

	return false;
}

HRESULT APIENTRY MainContext::ApplyVertexBufferFix(IDirect3DDevice9 *pIDirect3DDevice9, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
	if (autofix == AutoFixes::NONE) return pIDirect3DDevice9->CreateVertexBuffer(Length,Usage,FVF,Pool,ppVertexBuffer,pSharedHandle);

	// Final Fantasy XIII
	if (autofix == FINAL_FANTASY_XIII)
	{
		if (Length == 358400 && FVF == 0 && Pool == D3DPOOL_MANAGED) { 
			Usage = D3DUSAGE_DYNAMIC; Pool = D3DPOOL_SYSTEMMEM;

			//IDirect3DVertexBuffer9* buffer = nullptr;
			//HRESULT hr = pIDirect3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, &buffer, NULL);
			//if (FAILED(hr))
			//{
			//	return pIDirect3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
			//}

			//if(ppVertexBuffer) *ppVertexBuffer = new hkIDirect3DVertexBuffer9(pIDirect3DDevice9, buffer);
			//return hr;
		}
	}
	return pIDirect3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}