#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = 0.07f;
}

void CGameObject::Update(DWORD dt)
{
	x += vx*dt;
	if ((vx>0 && x > 290) || (x < 0 && vx<0)) vx=-vx;
}

void CGameObject::Render()
{
	LPANIMATION ani;
	if (vx>0) ani = animations[0]; else ani = animations[1];
	//ani = animations[0];
	ani->Render(x, y);
}

void CGameObject::AddAnimation(int aniId)
{
	LPANIMATION ani = CAnimations::GetInstance()->Get(aniId);
	animations.push_back(ani);
}



CGameObject::~CGameObject()
{
}