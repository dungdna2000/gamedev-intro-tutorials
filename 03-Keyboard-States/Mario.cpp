#include "Mario.h"

void CMario::Update(DWORD dt)
{
	x += vx * dt;
	y += vy * dt; 

	// simple fall down
	vy += MARIO_GRAVITY*dt;

	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	DebugOutTitle(L"vx = %0.5f", this->vx);


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
		if (abs(ax) == MARIO_ACCEL_RUN_X) // TODO: need to optimize this
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
		if (ax < 0)
			aniId = ID_ANI_MARIO_BRACE_RIGHT;
		else if (ax == MARIO_ACCEL_RUN_X)
			aniId = ID_ANI_MARIO_RUNNING_RIGHT;
		else if (ax == MARIO_ACCEL_WALK_X)
			aniId = ID_ANI_MARIO_WALKING_RIGHT;
	}
	else // vx < 0
	{
		if (ax > 0)
			aniId = ID_ANI_MARIO_BRACE_LEFT;
		else if (ax == -MARIO_ACCEL_RUN_X)
			aniId = ID_ANI_MARIO_RUNNING_LEFT;
		else if (ax == -MARIO_ACCEL_WALK_X)
			aniId = ID_ANI_MARIO_WALKING_LEFT;
	}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	float d = 0;
	if (isSitting) d = MARIO_SIT_HEIGHT_ADJUST;

	animations->Get(aniId)->Render(x, y+d);
}

void CMario::SetState(int state)
{
	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
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
		ax = 0.0f;
		vx = 0.0f;
		break;
	}

	CGameObject::SetState(state);
}

