#pragma once
#include "EntityListWrapper.h"

class Radar : public EntityListManager
{
private:
	Rect		m_radarRect;
	Vector2		m_radarCenter;
	float		m_radarScale;
	bool		m_bRadar = false;
	float		m_scale = 50.f;
public:
	Radar();
	void Update();
	void DrawBGRadar();
	void DrawDelimiters();
	void DrawPlayer(int index);
	bool GetPlayerRadarRect(Rect& pos,  Vector2& center, Vector2& frwd, int index);

};

extern Radar* radar;
