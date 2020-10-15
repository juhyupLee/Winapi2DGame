#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"
#include "GlobalVariable.h"
#include "BackBuffer.h"
#include "MemoryTracer.h"

CBackBuffer* CBackBuffer::GetInstance()
{
    static CBackBuffer backBuffer;
    return &backBuffer;
}

BYTE* CBackBuffer::GetBackBuffer()
{
    return m_Buffer;
}

BITMAPINFO* CBackBuffer::GetDIBInfo()
{
    return &m_DIBInfo;
}

int32_t CBackBuffer::GetFileSize()
{
    return m_FileSize;
}

void CBackBuffer::DrawSprite(int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int srcPitch, BYTE* srcBuffer, __int32 iDrawLen )
{
    //------------------------------------
    //> dest�� ����ۿ� �׸�  ���� ��ġ
    //> src��  �ҽ��� �׸� ���� ��ġ
    //------------------------------------
    int clippingRight = srcWidth;      //> ���� Ŭ���� ��, ������ �׷��ߵǴ����� ���� ����
    int clippingLeft = 0;              //> ���� Ŭ���� ��, ������ �����ؾߵ���
    int clippingUp = 0;                //> ���� Ŭ���� ��, ������ �����ؾߵ���
    int clippingDown = srcHeight;      //> �Ʒ� Ŭ���� ��, ������ �׷��ߵǴ����� ���� ����

    if (destY < 0 )
    {
        clippingUp = -destY;
        destY = 0;
    }    
    if (destY > WINSIZE_HEIGHT - srcHeight)     //> ���� Ŭ���� ���� �Ʒ��� ħ�� �ϸ�
    {
        clippingDown = WINSIZE_HEIGHT - destY;  //> src�� 0���� �׸���,destY~ winsize Y���� �ۿ� �׸� ���ڿ� ����
    }   
    if (destX < 0)       //>���� ������ �Ѿ�� Ŭ������ ����� �� ���
    {
        //��|------------|
        //  |------------|
        clippingLeft = -destX;      //>���밪�� ���ؼ� �׸�ŭ�� ���� Sprite�� �׸���
        destX = 0;
    }
    if (destX+srcWidth > WINSIZE_WIDTH)     //>������ ������ �Ѿ�� Ŭ������ ����� �� ���
    {
        //|------------|��
        //|------------|
        //�Ѿ ���� �����ֱ�
        clippingRight = WINSIZE_WIDTH - destX;
    }
    
    int startX = destX*4;                                   //> ����ۿ��� Sprite��  �׸� ������ġ X
    int startY = destY * m_Pitch;                           //> ����ۿ��� Sprite��  �׸� ������ġ Y
    
    int cpyX = (srcX + clippingLeft)*4;                     //> Sprite ������ ������ǥ ���� 0���� ���� ���࿡, Ŭ���� ���ʿ����� ħ������ ���, 
    int cpyY = (srcY+ clippingUp) * srcPitch;               //> �׸�ŭ ħ�������� �����ϰ� �׷��� ��

    BYTE* byteDestPointer = m_Buffer + startX + startY;     //> ����ۿ� Sprite ������ġ�� ����� ���� ������    
    BYTE* byteSrcPointer = srcBuffer + cpyX+ cpyY;          //> ��������Ʈ ���� ���� ������

      
    DWORD* dwSrcPointer = (DWORD*)byteSrcPointer;           //> RGBA ���� ����Ͽ� 4����Ʈ ������ ������ ������ �ʿ��ϹǷ�,
    DWORD* dwDestPointer = (DWORD*)byteDestPointer;         //> DWORD ������ ���·� ĳ����

    // ��� �� ���� (0~100%)
    clippingRight =(__int32)((double)clippingRight*(iDrawLen / 100.0));

    for (int heightCnt = clippingUp; heightCnt < clippingDown; ++heightCnt) //> Ư���� �̻��� ���ٸ� 0~Width���� �׸�������
    {                                                                       //> Ŭ���� ó���� �ؾ��ϴ� ���, Sprite�� �Ϻθ� �׷��� �Ѵ�.
 
        for (int widthCnt = clippingLeft; widthCnt < clippingRight; ++widthCnt)
        {
                
                if (*dwSrcPointer != 0xffffffff)                //> Sprite������ �ȼ��� ����� �ƴ϶�� RGB(255,255,255,255)(���İ� ����)
                {
                    *dwDestPointer = *dwSrcPointer;          //>����� �����Ϳ� �� Sprite������ �ȼ��� ����.
                }
                dwSrcPointer++;
                dwDestPointer++;
        }
        byteSrcPointer += srcPitch;
        dwSrcPointer = (DWORD*)byteSrcPointer;
        byteDestPointer += m_Pitch;
        dwDestPointer =(DWORD*) byteDestPointer;
    }
}

