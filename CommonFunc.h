
#pragma once

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>


using namespace std;
//
static SDL_Window *g_window =NULL;
static SDL_Renderer *g_screen =NULL;
static SDL_Event g_event;
//
extern Mix_Chunk*  g_sound_bullet;
extern Mix_Chunk*  g_sound_explode;
extern Mix_Chunk*  g_sound_jump;
extern Mix_Chunk*  g_sound_coin;
extern Mix_Chunk*  g_sound_menu;
extern Mix_Chunk*  g_sound_hurt;
//
//game
extern bool gameRunning;
extern bool Title;
extern bool LevelChoose;
extern bool Pause;
extern bool GameOver;
extern bool is_quit;

extern bool SoundEnable;

//

const int SCREEN_WIDTH=1200;
const int SCREEN_HEIGHT=640;
const int SCEEN_BPP=32;


const int COLOR_KEY_R=167;
const int COLOR_KEY_G=175;
const int COLOR_KEY_B=180;


#define TILE_SIZE 64

#define MAX_MAP_X 400
#define MAX_MAP_Y 10
#define BLANK_TILE 0
#define COIN_TILE 4

struct Map
{
    int start_x_;
    int start_y_;

    int max_x_;
    int max_y_;//sau nay la 10*TILE_SIZE va 400*TILE_SIZE

    int tile[MAX_MAP_Y][MAX_MAP_X];
};

struct Input
{
    int left_;
    int right_;
    int jump_;
    int run_;
    int shot_;
    int grenade_;
};






