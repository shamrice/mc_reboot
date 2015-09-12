#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Sprite.h"
#include "Enums.H"

class Projectile : public Sprite
{
    public:
        Projectile(const char* bmpFileName, int x, int y, bool visible);

        //Public methods
        void Move();
        bool Fired();
        void Fire(int startX, int startY, Directions direction);
        int GetDamage();
        void SetProjectile(int damage, int distance, int stepDistance);

        //public properties
        Directions Direction;
        bool Enabled;
    protected:
    private:
        int _stepDistance;
        int _damage, _distance;
        int _startX, _startY;
};

#endif // PROJECTILE_H
