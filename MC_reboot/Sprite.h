#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>

class Sprite
{
    public:
        Sprite();
        Sprite(const char* bmpFileName, int x, int y, bool visible);
        virtual ~Sprite();

        //public methods
        bool SetSprite(SDL_Surface *surfaceToUse, int x, int y);
        bool SetSprite(const char* bmpFileName, int x, int y);
        SDL_Surface* GetSprite();
        SDL_Rect* GetDestinationRect();
        int GetWidth();
        int GetHeight();
        void SetXY(int x, int y);

        //public properties
        bool SpriteError;
        int X, Y;
        bool Visible;
    protected:
    private:
        void setUpSprite(int x, int y);
        SDL_Surface* _sprite;
        SDL_Rect* _dstRect;

};

#endif // SPRITE_H
