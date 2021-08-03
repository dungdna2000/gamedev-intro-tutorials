#include "Sprite.h"

void CSprite::Draw(float x, float y)
{
	CGame* g = CGame::GetInstance();

	D3DXMATRIX matTranslation;
	D3DXMatrixTranslation(&matTranslation, x, (g->GetBackBufferHeight() - y), 0.1f);
	this->sprite.matWorld = (this->matScaling * matTranslation);

	g->GetSpriteHandler()->DrawSpritesImmediate(&sprite, 1, 0, 0);
}

