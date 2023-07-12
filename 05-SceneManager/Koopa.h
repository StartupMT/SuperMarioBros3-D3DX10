#pragma once
#include "Enemy.h"

class CKoopa : public CEnemy
{
public:
	CKoopa(float x, float y);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};