#include <stdio.h>
#include <Windows.h>
#include "Sprite.h"


CSprite::CSprite(const WCHAR* fileName)
	:
	m_Buffer(nullptr),
	m_Height(0),
	m_Width(0),
	m_Pitch(0)
	
{
	FILE* filePtr = nullptr;

	while (filePtr == nullptr)
	{
		_wfopen_s(&filePtr, fileName, L"rb");
	}

	BITMAPFILEHEADER bitFileHeader;
	BITMAPINFOHEADER bitmapInfoHeder;

	if (0 == fread(&bitFileHeader, 1, sizeof(BITMAPFILEHEADER), filePtr))
	{
		MessageBox(nullptr, L"fread 오류", MB_OK, MB_OK);
	}
	if (*((short*)&bitFileHeader) != 0x4d42)
	{
		MessageBox(nullptr, L"BM 오류", MB_OK, MB_OK);
	}

	
	if (0 == fread(&bitmapInfoHeder, 1, sizeof(BITMAPINFOHEADER), filePtr))
	{
		MessageBox(nullptr, L"fread 오류", MB_OK, MB_OK);
	}

	//Pitchffmfdsfsddsfsdfsdklfnlksdfklsdm
	int pitch = bitmapInfoHeder.biWidth * (bitmapInfoHeder.biBitCount / 8);
	pitch = (pitch + 3) & ~3;
	//-----------------------------------

	m_Pitch = pitch;
	m_Height = bitmapInfoHeder.biHeight;
	m_Width = bitmapInfoHeder.biWidth;

	int fileSize = m_Pitch * m_Height;

	m_Buffer = new BYTE[fileSize];
	BYTE* startBuffer = m_Buffer;

	BYTE* cpyBuffer = new BYTE[fileSize];

	BYTE* srcBuffer = cpyBuffer;

	if (0 == fread(srcBuffer, 1, fileSize, filePtr))
	{
		MessageBox(nullptr, L"fread 오류", MB_OK, MB_OK);
	}

	//m_Buffer <--> srcBuffer (이미지파일을 거꾸로 담아오기)

	//맨끝에서 한줄뺀만큼 부터 출발
	srcBuffer = srcBuffer + ((__int64)fileSize - m_Pitch);

	for (int i = 0; i < m_Height; ++i)
	{
		memcpy(m_Buffer, srcBuffer, m_Pitch);
		m_Buffer += m_Pitch;
		srcBuffer -= m_Pitch;
	}

	delete[] cpyBuffer;

	m_Buffer = startBuffer;
	fclose(filePtr);
}

CSprite::~CSprite()
{
	delete[] m_Buffer;
}

int CSprite::GetPitch()
{
	return m_Pitch;
}

int CSprite::GetWidth()
{
	return m_Width;
}

int CSprite::GetHeight()
{
	return m_Height;
}

BYTE* CSprite::GetBuffer()
{
	return m_Buffer;
}

