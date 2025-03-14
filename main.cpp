#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"


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
    ImpTimer fps;

    if(InitData()==0) return -1;
    if(Loadbackground()==0) return -1;

    GameMap game_map;
    game_map.LoadMap("map//map01.dat");
    game_map.LoadTiles(g_screen);
//
    vector <ThreatObject*> threatlist;
    threatlist=game_map.GetThreatList();
    for(int i=0;i<threatlist.size();i++){
        threatlist[i]->LoadImg("img//threat_right.png",g_screen);
        threatlist[i]->set_clips();
    }


  //
    vector <BulletObject*> bulletlist;
    MainObject p_player;
    p_player.LoadImg("img//player_right.png",g_screen);
    p_player.set_clips();


    bool is_quit=false;
    while(!is_quit){

        fps.game_start();
        if(p_player.GetHealth()<=0) break;
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
        p_player.SetMapXY(map_data.start_x_,map_data.start_y_);


        p_player.DoPlayer(map_data);
        p_player.Show(g_screen);
        game_map.SetMap(map_data);

        game_map.DrawMap(g_screen);
        //
        bulletlist=p_player.Get_Bulletlist();

        //
        for(int i=0;i<threatlist.size();i++){
        threatlist[i]->SetMapXY(map_data.start_x_,map_data.start_y_);
        threatlist[i]->Attack_player(p_player);
        threatlist[i]->MovetoPlayer(p_player, map_data);
        threatlist[i]->AutoMoveThreat(map_data);
        threatlist[i]->Bullet_to_threat(bulletlist);
        threatlist[i]->Show(g_screen);
        if(threatlist[i]->Get_health()==0) threatlist.erase(threatlist.begin()+i);
    }
        p_player.Set_Bulletlist(bulletlist);
        game_map.SetThreatList(threatlist);

        //xu li dan
        for(int i=0;i<bulletlist.size();i++){
            bulletlist[i]->CheckToMap(map_data);
            if(bulletlist[i]->Get_is_move()==false) bulletlist.erase(bulletlist.begin()+i);
            bulletlist[i]->HandleBulletMove(map_data.start_x_,map_data.start_y_);
            bulletlist[i]->Render(g_screen,NULL);

        }
        p_player.Set_Bulletlist(bulletlist);

        SDL_RenderPresent(g_screen);
        int real_time_loop=fps.get_ticks();
        if(real_time_loop<one_frame_time){
            int delay_time=one_frame_time-real_time_loop;
            SDL_Delay(delay_time);
        }
    }


    close();
    return 1;
}
