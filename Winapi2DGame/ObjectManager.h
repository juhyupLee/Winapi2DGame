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
	void OtherPlayerActionInput(int scene, int id, int state, BYTE direction, int x, int y, bool inputStatus);
	void AttackCheck(int scene, int attackID, int damageID, int damageHP);
	void DeletePlayer(int scene, int id);
	void Update(int scene);
	void Render(int scene);
	void AddObject(__int32 sceneNum, IBaseObject* object);
	void AddCollisionObject(int key, int value );
	void ReleaseObject(__int32 scene);
	//>충돌체크를 해서, Object에게 알려주기?
	//>플레이어 입장에서 몬스터랑 충돌했을때,랑  누구랑 충돌했는지 알고싶지않겠어?
private:
	void CollisionCheck(int scene,IBaseObject* object);
private:
	CList<IBaseObject*> m_ObjectList[SCENE_MAX];
	std::unordered_map<int, int> m_CollisionObjectMap;

};