#include "Radar.h"
#include "GLDraw.h"
#include "Menu.h"

Radar* radar;
extern WindowInfo* wi;


Radar::Radar() : EntityListManager(*entityList)
{
	m_radarRect = { 0.f, 0.f, 193.79f , 195.29f};
	m_radarCenter = { m_radarRect.x + (m_radarRect.width / 2.f), m_radarRect.y + (m_radarRect.height / 2.f) };
	m_radarScale = 50.f;

	auto* pRadarSect = misc->AddSection("Radar", new bool(false));
	pRadarSect->AddButtom("Visible Radar", &m_bRadar);
	pRadarSect->AddSlider("Radar Scale", &m_scale, 0.1f, 100.f);
}

extern float offset;

void Radar::Update()
{
	if (m_bRadar)
	{
		DrawBGRadar();
		DrawDelimiters();
	}
}

void Radar::DrawBGRadar()
{
	GL::DrawRect(m_radarRect, rgb::black);

	for (int i = 1; i < entityList->GetMaxPlayers(); i++)
	{
		if (!entityList->isValid(i))
			continue;

		DrawPlayer(i);
	}
}

void Radar::DrawDelimiters()
{
	GL::DrawLine(m_radarRect.x, m_radarCenter.y, m_radarRect.x + m_radarRect.width, m_radarCenter.y, 0.2f, rgb::white);	//left to right
	GL::DrawLine(m_radarCenter.x , m_radarRect.y, m_radarCenter.x, m_radarRect.y + m_radarRect.height, 0.2f, rgb::white);	//up to down

	Vector2 top_left, top_right;
	top_left = top_right = { m_radarCenter.x, m_radarRect.y };
	Vector2::Rotate(m_radarCenter, top_left, -45.f);
	Vector2::Rotate(m_radarCenter, top_right, 45.f);

	GL::DrawLine(m_radarCenter.x, m_radarCenter.y, top_left.x, top_left.y, 0.2f, rgb::white);	//middle to top left corner
	GL::DrawLine(m_radarCenter.x, m_radarCenter.y, top_right.x, top_right.y, 0.2f, rgb::white);	//middle to top right corner

}

void Radar::DrawPlayer(int index)
{
	

	Rect	pl_pos_2d_rect;
	Vector2 pl_center;
	Vector2 pl_frwd;
	if (!GetPlayerRadarRect(pl_pos_2d_rect, pl_center, pl_frwd, index))
		return;

	auto* lineColor = (entityList->GetTeam(index) == 1) ? rgb::red : rgb::blue;

	GL::DrawLine(pl_center.x, pl_center.y, pl_frwd.x, pl_frwd.y, 2.f, lineColor);
	GL::DrawRect(pl_pos_2d_rect, lineColor);
}

bool Radar::GetPlayerRadarRect(Rect& pos, Vector2& center, Vector2& frwd, int index)
{
	Player * lp = GetPlayerByID(GetLocalPlayerID()), *pl = GetPlayerByID(index);
	Vector3&	lp_pos = lp->Pos3;
	Vector3&	pl_pos = pl->Pos3;

	Vector2 diff = { pl_pos.x - lp_pos.x, pl_pos.y - lp_pos.y };
	diff = diff / ((30.f * m_radarScale) / 100.f);

	Vector2 pl_pos_2d = m_radarCenter + diff;

	pl_pos_2d.y = m_radarRect.y + (m_radarRect.height - pl_pos_2d.y);
	frwd = pl_pos_2d + Vector2::GetForward(pl_pos_2d, -pl->Rot3.y, (m_radarScale * 20.f) / 100.f);
	Vector2::Rotate(m_radarCenter, pl_pos_2d, lp->Rot3.y - 90.f);
	Vector2::Rotate(m_radarCenter, frwd, lp->Rot3.y - 90.f);

	if (pl_pos_2d.x < m_radarRect.x ||
		pl_pos_2d.y < m_radarRect.y ||
		pl_pos_2d.x >(m_radarRect.x + m_radarRect.width) ||
		pl_pos_2d.y >(m_radarRect.y + m_radarRect.height))
		return false;

	*(Vector2*)&pos = pl_pos_2d;
	center = pl_pos_2d;

	pos.x -= 3.f;
	pos.y -= 3.f;
	pos.width = 6.f;
	pos.height = 6.f;
	return true;
}
