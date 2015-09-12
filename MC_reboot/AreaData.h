#ifndef AREADATA_H
#define AREADATA_H

#include <string>
#include "BadGuy.h"
#include "NPC.h"
#include "Inventory.h"

class AreaData
{
    public:
        AreaData(int numBadGuys, int numNpc, int numItems, std::string mapFilename);
        ~AreaData();

        int GetNumberOfBadGuys();
        int GetNumberOfNPC();
        int GetNumberOfItems();
        bool SetAreaData(BadGuy **badGuys, int numBadGuys);// NPC **npcs);//, Inventory *inventory);
        std::string GetMapFileName();
        Inventory *InventoryData;
        BadGuy **BadGuyData;
        NPC **NpcData;
    protected:
    private:
        int _numBadGuys, _numNpc, _numItems;
        std::string _mapFilename;

};

#endif // AREADATA_H
