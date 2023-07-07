#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BLOCK 400000
#define BLOCK_KIND_WALL 0
#define BLOCK_KIND_PHANTOM 1


class CBlock : public CGameObject 
{
protected:
	float w, h;
public:
	CBlock(float x, float y, float w, float h);
	~CBlock() {};
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return kind != 1; }
};