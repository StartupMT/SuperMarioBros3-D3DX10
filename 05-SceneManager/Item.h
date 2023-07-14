#pragma once

#include "GameObject.h"
#include "Animations.h"

#define ID_ANI_ITEM 300000

#define ITEM_BBOX_WIDTH 15
#define ITEM_BBOX_HEIGHT 15

class CItem : public CGameObject {
public:
	CItem(float x, float y);
	~CItem() {};
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }

	virtual int IsCollidable() { return 1; };
	virtual void OnNoCollision(DWORD dt) {
		x += vx * dt;
		y += vy * dt;
	};
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};