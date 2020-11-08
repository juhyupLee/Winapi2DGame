#include <iostream>
#include <Windows.h>
#include <cassert>
#include "SerializeBuffer.h"
#include "GlobalFunction.h"
#include <iostream>
#include <Windows.h>
#include "LogManager.h"
#include "PacketDefine.h"
//#include "PacketStructure.h"
#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include <iostream>
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"
#include <Windows.h>
#include "MyLinkedList.h"
#include "BaseScene.h"
#include <iostream>
#include "BaseObject.h"
#include <unordered_map>
#include "ObjectManager.h"
#include "GlobalVariable.h"



HANDLE g_Handle;

void Marshalling()
{
	char tempBuffer[1000];

	char header[HEADER_SIZE];

	BYTE code;
	BYTE payloadSize;
	BYTE type;

	while (true)
	{
		if (g_RecvRingBuffer.GetUsedSize() < HEADER_SIZE)
			break;
	
		int peekRtn = g_RecvRingBuffer.Peek((char*)&header, HEADER_SIZE);

		SerializeBuffer headerSerializeBuffer;
		headerSerializeBuffer.PutData((char*)&header, HEADER_SIZE);

		headerSerializeBuffer >> code >> payloadSize >> type;

		if (code != CODE)
		{
			SINGLETON(CLogManager)->PrintConsoleLog(L"Packet Code Error!!", 0);
			break;
		}
		if (g_RecvRingBuffer.GetUsedSize() < HEADER_SIZE + payloadSize)
			break;

		g_RecvRingBuffer.MoveFront(HEADER_SIZE);

		int deQRtn = g_RecvRingBuffer.Dequeue(tempBuffer, payloadSize);

		if (deQRtn != payloadSize)
		{
			SINGLETON(CLogManager)->PrintConsoleLog(L"Dequeue Fail", 0);
			break;
		}

		SerializeBuffer payloadBuffer;
		payloadBuffer.PutData(tempBuffer, payloadSize);

		switch (type)
		{
		case dfPACKET_SC_CREATE_MY_CHARACTER:
			CreateMyCharacter(&payloadBuffer);
			break;
		case dfPACKET_SC_CREATE_OTHER_CHARACTER:
			CreateOtherCharacter(&payloadBuffer);
			break;
		case dfPACKET_SC_DELETE_CHARACTER:
			DeleteCharacter(&payloadBuffer);
			break;
		case dfPACKET_SC_MOVE_START:
			MoveStart(&payloadBuffer);
			break;
		case dfPACKET_SC_MOVE_STOP:
			MoveStop(&payloadBuffer);
			break;
		case dfPACKET_SC_ATTACK1:
			Attack1(&payloadBuffer);
			break;
		case dfPACKET_SC_ATTACK2:
			Attack2(&payloadBuffer);
			break;
		case dfPACKET_SC_ATTACK3:
			Attack3(&payloadBuffer);
			break;
		case dfPACKET_SC_DAMAGE:
			Damage(&payloadBuffer);
			break;
		}
	}
}

void SendEvent()
{
    char tempBuffer[10000];

    while (g_SendRingBuffer.GetUsedSize() != 0)
    {
        int directDeQSize = g_SendRingBuffer.GetDirectDequeueSize();
        int peekRtn = g_SendRingBuffer.Peek(tempBuffer, directDeQSize);
        int sendRtn = send(g_Socket, tempBuffer, peekRtn, 0);
        if (sendRtn <= 0)
        {
#ifdef _DEBUG
            SINGLETON(CLogManager)->PrintConsoleLog(L"Send Error:%d\n", WSAGetLastError());
			SINGLETON(CLogManager)->PrintLog(L"ErrorLog.txt", L"Send() error", WSAGetLastError());
#endif
            return;
        }
        g_SendRingBuffer.MoveFront(sendRtn);
    }
}
void CreateMyCharacter(SerializeBuffer* payload)
{
	int32_t id;
	BYTE direction;
	int16_t x;
	int16_t y;
	BYTE hp;

	(*payload)>> id >> direction >> x >> y >> hp;

	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create My Character(SC)] ID:%d\n", id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create My Character(SC)] Direction:%d\n", direction);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create My Character(SC)] X:%d\n", x);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create My Character(SC)] Y:%d\n", y);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create My Character(SC)] HP:%d\n", hp);

	g_pPlayer = new CPlayer(id, direction, x, y, hp);

	SINGLETON(CObjectManager)->AddObject(g_pPlayer);
}

