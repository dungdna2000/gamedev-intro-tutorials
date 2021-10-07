/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 05 - SCENE MANAGER

	This sample illustrates how to:

		1/ Read scene (textures, sprites, animations and objects) from files 
		2/ Handle multiple scenes in game

	Key classes/functions:
		CScene
		CPlayScene		


HOW TO INSTALL Microsoft.DXSDK.D3DX
===================================
1) Tools > NuGet package manager > Package Manager Console
2) execute command :  Install-Package Microsoft.DXSDK.D3DX


================================================================ */

#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <list>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Animation.h"
#include "Animations.h"

#include "Mario.h"
#include "Brick.h"
#include "Goomba.h"
#include "Coin.h"
#include "Platform.h"

#include "SampleKeyEventHandler.h"

#include "AssetIDs.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"
#define WINDOW_ICON_PATH L"mario.ico"

#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255, 0.0f)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

void LoadAssetsMario()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPTEXTURE texMario = textures->Get(ID_TEX_MARIO);

	// IDLE
	sprites->Add(ID_SPRITE_MARIO_BIG_IDLE_RIGHT + 1, 246, 154, 246 + 13, 154 + 26, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_IDLE_LEFT + 1, 186, 154, 186 + 13, 154 + 26, texMario);

	// WALKING LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 2, 275, 154, 290, 181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 3, 304, 154, 321, 181, texMario);

	// WALKING RIGHT
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 2, 155, 154, 170, 181, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 3, 125, 154, 140, 181, texMario);

	// RUNNING RIGHT 
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 1, 335, 154, 335 + 18, 154 + 26, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 2, 363, 154, 363 + 18, 154 + 26, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 3, 393, 154, 393 + 18, 154 + 26, texMario);

	// RUNNING LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 1, 92, 154, 92 + 18, 154 + 26, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 2, 66, 154, 66 + 18, 154 + 26, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 3, 35, 154, 35 + 18, 154 + 26, texMario);

	// JUMP WALK RIGHT & LEFT 
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_RIGHT + 1, 395, 275, 395 + 16, 275 + 25, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_LEFT + 1, 35, 275, 35 + 16, 275 + 25, texMario);

	// JUMP RUN RIGHT & LEFT 
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_RIGHT + 1, 395, 195, 395 + 18, 195 + 25, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_LEFT + 1, 33, 195, 33 + 18, 195 + 25, texMario);

	// SIT RIGHT/LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_SIT_RIGHT + 1, 426, 239, 426 + 14, 239 + 17, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_SIT_LEFT + 1, 5, 239, 5 + 14, 239 + 17, texMario);

	// BRACING RIGHT/LEFT
	sprites->Add(ID_SPRITE_MARIO_BIG_BRACE_RIGHT + 1, 425, 154, 425 + 15, 154 + 27, texMario);
	sprites->Add(ID_SPRITE_MARIO_BIG_BRACE_LEFT + 1, 5, 154, 5 + 15, 154 + 27, texMario);

	// MARIO DIE
	sprites->Add(ID_SPRITE_MARIO_DIE + 1, 215, 120, 231, 135, texMario);

	// SMALL MARIO 
	sprites->Add(ID_SPRITE_MARIO_SMALL_IDLE_RIGHT + 1, 247, 0, 259, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_IDLE_LEFT + 1, 187, 0, 198, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_RIGHT + 2, 275, 0, 291, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_RIGHT + 3, 306, 0, 320, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_LEFT + 2, 155, 0, 170, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_WALKING_LEFT + 3, 125, 0, 139, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 1, 275, 0, 275 + 15, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 2, 306, 0, 306 + 15, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 3, 335, 0, 335 + 15, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 1, 155, 0, 155 + 15, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 2, 125, 0, 125 + 15, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 3, 95, 0, 95 + 15, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_BRACE_LEFT + 1, 6, 0, 6 + 13, 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_BRACE_RIGHT + 1, 426, 0, 426 + 13, 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_LEFT + 1, 35, 80, 35 + 15, 80 + 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_RIGHT + 1, 395, 80, 395 + 15, 80 + 15, texMario);

	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_LEFT + 1, 65, 40, 65 + 15, 40 + 15, texMario);
	sprites->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_RIGHT + 1, 365, 40, 365 + 15, 40 + 15, texMario);

	LPANIMATION ani;

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_IDLE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_WALKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_IDLE_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_WALKING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_WALKING_LEFT, ani);

	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_RUNNING_RIGHT, ani);

	// Mario runs faster hence animation speed should be faster
	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_BIG_RUNNING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_RUNNING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_WALK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_WALK_LEFT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_WALK_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_RUN_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_JUMP_RUN_LEFT + 1);
	animations->Add(ID_ANI_MARIO_JUMP_RUN_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_SIT_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SIT_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_SIT_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SIT_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_BRACE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_BRACE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_BIG_BRACE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_BRACE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_DIE + 1);
	animations->Add(ID_ANI_MARIO_DIE, ani);

	//
	// SMALL MARIO 
	//
	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_SMALL_WALKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_IDLE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_IDLE_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_SMALL_WALKING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_SMALL_WALKING_LEFT, ani);

	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 2);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT + 3);
	animations->Add(ID_ANI_MARIO_SMALL_RUNNING_RIGHT, ani);

	ani = new CAnimation(50);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 1);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 2);
	ani->Add(ID_SPRITE_MARIO_SMALL_RUNNING_LEFT + 3);
	animations->Add(ID_ANI_MARIO_SMALL_RUNNING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_BRACE_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_BRACE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_BRACE_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_BRACE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_WALK_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_LEFT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_MARIO_SMALL_JUMP_RUN_RIGHT + 1);
	animations->Add(ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT, ani);
}


LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	CGame::GetInstance()->GetCurrentScene()->Update(dt);
}

/*
	Render a frame 
*/
void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

	spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

	FLOAT NewBlendFactor[4] = { 0,0,0,0 };
	pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

	CGame::GetInstance()->GetCurrentScene()->Render();

	spriteHandler->End();
	pSwapChain->Present(0, 0);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = (DWORD)(now - frameStart);

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			CGame::GetInstance()->ProcessKeyboard();			
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);	
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	SetDebugWindow(hWnd);

	LPGAME game = CGame::GetInstance();
	game->Init(hWnd, hInstance);
	game->InitKeyboard();


	//IMPORTANT: this is the only place where hardcoded a file name is allowed ! 
	game->Load(L"mario-sample.txt");  

	//TODO: move to asset file 
	LoadAssetsMario();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH*2, SCREEN_HEIGHT*2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}