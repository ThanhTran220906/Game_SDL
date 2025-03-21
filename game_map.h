
#pragma once

#include "CommonFunc.h"
#include "BaseObject.h"
#include "ThreatObject.h"

#define MAX_TILES 20
#define THREAT_TILE 6

class TileMat : public BaseObject
{    void SetThreatList(vector<ThreatObject*> v);
public:
    TileMat(){;};
    ~TileMat(){;};
};

class GameMap
{
public:
    GameMap(){;};
    ~GameMap(){;};

    void LoadMap(string name);
    void LoadTiles(SDL_Renderer* screen);
    void DrawMap(SDL_Renderer* screen);

    Map GetMap() const {return game_map_;};
    void SetMap(Map &map_data) { game_map_=map_data; }

    vector<ThreatObject*> GetThreatList(){ return threatlist;};
    void SetThreatList(vector<ThreatObject*> &v) { threatlist = v;};

private:
    Map game_map_;
    TileMat tile_mat_[MAX_TILES];
    vector<ThreatObject*> threatlist;
};


