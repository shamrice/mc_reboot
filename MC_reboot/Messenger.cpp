#include "Messenger.h"

Messenger::Messenger() : Sprite()
{
    //set text color to white
    //may become configurable down the road.
    TextColor.b = 255;
    TextColor.r = 255;
    TextColor.g = 255;
}

Messenger::~Messenger(){
    delete(Message);
}