void CBackBuffer::DrawSprite(int destX, int destY, CSpriteManager::stSprite* pSprite, DWORD dwColorKey, __int32 iDrawLen )
{
    if (pSprite == nullptr)
    {
        return;
    }
    int srcWidth = pSprite->m_iWidth;
    //---------------------------------------
    //��� �� ���� (0~100%)
    //---------------------------------------
    srcWidth = (__int32)((double)srcWidth * (iDrawLen / 100.0));
    //srcWidth = (__int32)(srcWidth * iDrawLen / 100);
    int srcHeight = pSprite->m_iHeight;
    int srcPitch = pSprite->m_iPitch;
    BYTE* srcBuffer = pSprite->m_bypImage;

    destX -= pSprite->m_iOriginX;
    destY -= pSprite->m_iOriginY;

    //------------------------------------
    //> dest�� ����ۿ� �׸�  ���� ��ġ
    //> src��  �ҽ��� �׸� ���� ��ġ
    //------------------------------------
    int clippingRight = srcWidth;      //> ���� Ŭ���� ��, ������ �׷��ߵǴ����� ���� ����
    int clippingLeft = 0;              //> ���� Ŭ���� ��, ������ �����ؾߵ���
    int clippingUp = 0;                //> ���� Ŭ���� ��, ������ �����ؾߵ���
    int clippingDown = srcHeight;      //> �Ʒ� Ŭ���� ��, ������ �׷��ߵǴ����� ���� ����

    if (destY < 0)
    {
        clippingUp = -destY;
        destY = 0;
    }
    if (destY > WINSIZE_HEIGHT - srcHeight)     //> ���� Ŭ���� ���� �Ʒ��� ħ�� �ϸ�
    {
        clippingDown = WINSIZE_HEIGHT - destY;  //> src�� 0���� �׸���,destY~ winsize Y���� �ۿ� �׸� ���ڿ� ����
    }
    if (destX < 0)       //>���� ������ �Ѿ�� Ŭ������ ����� �� ���
    {
        //��|------------|
        //  |------------|
        clippingLeft = -destX;      //>���밪�� ���ؼ� �׸�ŭ�� ���� Sprite�� �׸���
        destX = 0;
    }
    if (destX + srcWidth > WINSIZE_WIDTH)     //>������ ������ �Ѿ�� Ŭ������ ����� �� ���
    {
        //|------------|��
        //|------------|
        //�Ѿ ���� �����ֱ�
        clippingRight = WINSIZE_WIDTH - destX;
    }

    int startX = destX * 4;                                   //> ����ۿ��� Sprite��  �׸� ������ġ X
    int startY = destY * m_Pitch;                           //> ����ۿ��� Sprite��  �׸� ������ġ Y

    int cpyX = clippingLeft * 4;                     //> Sprite ������ ������ǥ ���� 0���� ���� ���࿡, Ŭ���� ���ʿ����� ħ������ ���, 
    int cpyY = clippingUp * srcPitch;               //> �׸�ŭ ħ�������� �����ϰ� �׷��� ��

    BYTE* byteDestPointer = m_Buffer + startX + startY;     //> ����ۿ� Sprite ������ġ�� ����� ���� ������    
    BYTE* byteSrcPointer = srcBuffer + cpyX + cpyY;          //> ��������Ʈ ���� ���� ������


    DWORD* dwSrcPointer = (DWORD*)byteSrcPointer;           //> RGBA ���� ����Ͽ� 4����Ʈ ������ ������ ������ �ʿ��ϹǷ�,
    DWORD* dwDestPointer = (DWORD*)byteDestPointer;         //> DWORD ������ ���·� ĳ����

    for (int heightCnt = clippingUp; heightCnt < clippingDown; ++heightCnt) //> Ư���� �̻��� ���ٸ� 0~Width���� �׸�������
    {                                                                       //> Ŭ���� ó���� �ؾ��ϴ� ���, Sprite�� �Ϻθ� �׷��� �Ѵ�.

        for (int widthCnt = clippingLeft; widthCnt < clippingRight; ++widthCnt)
        {

            if (*dwSrcPointer != dwColorKey)                //> Sprite������ �ȼ��� ����� �ƴ϶�� RGB(255,255,255,255)(���İ� ����)
            {
                *dwDestPointer = *dwSrcPointer;          //>����� �����Ϳ� �� Sprite������ �ȼ��� ����.
            }
           
            dwSrcPointer++;
            dwDestPointer++;
        }
        byteSrcPointer += srcPitch;
        dwSrcPointer = (DWORD*)byteSrcPointer;
        byteDestPointer += m_Pitch;
        dwDestPointer = (DWORD*)byteDestPointer;
    }

}
//void CBackBuffer::DrawSprite50(int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int srcPitch, BYTE* srcBuffer, __int32 iDrawLen)
//{
//
//}

