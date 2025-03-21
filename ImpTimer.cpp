
#include "ImpTimer.h"
#include "CommonFunc.h"


ImpTimer::ImpTimer()
{
    start_ticks=0;
}

ImpTimer::~ImpTimer()
{

}

void ImpTimer::game_start()
{
    start_ticks=SDL_GetTicks();
}

int ImpTimer::get_ticks()
{
    return SDL_GetTicks()-start_ticks;
}
