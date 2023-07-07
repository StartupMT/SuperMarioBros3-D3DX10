#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "debug.h"

#define MARIO_START_LIFE 3
#define ID_ANI_MARIO 100000

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

#define MARIO_UNTOUCHABLE_TIME 2500

class CMario : public CGameObject
{
	static CMario* __instance;

	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	bool untouchable;
	ULONGLONG untouchable_start;

	int coin;
	int life;

	void OnCollisionWithEnemy(LPCOLLISIONEVENT e);
	void OnCollisionWithItem(LPCOLLISIONEVENT e);
	void OnCollisionWithBlock(LPCOLLISIONEVENT e);

public:
	int isRun = false;

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		vy = OBJECT_GRAVITY;

		state = OBJECT_STATE_STAND;
		type = MARIO_TYPE_SMALL;
		untouchable = false;
		untouchable_start = -1;
		coin = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{
		return (state != OBJECT_STATE_DIE);
	}

	int IsBlocking() { return (state != OBJECT_STATE_DIE && untouchable == 0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void StartUntouchable() { untouchable = true; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	static CMario* GetInstance();
};