#ifndef ENGINE_H
#define ENGINE_H

#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include <SDL_ttf.h>
#include <fstream>
#include <sstream>
#include <string>


#include "Sprite.h"
#include "ImageResources.h"
#include "TextResources.h"
#include "Player.h"
#include "BadGuy.h"
#include "Projectile.h"
#include "NPC.h"
#include "Messenger.h"
#include "Tile.h"
#include "AreaData.h"

#define NUM_MESSENGERS 2

//debug
#define NUM_X_SCREENS 3
#define NUM_Y_SCREENS 2

class Engine
{
    public:
        Engine(int scrHeight, int scrWidth, bool fullScreen);
        virtual ~Engine();

        //public methods
        void FireProjectile();
        void ClearScreen();
        void RenderSprites();
        void RenderBackground();
        void UpdateScreen();
        void HandleEvents();
        void MovePlayer(Directions direction);
        void UseItem(ItemType type);

        //Debug method
        void ResetBadGuy();
        void Restart();
        void PrintPlayerInventory();

        //public properties
        bool EngineError;

    protected:
    private:
        //private methods
        void initializeSprites();
        bool setUpTiles();
        void setUpAreaData();
        void clipTiles();
        bool checkCollision(SDL_Rect *rect1, SDL_Rect *rect2);

        //private properties
        bool _refreshRequired;
        int _scrHeight, _scrWidth;
        TTF_Font *_font;
        SDL_Surface *_screen;
        SDL_Rect *_clips[TOTAL_TILE_SPRITES];
        Sprite *_testSprite, *_tileSheet;
        Player *_player;
        Messenger *_msgr[NUM_MESSENGERS];
        Tile *_tiles[NUM_TILES];
        AreaData *_areaData[NUM_X_SCREENS][NUM_Y_SCREENS];

};

#endif // ENGINE_H
