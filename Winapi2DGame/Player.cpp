#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include <iostream>
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"
#include "GlobalVariable.h"
//#include "MemoryTracer.h"

#include "BackBuffer.h"
#include <cassert>
#include "PacketDefine.h"
#include "PacketStructure.h"

#include <iostream>
#include <Windows.h>
#include "LogManager.h"

#include "PacketDefine.h"
#include "PacketStructure.h"
#include <iostream>
#include <Windows.h>
#include <cassert>
#include "SerializeBuffer.h"
#include "CreatePacket.h"

#include <Windows.h>
#include "MyLinkedList.h"
#include "BaseScene.h"
#include <iostream>
#include "BaseObject.h"
#include <unordered_map>
#include "ObjectManager.h"

#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include <iostream>
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"
#include "GlobalVariable.h"
#include "BackBuffer.h"
#include "Sprite.h"
#include "Effect.h"

#include <iostream>
#include <Windows.h>
#include <cassert>
#include "SerializeBuffer.h"

CPlayer::CPlayer(int id, int direction, int x, int y, int hp)
    :
    m_iStatus(1),
    m_PrevStatus(1),
    m_iAni_CurrentFrame(0),
    m_bActionStand(false),
    m_stCurrentSprite(nullptr),
    m_bFirst(true),
    m_iMoveDirection(0),
    m_YSpeed(2),
    m_XSpeed(3),
    //------------------------------------------------------------------
    //서버로부터 받은 생성메시지를 통해 초기화해야 될 멤버 변수.
    IBaseObject(x, y),
    m_ID(id),
    m_iDirection(direction),
    m_iHP(hp),
    //------------------------------------------------------------------
    m_PrevDirection(direction),
    m_bInputSetting(false),
    m_bAttackFlag(false),
    m_DamagedPlayerX(0),
    m_DamagedPlayerY(0),
    m_bDamageFlag(false),
    m_bDelete(false)
{
}

int CPlayer::GetID()
{
    return m_ID;
}

CPlayer::~CPlayer()
{

}

void CPlayer::Render()
{
    
    if (m_stCurrentSprite == nullptr)
    {
        return;
    }
    //----------------------------------------
    // HP 게이지 
    //----------------------------------------
    SINGLETON(CBackBuffer)->DrawSprite(m_iX - 30, m_iY - 120, SINGLETON(CSpriteManager)->GetSprite(IBaseObject::HP_BAR, 0, 0, 0), 0x00ffffff, m_iHP);
    //----------------------------------------
    // 그림자 스프라이트
    //----------------------------------------
    SINGLETON(CBackBuffer)->DrawSprite(m_iX , m_iY, SINGLETON(CSpriteManager)->GetSprite(IBaseObject::SHADOW, 0, 0, 0), 0x00ffffff);                 
    //----------------------------------------
    // 캐릭터 스프라이트 
    //----------------------------------------
    if (g_pPlayer == this)
    {
        SINGLETON(CBackBuffer)->DrawSpriteRed1(m_iX, m_iY, m_stCurrentSprite, 0xffffffff);
    }
    else
    {
        SINGLETON(CBackBuffer)->DrawSprite(m_iX, m_iY, m_stCurrentSprite, 0xffffffff);
    }
    
}

bool CPlayer::Update()
{
    //---------------------------------------------------------
    // GameScene에서 키처리 or 네트워크 처리 --> ActionInput() 호출 -->  상태와 방향,프레임 세팅
    // 단 키입력이나, 네트워크 처리가 없을경우 , ActionInput을 호출하지 않는다.
    //---------------------------------------------------------

    if (m_bDelete || m_iHP<=0)
    {
        return false;
    }

    GenEffect();
    ActionProc();
    

    m_iAni_CurrentFrame = GetSprite();
    NextFrame();
    //---------------------------------------------------------
    // 입력 결과에 해당하는 Sprite 세팅
    //---------------------------------------------------------
    m_stCurrentSprite = SINGLETON(CSpriteManager)->GetSprite(IBaseObject::PLAYER, m_iDirection, m_iStatus, m_iAni_CurrentFrame);
   

    return true;
}

