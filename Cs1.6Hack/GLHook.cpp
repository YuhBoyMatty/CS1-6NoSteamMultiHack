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
#include "SpeedHack.h"

#pragma comment(lib, "OPENGL32.lib")

typedef BOOL(WINAPI *twglSwapbuffers)(
	HDC hDC
);

typedef void (WINAPI *tglBegin)(
	GLenum mode
);

typedef void (WINAPI* tglEnd)();


typedef void (WINAPI* tglVertex3f)(
	GLfloat x,
	GLfloat y,
	GLfloat z
);

typedef void (WINAPI* tglBindTexture)(
	GLenum target,
	GLuint texture
);

typedef void (*tPreDrawEntity)();

Trampoline<twglSwapbuffers>*	gTrampWglSwapbuffers;
Trampoline<tglBegin>*			gTrampGlBegin;
Trampoline<tglEnd>*				gTrampGlEnd;
Trampoline<tglVertex3f>*		gTrampGlVertex3f;
Trampoline<tglBindTexture>*		gTrampGlBindTexture;
Trampoline<tPreDrawEntity>*		gTrampPreDrawEntity;
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
		wi = new WindowInfo;
		glGetIntegerv(GL_VIEWPORT, (GLint*)wi);

		textRender = new GL::TextRender();
		menu = new Menu();

		entityList = new EntityListManager(hl);
		aimbot = new Aimbot(hl);
		esp = new ESP(hl);

		

		// Initialize Here Becouse If NOT Will Crash 
		misc = menu->rootSection->AddSection("Misc", new bool(false));
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
		SpeedHack::getInstance(misc)->Update();

		GL::RestoreGL();

	}
	return gTrampWglSwapbuffers->getBackup()(hDC);
}

Player* currRenderingEntity = nullptr;

void hPreDrawEntity()
{
	IGameStudioModelRender* thiz = (IGameStudioModelRender*)0x01A20F90;
	currRenderingEntity = thiz->pToEntity;

	return gTrampPreDrawEntity->getBackup()();
}

GLenum gMode;

bool bInitedBasicTexture = false;
uintptr_t textureId = 0;
uintptr_t lastBoundTextureId = 0;
uintptr_t lastBoundTextureType = 0;
uintptr_t oldTextureId;

void WINAPI hglBindTexture(
	GLenum target,
	GLuint texture
)
{
	lastBoundTextureId = texture;
	lastBoundTextureType = target;

	gTrampGlBindTexture->getBackup()(target, texture);
}

bool gDrawChamsDecision = false;
bool bRenderingModels = false;
bool gValidContext = false;

const Vector4 red = {
	1.f, 0.f, 0.f, 1.f
};

const Vector4 green = {
	0.f, 1.f, 0.f, 1.f
};

const Vector4 yellow = {
	1.f, 1.f, 0.f, 1.f
};

const Vector4* gFinalChamsColor = nullptr;

void WINAPI hglBegin(
	GLenum mode
)
{
	gMode = mode;

	if (!bInitedBasicTexture)
	{
		/*glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		unsigned char* data = new unsigned char[3 * 12 * 12 * sizeof(unsigned char)];

		for (unsigned int i = 0; i < 12 * 12; i++)
		{
			data[i * 3] = (unsigned char)(1.f * 255.0f);
			data[i * 3 + 1] = (unsigned char)(0.f * 255.0f);
			data[i * 3 + 2] = (unsigned char)(0.f * 255.0f);
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 12, 12, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		delete[] data;*/

		bInitedBasicTexture = true;
	}
	bRenderingModels = (gMode == GL_TRIANGLE_STRIP) || (gMode == GL_TRIANGLE_FAN);
	gValidContext = init && bRenderingModels;
	
	gDrawChamsDecision = gValidContext && currRenderingEntity && esp->m_bChamsEnabled;

	if (gDrawChamsDecision)
	{
		gDrawChamsDecision = false;

		if (currRenderingEntity->Valid) // is a player?
		{
			// Dealing WIth A Player

			if (entityList->isTeam(currRenderingEntity->PlayerId, entityList->GetLocalPlayer()->PlayerId)) // is a Friend?
			{
				if (esp->m_ChamsTeams)
				{
					gFinalChamsColor = &green;
					gDrawChamsDecision = true;
				}
			}
			else {
				// The Player is my enemy

				if (esp->m_ChamsEnemys)
				{
					gFinalChamsColor = &red;
					gDrawChamsDecision = true;
				}
			}
		}
		else {
			// the Entity is an Obj

			if (esp->m_ChamsObjects)
			{
				gFinalChamsColor = &yellow;
				gDrawChamsDecision = true;
			}
		}

		if (gDrawChamsDecision)
		{
			glDisable(GL_TEXTURE_2D);

			if (esp->m_ChamsLines)
				mode = GL_LINE_STRIP;
		}
	}

	if (gValidContext && esp->m_Wallhack)
		glDisable(GL_DEPTH_TEST);

	gTrampGlBegin->getBackup()(mode);
}

void WINAPI hglEnd(void)
{
	gTrampGlEnd->getBackup()();

	if (gValidContext && esp->m_Wallhack)
		glEnable(GL_DEPTH_TEST);

	if (gDrawChamsDecision)
		glEnable(GL_TEXTURE_2D);
}

void WINAPI hglVertex3f(
	GLfloat x,
	GLfloat y,
	GLfloat z
)
{
	if (gDrawChamsDecision)
		glColor4f(gFinalChamsColor->x, gFinalChamsColor->y, gFinalChamsColor->z, gFinalChamsColor->w);
	
	gTrampGlVertex3f->getBackup()(x, y, z);
}

bool GLInitHook()
{
	gTrampWglSwapbuffers	= new Trampoline<twglSwapbuffers>("OPENGL32.dll", "wglSwapBuffers", hwglSwapbuffers, 5, true);
	gTrampGlBegin			= new Trampoline<tglBegin>("OPENGL32.dll", "glBegin", hglBegin, 5, true);
	gTrampGlEnd				= new Trampoline<tglEnd>("OPENGL32.dll", "glEnd", hglEnd, 6, true);
	gTrampGlVertex3f		= new Trampoline<tglVertex3f>("OPENGL32.dll", "glVertex3f", hglVertex3f, 5, true);
	gTrampGlBindTexture		= new Trampoline<tglBindTexture>("OPENGL32.dll", "glBindTexture",hglBindTexture, 5, true);
	gTrampPreDrawEntity		= new Trampoline<tPreDrawEntity>(0x1D90660, hPreDrawEntity, 8, true);

	return true;
}

bool GLDisableHook()
{
	delete aimbot;
	delete esp;
	delete menu;
	delete radar;

	// Delete the Tramps Auto Unhook
	delete gTrampWglSwapbuffers;
	delete gTrampGlBegin;
	delete gTrampGlEnd;
	delete gTrampGlVertex3f;
	delete gTrampGlBindTexture;
	delete gTrampPreDrawEntity;
	return true;
}




