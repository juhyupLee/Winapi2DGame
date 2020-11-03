#include <Windows.h>
#include "MyLinkedList.h"
#include "BaseScene.h"
#include "BaseObject.h"
#include <unordered_map>
#include "ObjectManager.h"
#include "MemoryTracer.h"

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
    for (int i = 0; i < SCENE_MAX; ++i)
    {
        ReleaseObject(i);
    }
   
}
CObjectManager* CObjectManager::GetInstance()
{
    static CObjectManager objectManager;
    return &objectManager;
}

void CObjectManager::OtherPlayerActionInput(int scene, int id,int state, BYTE direction, int x, int y,bool inputStatus)
{
    auto iter = m_ObjectList[scene].begin();

    for (; iter != m_ObjectList[scene].end();++iter)
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

void CObjectManager::AttackCheck(int scene, int attackID, int damageID, int damageHP)
{
    auto iter = m_ObjectList[scene].begin();
    int damagePlayexX = 0;
    int damagePlayexY = 0;

    for (; iter != m_ObjectList[scene].end(); ++iter)
    {
        if ((*iter)->GetType() == IBaseObject::PLAYER)
        {
            if (((CPlayer*)(*iter))->GetID() == damageID)
            {
                ((CPlayer*)(*iter))->SetHP(damageHP);
                damagePlayexX = ((CPlayer*)(*iter))->GetX();
                damagePlayexY = ((CPlayer*)(*iter))->GetY();
            }
        }

    }

    iter = m_ObjectList[scene].begin();

    for (; iter != m_ObjectList[scene].end(); ++iter)
    {
        if ((*iter)->GetType() == IBaseObject::PLAYER)
        {
            if (((CPlayer*)(*iter))->GetID() == attackID)
            {
                ((CPlayer*)(*iter))->SetAttackFlag(true);
                ((CPlayer*)(*iter))->SetDamagePlayedXY(damagePlayexX, damagePlayexY);
            }
        }

    }
  
}

void CObjectManager::DeletePlayer(int scene, int id)
{
    auto iter = m_ObjectList[scene].begin();


    for (; iter != m_ObjectList[scene].end(); ++iter)
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

void CObjectManager::Update(int scene)
{
    auto iter = m_ObjectList[scene].begin();

    for (; iter != m_ObjectList[scene].end();)
    {
        CollisionCheck(scene, *iter);

        if (false == (*iter)->Update())
        {
            delete* iter;
            iter = m_ObjectList[scene].erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}
void CObjectManager::Render(int scene)
{

    m_ObjectList[scene].sort(temp);
    auto iter = m_ObjectList[scene].begin();

    for (; iter != m_ObjectList[scene].end();++iter)
    {
        (*iter)->Render();
    }
}

void CObjectManager::AddObject(__int32 sceneNum, IBaseObject* object)
{
    m_ObjectList[sceneNum].push_back(object);
}
void CObjectManager::AddCollisionObject(int key, int value)
{
    m_CollisionObjectMap.insert(std::make_pair(key, value));   //>Key : 충돌대상1, value : 충돌대상2
}

void CObjectManager::ReleaseObject(__int32 scene)
{
    auto iter = m_ObjectList[scene].begin();

    for (; iter != m_ObjectList[scene].end();)
    {
        delete (*iter);
        iter = m_ObjectList[scene].erase(iter);
    }
}

void CObjectManager::CollisionCheck(int scene, IBaseObject* object)
{
    auto mapIter = m_CollisionObjectMap.find(object->GetType());

    if (mapIter == m_CollisionObjectMap.end())
    {
        return;
    }
    auto listIter = m_ObjectList[scene].begin();

    for (; listIter != m_ObjectList[scene].end(); ++listIter)
    {
        if ((*mapIter).second == (*listIter)->GetType())
        {
            //충돌체크
            //충돌체크 트루면, 충돌알림

        }
    }

}


