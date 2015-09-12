#ifndef INVENTORY_H
#define INVENTORY_H

#include "TextResources.h"
#include "InventoryItem.h"


class Inventory
{
    public:
        Inventory(int numSlots);
        ~Inventory();
        bool AddItem(InventoryItem *item);
        InventoryItem *GetItem(int slotNumber);
        void RemoveItem(int slotNumber);
        int GetNumSlots();

    protected:
    private:
        bool *_slotTaken;
        int _numSlots;
        InventoryItem **_items;
};

#endif // INVENTORY_H
