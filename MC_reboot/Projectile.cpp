#include "Projectile.h"

Projectile::Projectile(const char* bmpFileName, int x, int y, bool visible) : Sprite(bmpFileName, x, y, visible)
{
    Direction = LEFT;
    Enabled = false;
    _damage = 0;
    _stepDistance = 0;
    _distance = 0;

}


void Projectile::Move()
{
    //only move if enabled and visible.
    if (Enabled && Visible)
    {
        int deltaX = 0, deltaY = 0;

        if (Direction == RIGHT)
            deltaX += _stepDistance;
        else if (Direction == LEFT)
            deltaX -= _stepDistance;
        else if (Direction == UP)
            deltaY -= _stepDistance;
        else if (Direction == DOWN)
            deltaY += _stepDistance;

        X += deltaX;
        Y += deltaY;

        //make sure the projectile is inside of it's stat range.
        if (_startX - X  < _distance && X - _startX < _distance && _startY - Y < _distance && Y - _startY < _distance)
            SetXY(X, Y);
        else
            Visible = false;
    }

}

bool Projectile::Fired()
{
    return Visible;
}

void Projectile::Fire(int startX, int startY, Directions direction)
{

    //only fire projectile if enabled
    if (Enabled)
    {
        Direction = direction;
        Visible = true;

        _startX = startX;
        _startY = startY;

        if (Direction == RIGHT)
        {
            _startX += _stepDistance;
            _startY += _stepDistance;
        }
        else if (Direction == LEFT)
        {
            _startX -= _stepDistance;
            _startY += _stepDistance;
        }
        else if (direction == DOWN)
        {
            _startX += _stepDistance / 2;
            _startY += _stepDistance * 2;
        }
        else if (direction == UP)
        {
            _startX += _stepDistance / 2;
            _startY = startY;
        }

        SetXY(_startX, _startY);
    }
}

int Projectile::GetDamage()
{
    return _damage;
}

void Projectile::SetProjectile(int damage, int distance, int stepDistance)
{
    Enabled = true;
    _damage = damage;
    _distance = distance;
    _stepDistance = stepDistance;
}
