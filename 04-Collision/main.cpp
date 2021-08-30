/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 04 - COLLISION

	This sample illustrates how to:

		1/ Implement SweptAABB algorithm between moving objects
		2/ Implement a simple (yet effective) collision frame work

	Key functions: 
		CGame::SweptAABB
		CGameObject::SweptAABBEx
		CGameObject::CalcPotentialCollisions
		CGameObject::FilterCollision

		CGameObject::GetBoundingBox
		
================================================================ */

#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Animation.h"
#include "Animations.h"

#include "Mario.h"
#include "Brick.h"
#include "Goomba.h"

#include "SampleKeyEventHandler.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"

#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255, 0.0f)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20

#define TEXTURES_DIR L"textures"
#define TEXTURE_PATH_MARIO TEXTURES_DIR "\\mario.png"
#define TEXTURE_PATH_MISC TEXTURES_DIR "\\misc.png"
#define TEXTURE_PATH_ENEMY TEXTURES_DIR "\\enemies.png"
#define TEXTURE_PATH_BBOX TEXTURES_DIR "\\bbox.png"

#define MARIO_START_X 50.0f
#define MARIO_START_Y 10.0f

#define ID_SPRITE_BRICK 20001

#define BRICK_X 0.0f
#define BRICK_Y GROUND_Y + 20.0f
#define NUM_BRICKS 50


CGame *game;

CMario *mario;
CGoomba *goomba;

vector<LPGAMEOBJECT> objects;

