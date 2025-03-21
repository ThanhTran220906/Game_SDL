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

    g_window = SDL_CreateWindow("RUN TO SURVICE",
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
    // Mở audio với tần số 22050 Hz, định dạng mặc định, 2 kênh (stereo), buffer 4096
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        return false;
    }
    g_sound_bullet = Mix_LoadWAV("mixer//shot.wav");
    g_sound_explode = Mix_LoadWAV("mixer//explode.wav");
    g_sound_jump = Mix_LoadWAV("mixer//jump.wav");
    g_sound_menu = Mix_LoadWAV("mixer//menu.wav");
    g_sound_coin = Mix_LoadWAV("mixer//coin.wav");
    g_sound_hurt = Mix_LoadWAV("mixer//hurt.wav");

    return success;
}

bool Loadbackground() {
    return g_background.LoadImg("img//background.png", g_screen);
}

void close() {
    g_background.~BaseObject();

    SDL_DestroyRenderer(g_screen);
    SDL_DestroyWindow(g_window);

    Mix_FreeChunk(g_sound_bullet);
    Mix_FreeChunk(g_sound_explode);
    Mix_FreeChunk(g_sound_jump);
    Mix_FreeChunk(g_sound_coin);
    Mix_FreeChunk(g_sound_menu);
    Mix_FreeChunk(g_sound_hurt);

    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}

template <typename T>
void ClearVector(vector<T*>& v) {
    for (int i = v.size() - 1; i >= 0; i--) {
        delete v[i];
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
    vector<GrenadeObject*> grenadelist ;
    vector<Explode*> explodelist ;

    TextObject Coin; Coin.SetXY(20,20); Coin.SetSize(20);
    //game loop
    while (!is_quit) {
        while (SDL_PollEvent(&g_event)) {
            //quit
            if (g_event.type == SDL_QUIT) {
                is_quit = true;
            }
            //xu li game
            int x=0,y=0;
            SDL_GetMouseState(&x, &y);
            if (Title) {
                title.handleEvents(g_event,x,y);
            }
            else if (Pause) {
                pause.handleEvents(g_event,x,y);
                if(pause.GetBoolLoading()){
                    ClearVector(bulletlist);
                    p_player.Set_Bulletlist(bulletlist);
                    ClearVector(threatlist);
                    game_map.SetThreatList(threatlist);
                    pause.SetBoolLoading(false);
                }
            }
            else if (LevelChoose){
                level.handleEvents(g_event, p_player,x,y);
                if(level.GetBoolLoading()){
                    file_map=level.GetFilemap();
                    game_map.LoadMap(file_map);
                    game_map.LoadTiles(g_screen);
                    threatlist = game_map.GetThreatList();
                    for (int i=threatlist.size()-1;i>=0;--i) {
                        threatlist[i]->LoadImg("img//threat_right.png", g_screen);
                        threatlist[i]->set_clips();
                    }
                    level.SetBoolLoading(false);
                }
            }
            else if(GameOver){
                game_over.handleEvents(g_event,p_player,x,y);
                if(game_over.GetBoolLoading()){
                    game_map.LoadMap(file_map);
                    game_map.LoadTiles(g_screen);
                    threatlist = game_map.GetThreatList();
                    for (int i=threatlist.size()-1;i>=0;--i) {
                        threatlist[i]->LoadImg("img//threat_right.png", g_screen);
                        threatlist[i]->set_clips();
                    }
                    game_over.SetBoolLoading(false);
                }
            }
            if(gameRunning){
                if(g_event.key.keysym.sym == SDLK_ESCAPE){
                    gameRunning = false;
                    Pause =true;
                    p_player.Clear();
                }
                p_player.HandleInputAction(g_event);
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

            //lay bulletlist tu player
            bulletlist = p_player.Get_Bulletlist();
            grenadelist = p_player.Get_Grenadelist();
            //xu li threatlist
            for(int i=threatlist.size()-1;i>=0;--i){
                threatlist[i]->SetMapXY(map_data.start_x_,map_data.start_y_);
                threatlist[i]->Do_Threat(map_data,p_player);
                threatlist[i]->Bullet_to_threat(bulletlist, explodelist);
                if(threatlist[i]->Get_health()<=0){
                    delete threatlist[i]; threatlist[i]=nullptr;
                    threatlist.erase(threatlist.begin()+i); continue;
                }
                threatlist[i]->Show(g_screen);

            }
            //xu li dan
            for(int i=bulletlist.size()-1;i>=0;--i){
                bulletlist[i]->CheckToMap(map_data);
                if(bulletlist[i]->Get_is_move()==false) {
                    delete bulletlist[i]; bulletlist[i]=nullptr;
                    bulletlist.erase(bulletlist.begin()+i); continue;
                }
                bulletlist[i]->HandleBulletMove(map_data.start_x_,map_data.start_y_);
                bulletlist[i]->Render(g_screen,NULL);

            }

            for(int i=grenadelist.size()-1;i>=0;--i){
                grenadelist[i]->CheckToMap(map_data);
                if(grenadelist[i]->Get_is_move()==false) {
                    Explode *tmp_explode = new Explode();
                    tmp_explode->CreateExplode(grenadelist[i]->Get_x_pos()-20,grenadelist[i]->Get_y_pos()-30,map_data.start_x_,map_data.start_y_);
                    tmp_explode->LoadImg("img//explode.png",g_screen);
                    explodelist.push_back(tmp_explode);
                    if(SoundEnable) int ret = Mix_PlayChannel(-1, g_sound_explode , 0); // tieng no bom
                    delete grenadelist[i]; grenadelist[i]=nullptr;
                    grenadelist.erase(grenadelist.begin()+i); continue;
                }
                grenadelist[i]->HandleGrenadeMove(map_data.start_x_,map_data.start_y_);
                grenadelist[i]->Render(g_screen,NULL);
            }

            for(int i=explodelist.size()-1;i>=0;--i){
                explodelist[i]->Show(g_screen,map_data.start_x_,map_data.start_y_);
                if(explodelist[i]->Get_delete()){
                    delete explodelist[i]; explodelist[i]=nullptr;
                    explodelist.erase(explodelist.begin()+i); continue;
                }
            }

            p_player.Set_Grenadelist(grenadelist);
            p_player.Set_Bulletlist(bulletlist);//cap nhat lai bulletlist cua player
            //coin
            Coin.SetText("Coin: "+to_string(p_player.Get_Coin()));
            Coin.loadFromRenderedText(g_screen);
            Coin.RenderText(g_screen);

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
                ClearVector(bulletlist); p_player.Set_Bulletlist(bulletlist);
                ClearVector(grenadelist); p_player.Set_Grenadelist(grenadelist);
                ClearVector(threatlist); game_map.SetThreatList(threatlist);
                p_player.Clear();
            }
            //winning
            if(p_player.GetBoolComplete()){
                LevelChoose = true;
                gameRunning = false;
                level.SaveLevel();
                ClearVector(bulletlist); p_player.Set_Bulletlist(bulletlist);
                ClearVector(grenadelist); p_player.Set_Grenadelist(grenadelist);
                ClearVector(threatlist); game_map.SetThreatList(threatlist);
                p_player.SetBoolComplete(false);
                p_player.Clear();
            }
        }
    }
    //xoa het tat ca
    ClearVector(bulletlist); p_player.Set_Bulletlist(bulletlist);
    ClearVector(grenadelist); p_player.Set_Grenadelist(grenadelist);
    ClearVector(threatlist); game_map.SetThreatList(threatlist);
    close();
}
