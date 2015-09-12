#ifndef MESSENGER_H
#define MESSENGER_H

#include "Sprite.h"


class Messenger : public Sprite
{
    public:

        Messenger();
        ~Messenger();

        const char *Message;
        SDL_Color TextColor;
    protected:
    private:
};

#endif // MESSENGER_H
