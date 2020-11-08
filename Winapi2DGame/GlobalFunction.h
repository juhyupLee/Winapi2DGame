#pragma once


void SendEvent();
void Marshalling();

void CreateMyCharacter(SerializeBuffer* payload);
//void CreateOtherCharacter(SerializeBuffer* payload);
//void MoveStart(SerializeBuffer* payload);
//void MoveStop(SerializeBuffer* payload);
//void Attack1(SerializeBuffer* payload);
//void Attack2(SerializeBuffer* payload);
//void Attack3(SerializeBuffer* payload);
//void Damage(SerializeBuffer* payload);
//void DeleteCharacter(SerializeBuffer* payload);

//void CreateMyCharacter(SerializeBuffer* payload);
void CreateOtherCharacter(SerializeBuffer* payload);
void MoveStart(SerializeBuffer* payload);
void MoveStop(SerializeBuffer* payload);
void Attack1(SerializeBuffer* payload);
void Attack2(SerializeBuffer* payload);
void Attack3(SerializeBuffer* payload);
void Damage(SerializeBuffer* payload);
void DeleteCharacter(SerializeBuffer* payload);
