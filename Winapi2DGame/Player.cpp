#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"
#include "GlobalVariable.h"
#include "MemoryTracer.h"

#include "BackBuffer.h"
#include <cassert>
#include "PacketDefine.h"
#include "PacketStructure.h"

#include <iostream>
#include <Windows.h>
#include "LogManager.h"

#include "PacketDefine.h"
#include "PacketStructure.h"
#include "CreatePacket.h"

#include <Windows.h>
#include "MyLinkedList.h"
#include "BaseScene.h"
#include "BaseObject.h"
#include <unordered_map>
#include "ObjectManager.h"

#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"
#include "GlobalVariable.h"
#include "BackBuffer.h"
#include "Sprite.h"
#include "Effect.h"

CPlayer::CPlayer(IBaseScene* pScene, int id, int direction, int x, int y, int hp)
    :

    m_iStatus(1),
    m_iAni_CurrentFrame(0),
    m_bActionStand(false),
    m_dwActionCur(0),
    m_stCurrentSprite(nullptr),
    m_rectHitBox{ 0, },
    m_rectCollisionBox{ 0, },
    m_bFirst(true),
    m_iMoveDirection(0),
    m_YSpeed(2),
    m_XSpeed(3),
    //------------------------------------------------------------------
    //서버로부터 받은 생성메시지를 통해 초기화해야 될 멤버 변수.
    IBaseObject(pScene, x, y),
    m_ID(id),
    m_iDirection(direction),
    m_iHP(hp),
    //------------------------------------------------------------------
    m_PrevDirection(direction),
    m_bInputSetting(false),
    m_bAttackFlag(false),
    m_DamagedPlayerX(0),
    m_DamagedPlayerY(0),
    m_bDelete(false)
   
