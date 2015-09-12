#ifndef INVENTORYITEM_H
#define INVENTORYITEM_H

#include <string>
#include "ImageResources.h"
#include "Sprite.h"

enum ItemType {WEAPON, HEALTH_ITEM, KEY, EMPTY};

class InventoryItem : public Sprite
{
    public:
        InventoryItem();
        InventoryItem(std::string name, ItemType type);
        InventoryItem(std::string name, ItemType type, int x, int y, bool visible);

        bool Used;
        ItemType Type;
        std::string Name;
        int Damage, Distance, StepDistance, HealthValue;
    protected:
    private:
        std::string _getFileName(ItemType type);
};

#endif // INVENTORYITEM_H
