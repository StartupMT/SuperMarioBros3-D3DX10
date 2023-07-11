#include "Block.h"


CBlock::CBlock(float x, float y, float w, float h) :CGameObject(x, y)
{
	this->w = w;
	this->h = h;
	isDraw = false;
}

void CBlock::Render()
{
	if (isDraw)
		CAnimations::GetInstance()->Get(ID_ANI_BLOCK + state + type)->Render(x, y);
	if (kind == BLOCK_KIND_WALL)
		RenderBoundingBox();
	RenderBoundingBox();
}

void CBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - w / 2;
	t = y - h / 2;
	r = l + w;
	b = t + h;
}