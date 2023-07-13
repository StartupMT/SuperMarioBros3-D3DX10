#include "Enemy.h"

CEnemy::CEnemy(float x, float y):CGameObject(x, y)
{
	die_start = -1;
	SetState(OBJECT_STATE_RUN);
}

void CEnemy::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == OBJECT_STATE_DIE)
	{
		left = x - ENEMY_BBOX_WIDTH / 2;
		top = y - ENEMY_BBOX_HEIGHT_DIE / 2;
		right = left + ENEMY_BBOX_WIDTH;
		bottom = y;
	}
	else
	{
		left = x - ENEMY_BBOX_WIDTH / 2;
		top = y - ENEMY_BBOX_HEIGHT / 2;
		right = left + ENEMY_BBOX_WIDTH;
		bottom = y;
	}
}

void CEnemy::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CEnemy::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (e->obj->GetTag() == OBJECT_TAG_ENEMY || e->obj->GetTag() == OBJECT_TAG_ITEM || state == OBJECT_STATE_DIE) return;

	if (e->ny < 0)
	{
		vy = 0;
		if (isJumpE)
			StartJump();
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (state == OBJECT_STATE_JUMP)
		JumpState();
	else
		vy = OBJECT_GRAVITY;
	if ((state == OBJECT_STATE_DIE) && (GetTickCount64() - die_start > ENEMY_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CEnemy::Render()
{
	int aniId = ID_ANI_ENEMY + type + state + kind;
	LPANIMATION anim = CAnimations::GetInstance()->Get(aniId);

	if (anim != NULL)
	{
		float l, t, r, b;
		GetBoundingBox(l, t, r, b);
		anim->Render(x, y, (b - t) / 2, vx);
	}
	RenderBoundingBox();
}

void CEnemy::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case OBJECT_STATE_DIE:
		if (isJumpE)
		{
			isJumpE = false;
			SetState(OBJECT_STATE_RUN);
			return;
		}
		die_start = GetTickCount64();
		y += (ENEMY_BBOX_HEIGHT - ENEMY_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		break;
	case OBJECT_STATE_RUN:
		vy = OBJECT_GRAVITY;
		vx = -OBJECT_RUN_SPEED;
		break;
	}
}
