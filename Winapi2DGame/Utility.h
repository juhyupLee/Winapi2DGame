#pragma once
class CUtility
{
private:
	CUtility();
	~CUtility();

public:
	static CUtility* GetInstance();
	//0~32��Ʈ 
	void BitInsert(unsigned int* out, int bitposition,bool value);
	int  GenerateRandomNum(int min, int max);

};