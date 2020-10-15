#pragma once
class CSpriteDib
{
public:
	struct st_SPRITE
	{
		BYTE* m_Image;
		__int32		m_Width;
		__int32		m_Height;
		__int32		m_Pitch;
		__int32		m_CenterPointX;
		__int32		m_CenterPointY;
	};

	CSpriteDib(int maxSprite, DWORD colorKey);
	virtual ~CSpriteDib();

	BOOL LoadDibSprite(__int32 spriteIndex, WCHAR* fileName, __int32 centerPointX, __int32 centerPointY);
	void ReleaseSprite(__int32 spriteIndex);

	void DrawSprite(__int32 spriteIndex, __int32 drawX, __int32 drawY, BYTE* dest, __int32 destWidth, __int32 destHeight, __int32 destPitch, __int32 drawLen = 100);
	void DrawSprite50(__int32 spriteIndex, __int32 drawX, __int32 drawY, BYTE* dest, __int32 destWidth, __int32 destHeight, __int32 destPitch);
	void DrawSpriteRed(__int32 spriteIndex, __int32 drawX, __int32 drawY, BYTE* dest, __int32 destWidth, __int32 destHeight, __int32 destPitch);
	void DrawImage(int spriteIndex, int drawX, int drawY, BYTE* dest, __int32 destWidth, __int32 destHeight, __int32 destPitch, int drawLen = 100);

protected:
	__int32 m_MaxSprite;
	st_SPRITE* m_Sprite;
	DWORD m_ColorKey;

};