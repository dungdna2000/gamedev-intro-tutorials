#pragma once
#include <Windows.h>
class CGameTimer
{
protected: 
	float speed; 
	static CGameTimer* __instance;

public:
	ULONGLONG GetTickCount()
	{
		return GetTickCount64();
	}

	CGameTimer()
	{
		speed = 1.0f;
	}

	void setSpeed(float sp)
	{
		speed = sp;
	}

	static CGameTimer* GetInstance();
};
typedef CGameTimer* LPGAMETIMER;

