#pragma 
#include "Animation.h"
#include "Animations.h"

#define TILESET_WIDTH 16
#define TILESET_HEIGHT 16

class Map
{
public:
	int IDMAP = 0;
	// Create BackGround
	int WidthMap = 0, HeightMap = 0;
	int ID, x, y;
	int dataBG[200][200];
	wstring filePath;

	int tileNum, tileCol;

public:
	Map();
	~Map();

	void CreateTileSet();
	void Render();
};

typedef Map* LPMAP;