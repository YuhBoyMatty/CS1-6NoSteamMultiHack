#include "Hook.h"

bool Detour32(BYTE* src, BYTE* dst, uintptr_t len)
{
	if (len < 5)
		return false;

	DWORD CurrProtect;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &CurrProtect);

	uintptr_t relativeJump = dst - src - 5;

	*src = 0xE9;
	*(uintptr_t*)(src + 1) = relativeJump;

	VirtualProtect(src, len, CurrProtect, &CurrProtect);
	return true;
}

BYTE* Trampoline32(BYTE* original, BYTE* remplace, uintptr_t len)
{
	if (len < 5)
		return 0;


	//gateway
	BYTE* gateway = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy_s(gateway, len, original, len);
	uintptr_t relativeJump = original - gateway - 5;
	*(BYTE*)((uintptr_t)gateway + len) = 0xE9;
	*(uintptr_t*)((uintptr_t)gateway + len + 1) = relativeJump;

	Detour32(original, remplace, len);

	return gateway;
}

void Patch(BYTE* dst, BYTE* src, uintptr_t len)
{
	DWORD oldprotect;
	VirtualProtect(dst, len, PAGE_EXECUTE_READWRITE, &oldprotect);

	memcpy(dst, src, len);
	VirtualProtect(dst, len, oldprotect, &oldprotect);
}


void UnHook(BYTE * backup, BYTE * original, uintptr_t len)
{
	BYTE* oldProlog = (BYTE*)malloc(len);
	if (!oldProlog)
		return;

	for (size_t i = 0; i < len; i++)
		oldProlog[i] = backup[i];

	Patch(original, oldProlog, len);
	free(oldProlog);
}
