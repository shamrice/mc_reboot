#include "AreaData.h"

AreaData::AreaData(int numBadGuys, int numNpc, int numItems, std::string mapFilename)
{
    printf("Creating area data:\nFileName: %s\n", mapFilename.c_str());
    _numBadGuys = numBadGuys;
    _numNpc = numNpc;
    _numItems = numItems;
    _mapFilename = mapFilename;
    InventoryData = new Inventory(numItems);

    BadGuyData = new BadGuy*[numBadGuys];
    NpcData = new NPC*[numNpc];
}

AreaData::~AreaData(){

    if (NULL != BadGuyData)
        delete [] BadGuyData;

    if (NULL != NpcData)
        delete [] NpcData;

    if (NULL != InventoryData)
        delete InventoryData;
}

int AreaData::GetNumberOfBadGuys(){
    return _numBadGuys;
}

int AreaData::GetNumberOfNPC(){
    return _numNpc;
}

int AreaData::GetNumberOfItems(){
    return _numItems;
}

std::string AreaData::GetMapFileName() {
    printf("Map filename: %s", _mapFilename.c_str());
    return _mapFilename;
}

bool AreaData::SetAreaData(BadGuy **badGuys, int numBadGuys){//, Inventory *inventory){

    BadGuyData = badGuys;
    _numBadGuys = numBadGuys;
    //_npcData = npcs;
//    InventoryData = inventory;

    return true;

}
