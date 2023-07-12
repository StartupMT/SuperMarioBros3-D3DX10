#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>
#include "Animations.h"
#include "Collision.h"
#include "AssetIDs.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define BBOX_ALPHA 0.5f		// Bounding box transparency

class CGameObject
{
protected:

	float x;
	float y;

	float vx;
	float vy;

	int nx;

	int tag;

	int state;

	int type = 0;

	int kind = 0;

	bool isDeleted;

	bool isDraw = true;

	float posYStartJump, velYStartFall, gravity, speedJump, jumpHeight;
public:
	int direction = 0;
	bool isAllowJump = true, isFall = false;

public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }
	int GetTag() { return this->tag; }
	int GetType() { return this->type; }
	int GetKind() { return this->kind; }
	virtual void Delete() { isDeleted = true; }
	bool IsDeleted() { return isDeleted; }
	bool IsDraw() { return isDraw; }

	void RenderBoundingBox();

	CGameObject();
	CGameObject(float x, float y) :CGameObject() { this->x = x; this->y = y; state = OBJECT_STATE_STAND; }


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	virtual void SetTag(int tag) { this->tag = tag; }
	virtual void SetType(int type) { this->type = type; }
	virtual void SetKind(int kind) { this->kind = kind; }
	virtual void SetDraw(bool flag) { isDraw = flag; }
	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return 0; };

	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {
		x += vx * dt;
		y += vy * dt;
		if (state != OBJECT_STATE_JUMP)
		{
			StartJump(0, 0);
			isFall = true;
		}
	};

	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};

	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return 1; }
	virtual int IsDirectionColliable(float nx, float ny) { return 1; }

	~CGameObject();

	static bool IsDeleted(const LPGAMEOBJECT& o) { return o->isDeleted; }

	virtual void Fall();
	virtual void JumpState();
	virtual void StartJump(float speed = OBJECT_JUMP_SPEED, float max = OBJECT_JUMP_HEIGHT, float gravity = OBJECT_GRAVITY);
};