void CBackBuffer::DrawSpriteRed1(int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int srcPitch, BYTE* srcBuffer, __int32 iDrawLen)
{
    //------------------------------------
    //> dest�� ����ۿ� �׸�  ���� ��ġ
    //> src��  �ҽ��� �׸� ���� ��ġ
    //------------------------------------
    int clippingRight = srcWidth;      //> ���� Ŭ���� ��, ������ �׷��ߵǴ����� ���� ����
    int clippingLeft = 0;       //> ���� Ŭ���� ��, ������ �����ؾߵ���
    int clippingUp = 0;         //> ���� Ŭ���� ��, ������ �����ؾߵ���
    int clippingDown = srcHeight;       //> �Ʒ� Ŭ���� ��, ������ �׷��ߵǴ����� ���� ����

    if (destY < 0)
    {
        clippingUp = -destY;
        destY = 0;
    }
    if (destY > WINSIZE_HEIGHT - srcHeight)     //> ���� Ŭ���� ���� �Ʒ��� ħ�� �ϸ�
    {
        clippingDown = WINSIZE_HEIGHT - destY;  //> src�� 0���� �׸���,destY~ winsize Y���� �ۿ� �׸� ���ڿ� ����
    }
    if (destX < 0)       //>���� ������ �Ѿ�� Ŭ������ ����� �� ���
    {
        //��|------------|
        //  |------------|
        clippingLeft = -destX;      //>���밪�� ���ؼ� �׸�ŭ�� ���� Sprite�� �׸���
        destX = 0;
    }
    if (destX + srcWidth > WINSIZE_WIDTH)     //>������ ������ �Ѿ�� Ŭ������ ����� �� ���
    {
        //|------------|��
        //|------------|
        //�Ѿ ���� �����ֱ�
        clippingRight = WINSIZE_WIDTH - destX;
    }

    int startX = destX * 4;                                   //> ����ۿ��� Sprite��  �׸� ������ġ X
    int startY = destY * m_Pitch;                           //> ����ۿ��� Sprite��  �׸� ������ġ Y

    int cpyX = (srcX + clippingLeft) * 4;                     //> Sprite ������ ������ǥ ���� 0���� ���� ���࿡, Ŭ���� ���ʿ����� ħ������ ���, 
    int cpyY = (srcY + clippingUp) * srcPitch;               //> �׸�ŭ ħ�������� �����ϰ� �׷��� ��

    BYTE* byteDestPointer = m_Buffer + startX + startY;     //> ����ۿ� Sprite ������ġ�� ����� ���� ������    
    BYTE* byteSrcPointer = srcBuffer + cpyX + cpyY;          //> ��������Ʈ ���� ���� ������


    DWORD* dwSrcPointer = (DWORD*)byteSrcPointer;           //> RGBA ���� ����Ͽ� 4����Ʈ ������ ������ ������ �ʿ��ϹǷ�,
    DWORD* dwDestPointer = (DWORD*)byteDestPointer;         //> DWORD ������ ���·� ĳ����

    // ��� �� ���� (0~100%)
    clippingRight = (__int32)((double)clippingRight * (iDrawLen / 100.0));

    for (int heightCnt = clippingUp; heightCnt < clippingDown; ++heightCnt) //> Ư���� �̻��� ���ٸ� 0~Width���� �׸�������
    {                                                                       //> Ŭ���� ó���� �ؾ��ϴ� ���, Sprite�� �Ϻθ� �׷��� �Ѵ�.

        for (int widthCnt = clippingLeft; widthCnt < clippingRight; ++widthCnt)
        {

            if (*dwSrcPointer != 0xffffffff)                //> Sprite������ �ȼ��� ����� �ƴ϶�� RGB(255,255,255,255)(���İ� ����)
            {
                BYTE* bypTempSrc = (BYTE*)dwSrcPointer;
                *(bypTempSrc + 2) = 0xff;
                *dwDestPointer = *dwSrcPointer;          //>����� �����Ϳ� �� Sprite������ �ȼ��� ����.
            }
            dwSrcPointer++;
            dwDestPointer++;
        }
        byteSrcPointer += srcPitch;
        dwSrcPointer = (DWORD*)byteSrcPointer;
        byteDestPointer += m_Pitch;
        dwDestPointer = (DWORD*)byteDestPointer;
    }
}

