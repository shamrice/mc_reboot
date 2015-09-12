#include "Timer.h"

Timer::Timer()
{
    _startTicks = 0;
    _started = false;
}

int Timer::GetTicks(){
    if (_started)
        return SDL_GetTicks() - _startTicks;
    else
        return 0;
}

///Starts timer and resets _startTicks.
void Timer::Start(){
    _started = true;
    _startTicks = SDL_GetTicks();
}
