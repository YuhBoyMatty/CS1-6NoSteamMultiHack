#include "EntityListWrapper.h"

EntityListManager* entityList;

EntityListManager::EntityListManager(uintptr_t hlAddr)
{
	m_localPlayer		= (LocalPlayer*)(hlAddr + 0x1b5a47c);
	m_NamesList			= (Names*)(hlAddr + 0x1b5a47c);
	m_pInfoList			= (PlayersInfo*)(hlAddr + 0x625642);
	m_coordsInfoList	= (Coords*)(hlAddr + 0xAF90AC);
	m_EntityList		 = (EntityList**)(hlAddr + 0xAF9B04);
	m_localPlayerIDPtr	= (int*)(hlAddr + 0x617CBC);
	m_maxPlayers		= (int*)(hlAddr + 0x1B599C4);
}

EntityListManager::~EntityListManager()
{

}

PlayerInfoObj * EntityListManager::GetPlayerInfoByID(int id)
{
	return &m_pInfoList->m_PlayerInfoArr[id];
}

Player * EntityListManager::GetPlayerByID(int id)
{

	return &((*m_EntityList)->EntityList[id]);
}

bool EntityListManager::isValid(int id)
{
	if (id != *m_localPlayerIDPtr && isAlive(id) && m_coordsInfoList->m_coordsInfo[id - 1].m_nodormanting) // 
		return true;
	
	return false;
}

Player * EntityListManager::GetLocalPlayer()
{
	return m_localPlayer ? &((*m_EntityList)->EntityList[*m_localPlayerIDPtr]) : nullptr;
}

int EntityListManager::GetTeam(int id)
{
	return m_pInfoList->m_PlayerInfoArr[id].m_team;
}

bool EntityListManager::isTeam(int id_a, int id_b)
{
	if (GetTeam(id_a) == GetTeam(id_b))
		return true;

	return false;
}

bool EntityListManager::isAlive(int id)
{
	return !m_pInfoList->m_PlayerInfoArr[id].m_bDead;
}

void EntityListManager::GetHead_Offsetted(Player* co, Vector3 & result)
{
	float& pYaw = co->Rot3.x;;
	result.x = (cosf(degreesToRadians(pYaw)) * 8.6f);
	result.y = (sinf(degreesToRadians(pYaw)) * 8.6f);
	result.z = 5.f;
	result = result + co->Pos3;
}

void EntityListManager::GetFoot(Player* co, Vector3 & result)
{
	result = co->Pos3;
	result.z -= 37.f;
}

void EntityListManager::GetHead(Player* co, Vector3 & result)
{
	result = co->Pos3;
	result.z += 20.f;
}
