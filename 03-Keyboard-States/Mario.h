#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED 0.05f
#define MARIO_JUMP_SPEED_X	1.0f
#define MARIO_JUMP_SPEED_Y	1.0f
#define MARIO_GRAVITY		0.1f



#define MARIO_LEVEL_SMALL	0
#define MARIO_LEVEL_SUPER	1
#define MARIO_LEVEL_FIRE	2

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300


#define MARIO_ANI_IDLE_RIGHT	0
#define MARIO_ANI_IDLE_LEFT		1
#define MARIO_ANI_WALKING_RIGHT		2
#define MARIO_ANI_WALKING_LEFT		3

class CMario : public CGameObject
{
	int level; 
public: 

	void Update(DWORD dt);
	void Render();
	int GetLevel() { return level; }

	void SetState(int state);
};