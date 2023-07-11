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
	int IsBlockingX() { return kind != 0; }
	int IsDirectionColliable(float nx, float ny)
	{
		if (type == BLOCK_TYPE_WALL && kind == 1)	//Không va chạm color box X và Y
			if (nx != COLLISION_NONE || ny == COLLISION_BOTTOM)
				return 0;
		return 1;
	}
};