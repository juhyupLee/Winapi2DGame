#pragma once
class IBaseScene
{
public:
	IBaseScene()
		:
		m_dwKeyResult(0)
	{

	}
	virtual ~IBaseScene()
	{

	}
public:
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual bool Loading() = 0;
	virtual void KeyProcess() = 0;
public:
	unsigned __int32 GetKeyResult();
	void ResetKey();
protected:
	unsigned __int32 m_dwKeyResult;
};