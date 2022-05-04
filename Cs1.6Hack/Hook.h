#pragma once
#include <Windows.h>

#ifdef _M_IX86 
#define HOOK_SZ 5
#endif

bool Detour32(BYTE* src, BYTE* dst, uintptr_t len);

BYTE* Trampoline32(BYTE* original, BYTE* remplace, uintptr_t len);

void UnHook(BYTE * backup, BYTE * original, uintptr_t len);

void Patch(BYTE* dst, BYTE* src, uintptr_t len);
