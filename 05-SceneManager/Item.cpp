#include "Item.h"

void CItem::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_ITEM + state + type)->Render(x, y);

	RenderBoundingBox();
}

void CItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - ITEM_BBOX_WIDTH / 2;
	t = y - ITEM_BBOX_HEIGHT / 2;
	r = l + ITEM_BBOX_WIDTH;
	b = t + ITEM_BBOX_HEIGHT;
}