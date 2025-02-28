#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"


using namespace std;

BaseObject g_background;

bool InitData()
{
    bool success=true;
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");

    g_window=SDL_CreateWindow("SDL_GAME_DEMO",
                                SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH,SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);
    if(g_window==NULL){ success=0;}
    else {
            g_screen=SDL_CreateRenderer(g_window,-1,SDL_RENDERER_ACCELERATED);
            if(g_screen==NULL){success=0;}
            else {
                SDL_SetRenderDrawColor(g_screen,255,255,255,255);
                if(!IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG)) success=0;
            }
    }

    return success;
}

bool Loadbackground()
{
    return g_background.LoadImg("img//background.png",g_screen);
}

void close()
{
    g_background.~BaseObject();

    SDL_DestroyRenderer(g_screen);
    g_screen=NULL;
    SDL_DestroyWindow(g_window);
    g_window=NULL;

    IMG_Quit();
    SDL_Quit();
}



int main(int argc, char* argv[])
{
    if(InitData()==0) return -1;
    if(Loadbackground()==0) return -1;

    GameMap game_map;
    game_map.LoadMap("map//map01.dat");
    game_map.LoadTiles(g_screen);

    MainObject p_player;
    p_player.LoadImg("img//player_right.png",g_screen);
    p_player.set_clips();


    bool is_quit=false;
    while(!is_quit){
        if(SDL_PollEvent(&g_event)!=0){
            if(g_event.type==SDL_QUIT){
                is_quit=true;
            }
            p_player.HandleInputAction(g_event,g_screen);
        }
        SDL_SetRenderDrawColor(g_screen,255,255,255,255);
        SDL_RenderClear(g_screen);

        g_background.Render(g_screen,NULL);


        Map map_data=game_map.GetMap();
        p_player.SetMapX(map_data.start_x_);


        p_player.DoPlayer(map_data);
        p_player.Show(g_screen);
        game_map.SetMap(map_data);

        game_map.DrawMap(g_screen);

        SDL_RenderPresent(g_screen);
        SDL_Delay(20);
    }


    close();
}
