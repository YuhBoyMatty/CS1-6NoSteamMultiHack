#pragma once

#include <Windows.h>
#include <string>
#include "Hook.h"

template<typename tFunc>
class Trampoline
{
private:
	tFunc m_Backup;
	tFunc m_OriginalEntry;
	tFunc m_Remplace;
	bool  m_bEnabled;
	uintptr_t m_EntryHookInstBytesLen;

	void InitBackup(uintptr_t entryHookInstBytesCnt);
public:
	Trampoline(const std::string& moduleName, const std::string& functionName, void* pfnRepalce, uintptr_t entryHookInstBytesCnt, bool bEnable = true);
	~Trampoline();

	tFunc getBackup();
	void Enable();
	void Disable();
	void Toggle();
};

template<typename tFunc>
inline void Trampoline<tFunc>::InitBackup(uintptr_t entryHookInstBytesCnt)
{
	if (entryHookInstBytesCnt < 5)
		throw "To Few Bytes to Replace, More than 5";

	m_Backup = (tFunc)VirtualAlloc(nullptr, entryHookInstBytesCnt + HOOK_SZ, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (!m_Backup)
		throw "Unable to Allocate Backup";
}

template<typename tFunc>
inline Trampoline<tFunc>::Trampoline(const std::string & moduleName, const std::string & functionName, void* pfnRepalce, uintptr_t entryHookInstBytesCnt, bool bEnable)
	: m_bEnabled(false)
	, m_EntryHookInstBytesLen(entryHookInstBytesCnt)
{
	InitBackup(entryHookInstBytesCnt);
	m_Remplace = (tFunc)pfnRepalce;

	HMODULE hMod = GetModuleHandleA(moduleName.c_str());

	if (!hMod)
		throw "Module Not Found";

	m_OriginalEntry = (tFunc)GetProcAddress(hMod, functionName.c_str());

	if (bEnable)
		Enable();
}

template<typename tFunc>
inline Trampoline<tFunc>::~Trampoline()
{
	if (m_Remplace)
		delete m_Remplace;
}

template<typename tFunc>
inline tFunc Trampoline<tFunc>::getBackup()
{
	return m_Backup;
}

template<typename tFunc>
inline void Trampoline<tFunc>::Enable()
{
	if (!m_bEnabled)
	{
#ifdef _M_IX86 
		m_Backup = (tFunc) Trampoline32((BYTE*)m_OriginalEntry, (BYTE*)m_Remplace, m_EntryHookInstBytesLen);
#endif

		m_bEnabled = true;
	}
}

template<typename tFunc>
inline void Trampoline<tFunc>::Disable()
{
	if (m_bEnabled)
	{
		Patch((BYTE*)m_OriginalEntry, (BYTE*)m_Backup, m_EntryHookInstBytesLen);

		m_bEnabled = false;
	}
}

template<typename tFunc>
inline void Trampoline<tFunc>::Toggle()
{
	if (m_bEnabled)
		Disable();
	else
		Enable();
}
