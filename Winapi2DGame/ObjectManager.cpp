#include <Windows.h>
#include "MyLinkedList.h"
#include "BaseScene.h"
#include "BaseObject.h"
#include <unordered_map>
#include "ObjectManager.h"
//#include "MemoryTracer.h"

#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"

#include <iostream>
#include <Windows.h>
#include "LogManager.h"

bool temp(IBaseObject* temp1, IBaseObject* temp2)
{
    return temp1->GetY() > temp2->GetY();
}

CObjectManager::CObjectManager()
{

}
CObjectManager::~CObjectManager()
{
    ReleaseObject();
}
CObjectManager* CObjectManager::GetInstance()
{
    static CObjectManager objectManager;
    return &objectManager;
}

void CObjectManager::OtherPlayerActionInput(int id, int state, BYTE direction, int x, int y,bool inputStatus)
{
    auto iter = m_ObjectList.begin();

    for (; iter != m_ObjectList.end();++iter)
    {
        if ((*iter)->GetType() == IBaseObject::PLAYER)
        {
            if (((CPlayer*)(*iter))->GetID() == id)
            {
                ((CPlayer*)(*iter))->ActionInput(state, direction, direction);
                ((CPlayer*)(*iter))->SetXY(x, y);
                ((CPlayer*)(*iter))->SetInputStatus(inputStatus);                
                break;
            }
        }

    }
}

void CObjectManager::AttackCheck(int attackID, int damageID, int damageHP)
{
    auto iter = m_ObjectList.begin();
    int damagePlayexX = 0;
    int damagePlayexY = 0;

    for (; iter != m_ObjectList.end(); ++iter)
    {
        if ((*iter)->GetType() == IBaseObject::PLAYER)
        {
            if (((CPlayer*)(*iter))->GetID() == damageID)
            {
                ((CPlayer*)(*iter))->SetHP(damageHP);
                damagePlayexX = ((CPlayer*)(*iter))->GetX();
                damagePlayexY = ((CPlayer*)(*iter))->GetY();
                break;
            }
        }

    }

    iter = m_ObjectList.begin();

    for (; iter != m_ObjectList.end(); ++iter)
    {
        if ((*iter)->GetType() == IBaseObject::PLAYER)
        {
            if (((CPlayer*)(*iter))->GetID() == attackID)
            {
                ((CPlayer*)(*iter))->SetAttackFlag(true);
                ((CPlayer*)(*iter))->SetDamagePlayedXY(damagePlayexX, damagePlayexY);
                break;
            }
        }
    }
  
}

void CObjectManager::DeletePlayer(int id)
{
    auto iter = m_ObjectList.begin();

    for (; iter != m_ObjectList.end(); ++iter)
    {
        if ((*iter)->GetType() == IBaseObject::PLAYER)
        {
            if (((CPlayer*)(*iter))->GetID() == id)
            {
                ((CPlayer*)(*iter))->SetDelete(true);
            }
        }

    }
}

void CObjectManager::Update()
{
    auto iter = m_ObjectList.begin();

    for (; iter != m_ObjectList.end();)
    {
        if (false == (*iter)->Update())
        {
            delete* iter;
            iter = m_ObjectList.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}
void CObjectManager::Render()
{
    m_ObjectList.sort(temp);
    
    auto iter = m_ObjectList.begin();
    int i = 1;

    for (; iter != m_ObjectList.end();++iter)
    {
        (*iter)->Render();
       // CLogManager::GetInstance()->PrintConsoleLog(L"Y Sort Log Y[%d]:%d\n",i, (*iter)->GetY());
        //++i;
    }
}

void CObjectManager::AddObject(IBaseObject* object)
{
    m_ObjectList.push_back(object);
}


void CObjectManager::ReleaseObject()
{
    auto iter = m_ObjectList.begin();

    for (; iter != m_ObjectList.end();)
    {
        delete (*iter);
        iter = m_ObjectList.erase(iter);
    }
}



