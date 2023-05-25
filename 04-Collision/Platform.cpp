#include "Platform.h"

#include "Sprite.h"
#include "Sprites.h"

void CPlatform::Render()
{
	if (this->length <= 0) return; 
	float xx = x; 
	CSprites * s = CSprites::GetInstance();

	s->Get(this->spriteIdBegin)->Draw(xx, y);
	xx += this->cellWidth;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddle)->Draw(xx, y);
		xx += this->cellWidth;
	}
	if (length>1)
		s->Get(this->spriteIdEnd)->Draw(xx, y);

	//
	//RenderBoundingBox();
	//
}

void CPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth * this->length - cellWidth_div_2;
	b = t + this->cellHeight;
}

int CPlatform::IsDirectionColliable(float nx, float ny)
{
	if (nx == 0 && ny == -1) return 1;
	else return 0;
}
