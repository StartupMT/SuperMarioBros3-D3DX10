#pragma once

#include "Block.h"


class CBlockItem : public CBlock
{
protected:
	float w, h;
public:
	CBlockItem(float x, float y, float w, float h);
	~CBlockItem() {};

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};