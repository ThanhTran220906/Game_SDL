#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "Menu.h"
#include "Level.h"

using namespace std;

BaseObject g_background;

bool InitData() {
    bool success = true;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("SDL_GAME_DEMO",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (g_window == NULL) {
        success = false;
    }
    else {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL) {
            success = false;
        }
        else {
            SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
            if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) success = false;
        }
    }
    if (TTF_Init() == -1) {
        success = false;
    }
    return success;
}

bool Loadbackground() {
    return g_background.LoadImg("img//background.png", g_screen);
}

void close() {
    g_background.~BaseObject();

    SDL_DestroyRenderer(g_screen);
    SDL_DestroyWindow(g_window);

    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}

template <typename T>
void ClearVector(vector<T*>& v) {
    for (int i = v.size() - 1; i >= 0; i--) {
        delete v[i];
        v[i] = nullptr;
        v.erase(v.begin() + i);
    }
}

int main(int argc, char* argv[]) {
    //init
    ImpTimer fps;
    titleScreen title;
    Level level;
    pauseMenu pause;
    GameOverMenu game_over;
    if (!InitData() || !Loadbackground()) return -1;

    //map
    GameMap game_map;
    string file_map="";
    vector<ThreatObject*> threatlist ;

    //player
    MainObject p_player;
    p_player.LoadImg("img//player_right.png", g_screen);
    p_player.set_clips();
    vector<BulletObject*> bulletlist ;


    //game loop
    while (!is_quit) {
        while (SDL_PollEvent(&g_event)) {
            //quit
            if (g_event.type == SDL_QUIT) {
                is_quit = true;
            }
            //xu li game
            if (g_event.type == SDL_KEYDOWN) {
                if (Title) {
                    title.handleEvents(g_event);
                }
                else if (Pause) {
                    pause.handleEvents(g_event);
                    if(pause.GetBoolLoading()){
                        ClearVector(bulletlist);
                        p_player.Set_Bulletlist(bulletlist);
                        ClearVector(threatlist);
                        game_map.SetThreatList(threatlist);
                        pause.SetBoolLoading(false);
                    }
                }
                else if (LevelChoose){
                    level.handleEvents(g_event, p_player);
                    if(level.GetBoolLoading()){
                        file_map=level.GetFilemap();
                        game_map.LoadMap(file_map);
                        game_map.LoadTiles(g_screen);
                        game_map.SpawnThreat();
                        threatlist = game_map.GetThreatList();
                        for (int i=threatlist.size()-1;i>=0;--i) {
                            threatlist[i]->LoadImg("img//threat_right.png", g_screen);
                            threatlist[i]->set_clips();
                        }
                        level.SetBoolLoading(false);
                    }
                }
                else if(GameOver){
                    game_over.handleEvents(g_event,p_player);
                    if(game_over.GetBoolLoading()){
                        game_map.LoadMap(file_map);
                        game_map.LoadTiles(g_screen);
                        game_map.SpawnThreat();
                        threatlist = game_map.GetThreatList();
                        for (int i=threatlist.size()-1;i>=0;--i) {
                            threatlist[i]->LoadImg("img//threat_right.png", g_screen);
                            threatlist[i]->set_clips();
                        }
                        game_over.SetBoolLoading(false);
                    }
                }
            }
            if(gameRunning){
                if(g_event.key.keysym.sym == SDLK_ESCAPE){
                    gameRunning = false;
                    Pause =true;
                    p_player.Clear();
                }
                p_player.HandleInputAction(g_event,g_screen);
            }
        }
        if (Title) {
            title.update();
            title.render(g_screen);
        }
        else if (Pause) {
            pause.update();
            pause.render(g_screen);
        }
        else if (LevelChoose){
            level.update();
            level.render(g_screen);
        }
        else if(GameOver){
            game_over.update();
            game_over.render(g_screen);
        }

        else if (gameRunning){

            fps.game_start();
            //clear screen
            SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
            SDL_RenderClear(g_screen);
            g_background.Render(g_screen, NULL);

            Map map_data = game_map.GetMap();

            p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
            p_player.DoPlayer(map_data);
            p_player.Show(g_screen);

            game_map.SetMap(map_data);
            game_map.DrawMap(g_screen);
            //lay bulletlist u player
            bulletlist = p_player.Get_Bulletlist();
            //xu li threatlist
            for(int i=threatlist.size()-1;i>=0;--i){
                threatlist[i]->SetMapXY(map_data.start_x_,map_data.start_y_);
                threatlist[i]->Attack_player(p_player);
                threatlist[i]->MovetoPlayer(p_player, map_data);
                threatlist[i]->AutoMoveThreat(map_data);
                threatlist[i]->Bullet_to_threat(bulletlist);
                threatlist[i]->Show(g_screen);
                if(threatlist[i]->Get_health()<=0){
                    delete threatlist[i];
                    threatlist[i]=nullptr;
                    threatlist.erase(threatlist.begin()+i);
                }
            }
            //xu li dan
            for(int i=bulletlist.size()-1;i>=0;--i){
                bulletlist[i]->CheckToMap(map_data);
                if(bulletlist[i]->Get_is_move()==false) {
                    delete bulletlist[i];
                    bulletlist[i]=nullptr;
                    bulletlist.erase(bulletlist.begin()+i);
                    continue;
                }
                bulletlist[i]->HandleBulletMove(map_data.start_x_,map_data.start_y_);
                bulletlist[i]->Render(g_screen,NULL);

            }
            p_player.Set_Bulletlist(bulletlist);//cap nhat lai bulletlist cua player

            SDL_RenderPresent(g_screen); //in ra man hinh
            //xu li fps
            int real_time_loop = fps.get_ticks();
            if (real_time_loop < one_frame_time) {
                SDL_Delay(one_frame_time - real_time_loop);
            }
            //player die
            if (p_player.GetHealth() <= 0) {
                GameOver = true;
                gameRunning = false;
                ClearVector(bulletlist);
                p_player.Set_Bulletlist(bulletlist);
                ClearVector(threatlist);
                game_map.SetThreatList(threatlist);
                p_player.Clear();
            }
            //winning
            if(p_player.GetBoolComplete()){
                LevelChoose = true;
                gameRunning = false;
                level.SaveLevel();
                ClearVector(bulletlist);
                p_player.Set_Bulletlist(bulletlist);
                ClearVector(threatlist);
                game_map.SetThreatList(threatlist);
                p_player.SetBoolComplete(false);
                p_player.Clear();
            }
        }
    }
    //xoa het tat ca
    ClearVector(bulletlist);
    p_player.Set_Bulletlist(bulletlist);
    ClearVector(threatlist);
    game_map.SetThreatList(threatlist);
    close();
}
