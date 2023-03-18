#include "GameTimer.h"

LPGAMETIMER CGameTimer::__instance = NULL;

LPGAMETIMER CGameTimer::GetInstance()
{
	if (__instance == NULL) __instance = new CGameTimer();
	return __instance;
}
