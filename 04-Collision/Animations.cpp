#include "Animations.h"
#include "debug.h"

CAnimations* CAnimations::__instance = NULL;

CAnimations* CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(int id, LPANIMATION ani)
{
	if (animations[id] != NULL)
		DebugOut(L"[WARNING] Animation %d already exists", id);

	animations[id] = ani;
}

LPANIMATION CAnimations::Get(int id)
{
	return animations[id];
}