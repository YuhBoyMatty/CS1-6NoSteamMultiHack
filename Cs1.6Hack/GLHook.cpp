#include "GLHook.h"
#include "GLDraw.h"
#include "Hook.h"
#include "EntityListWrapper.h"
#include "ESP.h"
#include "Aimbot.h"
#include "Menu.h"
#include "Radar.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "BHOP.h"
#include "Trampoline.h"

#pragma comment(lib, "OPENGL32.lib")

typedef BOOL(WINAPI *twglSwapbuffers)(
	HDC hDC
);

void (WINAPI *tglBegin)(
	GLenum mode
);

Trampoline<twglSwapbuffers>* gTrampWglSwapbuffers;
bool			init = false;

uintptr_t hl;
WindowInfo* wi;
Section* misc;

bool TestTraceLine = false;

BOOL WINAPI hwglSwapbuffers(HDC hDC){
	HDC currentHDC = wglGetCurrentDC();

	if (!init)
	{
		hl = (uintptr_t)GetModuleHandle(L"hl.exe");
		wi = (WindowInfo*)(hl + 0x1092084);

		textRender = new GL::TextRender();
		menu = new Menu();

		// Initialize Here Becouse If NOT Will Crash 
		misc = menu->rootSection->AddSection("Misc", new bool(false));

		entityList = new EntityListManager(hl);
		aimbot = new Aimbot(hl);
		esp = new ESP(hl);
		radar = new Radar();

		
		printf("wglSwapbuffers Hookeado, GamerzHacking // Guidedhacking\nBy Mstik!\n");

		init = true;
	}
	else {
		GL::SetupOrtho();

		textRender->Validate(currentHDC);
		aimbot->Update();
		esp->Update();
		menu->Update();
		radar->Update();
		BHOP::getInstance()->Update();

		GL::RestoreGL();

	}

	return gTrampWglSwapbuffers->getBackup()(hDC);
}

void WINAPI hglBegin(
	GLenum mode
)
{

}

void WINAPI hglEnd(void)
{

}

bool GLInitHook()
{
	gTrampWglSwapbuffers = new Trampoline<twglSwapbuffers>("OPENGL32.dll", "wglSwapBuffers", hwglSwapbuffers, 5, true);

	return true;
}

bool GLDisableHook()
{
	gTrampWglSwapbuffers->Disable();
	delete aimbot;
	delete esp;
	delete menu;
	delete radar;
	return true;
}




