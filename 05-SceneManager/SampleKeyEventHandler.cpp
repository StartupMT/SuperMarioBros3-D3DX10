#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(OBJECT_STATE_SIT);
		break;
	case DIK_S:
		mario->SetState(OBJECT_STATE_JUMP);
		break;
	case DIK_0:
		mario->SetType(MARIO_TYPE_SMALL);
		break;
	case DIK_1:
		mario->SetType(MARIO_TYPE_BIG);
		break;
	case DIK_2:
		mario->SetType(MARIO_TYPE_RACCCOON);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		mario->SetState(0);
		break;
	case DIK_DOWN:
		mario->SetState(0);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	//if (game->IsKeyDown(DIK_RIGHT))
	//{
	//	if (game->IsKeyDown(DIK_A))
	//		mario->SetState(MARIO_STATE_RUNNING_RIGHT);
	//	else
	//		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	//}
	//else if (game->IsKeyDown(DIK_LEFT))
	//{
	//	if (game->IsKeyDown(DIK_A))
	//		mario->SetState(MARIO_STATE_RUNNING_LEFT);
	//	else
	//		mario->SetState(MARIO_STATE_WALKING_LEFT);
	//}
	//else
	//	mario->SetState(MARIO_STATE_IDLE);
}