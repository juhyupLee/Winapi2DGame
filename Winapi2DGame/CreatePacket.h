#pragma once

void CreateMoveStartPacket(SerializeBuffer* serializeBuffer, BYTE direction, int16_t x, int16_t y);
void CreateMoveStopPacket(SerializeBuffer* serializeBuffer, BYTE direction, int16_t x, int16_t y);

void CreateAttack1Packet(SerializeBuffer* serializeBuffer, BYTE direction, int16_t x, int16_t y);

void CreateAttack2Packet(SerializeBuffer* serializeBuffer, BYTE direction, int16_t x, int16_t y);

void CreateAttack3Packet(SerializeBuffer* serializeBuffer, BYTE direction, int16_t x, int16_t y);

void CreateStopPacket(SerializeBuffer* serializeBuffer, BYTE direction, int16_t x, int16_t y);