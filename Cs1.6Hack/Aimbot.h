#pragma once
#include <Windows.h>
#include "EntityListWrapper.h"

enum AimAt {
	Head,
	Chest,
	Pelvis
};

enum AimType {
	Distance,
	Angle
};

class Aimbot : public EntityListManager 
{
private:

	bool		m_byFov = true;
	bool		m_byDistance = false;
	bool		m_aimLeftClick = false;
	bool		m_aimRightClick = true;
	bool		m_aimHead = true;
	bool		m_aimHip = false;
	bool		m_aimPelvis = false;
	bool		m_bRandomlySelectAimAt = false;
	int			m_localPlayerLastScore = -1;

public:
	Aimbot(uintptr_t hlAddr);
	~Aimbot();
	void	Update();
	void	RawUpdate();
	int		GetBest(int aimType);
	void	AutoAim(int id);
	void	RandomlySelectAimAt();
	static Vector2 CalcAngle(Vector3& a, Vector3& b);
	static int RandomBodyPartIDGenerator();

	float m_fov;
	float m_smooth;
	Vector2*	m_angles;
};

extern Aimbot* aimbot;