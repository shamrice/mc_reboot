#ifndef BADGUY_H
#define BADGUY_H

#include "Sprite.h"
#include "Enums.H"
#include "ImageResources.h"

class BadGuy : public Sprite
{
    public:
        BadGuy();
        BadGuy(const char* bmpFileName, int x, int y, bool visible);

        bool Alive();
        int GetDamage();
        void Move(int targetX, int targetY);
        void KnockBack();

        int Health;
        bool Collision;
        Directions Direction;
    protected:
    private:
        int _damage;
        int _stepDistance, _lastX, _lastY;
        bool _deadSpriteSet, _damagedSpriteSet;
};

#endif // BADGUY_H
