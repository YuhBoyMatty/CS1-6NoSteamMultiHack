#include "ESP.h"
#include "GLDraw.h"
#include <math.h>
#include "Aimbot.h"
#include "Menu.h"

#define toRad(angleDegrees) ((angleDegrees) * PI / 180.0)
#define toDeg(angleRadians) ((angleRadians) * 180.0 / PI)

ESP* esp;
extern WindowInfo* wi;

ESP::ESP(uintptr_t hlAddr) : EntityListManager(*entityList)
{
	m_ViewMatrixAddr = (float*)(hlAddr + 0x1820100);
	m_width = 1024;
	m_height = 768;

	auto* pESPSect = menu->rootSection->AddSection("ESP", &m_ESP);
	pESPSect->AddButtom("Line", &m_Line);

	std::vector<ButtonCreationInfo> tiBox;
	tiBox.push_back({"Box 2D", &m_Box2D});
	tiBox.push_back({"Box 3D" ,	&m_Box3D });
	
	Section* pESPBoxSect = pESPSect->AddSection("ESP Box", new bool(false));
	pESPBoxSect->AddLinkedButtonList(tiBox);

	pESPSect->AddButtom("Forward", &m_DrawForward);
	pESPSect->AddButtom("FOV", &m_DrawFov);
	pESPSect->AddButtom("SysCoords", &m_DrawSystemCoords);
	pESPSect->AddButtom("Velocity", &m_drawVelocity);
}

void ESP::Update()
{
	Player* localPlayer = GetLocalPlayer();

	if (localPlayer)
	{
		for (int i = 0; i < GetMaxPlayers(); i++)
		{
			Player* currPlayer = GetPlayerByID(i);
			if (m_drawVelocity)
			{
				m_dir[i] = m_oldPos[i] - currPlayer->Pos3;
				m_oldPos[i] = currPlayer->Pos3;
			}
			if (!entityList->isValid(i))
				continue;

			

			auto* lineColor = (entityList->GetTeam(i) == 1) ? rgb::red : rgb::blue;
			Vector3 playerPos2D;
			
			if(m_Line)
				if (W2S(currPlayer->Pos3, playerPos2D))
					GL::DrawLine(wi->width / 2.f, 0.f, playerPos2D.x, playerPos2D.y, 0.2f, lineColor);

			if(m_DrawSystemCoords)
				DrawSystemCoordinates(currPlayer, 9.f);

			if(m_DrawForward)
				DrawForward(currPlayer, 18.f, rgb::green);

			if(m_Box2D)
				DrawBox(currPlayer, lineColor);
			else if(m_Box3D)
				DrawBox3D(currPlayer, 18.f, 1.f, lineColor);
				
			if(m_drawVelocity)
				DrawVelocity(i, 7.f, rgb::green);

		}
		if(m_DrawFov)
			DrawFov();
	}
}

bool ESP::W2S(Vector3 & pos, Vector3 & pos2d)
{

	/*glm::vec3 tmp_pos(pos.x, pos.y, pos.z);
	glm::vec3 tmp_result(0.f);
	bool isInFront = camera->WorldToScreen(tmp_pos, tmp_result);
	pos2d = { tmp_result.x, tmp_result.y, tmp_result.z };
	return isInFront;*/

	Vector4 clipCoords;
	clipCoords.x = pos.x*m_ViewMatrixAddr[0] + pos.y*m_ViewMatrixAddr[4] + pos.z*m_ViewMatrixAddr[8] + m_ViewMatrixAddr[12];
	clipCoords.y = pos.x*m_ViewMatrixAddr[1] + pos.y*m_ViewMatrixAddr[5] + pos.z*m_ViewMatrixAddr[9] + m_ViewMatrixAddr[13];
	clipCoords.z = pos.x*m_ViewMatrixAddr[2] + pos.y*m_ViewMatrixAddr[6] + pos.z*m_ViewMatrixAddr[10] + m_ViewMatrixAddr[14];
	clipCoords.w = pos.x*m_ViewMatrixAddr[3] + pos.y*m_ViewMatrixAddr[7] + pos.z*m_ViewMatrixAddr[11] + m_ViewMatrixAddr[15];

	

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	Vector3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	//Transform to window coordinates
	pos2d.x = (wi->width / 2 * NDC.x) + (NDC.x + wi->width / 2);
	pos2d.y = -(wi->height / 2 * NDC.y) + (NDC.y + wi->height / 2);
	pos2d.z = clipCoords.w;
	if (clipCoords.w < 0.1f)
		return false;

	return true;
}

void ESP::DrawBox(Player* p, const GLubyte * color)
{
	Vector3 headPos; entityList->GetHead(p, headPos);

	Vector3 footPos; entityList->GetFoot(p, footPos);

	Vector3 headPos2D, footPos2D;

	if (!W2S(headPos, headPos2D) || !W2S(footPos, footPos2D))
		return;

	float playerWidth = (ABS(headPos2D.y - footPos2D.y)) / 4;

	Vector3 tl, tr;

	tl = tr = headPos2D;
	tl.x -= playerWidth;
	tr.x += playerWidth;

	Vector3 bl, br;

	bl = br = footPos2D;
	bl.x -= playerWidth;
	br.x += playerWidth;

	DrawLine(tl, tr, 2.f, color);
	DrawLine(tr, br, 2.f, color);
	DrawLine(br, bl, 2.f, color);
	DrawLine(bl, tl, 2.f, color);

}

