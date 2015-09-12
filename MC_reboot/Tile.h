#ifndef TILE_H
#define TILE_H

#include <SDL.h>

#include "TextResources.h"
#include "Enums.H"



class Tile
{
    public:
        Tile(int x, int y, int tileType);
        ~Tile();
        SDL_Rect *Box;
        int Type;
    protected:
    private:

};

#endif // TILE_H
