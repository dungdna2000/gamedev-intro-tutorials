#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"

/*
	Initialize game object and load a texture
*/
CGameObject::CGameObject(LPCWSTR texturePath)
{
	x = y = 0;
	texture = CGame::GetInstance()->LoadTexture(texturePath);
}

void CGameObject::Update(DWORD dt)
{
}

void CGameObject::Render()
{
	CGame::GetInstance()->Draw(x, y, texture);
}


CGameObject::~CGameObject()
{
	if (texture != NULL) texture->Release();
}

void CMario::Update(DWORD dt)
{
	x += 0.1f*dt;
	if (x > 320) x = 0;
}
