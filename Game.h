
#pragma once

#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "Menu.h"
#include "Level.h"

class Game
{
public:
    Game();
    ~Game();
    void Init();
    void ClearData();
    void HandleThreat();
    void HandleBullet();
    void HandleGrenade();
    void HandleExplode();
    void HandleSystem();
    void RenderSystem();
    void Logic();

private:
    BaseObject g_background;
    GameMap game_map;
    Map map_data;
    string file_map;

    ImpTimer fps;
    titleScreen title;
    Level level;
    pauseMenu pause;
    GameOverMenu game_over;

        //map
    vector<ThreatObject*> threatlist;
        //player
    MainObject p_player;
    vector<BulletObject*> bulletlist ;
    vector<GrenadeObject*> grenadelist ;
    vector<Explode*> explodelist ;

    TextObject Coin;
};
