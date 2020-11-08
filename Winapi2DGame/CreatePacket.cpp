#include "PacketDefine.h"
//#include "PacketStructure.h"
#include <iostream>
#include <Windows.h>
#include <cassert>
#include "SerializeBuffer.h"
#include "CreatePacket.h"



void CreateMoveStartPacket(SerializeBuffer* serializeBuffer, BYTE direction, int16_t x, int16_t y)
{
	BYTE code;
	BYTE size;
	BYTE type;

	code = CODE;
	size = sizeof(direction) + sizeof(x) + sizeof(y);
	type = dfPACKET_CS_MOVE_START;

	*serializeBuffer << code << size << type;
	*serializeBuffer << direction << x << y;

}

void CreateMoveStopPacket(SerializeBuffer* serializeBuffer, BYTE direction, int16_t x, int16_t y)
{
	BYTE code;
	BYTE size;
	BYTE type;

	code = CODE;
	size = sizeof(direction) + sizeof(x) + sizeof(y);
	type = dfPACKET_CS_MOVE_STOP;

	*serializeBuffer << code << size << type;
	*serializeBuffer << direction << x << y;
}

void CreateAttack1Packet(SerializeBuffer* serializeBuffer, BYTE direction, int16_t x, int16_t y)
{
	BYTE code;
	BYTE size;
	BYTE type;

	code = CODE;
	size = sizeof(direction) + sizeof(x) + sizeof(y);
	type = dfPACKET_CS_ATTACK1;

	*serializeBuffer << code << size << type;
	*serializeBuffer << direction << x << y;
}

void CreateAttack2Packet(SerializeBuffer* serializeBuffer, BYTE direction, int16_t x, int16_t y)
{
	BYTE code;
	BYTE size;
	BYTE type;

	code = CODE;
	size = sizeof(direction) + sizeof(x) + sizeof(y);
	type = dfPACKET_CS_ATTACK2;

	*serializeBuffer << code << size << type;
	*serializeBuffer << direction << x << y;
}

void CreateAttack3Packet(SerializeBuffer* serializeBuffer, BYTE direction, int16_t x, int16_t y)
{
	BYTE code;
	BYTE size;
	BYTE type;

	code = CODE;
	size = sizeof(direction) + sizeof(x) + sizeof(y);
	type = dfPACKET_CS_ATTACK3;

	*serializeBuffer << code << size << type;
	*serializeBuffer << direction << x << y;
}

void CreateStopPacket(SerializeBuffer* serializeBuffer, BYTE direction, int16_t x, int16_t y)
{
	BYTE code;
	BYTE size;
	BYTE type;

	code = CODE;
	size = sizeof(direction) + sizeof(x)+ sizeof(y);
	type = dfPACKET_CS_MOVE_STOP;

	*serializeBuffer << code << size << type;
	*serializeBuffer << direction << x << y;

}


