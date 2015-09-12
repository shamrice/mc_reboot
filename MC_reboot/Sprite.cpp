#include "Sprite.h"

Sprite::Sprite()
{
    X = 0;
    Y = 0;
    SpriteError = false;
    Visible = false;
}

Sprite::Sprite(const char* bmpFileName, int x, int y, bool visible)
{

    SpriteError = false;

    if (sizeof(bmpFileName) > 0)
    {
        SDL_Surface *temp = SDL_LoadBMP(bmpFileName);
        if (!temp)
        {
            printf("Sprite Constructor: Unable to load bitmap: %s\n", SDL_GetError());
            SpriteError = true;
        }
        else
        {
            //convert to display format
            _sprite = SDL_DisplayFormat(temp);
        }
        SDL_FreeSurface(temp);

    }
    else
    {
        SpriteError = true;
    }

    Visible = visible;

    setUpSprite(x, y);
}

Sprite::~Sprite()
{
    // free loaded bitmap
    SDL_FreeSurface(_sprite);
    delete(_sprite);
    delete(_dstRect);

}

bool Sprite::SetSprite(SDL_Surface *surfaceToUse, int x, int y)
{
    if (!SpriteError)
    {
        Visible = Visible;

        if (NULL != surfaceToUse)
        {
            SDL_Surface *temp = surfaceToUse;
            _sprite = SDL_DisplayFormat(temp);
            SDL_FreeSurface(temp);
        }
        else
        {
            SpriteError = true;
        }

        setUpSprite(x, y);
    }
    return !SpriteError;
}

bool Sprite::SetSprite(const char* bmpFileName, int x, int y)
{
    if (!SpriteError)
    {
        if (sizeof(bmpFileName) > 0)
        {
            SDL_Surface *temp = SDL_LoadBMP(bmpFileName);
            if (!temp)
            {
                printf("SetSprite: Unable to load bitmap: %s\n", SDL_GetError());
                SpriteError = true;
            }
            else
            {
                //convert to display format
                _sprite = SDL_DisplayFormat(temp);
            }
            SDL_FreeSurface(temp);

        }
        else
        {
            SpriteError = true;
        }


        setUpSprite(x, y);
    }
    else
    {
        SpriteError = true;
    }

    printf("Setting sprite: %s\n", bmpFileName);
    printf("SpriteError = %d...%d\n", SpriteError, true);

    //return true if sprite was set correctly.
    return !SpriteError;
}

SDL_Surface* Sprite::GetSprite()
{
    if (NULL != _sprite)
    {
        return _sprite;
    }
    else
    {
        return NULL;
    }
}

int Sprite::GetWidth()
{
    if (NULL != _sprite)
    {
        return _sprite->w;
    }
    else
    {
        return -1;
    }
}

int Sprite::GetHeight()
{
    if (NULL != _sprite)
    {
        return _sprite->h;
    }
    else
    {
        return -1;
    }
}

void Sprite::SetXY(int x, int y)
{
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    _dstRect->x = x;
    _dstRect->y = y;

    X = x;
    Y = y;
}

SDL_Rect* Sprite::GetDestinationRect()
{
    return _dstRect;
}

// Private Methods

/// Shared sprite constructor commands
void Sprite::setUpSprite(int x, int y)
{

    _dstRect = new SDL_Rect();
    SetXY(x,y);

    //set transparency
    if (NULL != _sprite)
    {
        if (SDL_SetColorKey(_sprite, SDL_SRCCOLORKEY, SDL_MapRGB(_sprite->format, 255, 0, 255)) != 0)
        {
            printf("Sprite setUpSprite: Unable to set transparency: %s\n", SDL_GetError());
            SpriteError = true;
        }
    }
    else
    {
        SpriteError = true;
    }

}


