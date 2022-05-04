#include <Windows.h>
#include <iostream>
#include "Aimbot.h"
#include "GLHook.h"

void HackThread(HMODULE hMod)
{
	/*FILE* f;
	AllocConsole();
	freopen_s(&f, "CONOUT$", "w", stdout);*/

	GLInitHook();

	while (!(GetAsyncKeyState(VK_DELETE) & 1))
	{	
		Sleep(10);
	}

	GLDisableHook();
	//FreeConsole();
	Sleep(100);
	//fclose(f);
	FreeLibraryAndExitThread(hMod, 1);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HackThread, (void*)hModule, NULL, NULL);
		DisableThreadLibraryCalls(hModule);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