void CBackBuffer::DrawSpriteRed1(int destX, int destY, CSpriteManager::stSprite* pSprite, DWORD dwColorKey, __int32 iDrawLen)
{
    if (pSprite == nullptr)
    {
        return;
    }
    int srcWidth = pSprite->m_iWidth;
    //---------------------------------------
    //��� �� ���� (0~100%)
    //---------------------------------------
    srcWidth = (__int32)((double)srcWidth * (iDrawLen / 100.0));

    int srcHeight = pSprite->m_iHeight;
    int srcPitch = pSprite->m_iPitch;
    BYTE* srcBuffer = pSprite->m_bypImage;

    destX -= pSprite->m_iOriginX;
    destY -= pSprite->m_iOriginY;

    //------------------------------------
    //> dest�� ����ۿ� �׸�  ���� ��ġ
    //> src��  �ҽ��� �׸� ���� ��ġ
    //------------------------------------
    int clippingRight = srcWidth;      //> ���� Ŭ���� ��, ������ �׷��ߵǴ����� ���� ����
    int clippingLeft = 0;              //> ���� Ŭ���� ��, ������ �����ؾߵ���
    int clippingUp = 0;                //> ���� Ŭ���� ��, ������ �����ؾߵ���
    int clippingDown = srcHeight;      //> �Ʒ� Ŭ���� ��, ������ �׷��ߵǴ����� ���� ����

    if (destY < 0)
    {
        clippingUp = -destY;
        destY = 0;
    }
    if (destY > WINSIZE_HEIGHT - srcHeight)     //> ���� Ŭ���� ���� �Ʒ��� ħ�� �ϸ�
    {
        clippingDown = WINSIZE_HEIGHT - destY;  //> src�� 0���� �׸���,destY~ winsize Y���� �ۿ� �׸� ���ڿ� ����
    }
    if (destX < 0)       //>���� ������ �Ѿ�� Ŭ������ ����� �� ���
    {
        //��|------------|
        //  |------------|
        clippingLeft = -destX;      //>���밪�� ���ؼ� �׸�ŭ�� ���� Sprite�� �׸���
        destX = 0;
    }
    if (destX + srcWidth > WINSIZE_WIDTH)     //>������ ������ �Ѿ�� Ŭ������ ����� �� ���
    {
        //|------------|��
        //|------------|
        //�Ѿ ���� �����ֱ�
        clippingRight = WINSIZE_WIDTH - destX;
    }

    int startX = destX * 4;                                   //> ����ۿ��� Sprite��  �׸� ������ġ X
    int startY = destY * m_Pitch;                           //> ����ۿ��� Sprite��  �׸� ������ġ Y

    int cpyX = clippingLeft * 4;                     //> Sprite ������ ������ǥ ���� 0���� ���� ���࿡, Ŭ���� ���ʿ����� ħ������ ���, 
    int cpyY = clippingUp * srcPitch;               //> �׸�ŭ ħ�������� �����ϰ� �׷��� ��

    BYTE* byteDestPointer = m_Buffer + startX + startY;     //> ����ۿ� Sprite ������ġ�� ����� ���� ������    
    BYTE* byteSrcPointer = srcBuffer + cpyX + cpyY;          //> ��������Ʈ ���� ���� ������


    DWORD* dwSrcPointer = (DWORD*)byteSrcPointer;           //> RGBA ���� ����Ͽ� 4����Ʈ ������ ������ ������ �ʿ��ϹǷ�,
    DWORD* dwDestPointer = (DWORD*)byteDestPointer;         //> DWORD ������ ���·� ĳ����

    for (int heightCnt = clippingUp; heightCnt < clippingDown; ++heightCnt) //> Ư���� �̻��� ���ٸ� 0~Width���� �׸�������
    {                                                                       //> Ŭ���� ó���� �ؾ��ϴ� ���, Sprite�� �Ϻθ� �׷��� �Ѵ�.

        for (int widthCnt = clippingLeft; widthCnt < clippingRight; ++widthCnt)
        {

            if (*dwSrcPointer != dwColorKey)                //> Sprite������ �ȼ��� ����� �ƴ϶�� RGB(255,255,255,255)(���İ� ����)
            {
                //BYTE* bypTempSrc = (BYTE*)dwSrcPointer;
                //*(bypTempSrc + 2) = 0xff;
                *dwDestPointer = *dwSrcPointer;          //>����� �����Ϳ� �� Sprite������ �ȼ��� ����.
                BYTE* bypTempDest = (BYTE*)dwDestPointer;
                *(bypTempDest + 2) = 0xff;
            }

            dwSrcPointer++;
            dwDestPointer++;
        }
        byteSrcPointer += srcPitch;
        dwSrcPointer = (DWORD*)byteSrcPointer;
        byteDestPointer += m_Pitch;
        dwDestPointer = (DWORD*)byteDestPointer;
    }

}

