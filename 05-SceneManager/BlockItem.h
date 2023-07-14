#pragma once

#include "Block.h"

#define BLOCK_QUESTION_KIND_COIN 0
#define BLOCK_QUESTION_KIND_POWERUP 1
#define BLOCK_QUESTION_KIND_LIFEUP 2

class CBlockItem : public CBlock
{
public:
	CBlockItem(float x, float y, float w, float h) :CBlock(x, y, w, h) { state = OBJECT_STATE_RUN; }
	~CBlockItem() {};

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsDirectionColliable(float nx, float ny) { return 1; }
	int IsCollidable() { return 0; };
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void CreateItem();
	void OnNoCollision(DWORD dt) {
		x += vx * dt;
		y += vy * dt;
	}
};