__int32 CPlayer::GetType()
{
    return PLAYER;
}

void CPlayer::ActionInput(__int32 iStatus, __int32 iDirection, __int32 iMoveDirection)
{
    if (g_pPlayer ==this)
    {
        if (m_bActionStand)
        {
            return;
        }
    }
    if (g_pPlayer==this)
    {
        if (m_bFirst)
        {
            m_bFirst = false;
            SetSprite(0,
                SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::PLAYER, iStatus)->m_iFrameMax,
                SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::PLAYER, iStatus)->m_iDelay);
        }
        else if (m_iStatus != iStatus || m_iDirection != iDirection)
        {
            SetSprite(0,
                SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::PLAYER, iStatus)->m_iFrameMax,
                SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::PLAYER, iStatus)->m_iDelay);
        }
    }
    else
    {
        SetSprite(0,
            SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::PLAYER, iStatus)->m_iFrameMax,
            SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::PLAYER, iStatus)->m_iDelay);
    }
    

    if (iDirection == dfPACKET_MOVE_DIR_UU 
       || iDirection == dfPACKET_MOVE_DIR_DD)
    {
        m_iDirection = m_iDirection;
    }
    else if (iDirection == dfPACKET_MOVE_DIR_LL
        || iDirection == dfPACKET_MOVE_DIR_LU
        || iDirection == dfPACKET_MOVE_DIR_LD)
    {
        m_iDirection = dfPACKET_MOVE_DIR_LL;
    }
    else if (iDirection == dfPACKET_MOVE_DIR_RU
        || iDirection == dfPACKET_MOVE_DIR_RR
        || iDirection == dfPACKET_MOVE_DIR_RD)
    {
        m_iDirection = dfPACKET_MOVE_DIR_RR;
    }

    m_iStatus = iStatus;
    m_iMoveDirection = iMoveDirection;
}

void CPlayer::SetXY(int x, int y)
{
    m_iX = x;
    m_iY = y;
}

void CPlayer::SetInputStatus(bool ActionInput)
{
    m_bInputSetting = ActionInput;
}

void CPlayer::SetHP(int damage)
{
    m_iHP = damage;
}

void CPlayer::SetAttackFlag(bool bAttack)
{
    m_bAttackFlag = bAttack;
}

void CPlayer::SetDamageFlag(bool bDamage)
{
    m_bDamageFlag = bDamage;
}

void CPlayer::SetDamagePlayedXY(int x, int y)
{
    m_DamagedPlayerX = x;
    m_DamagedPlayerY = y;
}

void CPlayer::GenEffect()
{
    if (m_bAttackFlag == false)
    {
        return;
    }

    int sizeeffect = sizeof(Effect);

    if (m_iStatus == ATTACK1 || m_iStatus == ATTACK2 || m_iStatus == ATTACK3)
    {
        if (m_iSpriteNow == m_iSpriteMax-2)
        {
            //Effect를 생성시킴
            m_bAttackFlag = false;
            SINGLETON(CLogManager)->PrintConsoleLog(L"GenEffect\n",0);
            SINGLETON(CObjectManager)->AddObject(new Effect(m_DamagedPlayerX, m_DamagedPlayerY));
        }
    }
}

void CPlayer::SetDelete(bool bDelete)
{
    m_bDelete = bDelete;
}

