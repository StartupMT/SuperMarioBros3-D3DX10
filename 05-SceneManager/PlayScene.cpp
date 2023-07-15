#pragma warning(disable : 4996)

#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Item.h"
#include "Block.h"
#include "BlockItem.h"

#include "MainKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CMainKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_BACKGROUND	3
#define SCENE_SECTION_WALLVIEW	4

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 7) return;

	//#TAG	TYPE	KIND	X	Y	W	H
	int object_tag = atoi(tokens[0].c_str());
	int object_type = atoi(tokens[1].c_str());
	int object_kind = atoi(tokens[2].c_str());
	float x = (float)atof(tokens[3].c_str());
	float y = (float)atof(tokens[4].c_str());
	float w = (float)atof(tokens[5].c_str());
	float h = (float)atof(tokens[6].c_str());
	x = x + w / 2; y = y + h / 2;
	CGameObject* obj = NULL;

	switch (object_tag)
	{
	case OBJECT_TAG_PLAYER:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		y = y - 120;
		player = CMario::GetInstance();
		obj = player;
		obj->SetType(object_type);
		obj->SetState(OBJECT_STATE_STAND);
		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TAG_BLOCK:
		switch (object_type)
		{
		case BLOCK_TYPE_WALL:
			obj = new CBlock(x, y, w, h);
			break;
		case BLOCK_TYPE_QUESTION:
			obj = new CBlockItem(x, y, w, h);
			obj->sy = y;
			break;
		}
		break;
	case OBJECT_TAG_ENEMY:
		switch (object_type)
		{
		case ENEMY_TYPE_GOOMBA:
			obj = new CEnemy(x, y);
			if (object_kind == 1)
				dynamic_cast<LPEnemy>(obj)->isJumpE = true;
			break;
		case ENEMY_TYPE_KOOPA:
			if (object_kind == 0)
				obj = new CKoopa(x, y);
			else
				obj = new CEnemy(x, y);

			if (object_kind == 2)
				dynamic_cast<LPEnemy>(obj)->isJumpE = true;
			break;
		}
		break;
	case OBJECT_TAG_ITEM:
		obj = new CItem(x, y);
		break;

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}
	if (obj == NULL) return;

	// General object setup
	obj->SetPosition(x, y);
	obj->SetTag(object_tag);
	obj->SetType(object_type);
	obj->SetKind(object_kind);

	objects.push_back(obj);
}

// Parse a line in section [BACKGROUND]
void CPlayScene::_ParseSection_BACKGROUND(string line)
{
	vector<string> tokens = split(line, ",");
	int size = tokens.size() - 1;
	if (size < 3)
	{
		return;
	}
	if (size == 3)
	{
		map->IDMAP = atoi(tokens[0].c_str());
		map->tileNum = atoi(tokens[1].c_str());
		map->tileCol = atoi(tokens[2].c_str());
		map->filePath = ToWSTR(tokens[3]);
		map->CreateTileSet();
		return;
	}
	map->WidthMap = size > map->WidthMap ? size : map->WidthMap;
	for (int i = 0; i < size; i++)
	{
		map->dataBG[map->HeightMap][i] = atoi(tokens[i].c_str());
	}
	map->HeightMap++;
}

void CPlayScene::_ParseSection_VIEW(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return;
	//X		Y	W	H
	float x = atoi(tokens[0].c_str());
	float y = atoi(tokens[1].c_str());
	float w = atoi(tokens[2].c_str());
	float h = atoi(tokens[3].c_str());
	RECT rect = { x , y , w, h };
	rectView.push_back(rect);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);
	if (map == nullptr) { map = new Map(); }
	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line == "[BACKGROUND]") { section = SCENE_SECTION_BACKGROUND; map->HeightMap = 0; continue; };
		if (line == "[WALLVIEW]") { section = SCENE_SECTION_WALLVIEW; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_BACKGROUND: _ParseSection_BACKGROUND(line); break;
		case SCENE_SECTION_WALLVIEW: _ParseSection_VIEW(line); break;
		}
	}

	f.close();
	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	float cx, cy;
	RECT boundCamera;
	float width = CGame::GetInstance()->GetBackBufferWidth();
	float height = CGame::GetInstance()->GetBackBufferHeight();

	CGame::GetInstance()->GetCamPos(cx, cy);
	boundCamera.left = cx - 32;
	boundCamera.top = cy - 32;
	boundCamera.right = boundCamera.left + width + 32;
	boundCamera.bottom = boundCamera.top + height + 32;

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetTag() == OBJECT_TAG_PLAYER) continue;
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		float x, y; objects[i]->GetPosition(x, y);
		if (objects[i]->GetTag() == OBJECT_TAG_PLAYER)
			objects[i]->Update(dt, &coObjects);
		else if (CCollision::isCollision(x, y, boundCamera))
		{
			objects[i]->SetDraw(true);
			objects[i]->Update(dt, &coObjects);
		}
		else
		{
			objects[i]->SetDraw(false);
		}
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;
	// Update camera to follow mario
	player->GetPosition(cx, cy);

	//////Check die
	//if (cy > 720)
	//{
	//	CGame::GetInstance()->InitiateSwitchScene(1);
	//	CGame::GetInstance()->SwitchScene();
	//}

	RECT _rect = rectView[0];
	for (size_t i = 0; i < rectView.size(); i++)
	{
		if (CCollision::isCollision(cx, cy, rectView[i]))
		{
			_rect = rectView[i];
			break;
		}
	}

	cx -= width / 2;
	cy -= height / 2;

	if (cx < 0) cx = 0;
	if (cy + height > _rect.bottom)
		cy = _rect.bottom - height;
	else if (cy < _rect.top)
		cy = _rect.top;

	CGame::GetInstance()->SetCamPos(cx, cy);

	CGame::GetInstance()->InitiateSwitchScene(1);
	CGame::GetInstance()->SwitchScene();

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	map->Render();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

void CPlayScene::AddObject(LPGAMEOBJECT obj)
{
	objects.push_back(obj);
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i] == player) continue; //Don't delete mario
		delete objects[i];
	}

	objects.clear();
	player = NULL;
	delete map;
	map = nullptr;
	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}