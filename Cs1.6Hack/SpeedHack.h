#pragma once

#include "Menu.h"

class SpeedHack
{
private:
	inline static SpeedHack* m_Instance = nullptr;

	bool m_bEnabled = false;
	float m_Velocity = 1.f;
	float* m_GameVelocity;
public:
	static SpeedHack* getInstance(Section* pSecMisc);
	SpeedHack(Section* pSecMisc);
	void Update();
};

