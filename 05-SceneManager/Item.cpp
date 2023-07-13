#include "Item.h"

void CItem::Render()
{
	int aniId = ID_ANI_ITEM + type + state + kind;
	LPANIMATION anim = CAnimations::GetInstance()->Get(aniId);

	if (anim != NULL)
	{
		float l, t, r, b;
		GetBoundingBox(l, t, r, b);
		anim->Render(x, y, (b - t) / 2, vx);
	}
}

void CItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - ITEM_BBOX_WIDTH / 2;
	t = y - ITEM_BBOX_HEIGHT;
	r = l + ITEM_BBOX_WIDTH;
	b = y;
}