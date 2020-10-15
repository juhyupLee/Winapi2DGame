#include <iostream>
#include <Windows.h>
#include "SpriteDib.h"

CSpriteDib::CSpriteDib(int maxSprite, DWORD colorKey)
{
    m_MaxSprite = maxSprite;
    m_ColorKey = colorKey;

    m_Sprite = NULL;

    //---------------------
    //�ִ� �о�� ������ŭ �̸� �Ҵ�޴´�.
    //-----------------------------

    m_Sprite = new st_SPRITE[m_MaxSprite];
    memset(m_Sprite, 0, sizeof(st_SPRITE) * m_MaxSprite);

}

CSpriteDib::~CSpriteDib()
{
   
    //��ü�� ���鼭 ��� ������
    for (int count = 0; count < m_MaxSprite; ++count)
    {
        ReleaseSprite(count);
    }
}
BOOL CSpriteDib::LoadDibSprite(__int32 spriteIndex, WCHAR* fileName, __int32 centerPointX, __int32 centerPointY)
{
    HANDLE hFile;
    DWORD read;
    int pitch;
    int imageSize;

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    //--------------------------------
    //�ϴ� ������ ����
    //-------------------------------

    hFile = CreateFile(fileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        return FALSE;
    }
    ReleaseSprite(spriteIndex); //  �ѹ� Release �ϰ� �ε��� �ϳ�����??

    ReadFile(hFile, &fileHeader, sizeof(BITMAPINFOHEADER), &read, NULL);
    if (0x4d42 == fileHeader.bfType)
    {
        //-----------------------------------
        //��������� �о ���� & 32��Ʈ Ȯ��
        //-----------------------------
        ReadFile(hFile, &infoHeader, sizeof(BITMAPINFOHEADER), &read, NULL);
        if (32 == infoHeader.biBitCount)
        {
            //���� ������ ��ġ���� ���Ѵ�.
            pitch = ((infoHeader.biWidth * 4) + 3) & ~3;
            //������� ��ȣ������ �Ⱦ����µ�, ����� �´°���?
            //-----------------------------------------
            //��������Ʈ ����ü�� ũ�� ����
            //-----------------------------------------

            m_Sprite[spriteIndex].m_Width = infoHeader.biWidth;
            m_Sprite[spriteIndex].m_Height = infoHeader.biHeight;
            m_Sprite[spriteIndex].m_Pitch = pitch;

            //�̹����� ���� ��ü ũ�⸦ ���ϰ�, �޸��Ҵ�
            imageSize = pitch * infoHeader.biHeight;
            m_Sprite[spriteIndex].m_Image = new BYTE[imageSize];

            //---------------------------------------
            //�̹��� �κ��� ��������Ʈ ���۷� �о�´�
            //DIB�� �������� �����Ƿ� �̸� �ٽ� ������.
            BYTE* tempBuffer = new BYTE[imageSize];
            BYTE* spriteTemp = m_Sprite[spriteIndex].m_Image;
            BYTE* turnTemp;

            ReadFile(hFile, tempBuffer, imageSize, &read, NULL);
            //-----------------------
            //���� ���� ������
            //-----------------------
            turnTemp = tempBuffer + pitch * (infoHeader.biHeight - 1);
            
            for (int count = 0; count < infoHeader.biHeight; ++count)
            {
                memcpy(spriteTemp, turnTemp, pitch);
                spriteTemp += pitch;
                turnTemp -= pitch;
            }
            delete[] tempBuffer;

            m_Sprite[spriteIndex].m_CenterPointX = centerPointX;
            m_Sprite[spriteIndex].m_CenterPointY = centerPointY;
            CloseHandle(hFile);
            return TRUE;

        }

    }
    CloseHandle(hFile);
    return FALSE;
}

void CSpriteDib::ReleaseSprite(__int32 spriteIndex)
{
    //-----------------------
    //�ִ� �Ҵ�� ��������Ʈ�� �Ѿ�� �ȵſ�~
    //-----------------------
    if (m_MaxSprite <= spriteIndex)
    {
        return;
    }
    if (NULL != m_Sprite[spriteIndex].m_Image)
    {
        //--------------------------
        //���� �� �ʱ�ȭ
        //--------------------------
        delete[] m_Sprite[spriteIndex].m_Image;
        memset(&m_Sprite[spriteIndex], 0, sizeof(st_SPRITE));
    }
}

