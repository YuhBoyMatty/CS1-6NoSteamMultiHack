#pragma once
#include "EntityListWrapper.h"
#include "GLDraw.h"

class ESP : public EntityListManager
{
private:
	float*	m_ViewMatrixAddr;
	bool	m_ESP = false;
	bool	m_DrawForward = false;
	bool	m_DrawSystemCoords = false;
	bool	m_DrawFov = false;
	bool	m_Line = false;
	bool	m_Box2D = false;
	bool	m_Box3D = false;
	bool	m_drawVelocity = false;
	Vector3 m_dir[64];
	Vector3 m_oldPos[64];
public:
	float	m_width;
	float	m_height;
	ESP(uintptr_t accli);
	void Update();
	bool W2S(Vector3& pos, Vector3& pos2d);
	void DrawBox(Player* p, const GLubyte* color);
	Vector3 GetForward(Player* p, float d);
	void DrawForward(Player* p, float d, const GLubyte * color);
	void DrawSystemCoordinates(Player* p, float distance);
	void DrawBox3D(Player* p,float d, float tickness, const GLubyte * color);
	void DrawVelocity(int id, float d, const GLubyte * color);
	void DrawFov();
	static void DrawLine(Vector3& a, Vector3& b, float thickness, const GLubyte * color);

	bool	m_bChamsEnabled = false;
	bool	m_Wallhack = true;
	bool	m_ChamsSolid = true;
	bool	m_ChamsLines = false;
	bool	m_ChamsEnemys = true;
	bool	m_ChamsTeams = true;
	bool	m_ChamsObjects = true;
	
};

extern ESP* esp;
