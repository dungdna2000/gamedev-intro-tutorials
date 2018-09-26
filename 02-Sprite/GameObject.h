#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"


using namespace std;

class CGameObject
{
	float x; 
	float y;

	float vx;

	int currentState;

	vector<LPANIMATION> animations;

public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetState(int state) { this->currentState = state; }
	void AddAnimation(int aniId);

	CGameObject();

	void Update(DWORD dt);
	void Render();
	~CGameObject();
};