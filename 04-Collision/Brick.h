#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};