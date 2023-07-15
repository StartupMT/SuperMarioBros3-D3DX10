#include "Koopa.h"

CKoopa::CKoopa(float x, float y) :CEnemy(x, y)
{
	vy = OBJECT_GRAVITY;
	die_start = -1;
	SetState(OBJECT_STATE_RUN);
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (e->obj->GetTag() == OBJECT_TAG_ENEMY || e->obj->GetTag() == OBJECT_TAG_ITEM || state == OBJECT_STATE_DIE) return;

	if (e->ny < 0)
	{
		vy = 0;
		float l, t, r, b;
		e->obj->GetBoundingBox(l, t, r, b);
		if (isJumpE)
			StartJump();
		if (x < l)
		{
			vx = OBJECT_RUN_SPEED;
		}
		else if (x > r)
		{
			vx = -OBJECT_RUN_SPEED;
		}
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt, coObjects);
	vy = OBJECT_GRAVITY;
	if ((state == OBJECT_STATE_DIE) && (GetTickCount64() - die_start > ENEMY_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}