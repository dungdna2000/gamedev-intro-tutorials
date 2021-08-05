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
	int aniId = -1;

	// Mario is still on air check, this will not work when Mario is just stand up
	if (y < GROUND_Y)   
	{
		if (abs(vx) == MARIO_RUNNING_SPEED)
		{
			if (nx >= 0) 
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else 
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else 
		{
			if (nx >= 0) 
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else 
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else 
	if (isSitting)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_SIT_RIGHT;
		else
			aniId = ID_ANI_MARIO_SIT_LEFT;
	}
	else 
	if (vx == 0)
	{
		if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
		else aniId = ID_ANI_MARIO_IDLE_LEFT;
	}
	else if (vx > 0)
	{
		if (vx == MARIO_RUNNING_SPEED)
			aniId = ID_ANI_MARIO_RUNNING_RIGHT;
		else if (vx == MARIO_WALKING_SPEED)
			aniId = ID_ANI_MARIO_WALKING_RIGHT;
	}
	else
	{
		if (vx == -MARIO_RUNNING_SPEED)
			aniId = ID_ANI_MARIO_RUNNING_LEFT;
		else if (vx == -MARIO_WALKING_SPEED)
			aniId = ID_ANI_MARIO_WALKING_LEFT;
	}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	int d = 0;
	if (isSitting) d = MARIO_SIT_HEIGHT_ADJUST;

	animations->Get(aniId)->Render(x, y+d);
}

void CMario::SetState(int state)
{
	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		vx = MARIO_RUNNING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		vx = -MARIO_RUNNING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP: 
		if (isSitting) break;
		if (y == GROUND_Y)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else 
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;
			
	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y/2;
		break;

	case MARIO_STATE_SIT: 
		if (y == GROUND_Y) 
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0;
			//y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		isSitting = false;
		state = MARIO_STATE_IDLE;
		//y -= MARIO_SIT_HEIGHT_ADJUST;
		break;

	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	}

	CGameObject::SetState(state);
}

