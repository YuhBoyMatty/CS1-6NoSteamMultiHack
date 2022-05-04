#pragma once
#include "reclass.h"

class EntityListManager
{
private:
	LocalPlayer*    m_localPlayer;
	Names*			m_NamesList;
	PlayersInfo*	m_pInfoList;
	Coords*			m_coordsInfoList;
	EntityList**	m_EntityList;
	int*			m_localPlayerIDPtr;
	int*			m_maxPlayers;
public:
	EntityListManager(uintptr_t hlAddr);
	~EntityListManager();
	PlayerInfoObj* GetPlayerInfoByID(int id);
	Player* GetPlayerByID(int id);
	bool	isValid(int ent);
	Player* GetLocalPlayer();
	int		GetTeam(int id);
	bool	isTeam(int id_a, int id_b);
	int		GetLocalPlayerID() { return *m_localPlayerIDPtr; };
	int		GetMaxPlayers() { return *m_maxPlayers + 1; };
	bool	isAlive(int id);
	void GetHead(Player* co, Vector3 & result);
	void GetHead_Offsetted(Player* co, Vector3 & result);
	void GetFoot(Player* co, Vector3 & result);
};

extern EntityListManager* entityList;

