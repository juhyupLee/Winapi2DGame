#pragma once
class IBaseScene;
class CSceneManager
{
public:
	enum
	{
		START_SCENE,
		GAME_SCENE,
		END_SCENE,
		EXIT
	};
private:
	CSceneManager();
	~CSceneManager();

public:
	static CSceneManager* GetInstance();
public:
	bool Run();
	bool ChangeScene(__int32 scene);
	
private:
	IBaseScene* m_currentScene;
	bool m_bEnterFrame;
	bool m_bEnterRender;
	bool m_bRenderSkip;
	__int32  m_SkipCount;
	__int32	 m_NonSkipCount;

};