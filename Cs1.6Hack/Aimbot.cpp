#include "Aimbot.h"
#include <Windows.h>
#include "Hook.h"
#include "Menu.h"
#include <time.h>

Aimbot* aimbot;

Aimbot::Aimbot(uintptr_t hlAddr) : EntityListManager(*entityList)
{
	m_angles = (Vector2*)(hlAddr + 0x19E10C4);
	Section* pAimbotSection = menu->rootSection->AddSection("Aimbot", new bool(false));

	pAimbotSection->AddSlider("FOV", &m_fov, 0.f, 180.f);
	pAimbotSection->AddSlider("Smooth", &m_smooth, 0.f, 100.f);

	Section* pBestTargetSection = pAimbotSection->AddSection("Best Target By", new bool(false));

	std::vector<ButtonCreationInfo> tinfosAimType;
	tinfosAimType.push_back({ "FOV", &m_byFov });
	tinfosAimType.push_back({ "Distance",&m_byDistance });
	pBestTargetSection->AddLinkedButtonList(tinfosAimType, false);

	Section* pAimKeySection = pAimbotSection->AddSection("Aimbot Key", new bool(false));

	std::vector<ButtonCreationInfo> tinfosAimButton;
	tinfosAimButton.push_back({ "Left Click", &m_aimLeftClick });
	tinfosAimButton.push_back({ "Right Click", &m_aimRightClick });
	pAimKeySection->AddLinkedButtonList(tinfosAimButton, false);

	Section* pAimAtSection = pAimbotSection->AddSection("Aimbot At", new bool(false));

	std::vector<ButtonCreationInfo> tinfosAimAt;
	tinfosAimAt.push_back({ "Head", &m_aimHead });
	tinfosAimAt.push_back({ "Chest", &m_aimHip });
	tinfosAimAt.push_back({ "Pelvis", &m_aimPelvis });
	pAimAtSection->AddLinkedButtonList(tinfosAimAt, false);

	m_fov = 30.f;
	m_smooth = 2.6f;

}

Aimbot::~Aimbot()
{
}

void Aimbot::Update()
{
	int vkSelected = (m_aimLeftClick) ? VK_LBUTTON : VK_RBUTTON;
	if (GetAsyncKeyState(vkSelected))
	{
		int bestBy = (m_byFov) ? Angle : Distance;
		int bestTarget = GetBest(bestBy);

		if (bestTarget != -1)
			AutoAim(bestTarget);
	}
}

void Aimbot::RawUpdate()
{
	int bestTarget = GetBest(Angle);

	if (bestTarget != -1)
		AutoAim(bestTarget);
}


int Aimbot::GetBest(int aimType)
{
	float	bestMetric = 0.f;
	int		bestTarget	= -1;

	Player* localPlayer = GetLocalPlayer();

	if (localPlayer)
	{
		switch (aimType)
		{
			case Distance:
			{
				bestMetric = 9999.f;
				for (int i = 0; i < GetMaxPlayers(); i++)
				{
					Player* currEnt = GetPlayerByID(i);
					if (!isValid(i))
						continue;

					if (isTeam(GetLocalPlayerID(), i))
						continue;

					float tmpDist = Vector3::Distance(currEnt->Pos3, localPlayer->Pos3);
					if (tmpDist < bestMetric)
					{
						bestMetric = tmpDist;
						bestTarget = i;
					}
				}
			}
			break;

			case Angle:
			{
				Vector2 angles = *m_angles;

				bestMetric = m_fov;
				for (int i = 0; i < GetMaxPlayers(); i++)
				{
					Player* currEnt = entityList->GetPlayerByID(i);
					if (!isValid(i))
						continue;

					if (isTeam(GetLocalPlayerID(), i))
						continue;

					Vector2 aimAngles = CalcAngle(localPlayer->Pos3, currEnt->Pos3);

					float tmpAngle = Vector2::GetAngle(aimAngles, angles);
					if (tmpAngle < bestMetric)
					{
						bestMetric = tmpAngle;
						bestTarget = i;
					}
				}
			}
			break;
		}
	}

	return bestTarget;
}

void Aimbot::AutoAim(int id)
{
	Player* localPlayer = GetLocalPlayer();

	if (localPlayer)
	{
		Player* bestTarget = GetPlayerByID(id);
		Vector3 bestTargetPosToAim;
		if (m_aimHead)
			entityList->GetHead_Offsetted(bestTarget, bestTargetPosToAim);
		else if (m_aimHip)
		{
			Vector3 hipPos = bestTarget->Pos3;
			bestTargetPosToAim = hipPos;
		}
		else if (m_aimPelvis)
		{
			bestTargetPosToAim = bestTarget->Pos3;
		}
		Vector2 aimAngles = CalcAngle(localPlayer->Pos3, bestTargetPosToAim);
		aimAngles = Vector2::LerpAngles(*m_angles, aimAngles, (100.f - m_smooth) / 100.f);
		*m_angles = aimAngles;

		//RandomlySelectAimAt();
	}
}

void Aimbot::RandomlySelectAimAt()
{
	int newBodyPartGenerated = RandomBodyPartIDGenerator();

	switch (newBodyPartGenerated)
	{
	case Head:
		m_aimHead = true;
		m_aimHip = false;
		m_aimPelvis = false;
		break;

	case Chest:
		m_aimHead = false;
		m_aimHip = true;
		m_aimPelvis = false;
		break;

	case Pelvis:
		m_aimHead = false;
		m_aimHip = false;
		m_aimPelvis = true;
		break;
	}
}

Vector2 Aimbot::CalcAngle(Vector3 & a, Vector3 & b)
{
	Vector2 newAngles;

	Vector3 diff = b - a;
	float hyp = Vector3::Distance(a, b);
	newAngles.x = radiansToDegrees(-asinf(diff.z / hyp));
	newAngles.y = radiansToDegrees(atan2f(diff.y, diff.x));

	return newAngles;
}

int Aimbot::RandomBodyPartIDGenerator()
{
	srand((unsigned)time(0));
	return rand() % 3;
}

