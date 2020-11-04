#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"
#include "GlobalVariable.h"
#include "TimeManager.h"
#include "LogManager.h"
#include "SceneManager.h"

#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"
#include "MyLinkedList.h"
#include <unordered_map>
#include "GameScene.h"
//#include "MemoryTracer.h"
CSceneManager::CSceneManager()
    :
    m_bEnterFrame(false),
    m_bRenderSkip(false),
    m_SkipCount(0),
    m_NonSkipCount(0),
    m_bEnterRender(false),
    m_currentScene(nullptr)
{
}
CSceneManager::~CSceneManager()
{
    delete m_currentScene;
}

CSceneManager* CSceneManager::GetInstance()
{
    static CSceneManager sceneManager;

    return &sceneManager;
}

bool CSceneManager::Run()
{
    if (nullptr == m_currentScene)
    {
        return false;
    }

    //---------------------------
    // �� ���� ���� 
    //---------------------------
   
    m_currentScene->KeyProcess();
    m_currentScene->Update();

    //---------------------------
    // ���� ������  �ð� üũ
    //---------------------------
    SINGLETON(CTimeManager)->LogicTimeStart();
    if (SINGLETON(CTimeManager)->IsRenderSkip())
    {
        m_bRenderSkip = true;
        m_SkipCount++;
    }
    else
    {
        m_bRenderSkip = false;
        m_NonSkipCount++;
    }
    //---------------------------
    // ���� ������ �ð��� ���� ���, Sleep
    //---------------------------
    Sleep((DWORD)SINGLETON(CTimeManager)->GetSleepTime());

    //---------------------------
    // ���� ��ŵ  or ����
    //---------------------------
    if (!m_bRenderSkip)
    {
        m_currentScene->Render();
    }
    //---------------------------
    // �α�
    //---------------------------
    //SINGLETON(CLogManager)->PrintConsoleLog(L"Skip:%d // NonSkip:%d\n", m_SkipCount, m_NonSkipCount);
    //SINGLETON(CLogManager)->PrintConsoleLog(L"logicTime:%d\n", SINGLETON(CTimeManager)->GetLogicTime());
    //SINGLETON(CLogManager)->PrintConsoleLog(L"OverTime:%d\n", SINGLETON(CTimeManager)->GetSpareTime());
    
    //system("cls");
    return true;
}

bool CSceneManager::ChangeScene(__int32 scene)
{
    switch (scene)
    {
    case GAME_SCENE:
        m_currentScene = new CGameScene();
        break;
    }

    return true;
}
