
#pragma once

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"
#include "MainObject.h"

#define THREAT_VAL 6
#define THREAT_MAX_HEALTH 2

class ThreatObject: public BaseObject
{
public:
    ThreatObject();
    ~ThreatObject();

    enum WalkType
    {
        WALK_RIGHT=0,
        WALK_LEFT=1,
        ATTACK_LEFT=2,
        ATTACK_RIGHT=3,
        RUN_LEFT=4,
        RUN_RIGHT=5,
        RUN_ATTACK_RIGHT=6,
        RUN_ATTACK_LEFT=7,
        DEAD_LEFT=8,
        DEAD_RIGHT=9,
    };

    bool LoadImg(string path,SDL_Renderer *screen);
    void Show(SDL_Renderer *des);
    void set_clips();

    void Set_pos(int x,int y){ x_pos_=x;y_pos_=y;};
    void SetMapXY(int x,int y){ map_x_=x; map_y_=y;}
    int Get_health(){return current_health;};

    void AutoMoveThreat(Map &map_data);
    void MovetoPlayer(MainObject player,Map map_data);
    void Attack_player(MainObject &player);
    void Run_attack(MainObject &player, Map map_data);
    void Dead();
    void Do_Threat(Map &map_data, MainObject &player);

    void Bullet_to_threat(vector <BulletObject*> &bulletlist,vector <Explode*> explodelist);


    void RenderHealthBar(SDL_Renderer* renderer);


private:
    float x_val_;
    float y_val_;


    float x_pos_;
    float y_pos_;

    int width_frame_;
    int height_frame_;

    SDL_Rect frame_clip_[8];//1 ảnh 8 frame

    int frame_;//thứ tự của frame trong 8 frame
    int status_;// sang trai hay sang phai

    int map_x_;
    int map_y_;

    int current_health;

    bool move_to_player;
    bool is_attack;
    bool run_attack;
    bool dead;
    bool delay;
};

