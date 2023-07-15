#include "Koopa.h"

CKoopa::CKoopa(float x, float y) :CEnemy(x, y)
{
	vy = OBJECT_GRAVITY;
	die_start = -1;
	SetState(OBJECT_STATE_RUN);
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopa*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
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