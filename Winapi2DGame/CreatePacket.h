#pragma once

void CreateAttack1Packet(PacketHeader* header, Packet_CS_Attack1* payload, BYTE direction, int x, int y);

void CreateAttack2Packet(PacketHeader* header, Packet_CS_Attack2* payload, BYTE direction, int x, int y);

void CreateAttack3Packet(PacketHeader* header, Packet_CS_Attack3* payload, BYTE direction, int x, int y);

void CreateStopPacket(PacketHeader* header, Packet_CS_Move_Stop* payload, BYTE direction, int x, int y);