void CSpriteDib::DrawSprite(__int32 spriteIndex, __int32 drawX, __int32 drawY, BYTE* dest, __int32 destWidth, __int32 destHeight, __int32 destPitch, __int32 drawLen)
{
    //--------------------------------
    //�ִ� ��������Ʈ ������ �ʰ��ϰų�, �ε�����ʴ� ��������Ʈ��� ����
    //--------------------------------
    if (spriteIndex >= m_MaxSprite)
    {
        return;
    }

    if (NULL == m_Sprite[spriteIndex].m_Image)
    {
        return;
    }

    st_SPRITE* sprite = &m_Sprite[spriteIndex];

    //----------------------------------------
    //��������Ʈ ����� ���� ������ ����
    //----------------------------------------

    __int32 spriteWidth = sprite->m_Width;
    __int32 spriteHeight = sprite->m_Height;
    __int32 countX= 0;
    __int32 countY=0;

    //-------------------------------
    // ��� ���� ����
    //-------------------------------
    spriteWidth = spriteWidth * drawLen / 100;

    DWORD* pdwDest = (DWORD*)dest;
    DWORD* dwpSprite = (DWORD*)(sprite->m_Image);

    //---------------------------------------------
    //��ܿ� ���� ��������Ʈ ��� ��ġ ���(��� Ŭ����)
    //---------------------------------------------
    if(0>drawY)
    {
        spriteHeight = spriteHeight - (-drawY);
        dwpSprite = (DWORD*)(sprite->m_Image + sprite->m_Pitch * (drawY));
        //--------------------
        //������ ©���� ����̹Ƿ� ��������Ʈ ���� ��ġ�� �Ʒ��� �����ش�
        //-------------------

        drawY = 0;
    }
    //--------------------------------------
     //�ϴܿ� ������ ��� (�ϴ�Ŭ����)
     //---------------------------------------
    if (destHeight < drawY + sprite->m_Height)
    {
        spriteHeight -= ((drawY + sprite->m_Height) - destHeight);
    }
    //--------------------------------
    //���� ��� ��ġ ��� (���� Ŭ����)
    //--------------------------------
    if (0 > drawX)
    {
        //�̺κ� �� ���ذ� �ȵǳ�????
        //spriteWidth ���� �� drawX��ŭ ������?
        spriteWidth = spriteWidth - (-drawX);
        dwpSprite = dwpSprite + (-drawX);
        //-------------------------------------------------------
        //������ ���� �߸��Ƿ� ��� ���� ��ġ�� ���������� �δ�.
        //-------------------------------------------------------
        drawX = 0;

    }

    //--------------------------------
    //������ ��� ��ġ ��� (���� Ŭ����)
    //--------------------------------
    if (destWidth < drawX + sprite->m_Width)
    {
        spriteWidth -= ((drawX + sprite->m_Width) - destHeight);
    }

    //--------------------------------
    //���� �׸��� ���ٸ� �����Ѵ�
    //--------------------------------
    if (spriteWidth <= 0 || spriteHeight <= 0)
        return;

    //---------------------------------
    //��ü ũ�⸦ ���鼭 �����Ѵ�
    //---------------------------------
    for (countY = 0; spriteHeight > countY; countY++)
    {
        memcpy(pdwDest, dwpSprite, spriteWidth * 4);
        //---------------------
        //���� �ٷ� �̵�, ȭ��� ��������Ʈ ���
        //------------------------------------
        pdwDest = (DWORD*)((BYTE*)pdwDest + destPitch);
        dwpSprite = (DWORD*)((BYTE*)dwpSprite + sprite->m_Pitch);
    }
}


void CSpriteDib::DrawSprite50(__int32 spriteIndex, __int32 drawX, __int32 drawY, BYTE* dest, __int32 destWidth, __int32 destHeight, __int32 destPitch)
{
}

void CSpriteDib::DrawSpriteRed(__int32 spriteIndex, __int32 drawX, __int32 drawY, BYTE* dest, __int32 destWidth, __int32 destHeight, __int32 destPitch)
{
}

void CSpriteDib::DrawImage(int spriteIndex, int drawX, int drawY, BYTE* dest, __int32 destWidth, __int32 destHeight, __int32 destPitch, int drawLen)
{
}
