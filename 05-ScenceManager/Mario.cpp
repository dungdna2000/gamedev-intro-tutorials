#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

#define BLOCK_PUSH_FACTOR 0.4f

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY*dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		ScanCollions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0) {
		x += dx; 
		y += dy;
	}
	else {

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;

		LPCOLLISIONEVENT colX = NULL; 
		LPCOLLISIONEVENT colY = NULL;
		FilterCollision(coEvents, colX, colY);

		if (colX != NULL && colY != NULL) {
			if (colY->t < colX->t) { // if collision on Y first 
				//push back on Y as usual
				y += colY->t*dy + colY->ny*BLOCK_PUSH_FACTOR;
				vy = 0; dy = 0;

				colY->isDeleted = true;

				coEventsResult.push_back(colY);

				// check if still collide on X ?
				LPCOLLISIONEVENT colX_ = SweptAABBEx(colX->obj);
				if (colX_->t > 0 && colX_->t <= 1.0f) {
					x += colX_->t*dx + colX_->nx*BLOCK_PUSH_FACTOR;
					vx = 0; dx = 0; 

					coEventsResult.push_back(colX_); // be aware of memory leak !!

					DebugOut(L"Mario hit on X (case 2) \n");
				}
				else {
					colX->isDeleted = true; 

					// search "next" collision on X
					LPCOLLISIONEVENT colX_next,colY_next;
					FilterCollision(coEvents, colX_next, colY_next);
					if (colX_next != NULL) {
						x += colX_next->t*dx + colX_next->nx*BLOCK_PUSH_FACTOR;
						vx = 0; dx = 0;

						coEventsResult.push_back(colX_next);

						DebugOut(L"Mario hit on X (case 3) \n");
					}
					else 
						x += dx;
				}
			}
			else { // collision on X first
				//TODO: similar logic as Y first 
				//push back both X & Y 
				x += colX->t*dx + colX->nx*BLOCK_PUSH_FACTOR;
				y += colY->t*dy + colY->ny*BLOCK_PUSH_FACTOR;

				coEventsResult.push_back(colX);
				coEventsResult.push_back(colY);
			}
		}
		else if (colX!= NULL ) {
			x += colX->t*dx + colX->nx*BLOCK_PUSH_FACTOR;
			vx = 0;
			dx = 0; 

			y += dy;

			coEventsResult.push_back(colX);
			
			DebugOut(L"Mario hit on X \n");
		}
		else { // colY != NULL
			y += colY->t*dy + colY->ny*BLOCK_PUSH_FACTOR;
			vy = 0;
			dy = 0; 

			coEventsResult.push_back(colY);


			x += dx;
		}

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else 
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}

	}

	// clean up collision events
	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else
	if (level == MARIO_LEVEL_BIG)
	{
		if (vx == 0)
		{
			if (nx>0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
			else ani = MARIO_ANI_BIG_IDLE_LEFT;
		}
		else if (vx > 0) 
			ani = MARIO_ANI_BIG_WALKING_RIGHT; 
		else ani = MARIO_ANI_BIG_WALKING_LEFT;
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (vx == 0)
		{
			if (nx>0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else if (vx > 0)
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
		else ani = MARIO_ANI_SMALL_WALKING_LEFT;
	}

	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

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
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		vy = -MARIO_JUMP_SPEED_Y;
		break; 
	case MARIO_STATE_IDLE: 
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 

	if (level==MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

