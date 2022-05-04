#include "GLDraw.h"
#include "Vectors.h"

GL::TextRender* textRender;

void GL::SetupOrtho()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);
	glDisable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}

void GL::RestoreGL()
{
	glPopMatrix();
	glPopAttrib();
}

void GL::DrawFilledRectangle(float x, float y, float width, float height, const GLubyte color[3])
{
	glDisable(GL_TEXTURE_2D);
	glColor3ub(color[0], color[1], color[2]);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void GL::DrawOutLine(float x1, float y1, float x2, float y2, float width, const GLubyte color[3])
{
	glDisable(GL_TEXTURE_2D);
	glLineWidth(width);
	glBegin(GL_LINE_STRIP);	
	glColor3ub(color[0], color[1], color[2]);
	glVertex2f(x1 - 0.5f, y1 - 0.5f);
	glVertex2f(x1 + x2 + 0.5f, y1 - 0.5f);
	glVertex2f(x1 + x2 + 0.5f, y1 + y2 + 0.5f);
	glVertex2f(x1 - 0.5f, y1 + y2 + 0.5f);
	glVertex2f(x1 - 0.5f, y1 - 0.5f);
	glEnd();
	glEnable(GL_TEXTURE_2D);

}

void GL::DrawLine(float x1, float y1, float x2, float y2, float width, const GLubyte color[3])
{
	glDisable(GL_TEXTURE_2D);
	glLineWidth(width);
	glBegin(GL_LINE_STRIP);
	glColor3ub(color[0], color[1], color[2]);
	glVertex2f(x1,y1);
	glVertex2f(x2, y2);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void GL::DrawCircle(float x, float y, float radio, int sectors, const GLubyte color[3])
{
	float step = 360.f / sectors;

	for (float stepCount = 0.f; stepCount < 360.f; stepCount += step)
	{
		float x1, y1, x2, y2;

		x1 = x2 = x;
		y1 = y2 = y;

		x1 += cosf(degreesToRadians(stepCount)) * radio;
		y1 += sinf(degreesToRadians(stepCount)) * radio;

		x2 += cosf(degreesToRadians(stepCount + step)) * radio;
		y2 += sinf(degreesToRadians(stepCount + step)) * radio;

		DrawLine(x1, y1, x2, y2, 1.f, color);
	}
}

void GL::DrawFilledCircle(float x, float y, float radio, const GLubyte color[3])
{
	
}

void GL::DrawRect(Rect & r, const GLubyte color[3])
{
	GL::DrawFilledRectangle(r.x, r.y, r.width, r.height, color);
}

void GL::DrawCross(float x, float y, const GLubyte color[3])
{
	Vector2 cx1, cx2,
			cy1, cy2;

	cx1.x = cx2.x = x;
	cx1.y = cx2.y = y;
	cx1.x -= 10.f;
	cx2.x += 10.f;

	cy1.x = cy2.x = x;
	cy1.y = cy2.y = y;
	cy1.y -= 10.f;
	cy2.y += 10.f;

	GL::DrawLine(cx1.x, cx1.y, cx2.x, cx2.y, 1.f, color);
	GL::DrawLine(cy1.x, cy1.y, cy2.x, cy2.y, 1.f, color);
}

void GL::TextRender::BuildText(int h)
{

	hdc = wglGetCurrentDC();
	charbuff = glGenLists(96);
	HFONT fonts = CreateFontA(-h, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Consolas");

	HFONT oldFonts = (HFONT)SelectObject(hdc, fonts);
	wglUseFontBitmaps(hdc, 32, 96, charbuff);
	SelectObject(hdc, oldFonts);
	DeleteObject(fonts);
	bTextCompiled = true;
	
}

void GL::TextRender::DrawString(float x, float y, const unsigned char color[3], const char * format, ...)
{
	glDisable(GL_TEXTURE_2D);
	glColor3ub(color[0], color[1], color[2]);
	glRasterPos2f(x, y);

	char text[100];
	va_list args;

	va_start(args, format);
	vsprintf_s(text, 100, format, args);
	va_end(args);

	glPushAttrib(GL_LIST_BIT);
	glListBase(charbuff - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
	glEnable(GL_TEXTURE_2D);
}

void GL::TextRender::Validate(HDC _currHdc)
{
	if (!bTextCompiled || _currHdc != hdc)
		BuildText(13);
}

float GL::TextRender::CenterText(float x, float w, int cN)
{
	return (x + (w / 2)) - (3.4f * cN);
}