CSampleKeyHandler * keyHandler; 

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
	Load all game resources 
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{

/*
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MARIO, TEXTURE_PATH_MARIO);
	textures->Add(ID_TEX_MISC, TEXTURE_PATH_MISC);
	textures->Add(ID_TEX_ENEMY, TEXTURE_PATH_ENEMY);
	textures->Add(ID_TEX_BBOX, TEXTURE_PATH_BBOX);


	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	
	LPTEXTURE texMario = textures->Get(ID_TEX_MARIO);

	// big
	sprites->Add(10001, 246, 154, 260, 181, texMario);		// idle right

	sprites->Add(10002, 275, 154, 290, 181, texMario);		// walk
	sprites->Add(10003, 304, 154, 321, 181, texMario);

	sprites->Add(10011, 186, 154, 200, 181, texMario);		// idle left
	sprites->Add(10012, 155, 154, 170, 181, texMario);		// walk
	sprites->Add(10013, 125, 154, 140, 181, texMario);

	sprites->Add(10099, 215, 120, 231, 135, texMario);		// die 

	// small
	sprites->Add(10021, 247, 0, 259, 15, texMario);			// idle small right
	sprites->Add(10022, 275, 0, 291, 15, texMario);			// walk 
	sprites->Add(10023, 306, 0, 320, 15, texMario);			// 

	sprites->Add(10031, 187, 0, 198, 15, texMario);			// idle small left

	sprites->Add(10032, 155, 0, 170, 15, texMario);			// walk
	sprites->Add(10033, 125, 0, 139, 15, texMario);			// 


	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(20001, 408, 225, 424, 241, texMisc);

	LPTEXTURE texEnemy = textures->Get(ID_TEX_ENEMY);
	sprites->Add(30001, 5, 14, 21, 29, texEnemy);
	sprites->Add(30002, 25, 14, 41, 29, texEnemy);

	sprites->Add(30003, 45, 21, 61, 29, texEnemy); // die sprite

	LPANIMATION ani;

	ani = new CAnimation(100);	// idle big right
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(100);	// idle big left
	ani->Add(10011);
	animations->Add(401, ani);

	ani = new CAnimation(100);	// idle small right
	ani->Add(10021);
	animations->Add(402, ani);

	ani = new CAnimation(100);	// idle small left
	ani->Add(10031);
	animations->Add(403, ani);

	ani = new CAnimation(100);	// walk right big
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	animations->Add(500, ani);

	ani = new CAnimation(100);	// // walk left big
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	animations->Add(501, ani);

	ani = new CAnimation(100);	// walk right small
	ani->Add(10021);
	ani->Add(10022);
	ani->Add(10023);
	animations->Add(502, ani);

	ani = new CAnimation(100);	// walk left small
	ani->Add(10031);
	ani->Add(10032);
	ani->Add(10033);
	animations->Add(503, ani);


	ani = new CAnimation(100);		// Mario die
	ani->Add(10099);
	animations->Add(599, ani);

	

	ani = new CAnimation(100);		// brick
	ani->Add(20001);
	animations->Add(601, ani);

	ani = new CAnimation(300);		// Goomba walk
	ani->Add(30001);
	ani->Add(30002);
	animations->Add(701, ani);

	ani = new CAnimation(1000);		// Goomba dead
	ani->Add(30003);
	animations->Add(702, ani);

	mario = new CMario(50.0f, 0.0f);
	mario->AddAnimation(400);		// idle right big
	mario->AddAnimation(401);		// idle left big
	mario->AddAnimation(402);		// idle right small
	mario->AddAnimation(403);		// idle left small

	mario->AddAnimation(500);		// walk right big
	mario->AddAnimation(501);		// walk left big
	mario->AddAnimation(502);		// walk right small
	mario->AddAnimation(503);		// walk left big

	mario->AddAnimation(599);		// die

	mario->SetPosition(50.0f, 0);
	objects.push_back(mario);

	for (int i = 0; i < 5; i++)
	{
		CBrick *brick = new CBrick();
		brick->AddAnimation(601);
		brick->SetPosition(100.0f + i*60.0f, 74.0f);
		objects.push_back(brick);

		brick = new CBrick();
		brick->AddAnimation(601);
		brick->SetPosition(100.0f + i*60.0f, 90.0f);
		objects.push_back(brick);

		brick = new CBrick();
		brick->AddAnimation(601);
		brick->SetPosition(84.0f + i*60.0f, 90.0f);
		objects.push_back(brick);
	}


	for (int i = 0; i < 30; i++)
	{
		CBrick *brick = new CBrick();
		brick->AddAnimation(601);
		brick->SetPosition(0 + i*16.0f, 150);
		objects.push_back(brick);
	}

	// and Goombas 
	for (int i = 0; i < 4; i++)
	{
		goomba = new CGoomba();
		goomba->AddAnimation(701);
		goomba->AddAnimation(702);
		goomba->SetPosition(200.0f + i*60.0f, 135.0f);
		goomba->SetState(GOOMBA_STATE_WALKING);
		objects.push_back(goomba);
	}
*/
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MARIO, TEXTURE_PATH_MARIO);
	textures->Add(ID_TEX_MISC, TEXTURE_PATH_MISC);
	textures->Add(ID_TEX_BBOX, TEXTURE_PATH_BBOX);


	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	
	LPTEXTURE texMario = textures->Get(ID_TEX_MARIO);

	sprites->Add(10001, 246, 154, 260, 181, texMario);

	sprites->Add(10002, 275, 154, 290, 181, texMario);
	sprites->Add(10003, 304, 154, 321, 181, texMario);

	sprites->Add(10011, 186, 154, 200, 181, texMario);

	sprites->Add(10012, 155, 154, 170, 181, texMario);
	sprites->Add(10013, 125, 154, 140, 181, texMario);

	// RUNNING RIGHT 
	sprites->Add(10021, 335, 154, 335 + 18, 154 +26, texMario);
	sprites->Add(10022, 363, 154, 363 + 18, 154 + 26, texMario);
	sprites->Add(10023, 393, 154, 393 + 18, 154 + 26, texMario);

	// RUNNING LEFT
	sprites->Add(10031, 92, 154, 92 + 18, 154 + 26, texMario);
	sprites->Add(10032, 66, 154, 66 + 18, 154 + 26, texMario);
	sprites->Add(10033, 35, 154, 35 + 18, 154 + 26, texMario);

	// JUMP WALK RIGHT & LEFT 
	sprites->Add(10041, 395, 275, 395 + 16, 275 + 25, texMario);
	sprites->Add(10042, 35, 275, 35 + 16, 275 + 25, texMario);

	// JUMP RUN RIGHT & LEFT 
	sprites->Add(10043, 395, 195, 395 + 18, 195 + 25, texMario);
	sprites->Add(10044, 33, 195, 33 + 18, 195 + 25, texMario);

	// SIT RIGHT/LEFT
	sprites->Add(10051, 426, 239, 426 + 14, 239 + 17, texMario);
	sprites->Add(10052, 5, 239, 5 + 14, 239 + 17, texMario);

	// BRACING RIGHT/LEFT
	sprites->Add(10061, 425, 154, 425 + 15, 154 + 27, texMario);
	sprites->Add(10062, 5, 154, 5 + 15, 154 + 27, texMario);

	LPANIMATION ani;

	ani = new CAnimation(100);	
	ani->Add(10001);
	animations->Add(ID_ANI_MARIO_IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(10011);
	animations->Add(ID_ANI_MARIO_IDLE_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	animations->Add(ID_ANI_MARIO_WALKING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	animations->Add(ID_ANI_MARIO_WALKING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(10021);
	ani->Add(10022);
	ani->Add(10023);
	animations->Add(ID_ANI_MARIO_RUNNING_RIGHT, ani);

	ani = new CAnimation(50);	// Mario runs faster hence animation speed should be faster
	ani->Add(10031);
	ani->Add(10032);
	ani->Add(10033);
	animations->Add(ID_ANI_MARIO_RUNNING_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(10041);
	animations->Add(ID_ANI_MARIO_JUMP_WALK_RIGHT, ani);

	ani = new CAnimation(100);	
	ani->Add(10042);
	animations->Add(ID_ANI_MARIO_JUMP_WALK_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(10043);
	animations->Add(ID_ANI_MARIO_JUMP_RUN_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(10044);
	animations->Add(ID_ANI_MARIO_JUMP_RUN_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(10051);
	animations->Add(ID_ANI_MARIO_SIT_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(10052);
	animations->Add(ID_ANI_MARIO_SIT_LEFT, ani);

	ani = new CAnimation(100);
	ani->Add(10061);
	animations->Add(ID_ANI_MARIO_BRACE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->Add(10062);
	animations->Add(ID_ANI_MARIO_BRACE_LEFT, ani);

	mario = new CMario(MARIO_START_X, MARIO_START_Y);
	objects.push_back(mario);

	// Brick objects 
	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(ID_SPRITE_BRICK, 372, 153, 372+15, 153+15, texMisc);

	ani = new CAnimation(100);
	ani->Add(ID_SPRITE_BRICK);
	animations->Add(ID_ANI_BRICK,ani);

	for (int i=0;i<NUM_BRICKS;i++) 
	{
		CBrick* b = new CBrick(BRICK_X + i * BRICK_WIDTH, BRICK_Y);
		objects.push_back(b);
	}

	for (int i = 0; i < 6; i++)
	{
		CBrick* b = new CBrick(BRICK_X + i * BRICK_WIDTH, BRICK_Y-40.0f);
		objects.push_back(b);
	}

	for (int i = 0; i < 6; i++)
	{
		CBrick* b = new CBrick(BRICK_X + 100.0f + i * BRICK_WIDTH, BRICK_Y - 80.0f);
		objects.push_back(b);
	}
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt,&coObjects);
	}


	// Update camera to follow mario
	float cx, cy;
	mario->GetPosition(cx, cy);

	cx -= SCREEN_WIDTH / 2;
	cy -= SCREEN_HEIGHT / 2;

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
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

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}

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
	wc.hIcon = NULL;
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
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();
			
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);	
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	SetDebugWindow(hWnd);

	game = CGame::GetInstance();
	game->Init(hWnd, hInstance);

	keyHandler = new CSampleKeyHandler();
	game->InitKeyboard(keyHandler);

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH*2, SCREEN_HEIGHT*2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}