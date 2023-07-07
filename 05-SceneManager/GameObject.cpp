#include <d3dx9.h>
#include <algorithm>


#include "debug.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"
#include "Mario.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
	state = -1;
	isDeleted = false;
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy; 
	CGame::GetInstance()->GetCamPos(cx, cy);

	CGame::GetInstance()->Draw(x - cx, y - cy, bbox, &rect, BBOX_ALPHA);
}

//Trạng thái nhảy
void CGameObject::JumpState()
{
	if (state != OBJECT_STATE_JUMP) return;

	if (isAllowJump)
	{
		posYStartJump = y;
		isFall = false;
	}
	isAllowJump = false;

	//Fall
	if (!isFall && (posYStartJump - y >= jumpHeight))
	{
		isFall = true;
		vy = speedJump;
	}

	if (isFall)
	{
		vy += CGame::GetInstance()->isFixUpdateFrame ? OBJECT_FALL_ACCEL : 0; //trừ vận tốc sẽ nhảy nhẹ 1 đoạn
		vy = vy > 0.5 ? gravity : vy;
		return;
	}
	//JumpUP
	vy = speedJump;
}

void CGameObject::StartJump(float speed, float max, float gravity)
{
	isAllowJump = true;
	isFall = false;
	this->gravity = gravity;
	speedJump = speed;
	jumpHeight = max;
	state = OBJECT_STATE_JUMP;
}

CGameObject::~CGameObject()
{

}