
#include "game_map.h"
#include <bits/stdc++.h>

void GameMap::LoadMap(string name)
{
    game_map_.tile[10][400] ={0};
    fstream fs;
    fs.open(name);

    if(!fs.is_open()) return;

    game_map_.max_x_=0;
    game_map_.max_y_=0;
    for(int i=0;i<MAX_MAP_Y;i++){
        for(int j=0;j<MAX_MAP_X;j++){
            fs>>game_map_.tile[i][j];
            int val =game_map_.tile[i][j];
            if(val>0) {
                if(j>game_map_.max_x_){
                    game_map_.max_x_=j;
                }
                if(i>game_map_.max_y_){
                    game_map_.max_y_=i;
                }
            }
        }
    }

    game_map_.max_x_=(game_map_.max_x_+1)*TILE_SIZE;
    game_map_.max_y_=(game_map_.max_y_+1)*TILE_SIZE;

    game_map_.start_x_=0;
    game_map_.start_y_=0;

    fs.close();
}

void GameMap::LoadTiles(SDL_Renderer* screen)
{
    char file_img[20];
    FILE *fp=NULL;

    for(int i=0;i<MAX_TILES;i++){
        sprintf_s(file_img,"map/%d.png",i);

        fopen_s(&fp,file_img,"rb");
        if(fp==NULL)
        {
            continue;
        }
        fclose(fp);

        tile_mat_[i].LoadImg(file_img,screen);

    }
}

void GameMap::DrawMap(SDL_Renderer *screen)
{
    int x1=0;
    int x2=0;

    int y1=0;
    int y2=0;
    int map_y=0;

    x1=(game_map_.start_x_%TILE_SIZE)*-1;//nho hon TILE_SIZE
    x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

    y1=(game_map_.start_y_%TILE_SIZE)*-1;
    y2= y1+SCREEN_HEIGHT;

    for(int i=y1;i<y2;i+=TILE_SIZE){
        int map_x = game_map_.start_x_/TILE_SIZE;
        for(int j=x1;j<x2;j+=TILE_SIZE){
            int val=game_map_.tile[map_y][map_x];
            if(val>0){
                tile_mat_[val].SetRect(j,i);
                tile_mat_[val].Render(screen);
            }
            map_x++;
        }
        map_y++;
    }
}

void GameMap::SpawnThreat()
{
    //Spawn Threat
    for(int i=0;i<MAX_MAP_Y;i++){
        for(int j=0;j<MAX_MAP_X;j++){
            if(game_map_.tile[i][j]==20){
                ThreatObject *thread_tmp =new ThreatObject;
                thread_tmp->Set_pos(j*TILE_SIZE,i*TILE_SIZE-27);
                game_map_.tile[i][j]=0;
                threatlist.push_back(thread_tmp);
            }
        }
    }      //
}











