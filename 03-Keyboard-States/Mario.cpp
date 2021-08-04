#include "Mario.h"

void CMario::Update(DWORD dt)
{
	x += vx * dt;
	y += vy * dt; 

	// simple fall down
	vy += MARIO_GRAVITY;

	// BAD & sinful platform collision handling, see next sample for correct collision handling
	if (y > GROUND_Y)
	{
		vy = 0; y = GROUND_Y;
	}

	// simple screen edge collision!!!
	if (vx > 0 && x > 290) x = 290;
	if (vx < 0 && x < 0) x = 0;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = 0;

	if (vx == 0)
	{
		if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
		else aniId = ID_ANI_MARIO_IDLE_LEFT;
	}
	else if (vx > 0)
	{
		if (state==MARIO_STATE_RUNNING_RIGHT)
			aniId = ID_ANI_MARIO_RUNNING_RIGHT;
		else if (state == MARIO_STATE_WALKING_RIGHT)
			aniId = ID_ANI_MARIO_WALKING_RIGHT;
	}
	else
	{
		if (state == MARIO_STATE_RUNNING_LEFT)
			aniId = ID_ANI_MARIO_RUNNING_LEFT;
		else if (state == MARIO_STATE_WALKING_LEFT)
			aniId = ID_ANI_MARIO_WALKING_LEFT;
	}

	animations->Get(aniId)->Render(x, y);
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		vx = MARIO_RUNNING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		vx = -MARIO_RUNNING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT: 
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP: 
		if (y== GROUND_Y)
			vy = -MARIO_JUMP_SPEED_Y;

	case MARIO_STATE_IDLE: 
		vx = 0;
		break;
	}
}

