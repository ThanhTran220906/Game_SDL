
#include <bits/stdc++.h>
#include "CommonFunc.h"

bool gameRunning =false;
bool Title =true;
bool LevelChoose =false;
bool Pause =false;
bool GameOver = false;
bool is_quit=false;

bool SoundEnable =true;

Mix_Chunk*  g_sound_bullet =NULL;
Mix_Chunk*  g_sound_explode =NULL;
Mix_Chunk*  g_sound_jump =NULL;
Mix_Chunk*  g_sound_coin =NULL;
Mix_Chunk*  g_sound_hurt =NULL;
Mix_Chunk*  g_sound_menu =NULL;
