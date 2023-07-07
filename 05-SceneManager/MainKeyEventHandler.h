#pragma once

#include "Scene.h"

class CMainKeyHandler: public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CMainKeyHandler(LPSCENE s) :CSceneKeyHandler(s) {};
};
