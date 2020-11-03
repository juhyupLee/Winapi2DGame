#include <iostream>
#include <Windows.h>
#include "SpriteManager.h"
#include "MemoryTracer.h"
CSpriteManager::CSpriteManager()
	:
	m_stSprite{ 0, },
	m_stAnimationStatus{ 0, }
{
}

CSpriteManager::~CSpriteManager()
{
	for (size_t index1 = 0; index1 < OBJECT_MAX; index1++)
	{
		for (size_t index2 = 0; index2 < DIRECTION_MAX; index2++)
		{
			for (size_t index3 = 0; index3 < STATUS_MAX; index3++)
			{
				for (size_t index4 = 0; index4 < FRAME_MAX; index4++)
				{
					delete[] m_stSprite[index1][index2][index3][index4].m_bypImage;;

				}
			}
		}
	}
}

CSpriteManager* CSpriteManager::GetInstance()
{
	static CSpriteManager spriteManager;

	return &spriteManager;
}

bool CSpriteManager::LoadingSprite(__int32 iObjectType, __int32 iDirection, __int32 iStatus, __int32 iFrame, const WCHAR* szFileName, __int32 iOriginX, __int32 iOriginY, RECT rectCollisionBox, RECT rectHitBox)
{
	FILE* filePtr = nullptr;

	while (filePtr == nullptr)
	{
		_wfopen_s(&filePtr, szFileName, L"rb");
	}

	BITMAPFILEHEADER bitFileHeader;
	BITMAPINFOHEADER bitmapInfoHeder;

	if (0 == fread(&bitFileHeader, 1, sizeof(BITMAPFILEHEADER), filePtr))
	{
		MessageBox(nullptr, L"fread 오류", MB_OK, MB_OK);
		fclose(filePtr);
		return false;
	}
	if (*((short*)&bitFileHeader) != 0x4d42)
	{
		MessageBox(nullptr, L"BM 오류", MB_OK, MB_OK);
		fclose(filePtr);
		return false;
	}
	if (0 == fread(&bitmapInfoHeder, 1, sizeof(BITMAPINFOHEADER), filePtr))
	{
		MessageBox(nullptr, L"fread 오류", MB_OK, MB_OK);
		fclose(filePtr);
		return false;
	}

	int pitch = bitmapInfoHeder.biWidth * (bitmapInfoHeder.biBitCount / 8);
	pitch = (pitch + 3) & ~3;
	//-----------------------------------

	m_stSprite[iObjectType][iDirection][iStatus][iFrame].m_iPitch = pitch;
	m_stSprite[iObjectType][iDirection][iStatus][iFrame].m_iHeight = bitmapInfoHeder.biHeight;
	m_stSprite[iObjectType][iDirection][iStatus][iFrame].m_iWidth = bitmapInfoHeder.biWidth;
	m_stSprite[iObjectType][iDirection][iStatus][iFrame].m_rectCollisionBox = rectCollisionBox;
	m_stSprite[iObjectType][iDirection][iStatus][iFrame].m_rectHitBox = rectHitBox;
	m_stSprite[iObjectType][iDirection][iStatus][iFrame].m_iOriginX = iOriginX;
	m_stSprite[iObjectType][iDirection][iStatus][iFrame].m_iOriginY = iOriginY;
	int fileSize = pitch * bitmapInfoHeder.biHeight;

	m_stSprite[iObjectType][iDirection][iStatus][iFrame].m_bypImage = new BYTE[fileSize];

	BYTE* startBuffer = m_stSprite[iObjectType][iDirection][iStatus][iFrame].m_bypImage;

	BYTE* cpyBuffer = new BYTE[fileSize];

	BYTE* srcBuffer = cpyBuffer;

	if (0 == fread(srcBuffer, 1, fileSize, filePtr))
	{
		MessageBox(nullptr, L"fread 오류", MB_OK, MB_OK);
	}

	//m_Buffer <--> srcBuffer (이미지파일을 거꾸로 담아오기)

	//맨끝에서 한줄뺀만큼 부터 출발
	srcBuffer = srcBuffer + ((__int64)fileSize - pitch);

	for (int i = 0; i < bitmapInfoHeder.biHeight; ++i)
	{
		memcpy(m_stSprite[iObjectType][iDirection][iStatus][iFrame].m_bypImage, srcBuffer, pitch);
		m_stSprite[iObjectType][iDirection][iStatus][iFrame].m_bypImage += pitch;
		srcBuffer -= pitch;
	}

	delete[] cpyBuffer;

	m_stSprite[iObjectType][iDirection][iStatus][iFrame].m_bypImage = startBuffer;

	fclose(filePtr);

	return true;
}

CSpriteManager::stSprite* CSpriteManager::GetSprite(__int32 iObjectType, __int32 iDirection, __int32 iStatus, __int32 iFrame)
{
	return &m_stSprite[iObjectType][iDirection][iStatus][iFrame];
}

CSpriteManager::stAnimationStatus* CSpriteManager::GetAnimationStatus(__int32 iObjectType, __int32 iStatus)
{
	return &m_stAnimationStatus[iObjectType][iStatus];
}

void CSpriteManager::SetAnimationStatus(__int32 iObjectType, __int32 iStatus, __int32 iDelay, __int32 iFrameMax)
{
	m_stAnimationStatus[iObjectType][iStatus].m_iDelay = iDelay;
	m_stAnimationStatus[iObjectType][iStatus].m_iFrameMax = iFrameMax;
}

