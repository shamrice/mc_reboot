#include "InventoryItem.h"
/*
InventoryItem::InventoryItem() : Sprite(){
}

InventoryItem::InventoryItem(std::string name, ItemType type) : Sprite()
{
    InventoryItem(name, type, 0, 0, false);
}
*/
InventoryItem::InventoryItem(std::string name, ItemType type, int x, int y, bool visible) : Sprite(this->_getFileName(type).c_str(), x, y, visible){
    Name = name;
    Type = type;
    Used = false;

    //generic debug settings. these will be set later by actual stats.

    if (Type == KEY){
        StepDistance = 0;
        Distance = 0;
        Damage = 0;
        HealthValue = 0;
    } else if (Type == HEALTH_ITEM){
        StepDistance = 0;
        Distance = 0;
        Damage = 0;
        HealthValue = 20;
    } else if (Type == WEAPON){
        StepDistance = 10;
        Distance = 100;
        Damage = 5;
        HealthValue = 0;
    } else {
        StepDistance = 0;
        Distance = 0;
        Damage = 0;
        HealthValue = 0;

    }
}

std::string InventoryItem::_getFileName(ItemType type){
    switch (type){
        case WEAPON:
            return WEAPON_FILENAME;
            break;
        case HEALTH_ITEM:
            return HEALTH_ITEM_FILENAME;
            break;
        case KEY:
            return KEY_FILENAME;
            break;
        default:
            return TEST_FILENAME;
            break;
    }
}
