
#include "ImpTimer.h"
#include "CommonFunc.h"


ImpTimer::ImpTimer()
{
    start_ticks=0;
    pause_ticks=0;

    is_start=false;
    is_pause=false;
}

ImpTimer::~ImpTimer()
{

}

void ImpTimer::game_start()
{
    is_start=true;
    is_pause=false;

    start_ticks=SDL_GetTicks();

}

int ImpTimer::get_ticks()
{
    return SDL_GetTicks()-start_ticks;
}
