#include "BlockItem.h"
#include "Mario.h"
#include "Item.h"


void CBlockItem::Render()
{
	int aniId = ID_ANI_BLOCK + type + state;
	LPANIMATION anim = CAnimations::GetInstance()->Get(aniId);

	if (anim != NULL)
	{
		anim->Render(x, y);
	}
}

void CBlockItem::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (e->obj->GetTag() != OBJECT_TAG_ITEM) return;
	e->obj->StartJump();
	e->obj->sx = x;
}

void CBlockItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == OBJECT_STATE_JUMP)
		JumpState();

	if (y > sy)
	{
		CreateItem();
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBlockItem::CreateItem()
{
	state = OBJECT_STATE_STAND;
	y = sy;
	vx = 0; vy = 0;
	CGameObject* obj = NULL;
	switch (kind)
	{
	case BLOCK_QUESTION_KIND_POWERUP:
		if (CMario::GetInstance()->GetType() == MARIO_TYPE_SMALL)
		{
			obj = new CItem(x, y);
			obj->SetKind(0);
			obj->StartJump(0.06, 9);
		}
		else
		{
			//obj = new SuperLeaf();
			obj->StartJump(OBJECT_JUMP_SPEED * 1.2, OBJECT_JUMP_HEIGHT * 2, OBJECT_GRAVITY / 8);
			obj->SetKind(1);
		}
		obj->sx = x;
		obj->SetTag(OBJECT_TAG_ITEM);
		obj->SetType(ITEM_TYPE_SUPERMUSHROOM);
		//ObjectManager::GetInstance()->AddObjectMap(obj);
		break;
	case 2:
		obj = new CItem(x, y);
		obj->SetTag(OBJECT_TAG_ITEM);
		obj->SetType(ITEM_TYPE_UPMUSHROOM);
		obj->StartJump(0.06, 9);
		break;
	default:
		obj = new CItem(x, y);
		obj->SetTag(OBJECT_TAG_ITEM);
		obj->StartJump(OBJECT_JUMP_SPEED * 1.3);
		break;
	}
	float marioX, marioY;
	CMario::GetInstance()->GetPosition(marioX, marioY);
	obj->sx = marioX;
	//ObjectManager::GetInstance()->AddObjectMap(obj);
}