#pragma once

#include "GameObject.h"
#include "Animations.h"

#define ID_ANI_ITEM 300000

#define ITEM_BBOX_WIDTH 15
#define ITEM_BBOX_HEIGHT 15

class CItem : public CGameObject {
public:
	CItem(float x, float y) : CGameObject(x, y) { type = ITEM_TYPE_COIN; }
	~CItem() {};
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};