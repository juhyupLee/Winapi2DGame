#pragma once
class CSprite
{

public:
	CSprite(const WCHAR* fileName);
	~CSprite();

	int GetPitch();
	int GetWidth();
	int GetHeight();
	BYTE* GetBuffer();
private:
	int m_Pitch;
	int m_Width;
	int m_Height;
	BYTE* m_Buffer;

};