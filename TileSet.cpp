#include "TileSet.h"

TileSet::TileSet(int tileWidth, int tileHeight, std::string file) {
	tileSet = Sprite(file);
	TileSet::tileWidth = tileWidth;
	TileSet::tileHeight = tileHeight;
	columns = tileSet.GetWidth()/tileWidth;
	rows = tileSet.GetHeight()/tileHeight;
}

TileSet::~TileSet() {

}

void TileSet::Render(int index, int x, int y) {
	if(index > -1 && index < columns*rows) {
		tileSet.SetClip(index%columns*tileWidth, index/columns*tileHeight, tileWidth, tileHeight);
		tileSet.Render(x, y);
	}
}

Sprite TileSet::GetTileSet() {
	return tileSet;
}

int TileSet::GetTileWidth() {
	return tileWidth;
}

int TileSet::GetTileHeight() {
	return tileHeight;
}

int TileSet::GetColumns() {
	return columns;
}

int TileSet::GetRows() {
	return rows;
}
