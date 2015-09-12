#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <cstdio>
#include "Sprite.h"
#include "Inventory.h"
#include "Projectile.h"
#include "Enums.H"
#include "ImageResources.h"
#include "TextResources.h"

class Player : public Sprite
{
    public:

        Player(const char* bmpFileName, int x, int y, bool visible);
        ~Player();

        //public methods
        bool Alive();
        bool Heal(int health);
        void Move(Directions direction);
        void MoveToScreen(Directions direction);
        void KnockBack();
        void Damage(int damage);
        void ClearDamage();
        const char* HealthString();
        int GetStepDistance();

        //public properties
        int Health, MapXLocation, MapYLocation;
        bool Hurt;
        Inventory *InventoryData;
        Projectile *Weapon;
        Directions Direction;
    protected:
    private:
        int _stepDistance, _lastX, _lastY, _maxHealth;
        bool _damagedSpriteSet, _deadSpriteSet;



};

#endif // PLAYER_H