{
    m_stCurrentSprite = SINGLETON(CSpriteManager)->GetSprite(IBaseObject::PLAYER, m_iDirection, STAND, 0);
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

RECT CPlayer::GetHitBox()
{
    return RECT();
}

RECT CPlayer::GetCollisionBox()
{
    return RECT();
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

    if (m_iStatus == ATTACK1 || m_iStatus == ATTACK2 || m_iStatus == ATTACK3)
    {
        if (m_iSpriteNow == m_iSpriteMax-2)
        {
            //Effect를 생성시킴
            m_bAttackFlag = false;
            SINGLETON(CLogManager)->PrintConsoleLog(L"GenEffect\n",0);
            SINGLETON(CObjectManager)->AddObject(CObjectManager::GAME_SCENE, new Effect(m_pCurrentScene, m_DamagedPlayerX, m_DamagedPlayerY));
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
                PacketHeader header;
                Packet_CS_Move_Stop payload;
                CreateStopPacket(&header, &payload, m_iDirection, m_iX, m_iY);
                SINGLETON(CLogManager)->PrintConsoleLog(L"m_iDirection:%d // m_iX:%d //m_iX:%d\n",m_iDirection, m_iX, m_iY,0);

                int enQRtn = g_SendRingBuffer.Enqueue((char*)&header, sizeof(PacketHeader));
                enQRtn = g_SendRingBuffer.Enqueue((char*)&payload, sizeof(Packet_CS_Move_Stop));
            }
            if ((m_iStatus == ATTACK1 && m_iSpriteNow == 0 && m_iDelayCount == 0))
            {
                PacketHeader header;
                Packet_CS_Attack1 payload;
                CreateAttack1Packet(&header, &payload, m_iDirection, m_iX, m_iY);
                SINGLETON(CLogManager)->PrintConsoleLog(L"m_iStatus:%d // direction:%d // m_iX:%d //m_iX:%d\n", m_iStatus, m_iDirection, m_iX, m_iY);

                int enQRtn = g_SendRingBuffer.Enqueue((char*)&header, sizeof(PacketHeader));
                enQRtn = g_SendRingBuffer.Enqueue((char*)&payload, sizeof(Packet_CS_Attack1));
            }
            else if ((m_iStatus == ATTACK2 && m_iSpriteNow == 0 && m_iDelayCount == 0))
            {
                PacketHeader header;
                Packet_CS_Attack2 payload;
                CreateAttack2Packet(&header, &payload, m_iDirection, m_iX, m_iY);
                SINGLETON(CLogManager)->PrintConsoleLog(L"m_iStatus:%d // direction:%d // m_iX:%d //m_iX:%d\n", m_iStatus, m_iDirection, m_iX, m_iY);

                int enQRtn = g_SendRingBuffer.Enqueue((char*)&header, sizeof(PacketHeader));
                enQRtn = g_SendRingBuffer.Enqueue((char*)&payload, sizeof(Packet_CS_Attack2));
            }
            else if ((m_iStatus == ATTACK3 && m_iSpriteNow == 0 && m_iDelayCount == 0))
            {
                PacketHeader header;
                Packet_CS_Attack3 payload;
                CreateAttack3Packet(&header, &payload, m_iDirection, m_iX, m_iY);
                SINGLETON(CLogManager)->PrintConsoleLog(L"m_iStatus:%d // direction:%d // m_iX:%d //m_iX:%d\n", m_iStatus, m_iDirection, m_iX, m_iY);

                int enQRtn = g_SendRingBuffer.Enqueue((char*)&header, sizeof(PacketHeader));
                enQRtn = g_SendRingBuffer.Enqueue((char*)&payload, sizeof(Packet_CS_Attack3));
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
            Packet_CS_Move_Start tempMoveStart;
            PacketHeader tempHeader;

            tempHeader.code = 0x89;
            tempHeader.size = sizeof(Packet_CS_Move_Start);
            tempHeader.type = dfPACKET_CS_MOVE_START;
            int enQRtn = g_SendRingBuffer.Enqueue((char*)&tempHeader, sizeof(PacketHeader));

            tempMoveStart.direction = m_iMoveDirection;
            tempMoveStart.x = m_iX;
            tempMoveStart.y = m_iY;
            enQRtn = g_SendRingBuffer.Enqueue((char*)&tempMoveStart, sizeof(Packet_CS_Move_Start));
        }
        else if (m_PrevStatus == MOVE && m_iStatus == STAND)
                //|| m_PrevStatus == ATTACK1 && m_iStatus == STAND
                //|| m_PrevStatus == ATTACK2 && m_iStatus == STAND
                //|| m_PrevStatus == ATTACK3 && m_iStatus == STAND)
        {
            Packet_CS_Move_Stop tempMoveStop;
            PacketHeader tempHeader;

            tempHeader.code = 0x89;
            tempHeader.size = sizeof(Packet_CS_Move_Stop);
            tempHeader.type = dfPACKET_CS_MOVE_STOP;
            int enQRtn = g_SendRingBuffer.Enqueue((char*)&tempHeader, sizeof(PacketHeader));

            tempMoveStop.direction = m_iDirection;
            tempMoveStop.x = m_iX;
            tempMoveStop.y = m_iY;
            enQRtn = g_SendRingBuffer.Enqueue((char*)&tempMoveStop, sizeof(Packet_CS_Move_Stop));
        }

   
    }
  
    m_PrevStatus = m_iStatus;
    m_PrevDirection = m_iMoveDirection;
}

void CPlayer::UpdateRect()
{
    RECT rectCollisonBox = m_stCurrentSprite->m_rectCollisionBox;
    RECT rectHitBox = m_stCurrentSprite->m_rectHitBox;
    __int32 originX = m_stCurrentSprite->m_iOriginX;
    __int32 originY = m_stCurrentSprite->m_iOriginY;

    int zeroPointX = m_iX - originX;
    int zeroPointY = m_iY - originY;

    int rectLeft = zeroPointX + rectCollisonBox.left;
    int rectTop = zeroPointY + rectCollisonBox.top;
    int rectRight = rectLeft + rectCollisonBox.right - rectCollisonBox.left;
    int rectBottom = rectTop + rectCollisonBox.bottom - rectCollisonBox.top;

    m_rectCollisionBox.left = rectLeft;
    m_rectCollisionBox.top = rectTop;
    m_rectCollisionBox.right = rectRight;
    m_rectCollisionBox.bottom = rectBottom;

    rectLeft = zeroPointX + rectHitBox.left;
    rectTop = zeroPointY + rectHitBox.top;
    rectRight = rectLeft + rectHitBox.right - rectHitBox.left;
    rectBottom = rectTop + rectHitBox.bottom - rectHitBox.top;

    m_rectHitBox.left = rectLeft;
    m_rectHitBox.top = rectTop;
    m_rectHitBox.right = rectRight;
    m_rectHitBox.bottom = rectBottom;

}

