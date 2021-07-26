#pragma once
#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>


#define MAX_FRAME_RATE 60

/*
	The game framework!
*/
class CGame
{
	static CGame * __instance;
	HWND hWnd;									// Window handle

	int backBufferWidth = 0;					// Backbuffer width & height, will be set during Direct3D initialization
	int backBufferHeight = 0;

	ID3D10Device* pD3DDevice = NULL;
	IDXGISwapChain* pSwapChain = NULL;
	ID3D10RenderTargetView* pRenderTargetView = NULL;

	ID3DX10Sprite* spriteObject = NULL;				// Sprite handling object 

public:
	// Init DirectX, Sprite Handler
	void Init(HWND hWnd);

	// Draw a full image at position (x,y) on the screen
	void Draw(float x, float y, ID3D10Texture2D * texture);

	ID3D10Texture2D * LoadTexture(LPCWSTR texturePath);

	ID3D10Device * GetDirect3DDevice() { return this->pD3DDevice; }
	IDXGISwapChain* GetSwapChain() { return this->pSwapChain; }
	ID3D10RenderTargetView* GetRenderTargetView() { return this->pRenderTargetView;  }
	ID3DX10Sprite* GetSpriteHandler() { return this->spriteObject; }

	int GetBackBufferWidth() { return backBufferWidth; }
	int GetBackBufferHeight() { return backBufferHeight; }

	static CGame * GetInstance();

	~CGame();
};