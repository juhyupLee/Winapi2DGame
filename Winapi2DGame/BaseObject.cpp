#include <Windows.h>
#include "BaseScene.h"
#include "BaseObject.h"
#include <iostream>
#include <Windows.h>
#include "LogManager.h"
IBaseObject::IBaseObject(IBaseScene* pScene,int x, int y)
	:
	m_iX(x),
	m_iY(y),
	m_pCurrentScene(pScene),
	m_iSpriteStart(0),
	m_iSpriteMax(0),
	m_iFrameDelay(0),
	m_iSpriteNow(0),
	m_iDelayCount(0),
	m_bEndFrame(false)
{
}

IBaseObject::IBaseObject(IBaseScene* pScene)
	:
	m_iX(0),
	m_iY(0),
	m_pCurrentScene(pScene),
	m_iSpriteStart(0),
	m_iSpriteMax(0),
	m_iFrameDelay(0),
	m_iSpriteNow(0),
	m_iDelayCount(0),
	m_bEndFrame(false)
{
}

IBaseObject::~IBaseObject()
{
}

void IBaseObject::SetScene(IBaseScene* pScene)
{
	m_pCurrentScene = pScene;
}

void IBaseObject::SetSprite(int iSpriteStart, int iSpriteMax, int iFrameDelay)
{
	m_iSpriteNow = iSpriteStart;
	m_iSpriteMax = iSpriteMax;
	m_iFrameDelay = iFrameDelay;

	m_iSpriteNow = iSpriteStart;
	m_iDelayCount = 0;
	m_bEndFrame = false;
}

int IBaseObject::GetSprite(void)
{
	return m_iSpriteNow;
}

bool IBaseObject::IsEndFrame()
{
	return m_bEndFrame;
}

void IBaseObject::NextFrame()
{
	
	if (0 > m_iSpriteStart)
		return;

	//---------------------------------------------------------
	//������ ������ ���� �Ѿ�� ���� ���������� �Ѿ��
	//---------------------------------------------------------
	m_iDelayCount++;

	if (m_iDelayCount >= m_iFrameDelay)
	{
		m_iDelayCount = 0;
		//---------------------------------------------------------
		//�ִϸ��̼� ������ �̵��Ǹ� ó������ ������
		//---------------------------------------------------------
		m_iSpriteNow++;
		
		//�� �ڵ��  m_iSpriteNow�� �ε����̹Ƿ� -1�� ���ش�
		if (m_iSpriteNow > m_iSpriteMax-1)
		{
			m_iSpriteNow = m_iSpriteStart;
			m_bEndFrame = true;
		}
	}
}

__int32 IBaseObject::GetY()
{
	return m_iY;
}

__int32 IBaseObject::GetX()
{
	return m_iX;
}