void CreateOtherCharacter(SerializeBuffer* payload)
{
	int32_t id;
	BYTE direction;
	int16_t x;
	int16_t y;
	BYTE hp;

	(*payload) >> id >> direction >> x >> y >> hp;

	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create Other Character(SC)] ID:%d\n", id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create Other Character(SC)] Direction:%d\n", direction);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create Other Character(SC)] X:%d\n", x);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create Other Character(SC)] Y:%d\n", y);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create Other Character(SC)] HP:%d\n", hp);

	CPlayer* otherPlayer;	
	otherPlayer = new CPlayer(id, direction, x, y, hp);
	SINGLETON(CObjectManager)->AddObject(otherPlayer);

}

void MoveStart(SerializeBuffer* payload)
{
	int32_t id;
	BYTE direction;
	int16_t x;
	int16_t y;

	*payload >> id >> direction >> x >> y;

	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Start(SC)] ID:%d\n", id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Start(SC)] Direction:%d\n", direction);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Start(SC)] X:%d\n", x);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Start(SC)] Y:%d\n", y);

	SINGLETON(CObjectManager)->OtherPlayerActionInput(id, CPlayer::MOVE, direction, x, y, true);
}

void MoveStop(SerializeBuffer* payload)
{
	int32_t id;
	BYTE direction;
	int16_t x;
	int16_t y;

	*payload >> id >> direction >> x >> y;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Stop(SC)] ID:%d\n", id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Stop(SC)] Direction:%d\n", direction);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Stop(SC)] X:%d\n", x);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Stop(SC)] Y:%d\n", y);

	SINGLETON(CObjectManager)->OtherPlayerActionInput(id, CPlayer::STAND, direction, x, y, false);

}

void Attack1(SerializeBuffer* payload)
{
	int32_t id;
	BYTE direction;
	int16_t x;
	int16_t y;

	*payload >> id >> direction >> x >> y;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Attack1(SC)] ID:%d // Direction:%d // X:%d // Y:%d\n", id, direction, x, y);
	SINGLETON(CObjectManager)->OtherPlayerActionInput(id, CPlayer::ATTACK1, direction, x, y, true);
}

void Attack2(SerializeBuffer* payload)
{
	int32_t id;
	BYTE direction;
	int16_t x;
	int16_t y;

	*payload >> id >> direction >> x >> y;

	SINGLETON(CLogManager)->PrintConsoleLog(L"[Attack2 (SC)] ID:%d // Direction:%d // X:%d // Y:%d\n", id, direction, x, y);
	SINGLETON(CObjectManager)->OtherPlayerActionInput(id, CPlayer::ATTACK2, direction, x, y, true);
}

void Attack3(SerializeBuffer* payload)
{
	int32_t id;
	BYTE direction;
	int16_t x;
	int16_t y;

	*payload >> id >> direction >> x >> y;

	SINGLETON(CLogManager)->PrintConsoleLog(L"[Attack3 (SC)] ID:%d // Direction:%d // X:%d // Y:%d\n", id, direction, x, y);
	SINGLETON(CObjectManager)->OtherPlayerActionInput(id, CPlayer::ATTACK3, direction, x, y, true);
}

void Damage(SerializeBuffer* payload)
{
	int32_t attackID;
	int32_t damageID;
	BYTE damageHP;

	*payload >> attackID >> damageID >> damageHP;

	SINGLETON(CLogManager)->PrintConsoleLog(L"[Damage (SC)] Attack ID:%d // Damage ID:%d // Damage HP:%d\n", attackID, damageID, damageHP, 0);
	SINGLETON(CObjectManager)->AttackCheck(attackID, damageID, damageHP);
}

void DeleteCharacter(SerializeBuffer* payload)
{
	int32_t id;
	(*payload) >> id;

	SINGLETON(CLogManager)->PrintConsoleLog(L"[Delete (SC)]  ID:%d \n", id);
	SINGLETON(CLogManager)->PrintLog(L"[Delete (SC)]  ID", id);
	SINGLETON(CObjectManager)->DeletePlayer(id);
}