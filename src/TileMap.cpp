#include "TileMap.h"

TileMap::TileMap(std::string file, TileSet* tileSet) {
	Load(file);
	SetTileSet(tileSet);
}

TileMap::~TileMap() {
	delete tileSet;
}

void TileMap::Load(std::string file) {
	std::FILE* fileReader = fopen(file.c_str(), "r");
	if(fileReader != NULL) {
		char n [2];
		fgets(n, 3, fileReader);
		mapWidth = atoi(n);
		fgets(n, 2, fileReader);
		fgets(n, 3, fileReader);
		mapHeight = atoi(n);
		fgets(n, 2, fileReader);
		fgets(n, 3, fileReader);
		mapDepth = atoi(n);
		fgets(n, 2, fileReader);
		for(int k = 0; k < mapDepth; k++) {
			tileMatrix.emplace_back(std::vector<std::vector<int>>());
			for(int j = 0; j < mapHeight; j++) {
				tileMatrix[k].emplace_back(std::vector<int>());
				for(int i = 0; i < mapWidth; i++) {
					fgets(n, 2, fileReader);
					fgets(n, 3, fileReader);
					tileMatrix[k][j].emplace_back(atoi(n)-1);
				}
				fgets(n, 2, fileReader);
			}
			fgets(n, 3, fileReader);
		}
	}else{
		printf("Unable to read %s", file.c_str());
	}
	fclose(fileReader);
}

void TileMap::SetTileSet(TileSet* tileSet) {
	TileMap::tileSet = tileSet;
}

int& TileMap::At(int x, int y, int z) {
	return tileMatrix[z][y][x];
}

void TileMap::RenderLayer(int layer, int cameraX, int cameraY) {
	for(int j = mapHeight-1; j > -1; j--) {
		for(int i = mapWidth-1; i > -1; i--) {
			tileSet->Render(tileMatrix[layer][j][i], i*tileSet->GetTileWidth() - cameraX, j*tileSet->GetTileHeight() - cameraY);
		}
	}
}

void TileMap::Render(int cameraX, int cameraY) {
	for(int k = 0; k < mapDepth; k++)
		RenderLayer(k, cameraX, cameraY);
}

int TileMap::GetWidth() {
	return mapWidth;
}

int TileMap::GetHeight() {
	return mapHeight;
}

int TileMap::GetDepth() {
	return mapDepth;
}

int TileMap::GetTileWidth() {
	return tileSet->GetTileWidth();
}

int TileMap::GetTileHeight() {
	return tileSet->GetTileHeight();
}
