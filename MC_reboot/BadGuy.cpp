#include "BadGuy.h"

BadGuy::BadGuy(){
    Health = 100;
    Direction = UP;
    Collision = false;
    _damage = 1;
    _stepDistance = 2;
    _deadSpriteSet = false;
    _damagedSpriteSet = false;
}

BadGuy::BadGuy(const char* bmpFileName, int x, int y, bool visible) : Sprite(bmpFileName, x, y, visible)
{
    Health = 100;
    Direction = UP;
    Collision = false;
    _damage = 1;
    _stepDistance = 2;
    _deadSpriteSet = false;
    _damagedSpriteSet = false;
}

bool BadGuy::Alive()
{
    if (Health > 0)
        return true;
    else
    {
        if (!_deadSpriteSet)
            _deadSpriteSet = SetSprite(BADGUY_DEAD_FILENAME.c_str(), X, Y);

        return false;
    }
}

int BadGuy::GetDamage()
{
    return _damage;
}

///Moves bad guy towards it's target
void BadGuy::Move(int targetX, int targetY)
{
    if (Alive())
    {
        _lastX = X;
        _lastY = Y;

        int deltaX = 0, deltaY = 0;

        if (rand() % 50 == 0)
        {
            if (rand() % 2 == 0){
                deltaX += rand() % _stepDistance + 5;
                deltaY += rand() % _stepDistance + 5;
            } else {
                deltaX -= rand() % _stepDistance + 5;
                deltaY -= rand() % _stepDistance + 5;
            }
        }
        else
        {
            if (targetX < X)
            {
                deltaX -= _stepDistance;
                Direction = LEFT;
            }
            if (targetX > X)
            {
                deltaX += _stepDistance;
                Direction = RIGHT;
            }
            if (targetY < Y)
            {
                deltaY -= _stepDistance;
                Direction = UP;
            }
            if (targetY > Y)
            {
                deltaY += _stepDistance;
                Direction = DOWN;
            }
        }



        SetXY(X + deltaX, Y + deltaY);

    }
}

///knocks bad guy back. used for collisions.
void BadGuy::KnockBack()
{
    if (Alive())
    {
        SetXY(_lastX, _lastY);
    }

}