void CBackBuffer::DrawSpriteTransParent(int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int srcPitch, BYTE* srcBuffer, __int32 iDrawLen)
{
    //------------------------------------
//> dest�� ����ۿ� �׸�  ���� ��ġ
//> src��  �ҽ��� �׸� ���� ��ġ
//------------------------------------
    int clippingRight = srcWidth;      //> ���� Ŭ���� ��, ������ �׷��ߵǴ����� ���� ����
    int clippingLeft = 0;       //> ���� Ŭ���� ��, ������ �����ؾߵ���
    int clippingUp = 0;         //> ���� Ŭ���� ��, ������ �����ؾߵ���
    int clippingDown = srcHeight;       //> �Ʒ� Ŭ���� ��, ������ �׷��ߵǴ����� ���� ����

    if (destY < 0)
    {
        clippingUp = -destY;
        destY = 0;
    }
    if (destY > WINSIZE_HEIGHT - srcHeight)     //> ���� Ŭ���� ���� �Ʒ��� ħ�� �ϸ�
    {
        clippingDown = WINSIZE_HEIGHT - destY;  //> src�� 0���� �׸���,destY~ winsize Y���� �ۿ� �׸� ���ڿ� ����
    }
    if (destX < 0)       //>���� ������ �Ѿ�� Ŭ������ ����� �� ���
    {
        //��|------------|
        //  |------------|
        clippingLeft = -destX;      //>���밪�� ���ؼ� �׸�ŭ�� ���� Sprite�� �׸���
        destX = 0;
    }
    if (destX + srcWidth > WINSIZE_WIDTH)     //>������ ������ �Ѿ�� Ŭ������ ����� �� ���
    {
        //|------------|��
        //|------------|
        //�Ѿ ���� �����ֱ�
        clippingRight = WINSIZE_WIDTH - destX;
    }

    int startX = destX * 4;                                   //> ����ۿ��� Sprite��  �׸� ������ġ X
    int startY = destY * m_Pitch;                           //> ����ۿ��� Sprite��  �׸� ������ġ Y

    int cpyX = (srcX + clippingLeft) * 4;                     //> Sprite ������ ������ǥ ���� 0���� ���� ���࿡, Ŭ���� ���ʿ����� ħ������ ���, 
    int cpyY = (srcY + clippingUp) * srcPitch;               //> �׸�ŭ ħ�������� �����ϰ� �׷��� ��

    BYTE* byteDestPointer = m_Buffer + startX + startY;     //> ����ۿ� Sprite ������ġ�� ����� ���� ������    
    BYTE* byteSrcPointer = srcBuffer + cpyX + cpyY;          //> ��������Ʈ ���� ���� ������


    DWORD* dwSrcPointer = (DWORD*)byteSrcPointer;           //> RGBA ���� ����Ͽ� 4����Ʈ ������ ������ ������ �ʿ��ϹǷ�,
    DWORD* dwDestPointer = (DWORD*)byteDestPointer;         //> DWORD ������ ���·� ĳ����

    // ��� �� ���� (0~100%)
    clippingRight = (__int32)((double)clippingRight * (iDrawLen / 100.0));

    for (int heightCnt = clippingUp; heightCnt < clippingDown; ++heightCnt) //> Ư���� �̻��� ���ٸ� 0~Width���� �׸�������
    {                                                                       //> Ŭ���� ó���� �ؾ��ϴ� ���, Sprite�� �Ϻθ� �׷��� �Ѵ�.

        for (int widthCnt = clippingLeft; widthCnt < clippingRight; ++widthCnt)
        {

            if (*dwSrcPointer != 0xffffffff)                //> Sprite������ �ȼ��� ����� �ƴ϶�� RGB(255,255,255,255)(���İ� ����)
            {
                BYTE* bypTempSrc = (BYTE*)dwSrcPointer;
                int id = (*bypTempSrc + *(bypTempSrc + 1) + *(bypTempSrc + 2)) / 20;
                *dwDestPointer = *(dwSrcPointer+id) & 0xf0f0f0f0;          //>����� �����Ϳ� �� Sprite������ �ȼ��� ����.
            }
            dwSrcPointer++;
            dwDestPointer++;
        }
        byteSrcPointer += srcPitch;
        dwSrcPointer = (DWORD*)byteSrcPointer;
        byteDestPointer += m_Pitch;
        dwDestPointer = (DWORD*)byteDestPointer;
    }
}

