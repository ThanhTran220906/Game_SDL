
#include "Game.h"

template <typename T>
void ClearVector(vector<T*>& v) {
    for (int i = v.size() - 1; i >= 0; i--) {
        delete v[i];
        v.erase(v.begin() + i);
    }
}

Game::Game()
{
    file_map="";
}

Game::~Game()
{
    ClearData();
}


void Game::Init()
{
    g_background.LoadImg("img//background.png", g_screen);
    game_map.LoadTiles(g_screen);
    p_player.LoadImg("img//player_right.png", g_screen);
    p_player.set_clips();
    Coin.TextInit(); Coin.SetXY(20,20); Coin.SetSize(20);
}

void Game::ClearData()
{
    ClearVector(bulletlist); p_player.Set_Bulletlist(bulletlist);
    ClearVector(grenadelist); p_player.Set_Grenadelist(grenadelist);
    ClearVector(explodelist);
    ClearVector(threatlist); game_map.SetThreatList(threatlist);
}

void Game::HandleThreat()
{
    for(int i=threatlist.size()-1;i>=0;--i){
        threatlist[i]->SetMapXY(map_data.start_x_,map_data.start_y_);
        threatlist[i]->Do_Threat(map_data,p_player);
        threatlist[i]->Bullet_to_threat(bulletlist, explodelist);
        if(threatlist[i]->Get_health()<0){
            delete threatlist[i]; threatlist[i]=nullptr;
            threatlist.erase(threatlist.begin()+i); continue;
        }
        threatlist[i]->Show(g_screen);
    }
}


void Game::HandleBullet()
{
    for(int i=bulletlist.size()-1;i>=0;--i){
        bulletlist[i]->CheckToMap(map_data);
        if(!bulletlist[i]->Get_is_move()) {
            delete bulletlist[i]; bulletlist[i]=nullptr;
            bulletlist.erase(bulletlist.begin()+i); continue;
        }
        bulletlist[i]->HandleBulletMove(map_data.start_x_,map_data.start_y_);
        bulletlist[i]->Render(g_screen,NULL);

    }
}

void Game::HandleGrenade()
{
    for(int i=grenadelist.size()-1;i>=0;--i){
        grenadelist[i]->CheckToMap(map_data);
        if(!grenadelist[i]->Get_is_move()) {
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
}

void Game::HandleExplode()
{
    for(int i=explodelist.size()-1;i>=0;--i){
        explodelist[i]->Show(g_screen,map_data.start_x_,map_data.start_y_);
        if(explodelist[i]->Get_delete()){
            delete explodelist[i]; explodelist[i]=nullptr;
            explodelist.erase(explodelist.begin()+i); continue;
        }
    }
}

void Game::HandleSystem()
{
    int x=0,y=0; SDL_GetMouseState(&x, &y);
    if (Title) {
        SDL_Delay(40);
        title.handleEvents(g_event,x,y);
    }
    else if (Pause) {
        SDL_Delay(40);
        pause.handleEvents(g_event,x,y);
        if(pause.GetBoolLoading()){
            ClearData();
            pause.SetBoolLoading(false);
        }
    }
    else if (LevelChoose){
        SDL_Delay(40);
        level.handleEvents(g_event, p_player,x,y);
        if(level.GetBoolLoading()){
            file_map=level.GetFilemap();
            threatlist = game_map.Loadgame(file_map,g_screen);
            level.SetBoolLoading(false);
        }
    }
    else if(GameOver){
        SDL_Delay(40);
        game_over.handleEvents(g_event,p_player,x,y);
        if(game_over.GetBoolLoading()){
            threatlist = game_map.Loadgame(file_map,g_screen);
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


void Game::RenderSystem()
{
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
}

void Game::Logic()
{
    fps.game_start();
    //clear screen
    SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
    SDL_RenderClear(g_screen);
    g_background.Render(g_screen, NULL);

    map_data = game_map.GetMap();

    p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
    p_player.DoPlayer(map_data);
    p_player.Show(g_screen);

    game_map.SetMap(map_data);
    game_map.DrawMap(g_screen);

    //lay bulletlist tu player
    bulletlist = p_player.Get_Bulletlist();
    grenadelist = p_player.Get_Grenadelist();
    //xu li threatlist
    HandleThreat();
    //xu li dan
    HandleBullet();
    //xu li grenade
    HandleGrenade();
    //xu li explode
    HandleExplode();

    p_player.Set_Grenadelist(grenadelist);
    p_player.Set_Bulletlist(bulletlist);//cap nhat lai bulletlist cua player
    //coin
    Coin.SetText("Coin: "+to_string(p_player.Get_Coin())); Coin.loadFromRenderedText(g_screen); Coin.RenderText(g_screen);

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
        ClearData();
        p_player.Clear();
    }
    //winning
    if(p_player.GetBoolComplete()){
        LevelChoose = true;
        gameRunning = false;
        level.SaveLevel();
        ClearData();
        p_player.Clear();
        p_player.SetBoolComplete(false);
    }
}













