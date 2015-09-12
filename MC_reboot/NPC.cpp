#include "NPC.h"

NPC::NPC(){

}

NPC::NPC(const char* bmpFileName, int x, int y, bool visible, const char* text) : Sprite(bmpFileName, x, y, visible)
{
    _text = text;
}

NPC::~NPC(){
    delete(_text);
}

const char* NPC::GetText(){
    return _text;
}
