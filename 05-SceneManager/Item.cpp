#include "Item.h"
#include "Mario.h"

CItem::CItem(float x, float y) :CGameObject(x, y)
{
	SetState(OBJECT_STATE_STAND);
}

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
	RenderBoundingBox(true);
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == OBJECT_STATE_JUMP)
		JumpState();
	else
		vy = OBJECT_GRAVITY;
	if (type == ITEM_TYPE_COIN)
	{
		if (state == OBJECT_STATE_JUMP)
		{
			if (vy > 0)
			{
				Delete();
				CMario::GetInstance()->AddCoin();
			}
		}
		else
			vy = 0;
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - ITEM_BBOX_WIDTH / 2;
	t = y - ITEM_BBOX_HEIGHT;
	r = l + ITEM_BBOX_WIDTH;
	b = y;
}


void CItem::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (e->obj->GetTag() == OBJECT_TAG_ENEMY || e->obj->GetTag() == OBJECT_TAG_ITEM) return;

	if (e->ny < 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}