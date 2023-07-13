#include "BlockItem.h"


CBlockItem::CBlockItem(float x, float y, float w, float h) :CBlock(x, y, w, h)
{
	this->w = w;
	this->h = h;
	isDraw = false;
}

void CBlockItem::Render()
{
	if (isDraw)
		CAnimations::GetInstance()->Get(ID_ANI_BLOCK + state + type)->Render(x, y);
	if (kind == BLOCK_KIND_WALL)
		RenderBoundingBox();
	RenderBoundingBox();
}

void CBlockItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy = OBJECT_GRAVITY;
	if ((state == OBJECT_STATE_DIE) && (GetTickCount64()))
	{
		isDeleted = true;
		return;
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}