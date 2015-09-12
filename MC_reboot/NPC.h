#ifndef NPC_H
#define NPC_H

#include "Sprite.h"


class NPC : public Sprite
{
    public:
        NPC();
        NPC(const char* bmpFileName, int x, int y, bool visible, const char* text);
        ~NPC();
        const char* GetText();
    protected:
    private:
        const char* _text;
};

#endif // NPC_H
