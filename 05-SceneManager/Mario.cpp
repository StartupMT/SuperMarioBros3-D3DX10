#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Enemy.h"
#include "Item.h"
#include "Block.h"

#include "Collision.h"

CMario* CMario::__instance = NULL;

CMario* CMario::GetInstance()
{
	if (__instance == NULL) __instance = new CMario(0, 0);
	return __instance;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	isMaxRunCount = accCount >= MARIO_MAX_ACCEL_COUNT;

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = false;
	}
	isCount = false;
	MoveX();
	JumpState();

	if (state == OBJECT_STATE_DIE)
	{
		y += OBJECT_GRAVITY * dt;
		return;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	if (state != OBJECT_STATE_JUMP)
	{
		StartJump(0, 0);
		isFall = true;
	}
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0)
		{
			if (isFall)
				SetState(OBJECT_STATE_STAND);
			vy = OBJECT_GRAVITY;
		}
	}
	else
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = 0;
		}

	if (dynamic_cast<CEnemy*>(e->obj))
		OnCollisionWithEnemy(e);
	else if (dynamic_cast<CItem*>(e->obj))
		OnCollisionWithItem(e);
	else if (dynamic_cast<CBlock*>(e->obj))
		OnCollisionWithBlock(e);
}

void CMario::OnCollisionWithEnemy(LPCOLLISIONEVENT e)
{
	CEnemy* enemy = dynamic_cast<CEnemy*>(e->obj);

	// jump on top >> kill Enemy and deflect a bit 
	if (e->ny < 0)
	{
		if (enemy->GetState() != OBJECT_STATE_DIE)
		{
			StartJump();
			enemy->SetState(OBJECT_STATE_DIE);
		}
	}
	else // hit by Enemy
	{
		if (untouchable == 0)
		{
			if (enemy->GetState() != OBJECT_STATE_DIE)
			{
				state = OBJECT_STATE_DIE;
			}
		}
	}
}

void CMario::OnCollisionWithItem(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithBlock(LPCOLLISIONEVENT e)
{
	//CPortal* p = (CPortal*)e->obj;
	//CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::Render()
{
	int _state = 0;
	if (state == OBJECT_STATE_RUN)
	{
		_state = isBake ? 3 : abs(vx) >= MARIO_MAX_SPEED ? 2 : abs(vx) >= MARIO_RUN_SPEED ? 1 : 0;
		_state = vx == 0 ? 0 : _state;
		state = vx == 0 ? OBJECT_STATE_STAND : state;
	}
	else if (state == OBJECT_STATE_JUMP)
	{
		_state = isFly ? 3 : isSpeedJump ? 2 : isFallDown;
	}
	//Nếu đang trên không trung mà không có bay hoặc đụng đầu thì rơi
	if (!isGround && vy < 0 && !isFly || isCollisionTop)
	{
		//if (state != OBJECT_STATE_DIE)
		//	Fall();
	}
	//Nếu đang tấn công thì đổi trạng thái
	state = isAttack ? OBJECT_STATE_ATTACK : state;

	int aniId = -1;
	aniId = ID_ANI_MARIO + type + state + _state;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y, direction);

	RenderBoundingBox();
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == OBJECT_STATE_DIE) return;

	switch (state)
	{
	case OBJECT_STATE_STAND:
		this->state = state;
		isAllowJump = true;
		isFall = false;
		vy = OBJECT_GRAVITY;
		if (direction != 0 || vx != 0)
		{
			SetState(OBJECT_STATE_RUN);
			return;
		}
		break;
	case OBJECT_STATE_RUN:
		if (this->state != OBJECT_STATE_STAND) return;
		if (isSitting) break;
		//nx = direction;
		break;
	case OBJECT_STATE_JUMP:
		if (this->state != state)
		{
			StartJump(OBJECT_JUMP_SPEED, MARIO_JUMP_HEIGHT);
		}
		break;
	}
	this->state = state;
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (type == MARIO_TYPE_SMALL)
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
	else
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
}

void CMario::MoveX()

{

	if (this->state == OBJECT_STATE_DIE) return;




	if (CGame::GetInstance()->isFixUpdateFrame)

	{

		isCount = isRun;

		if (isCount && !isBake)

			accCount++;

		else

			accCount--;




		accCount = accCount < 0 ? 0 : (accCount > MARIO_MAX_ACCEL_COUNT && isCount) ? MARIO_MAX_ACCEL_COUNT + 10 : accCount;

	}

	float accWalk = CGame::GetInstance()->isFixUpdateFrame ? MARIO_ACCEL_WALK : 0;

	float speed = vx;

	float speedRun = isRun ? MARIO_RUN_SPEED : MARIO_WALK_SPEED;

	speedRun = isMaxRunCount ? MARIO_MAX_SPEED : speedRun;

	isBake = false;

	if (direction > 0 && state != OBJECT_STATE_SIT)

	{

		isBake = speed < 0 && state == OBJECT_STATE_RUN;

		speed = speed < MARIO_WALK_SPEED ? speed + accWalk : speedRun;

	}

	else if (direction < 0 && state != OBJECT_STATE_SIT)

	{

		isBake = speed > 0 && state == OBJECT_STATE_RUN;

		speed = speed > -MARIO_WALK_SPEED ? speed - accWalk : -speedRun;

	}

	else

	{

		if (speed > 0)

		{

			speed = speed - accWalk;

			speed = speed < 0 ? 0 : speed;

		}

		else

		{

			speed = speed + accWalk;

			speed = speed > 0 ? 0 : speed;

		}

	}

	vx = speed;

}
