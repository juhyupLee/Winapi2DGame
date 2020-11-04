#pragma once


void SendEvent();
void Marshalling();

void CreateMyCharacter(char* payload);
void CreateOtherCharacter(char* payload);
void MoveStart(char* payload);
void MoveStop(char* payload);
void Attack1(char* payload);
void Attack2(char* payload);
void Attack3(char* payload);
void Damage(char* payload);
void DeleteCharacter(char* payload);

