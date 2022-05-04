#pragma once

#include <cstdint>
#include "Vectors.h"


class NamesObj
{
public:
	char m_name[16]; //0x0000
	char pad_0010[572]; //0x0010
}; //Size: 0x024C


class Names
{
public:
	class NamesObj m_Players[32]; //0x0000
}; //Size: 0x4980


class PlayerInfoObj
{
public:
	int16_t m_team; //0x0000
	char pad_0002[16]; //0x0002
	int16_t m_bDead; //0x0012
	char pad_0014[44]; //0x0014
	int32_t m_score; //0x0040
	char pad_0044[36]; //0x0044
}; //Size: 0x0068

class PlayersInfo
{
public:
	class PlayerInfoObj m_PlayerInfoArr[32]; //0x0000
}; //Size: 0x0D00


class LocalPlayer
{
public:
	char pad_0000[140]; //0x0000
	Vector2 m_rot; //0x008C
	char pad_0094[24]; //0x0094
	Vector3 m_pos; //0x00AC
}; //Size: 0x01B8

class CoordsObj
{
public:
	char pad_0000[16]; //0x0000
	int32_t m_nodormanting; //0x0010
	Vector3 m_pos; //0x0014
	char pad_0020[4]; //0x0020
	float m_yaw; //0x0024
}; //Size: 0x0028

class Coords
{
public:
	class CoordsObj m_coordsInfo[32]; //0x0000
}; //Size: 0x0500

class Player
{
public:
	int32_t PlayerId; //0x0000
	int32_t Valid; //0x0004
	char pad_0008[348]; //0x0008
	float TimeTranscurred; //0x0164
	char pad_0168[4]; //0x0168
	Vector3 Pos1; //0x016C
	Vector2 Rot1; //0x0178
	char pad_0180[164]; //0x0180
	int32_t bDuck; //0x0224
	char pad_0228[144]; //0x0228
	float TimeTranscurred2; //0x02B8
	char pad_02BC[4]; //0x02BC
	Vector3 Pos2; //0x02C0
	Vector2 Rot2; //0x02CC
	char pad_02D4[164]; //0x02D4
	int32_t bIsDucked2; //0x0378
	char pad_037C[1996]; //0x037C
	Vector3 Pos3; //0x0B48
	Vector2 Rot3; //0x0B54
	char pad_0B5C[92]; //0x0B5C
}; //Size: 0x0BB8


class EntityList
{
public:
	class Player EntityList[120]; //0x0000
}; //Size: 0x57E40
