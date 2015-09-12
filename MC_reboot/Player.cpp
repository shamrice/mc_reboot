#include "Player.h"

Player::Player(const char* bmpFileName, int x, int y, bool visible) : Sprite(bmpFileName, x, y, visible)
{
    _maxHealth = 100;
    _stepDistance = 20;
    _lastX = x;
    _lastY = y;
    _damagedSpriteSet = false;
    _deadSpriteSet = false;
    Health = _maxHealth;
    Hurt = false;
    Direction = LEFT;
    MapXLocation = 0;
    MapYLocation = 0;
    InventoryData = new Inventory(NUM_PLAYER_INVENTORY_SLOTS);
    Weapon = new Projectile(PLAYERPROJECTILE_FILENAME.c_str(), 0, 0, false);
}

Player::~Player()
{
    delete InventoryData;
    delete Weapon;
}

bool Player::Alive()
{
    if (Health > 0)
        return true;
    else
    {
        Health = 0;

        if (!_deadSpriteSet)
            _deadSpriteSet = SetSprite(PLAYERDEADSPRITE_FILENAME.c_str(), X, Y);

        return false;
    }
}

void Player::Move(Directions direction)
{
    if (Alive())
    {
        _lastX = X;
        _lastY = Y;

        //set direction
        Direction = direction;

        //set player movement
        int deltaX = 0, deltaY = 0;
        if (direction == RIGHT) deltaX += _stepDistance;
        else if (direction == LEFT) deltaX -= _stepDistance;
        else if (direction == DOWN) deltaY += _stepDistance;
        else if (direction == UP) deltaY -= _stepDistance;

        //move player
        SetXY(X + deltaX, Y + deltaY);
    }
}

void Player::MoveToScreen(Directions direction)
{
    if (Alive())
    {
        //set direction
        Direction = direction;

        //set player movement
        int newX = 0, newY = 0;
        if (direction == RIGHT)
        {
            printf("moving player screen right\n");
            MapXLocation++;
            newX = _stepDistance * 2;
            newY = Y;
        }
        else if (direction == LEFT)
        {
            printf("moving player screen left\n");
            MapXLocation--;
            newX = PLAYFIELD_WIDTH - _stepDistance * 2;
            newY = Y;
        }
        else if (direction == DOWN)
        {
            printf("moving player screen down\n");
            MapYLocation++;
            newX = X;
            newY = _stepDistance * 2;
        }
        else if (direction == UP)
        {
            printf("moving player screen up\n");
            MapYLocation--;
            newX = X;
            newY = PLAYFIELD_HEIGHT - _stepDistance * 2;
        }
        _lastX = newX;
        _lastY = newY;

        //move player
        SetXY(newX, newY);
    }
}

///knock player back. used in collisions
void Player::KnockBack()
{
    if (Alive())
    {
        SetXY(_lastX, _lastY);
    }
}

void Player::Damage(int damage)
{
    if (Alive())
    {
        Health -= damage;
        Hurt = true;

        if (!_damagedSpriteSet)
            _damagedSpriteSet = SetSprite(PLAYERHURTSPRITE_FILENAME.c_str(), X, Y);
    }
}

///returns true if the player was healed.
bool Player::Heal(int health){

    bool healed = false;

    if (Alive() && Health < _maxHealth){
        healed = true;
        Health += health;
        if (Health > _maxHealth)
            Health = _maxHealth;
    }

    return healed;
}

const char* Player::HealthString()
{
    //WARNING: This is probably not very safe... no error checking and most likely has buffer issues!

    char healthNumStr[21]; //enough to hold up to 64bit
    std::string healthString = "Health: ";
    sprintf(healthNumStr, "%d", Health);
    return (healthString + healthNumStr).c_str();
}

void Player::ClearDamage()
{
    if (Alive())
    {
        Hurt = false;
        _damagedSpriteSet = false;
        SetSprite(PLAYERSPRITE_FILENAME.c_str(), X, Y);
    }
}

int Player::GetStepDistance(){
    return _stepDistance;
}
