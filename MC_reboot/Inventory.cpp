#include "Inventory.h"

Inventory::Inventory(int numSlots)
{
    _numSlots = numSlots;

    _items = new InventoryItem*[_numSlots];
    _slotTaken = new bool[_numSlots];
    for (int i = 0; i < _numSlots; i++){
        _items[i] = new InventoryItem("Empty", EMPTY, 0, 0, false);
        _slotTaken[i] = false;
    }
}

Inventory::~Inventory(){

    if (NULL != _items)
        delete [] _items;

    if (NULL != _slotTaken)
        delete [] _slotTaken;
}

bool Inventory::AddItem(InventoryItem *item){

    //returns false if item can't be added b/c inventory is full
    for (int i = 0; i < _numSlots; i++){
        if (!_slotTaken[i])
        {
            _items[i] = new InventoryItem(item->Name, item->Type, item->X, item->Y, item->Visible);
            _slotTaken[i] = true;
            return true;
        }
    }
    return false;
}

InventoryItem* Inventory::GetItem(int slotNumber){
    if (NULL != _items[slotNumber]){
        if (!_items[slotNumber]->Used) {
            //printf("Got item %s\n", _items[slotNumber]->Name.c_str());
            return _items[slotNumber];
        }
    }

    return NULL;
}

void Inventory::RemoveItem(int slotNumber){

    if (slotNumber < _numSlots && slotNumber >= 0) {
        _items[slotNumber] = new InventoryItem("Empty", EMPTY, 0, 0, false);
        _slotTaken[slotNumber] = false;
    }
}

int Inventory::GetNumSlots(){
    return _numSlots;
}
