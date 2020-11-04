#pragma once
class CObjectManager
{
public:
	enum
	{
		TITLE_SCENE,
		GAME_SCENE,
		SCENE_MAX = 2
	};
private:
	CObjectManager();
	~CObjectManager();

public:
	static CObjectManager* GetInstance();

	

public:
	void OtherPlayerActionInput(int id, int state, BYTE direction, int x, int y, bool inputStatus);
	void AttackCheck(int attackID, int damageID, int damageHP);
	void DeletePlayer(int id);
	void Update();
	void Render();
	void AddObject(IBaseObject* object);
	
	void ReleaseObject();

private:
	CList<IBaseObject*> m_ObjectList;
	std::unordered_map<int, int> m_CollisionObjectMap;

};