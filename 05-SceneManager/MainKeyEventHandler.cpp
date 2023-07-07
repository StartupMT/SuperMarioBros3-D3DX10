#include "MainKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CMainKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch (KeyCode)
	{
	case DIK_DOWN:
		//mario->SetState(OBJECT_STATE_SIT);
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

void CMainKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		mario->isFall = true;
		break;
	case DIK_DOWN:
		break;
	}
}

void CMainKeyHandler::KeyState(BYTE* states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (game->IsKeyDown(DIK_RIGHT))
	{
		mario->direction = 1;
		if (game->IsKeyDown(DIK_A))
			mario->isRun = true;
		else
			mario->SetState(OBJECT_STATE_RUN);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		mario->direction = -1;
		if (game->IsKeyDown(DIK_A))
		{
			mario->isRun = true;
			mario->SetState(OBJECT_STATE_RUN);
		}
		else
			mario->SetState(OBJECT_STATE_RUN);
	}
	else if (mario->GetState() != OBJECT_STATE_JUMP)
	{
		mario->direction = 0;
		mario->SetState(OBJECT_STATE_STAND);
	}
}