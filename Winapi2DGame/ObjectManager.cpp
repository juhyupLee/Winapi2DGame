#include <Windows.h>
#include "MyLinkedList.h"
#include "BaseScene.h"
#include "BaseObject.h"
#include <unordered_map>
#include "ObjectManager.h"
#include "MemoryTracer.h"

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


