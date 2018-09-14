#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"

CGameObject::CGameObject(LPCWSTR texturePath)
{
	x = y = 0;

	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(texturePath, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n",texturePath);
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		texturePath,						// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 255, 255),			// Transparent color
		&info,
		NULL,
		&texture);								// Created texture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}

	DebugOut(L"[INFO] Texture loaded Ok: %s \n", texturePath);
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
