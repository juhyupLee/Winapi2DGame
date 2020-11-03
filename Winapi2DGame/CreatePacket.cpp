#include "PacketDefine.h"
#include "PacketStructure.h"
#include "CreatePacket.h"

void CreateAttack1Packet(PacketHeader* header, Packet_CS_Attack1* payload, BYTE direction, int x, int y)
{
	header->code = CODE;
	header->size = sizeof(Packet_CS_Attack1);
	header->type = dfPACKET_CS_ATTACK1;

	payload->direction = direction;
	payload->x = x;
	payload->y = y;
}

void CreateAttack2Packet(PacketHeader* header, Packet_CS_Attack2* payload, BYTE direction, int x, int y)
{
	header->code = CODE;
	header->size = sizeof(Packet_CS_Attack2);
	header->type = dfPACKET_CS_ATTACK2;

	payload->direction = direction;
	payload->x = x;
	payload->y = y;
}

void CreateAttack3Packet(PacketHeader* header, Packet_CS_Attack3* payload, BYTE direction, int x, int y)
{
	header->code = CODE;
	header->size = sizeof(Packet_CS_Attack3);
	header->type = dfPACKET_CS_ATTACK3;

	payload->direction = direction;
	payload->x = x;
	payload->y = y;
}

void CreateStopPacket(PacketHeader* header, Packet_CS_Move_Stop* payload, BYTE direction, int x, int y)
{
	header->code = CODE;
	header->size = sizeof(Packet_CS_Move_Stop);
	header->type = dfPACKET_CS_MOVE_STOP;

	payload->direction = direction;
	payload->x = x;
	payload->y = y;
}