void CPlayer::ActionProc()
{
    //--------------------------------------
    // 몇몇 동작시(공격)의 경우 강제적으로 해당  동작 처리를 완료해야만 한다.
    //--------------------------------------
    switch (m_iStatus)
    {
        //--------------------------------------
        // 공격 동작또는 데미지 동작은 애니메이션이 끝날때까지 강제적으로 애니메이션이 되어야하며,
        // 애니메이션이 끝난 후 기본동작으로 자동으로 돌아가야 한다.
        //--------------------------------------
    case ATTACK1:
    case ATTACK2:
    case ATTACK3:
        m_bActionStand = true;
        if (IsEndFrame())
        {
            m_bInputSetting = false;
            m_bActionStand = false;   
            
            m_iStatus = STAND;
            SetSprite(0,
                SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::PLAYER, m_iStatus)->m_iFrameMax,
                SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::PLAYER, m_iStatus)->m_iDelay);
        }
        if (g_pPlayer == this)
        {
            if (m_PrevStatus == MOVE)
            {

                SerializeBuffer serialBuffer;

                CreateStopPacket(&serialBuffer, m_iDirection, m_iX, m_iY);
                SINGLETON(CLogManager)->PrintConsoleLog(L"m_iDirection:%d // m_iX:%d //m_iX:%d\n",m_iDirection, m_iX, m_iY,0);

                int enQRtn = g_SendRingBuffer.Enqueue(serialBuffer.GetBufferPtr(), serialBuffer.GetDataSize());

            }
            if ((m_iStatus == ATTACK1 && m_iSpriteNow == 0 && m_iDelayCount == 0))
            {
                SerializeBuffer serialBuffer;

                CreateAttack1Packet(&serialBuffer, m_iDirection, m_iX, m_iY);
                SINGLETON(CLogManager)->PrintConsoleLog(L"m_iStatus:%d // direction:%d // m_iX:%d //m_iX:%d\n", m_iStatus, m_iDirection, m_iX, m_iY);

                int enQRtn = g_SendRingBuffer.Enqueue(serialBuffer.GetBufferPtr(), serialBuffer.GetDataSize());

            }
            else if ((m_iStatus == ATTACK2 && m_iSpriteNow == 0 && m_iDelayCount == 0))
            {
                SerializeBuffer serialBuffer;
                CreateAttack2Packet(&serialBuffer, m_iDirection, m_iX, m_iY);
                SINGLETON(CLogManager)->PrintConsoleLog(L"m_iStatus:%d // direction:%d // m_iX:%d //m_iX:%d\n", m_iStatus, m_iDirection, m_iX, m_iY);

                int enQRtn = g_SendRingBuffer.Enqueue(serialBuffer.GetBufferPtr(), serialBuffer.GetDataSize());
            }
            else if ((m_iStatus == ATTACK3 && m_iSpriteNow == 0 && m_iDelayCount == 0))
            {
                SerializeBuffer serialBuffer;
                CreateAttack3Packet(&serialBuffer,m_iDirection, m_iX, m_iY);
                SINGLETON(CLogManager)->PrintConsoleLog(L"m_iStatus:%d // direction:%d // m_iX:%d //m_iX:%d\n", m_iStatus, m_iDirection, m_iX, m_iY);

                int enQRtn = g_SendRingBuffer.Enqueue(serialBuffer.GetBufferPtr(), serialBuffer.GetDataSize());
            }
        }
      
        m_PrevStatus = m_iStatus;

        break;
    default:
        InputActionProc();
        break;
    }

}

