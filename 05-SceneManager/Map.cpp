#include "Map.h"
#include "Textures.h"
#include "debug.h"

Map::Map()
{

}

Map::~Map()
{

}

void Map::CreateTileSet()
{
	CTextures::GetInstance()->Add(IDMAP, filePath.c_str());
	LPTEXTURE tex = CTextures::GetInstance()->Get(IDMAP);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", IDMAP);
		return;
	}
	for (int i = 0; i < tileNum; i++)
	{
		int l = (i % tileCol) * TILESET_WIDTH;
		int t = (i / tileCol) * TILESET_HEIGHT;
		int r = l + TILESET_WIDTH - 1;
		int b = t + TILESET_HEIGHT - 1;
		CSprites::GetInstance()->Add(i, l, t, r, b, tex);
	}
}

void Map::Render()
{
	if (IDMAP == 0) return;	//Map không có background
	int xx = 100, yy = 100;
	for (int h = 0; h < HeightMap; h++)
		for (int w = 0; w < WidthMap; w++)
		{
			int id = dataBG[h][w];
			if (id > 0)
			{
				int x = (w + 0.5) * TILESET_WIDTH;	// Cộng trừ 0.5 do x,y là tâm
				int y = (h - 0.5) * TILESET_HEIGHT;
				CSprites::GetInstance()->Get(id - 1)->Draw(x, y);
			}
		}
}