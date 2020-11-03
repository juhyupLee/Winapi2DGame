#pragma once
#include <Windows.h>
#include <iostream>
#pragma pack(push,1)
struct PacketHeader
{
	BYTE code;
	BYTE size;
	BYTE type;
};
struct Packet_SC_MY_Character
{
	int32_t id;
	BYTE direction;
	int16_t x;
	int16_t y;
	BYTE  hp;
};

struct Packet_SC_Other_Character
{
	int32_t id;
	BYTE direction;
	int16_t x;
	int16_t y;
	BYTE  hp;
};

struct Packet_SC_Delete_Character
{
	int32_t id;
};

struct Packet_CS_Move_Start
{
	BYTE direction;
	int16_t x;
	int16_t y;
};

struct Packet_SC_Move_Start
{
	int32_t id;
	BYTE direction;
	int16_t x;
	int16_t y;
};

struct Packet_CS_Move_Stop
{
	BYTE direction;
	int16_t x;
	int16_t y;
};

struct Packet_SC_Move_Stop
{
	int32_t id;
	BYTE direction;
	int16_t x;
	int16_t y;
};

struct Packet_CS_Attack1
{
	BYTE direction;
	int16_t x;
	int16_t y;
};
struct Packet_SC_Attack1
{
	int32_t id;
	BYTE direction;
	int16_t x;
	int16_t y;
};

struct Packet_CS_Attack2
{
	BYTE direction;
	int16_t x;
	int16_t y;
};
struct Packet_SC_Attack2
{
	int32_t id;
	BYTE direction;
	int16_t x;
	int16_t y;
};

struct Packet_CS_Attack3
{
	BYTE direction;
	int16_t x;
	int16_t y;
};
struct Packet_SC_Attack3
{
	int32_t id;
	BYTE direction;
	int16_t x;
	int16_t y;
};
struct Packet_SC_Damage
{
	int32_t attackID;
	int32_t damageID;
	BYTE damageHP;
};


