#include <algorithm>

#include "Mario.h"
#include "Game.h"
#include "debug.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY*dt;


	//
	// Handle collision here
	//

	float t, nx, ny;

	vector<LPCOLLISIONEVENT> coEvents;

	for (int i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT coO = coObjects->at(i);

		float sl, st, sr, sb;
		float ml, mt, mr, mb;

		coO->GetBoundingBox(sl, st, sr, sb);
		GetBoundingBox(ml, mt, mr, mb);

		CGame::CheckCollision(
			ml, mt, mr, mb,
			dx, dy,
			sl, st, sr, sb,
			t, nx, ny
		);

		//DebugOut(L">> i: %d t:%0.5f n(%0.1f,%0.1f) obj(%1.0f,%1.0f) m(%0.5f,%0.5f)\n", i, t, nx, ny, coO->x, coO->y, this->x, this->y);

		if (t > 0 && t <= 1.0f)
		{
			CCollisionEvent * e = new CCollisionEvent(t, nx, ny, coO);
			coEvents.push_back(e);
		}
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		x += dx; 
		y += dy;
	}
	else
	{
		std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);

		float min_tx = 1.0f;
		float min_ty = 1.0f;
		float nx = 0;
		float ny = 0;

		for (int i = 0; i < coEvents.size(); i++)
		{
			LPCOLLISIONEVENT c = coEvents[i];

			float x, y;
			c->obj->GetPosition(x, y);

			//DebugOut(L">> i: %d t:%0.5f n(%0.1f,%0.1f) obj(%1.0f,%1.0f) m(%0.5f,%0.5f)\n", i, c->t, c->nx, c->ny, x, y, this->x, this->y);

			if (c->t < min_tx && c->nx != 0) {
				min_tx = c->t; nx = c->nx;
			} 
			if (c->t < min_ty  && c->ny != 0) {
				min_ty = c->t; ny = c->ny;
			}
		}

		// block
		x += min_tx*dx + nx*0.1f;
		y += min_ty*dy + ny*0.1f;
		
		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;

		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}

	//if (y > 100) { y = 100; vy = 0; }
}

void CMario::Render()
{
	int ani;
	if (vx == 0)
	{
		if (nx>0) ani = MARIO_ANI_IDLE_RIGHT;
		else ani = MARIO_ANI_IDLE_LEFT;
	}
	else if (vx > 0) 
		ani = MARIO_ANI_WALKING_RIGHT; 
	else ani = MARIO_ANI_WALKING_LEFT;

	animations[ani]->Render(x, y);

	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT: 
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP: 
		//if (state!=MARIO_STATE_JUMP)
			vy = -MARIO_JUMP_SPEED_Y;

	case MARIO_STATE_IDLE: 
		vx = 0;
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 
	right = x + MARIO_BIG_BBOX_WIDTH;
	bottom = y + MARIO_BIG_BBOX_HEIGHT;
}