void CPlayer::InputActionProc()
{
    //---------------------------------------------------------
    // Input이 세팅되있지않고, 상태가 Stand가 아니면, Stand 애니메이션으로세팅
    // 단, 처음 접속시, m_iStatus 가 stand아닌걸로 초기화해줘야 동작이됨.
    //---------------------------------------------------------
    if (m_bInputSetting == false)
    {
        if (m_iStatus != STAND)
        {
            m_iStatus = STAND;
            SetSprite(0,
                SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::PLAYER, m_iStatus)->m_iFrameMax,
                SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::PLAYER, m_iStatus)->m_iDelay);
        }
    }
    switch (m_iStatus)
    {
    case MOVE:
        if (m_iMoveDirection == dfPACKET_MOVE_DIR_LL)
        {
            m_iX -= m_XSpeed;
        }
        else if (m_iMoveDirection == dfPACKET_MOVE_DIR_LU)
        {
            m_iX -= m_XSpeed;
            m_iY -= m_YSpeed;
        }
        else if (m_iMoveDirection == dfPACKET_MOVE_DIR_RU)
        {
            m_iX += m_XSpeed;
            m_iY -= m_YSpeed;
        }
        else if (m_iMoveDirection == dfPACKET_MOVE_DIR_RR)
        {
            m_iX += m_XSpeed;
        }
        else if (m_iMoveDirection == dfPACKET_MOVE_DIR_RD)
        {
            m_iX += m_XSpeed;
            m_iY += m_YSpeed;
        }
        else if (m_iMoveDirection == dfPACKET_MOVE_DIR_LD)
        {
            m_iX -= m_XSpeed;
            m_iY += m_YSpeed;
        }
        else if (m_iMoveDirection == dfPACKET_MOVE_DIR_UU)
        {
            m_iY -= m_YSpeed;
        }
        else if (m_iMoveDirection == dfPACKET_MOVE_DIR_DD)
        {
            m_iY += m_YSpeed;
        }
        break;
    }

    //----------------------------
    // 화면 이동영역 제한
    //----------------------------
    if (m_iY < RANGE_MOVE_TOP)
    {
        m_iY = RANGE_MOVE_TOP;
        
        if (m_iMoveDirection == dfPACKET_MOVE_DIR_LU)
        {
            m_iX += m_XSpeed;
        }
        if (m_iMoveDirection == dfPACKET_MOVE_DIR_RU)
        {
            m_iX -= m_XSpeed;
        }
    }
    if (m_iY > RANGE_MOVE_BOTTOM)
    {
        m_iY = RANGE_MOVE_BOTTOM;

        if (m_iMoveDirection == dfPACKET_MOVE_DIR_LD)
        {
            m_iX += m_XSpeed;
        }
        if (m_iMoveDirection == dfPACKET_MOVE_DIR_RD)
        {
            m_iX -= m_XSpeed;
        }
    }
    if (m_iX > RANGE_MOVE_RIGHT)
    {
        m_iX = RANGE_MOVE_RIGHT;

        if (m_iMoveDirection == dfPACKET_MOVE_DIR_RU)
        {
            m_iY += m_YSpeed;
        }
        if (m_iMoveDirection == dfPACKET_MOVE_DIR_RD)
        {
            m_iY -= m_YSpeed;
        }
    }
    if (m_iX < RANGE_MOVE_LEFT)
    {
        m_iX = RANGE_MOVE_LEFT;
        if (m_iMoveDirection == dfPACKET_MOVE_DIR_LU)
        {
            m_iY += m_YSpeed;
        }
        if (m_iMoveDirection == dfPACKET_MOVE_DIR_LD)
        {
            m_iY -= m_YSpeed;
        }
    }

    //-------------------------------------------------
    // 상태 (Stand--> Move)  or 이전 방향과 현재방향이 다를 때,
    // Move Start Packet을 보낸다
    // 링버퍼에 씀
    //-------------------------------------------------
    if (this == g_pPlayer)
    {
        if (((m_PrevStatus == STAND) && (m_iStatus == MOVE)) || (m_iMoveDirection != m_PrevDirection))
        {
            SerializeBuffer serializeBuffer;
            CreateMoveStartPacket(&serializeBuffer, m_iMoveDirection, m_iX, m_iY);
            int enQRtn = g_SendRingBuffer.Enqueue(serializeBuffer.GetBufferPtr(), serializeBuffer.GetDataSize());
        }
        else if (m_PrevStatus == MOVE && m_iStatus == STAND)
                //|| m_PrevStatus == ATTACK1 && m_iStatus == STAND
                //|| m_PrevStatus == ATTACK2 && m_iStatus == STAND
                //|| m_PrevStatus == ATTACK3 && m_iStatus == STAND)
        {
            SerializeBuffer serializeBuffer;
            CreateMoveStopPacket(&serializeBuffer, m_iDirection, m_iX, m_iY);
            int enQRtn = g_SendRingBuffer.Enqueue(serializeBuffer.GetBufferPtr(), serializeBuffer.GetDataSize());
        }

   
    }
  
    m_PrevStatus = m_iStatus;
    m_PrevDirection = m_iMoveDirection;
}
