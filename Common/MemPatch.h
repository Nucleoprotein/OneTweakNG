#pragma once
#include <Windows.h>

namespace MemPatch
{
	class CUnprotect
	{
	public:
		CUnprotect(void* address, unsigned long size) : m_adress(address), m_size(size)
		{
			VirtualProtect(m_adress, m_size, PAGE_EXECUTE_READWRITE, &m_oldprotect);
		}

		~CUnprotect()
		{
			DWORD oldProtect;
			VirtualProtect(m_adress, m_size, m_oldprotect, &oldProtect);
		}
	private:
		void* m_adress;
		unsigned long m_size;
		DWORD m_oldprotect;
	};

	__forceinline static void Patch(void* address, void* data, unsigned long size)
	{
		CUnprotect unp(address, size);
		memcpy(address, data, size);
	}

	__forceinline static void Fill(void* address, unsigned char byte, unsigned long size)
	{
		CUnprotect unp(address, size);
		memset(address, byte, size);
	}

	__forceinline static void Nop(void* address, unsigned long size)
	{
		Fill(address, 0x90, size);
	}

}