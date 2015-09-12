#include "Tile.h"

Tile::Tile(int x, int y, int tileType)
{
    Box = new SDL_Rect();
    Box->x = x;
    Box->y = y;
    Box->h = TILE_HEIGHT;
    Box->w = TILE_WIDTH;

    Type = tileType;

}

Tile::~Tile(){
    delete(Box);
}