void CBackBuffer::DrawImage(int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int srcPitch, BYTE* srcBuffer, __int32 iDrawLen)
{
    //------------------------------------
//> dest�� ����ۿ� �׸�  ���� ��ġ
//> src��  �ҽ��� �׸� ���� ��ġ
//------------------------------------
    int clippingRight = srcWidth;      //> ���� Ŭ���� ��, ������ �׷��ߵǴ����� ���� ����
    int clippingLeft = 0;       //> ���� Ŭ���� ��, ������ �����ؾߵ���
    int clippingUp = 0;         //> ���� Ŭ���� ��, ������ �����ؾߵ���
    int clippingDown = srcHeight;       //> �Ʒ� Ŭ���� ��, ������ �׷��ߵǴ����� ���� ����

    if (destY < 0)
    {
        clippingUp = -destY;
        destY = 0;
    }
    if (destY > WINSIZE_HEIGHT - srcHeight)     //> ���� Ŭ���� ���� �Ʒ��� ħ�� �ϸ�
    {
        clippingDown = WINSIZE_HEIGHT - destY;  //> src�� 0���� �׸���,destY~ winsize Y���� �ۿ� �׸� ���ڿ� ����
    }
    if (destX < 0)       //>���� ������ �Ѿ�� Ŭ������ ����� �� ���
    {
        //��|------------|
        //  |------------|
        clippingLeft = -destX;      //>���밪�� ���ؼ� �׸�ŭ�� ���� Sprite�� �׸���
        destX = 0;
    }
    if (destX + srcWidth > WINSIZE_WIDTH)     //>������ ������ �Ѿ�� Ŭ������ ����� �� ���
    {
        //|------------|��
        //|------------|
        //�Ѿ ���� �����ֱ�
        clippingRight = WINSIZE_WIDTH - destX;
    }

    int startX = destX * 4;                                   //> ����ۿ��� Sprite��  �׸� ������ġ X
    int startY = destY * m_Pitch;                           //> ����ۿ��� Sprite��  �׸� ������ġ Y

    int cpyX = (srcX + clippingLeft) * 4;                     //> Sprite ������ ������ǥ ���� 0���� ���� ���࿡, Ŭ���� ���ʿ����� ħ������ ���, 
    int cpyY = (srcY + clippingUp) * srcPitch;               //> �׸�ŭ ħ�������� �����ϰ� �׷��� ��

    BYTE* byteDestPointer = m_Buffer + startX + startY;     //> ����ۿ� Sprite ������ġ�� ����� ���� ������    
    BYTE* byteSrcPointer = srcBuffer + cpyX + cpyY;          //> ��������Ʈ ���� ���� ������


    DWORD* dwSrcPointer = (DWORD*)byteSrcPointer;           //> RGBA ���� ����Ͽ� 4����Ʈ ������ ������ ������ �ʿ��ϹǷ�,
    DWORD* dwDestPointer = (DWORD*)byteDestPointer;         //> DWORD ������ ���·� ĳ����

    // ��� �� ���� (0~100%)
    clippingRight = (__int32)((double)clippingRight * (iDrawLen / 100.0));

    for (int heightCnt = clippingUp; heightCnt < clippingDown; ++heightCnt) //> Ư���� �̻��� ���ٸ� 0~Width���� �׸�������
    {                                                                       //> Ŭ���� ó���� �ؾ��ϴ� ���, Sprite�� �Ϻθ� �׷��� �Ѵ�.

        for (int widthCnt = clippingLeft; widthCnt < clippingRight; ++widthCnt)
        {
            //-------------------------------
            //�̹��� ���
            //-------------------------------
            *dwDestPointer = *dwSrcPointer; 
            dwSrcPointer++;
            dwDestPointer++;
        }
        byteSrcPointer += srcPitch;
        dwSrcPointer = (DWORD*)byteSrcPointer;
        byteDestPointer += m_Pitch;
        dwDestPointer = (DWORD*)byteDestPointer;
    }
}



CBackBuffer::CBackBuffer()
    :
    m_Width(WINSIZE_WIDTH), 
    m_Height(WINSIZE_HEIGHT),
    m_Buffer(nullptr),
    m_Pitch(0)
{
    memset(&m_DIBInfo, 0, sizeof(BITMAPINFO));
    m_Pitch = ((m_Width * 4) + 3) & ~3;
    int fileSize = m_Pitch * m_Height;
    m_Buffer = new BYTE[fileSize];

    memset(m_Buffer, 128, fileSize);

    m_DIBInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_DIBInfo.bmiHeader.biWidth = m_Width;
    m_DIBInfo.bmiHeader.biHeight = -m_Height;
    m_DIBInfo.bmiHeader.biPlanes = 1;
    m_DIBInfo.bmiHeader.biBitCount = 32;
    m_DIBInfo.bmiHeader.biCompression = 0;
    m_DIBInfo.bmiHeader.biSizeImage = 0;
    m_DIBInfo.bmiHeader.biClrUsed = 0;
    m_DIBInfo.bmiHeader.biClrImportant = 0;

}

CBackBuffer::~CBackBuffer()
{
    delete[] m_Buffer;
}
