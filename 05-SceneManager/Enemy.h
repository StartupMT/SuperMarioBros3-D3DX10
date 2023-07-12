#pragma once
#include "GameObject.h"

#define ID_ANI_ENEMY 200000

#define ENEMY_BBOX_WIDTH 16
#define ENEMY_BBOX_HEIGHT 14
#define ENEMY_BBOX_HEIGHT_DIE 7

#define ENEMY_DIE_TIMEOUT 500

class CEnemy : public CGameObject
{
protected:

	ULONGLONG die_start;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CEnemy(float x, float y);
	virtual void SetState(int state);
};