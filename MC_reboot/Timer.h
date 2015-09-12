#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

class Timer
{
    public:
        Timer();

        int GetTicks();
        void Start();

    protected:
    private:
        Uint32 _startTicks;
        bool _started;
};

#endif // TIMER_H
