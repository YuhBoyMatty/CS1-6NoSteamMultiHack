#pragma once

#include "GLDraw.h"

#pragma comment(lib, "OpenGL32.lib")
#include <Windows.h>
#include <stdio.h>
#include <gl/gl.h>
#include "Vectors.h"

namespace rgb
{
	const GLubyte red[3] = { 255,0,0 };
	const GLubyte skyblue[3] = { 135,206,235 };
	const GLubyte blue[3] = { 0,0,255 };
	const GLubyte black[3] = { 1,1,1 };
	const GLubyte white[3] = { 255,255,255 };
	const GLubyte green[3] = { 0,255,0 };
	const GLubyte transpGray[3] = { 85,86,87 };
	const GLubyte darkGray[3] = { 29,30,32 };
	const GLubyte cyan[3] = { 51,255,255 };
	const GLubyte yellow[3] = { 255,255,0 };

}

namespace GL {

	class TextRender
	{
	public:
		bool bTextCompiled = false;
		unsigned int charbuff;
		HDC hdc = nullptr;

		void BuildText(int h);
		void DrawString(float x, float y, const unsigned char color[3], const char* format, ...);
		void Validate(HDC _currHdc);
		static float CenterText(float x, float w, int cN);
	};

	void SetupOrtho();
	void RestoreGL();



	void DrawFilledRectangle(float x, float y, float width, float height, const GLubyte color[3]);
	void DrawOutLine(float x1, float y1, float x2, float y2, float width, const GLubyte color[3]);
	void DrawLine(float x1, float y1, float x2, float y2, float width, const GLubyte color[3]);
	void DrawCircle(float x, float y, float radio, int sectors, const GLubyte color[3]);
	void DrawFilledCircle(float x, float y, float radio, const GLubyte color[3]);
	void DrawRect(Rect& r, const GLubyte color[3]);
	void DrawCross(float x, float y, const GLubyte color[3]);

	
}

extern GL::TextRender* textRender;