Vector3 ESP::GetForward(Player* p, float d)
{
	Vector3 dir;
	dir.x = cosf(degreesToRadians(p->Rot3.x));
	dir.y = sinf(degreesToRadians(p->Rot3.x));
	dir.z = 0;
	dir = dir * d;
	return dir;
}

void ESP::DrawForward(Player* p,float d, const GLubyte * color)
{
	Vector3 headPos; entityList->GetHead(p, headPos);

	headPos.z -= 3.f;
	Vector3 headFrwd = headPos + GetForward(p, d);
	

	Vector3 headPos2D, headfrwd2D;
	if (W2S(headPos, headPos2D) && W2S(headFrwd, headfrwd2D))
		DrawLine(headPos2D, headfrwd2D, 1.5f, color);
}

void ESP::DrawSystemCoordinates(Player* p, float distance)
{
	Vector3 footPos; entityList->GetFoot(p, footPos);

	Vector3 footPosL, footPosF, footPosU;
	footPosL = footPosF = footPosU = footPos;

	footPosL = footPosL + (Vector3::get_left() * distance);
	footPosF = footPosF + (Vector3::get_forward() * distance);
	footPosU = footPosU + (Vector3::get_up() * distance);

	Vector3 footPos2D, footPosL2D, footPosF2D, footPosU2D;

	if (W2S(footPos, footPos2D) && W2S(footPosL, footPosL2D) && W2S(footPosF, footPosF2D) && W2S(footPosU, footPosU2D))
	{
		DrawLine(footPos2D, footPosL2D, 1.5f, rgb::red);
		DrawLine(footPos2D, footPosF2D, 1.5f, rgb::blue);
		DrawLine(footPos2D, footPosU2D, 1.5f, rgb::green);
	}
}

void ESP::DrawLine(Vector3& a, Vector3& b, float thickness, const GLubyte * color)
{
	GL::DrawLine(a.x, a.y, b.x, b.y, thickness, color);
}

void ESP::DrawBox3D(Player* p,float d,float tickness, const GLubyte * color)
{
	Vector3 headPos; entityList->GetHead(p, headPos);

	Vector3 footPos; entityList->GetFoot(p, footPos);
	Vector3 upperPlane[4];
	Vector3 LowerPlane[4];

	float step = 45.f;
	for (int i = 0; i < 4; i++, step += 90.f)
	{
		upperPlane[i].x = headPos.x + cosf(degreesToRadians(p->Rot3.x + step)) * d;
		upperPlane[i].y = headPos.y + sinf(degreesToRadians(p->Rot3.x + step)) * d;
		upperPlane[i].z = headPos.z;
	}

	for (int i = 0; i < 4; i++, step += 90.f)
	{
		LowerPlane[i].x = upperPlane[i].x;
		LowerPlane[i].y = upperPlane[i].y;
		LowerPlane[i].z = footPos.z;
	}

	Vector3 upperPlane2D[4];
	Vector3 lowerPlane2D[4];

	for (int i = 0; i < 4; i++)
		if (!W2S(upperPlane[i], upperPlane2D[i]) || !W2S(LowerPlane[i], lowerPlane2D[i]))
			return;

	for (int i = 0; i < 3; i++)
	{
		DrawLine(upperPlane2D[i], upperPlane2D[i + 1], tickness, color);
		DrawLine(lowerPlane2D[i], lowerPlane2D[i + 1], tickness, color);
	}

	DrawLine(upperPlane2D[3], upperPlane2D[0], tickness, color);
	DrawLine(lowerPlane2D[3], lowerPlane2D[0], tickness, color);

	for (int i = 0; i < 4; i++)
		DrawLine(lowerPlane2D[i], upperPlane2D[i], tickness, color);
}

void ESP::DrawVelocity(int id, float d, const GLubyte * color)
{
	Player * p = GetPlayerByID(id);
	Vector3& velocityOrigin = p->Pos3;
	Vector3 velocityEnd = velocityOrigin + ((m_dir[id] * d) * -1);

	Vector3 velocityOrigin2D, velocityEnd2D;
	if (!W2S(velocityOrigin, velocityOrigin2D) || !W2S(velocityEnd, velocityEnd2D))
		return;

	DrawLine(velocityOrigin2D, velocityEnd2D, 1.5f, color);
}

void ESP::DrawFov()
{
	Player* localPlayer = GetLocalPlayer();
	float fovDistanceinWith = ((aimbot->m_fov / 2) * 1024) / 53.f;

	GL::DrawCircle(wi->width / 2.f, m_height / 2.f, fovDistanceinWith, 120, rgb::green);
	int t = aimbot->GetBest(Angle);
	
	if (t == -1)
		return;

	Vector3 screenCenter = { wi->width / 2.f, (wi->height / 2.f) + 3.f };
	Vector3 pheadPos2D;
	Vector3 pheadPos; entityList->GetHead_Offsetted(GetPlayerByID(t), pheadPos);

	
	if (W2S(pheadPos, pheadPos2D))
		DrawLine(screenCenter, pheadPos2D, 1.f, rgb::white);
}
