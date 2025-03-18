
#pragma once

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"

#define GRAVITY 1.3
#define MAX_FALL_SPEED 15
#define PLAYER_SPEED 9
#define JUMP_VAL 17
#define PLAYER_MAX_HEALTH 10

#define COME_BACK_TIME 1000

class MainObject : public BaseObject
{
public:
    MainObject();
    ~MainObject();

    enum WalkType
    {
        IDLE_RIGHT=0,
        IDLE_LEFT=1,
        WALK_RIGHT=2,
        WALK_LEFT=3,
        RUN_LEFT=4,
        RUN_RIGHT=5,
    };

    bool LoadImg(string path,SDL_Renderer *screen);
    void Show(SDL_Renderer *des);
    void HandleInputAction(SDL_Event events,SDL_Renderer *screen);
    void set_clips();

    void DoPlayer(Map &map_data);
    void CheckToMap(Map &map_data);

    void SetMapXY(int x,int y){ map_x_=x; map_y_=y;}
    void CenterUntinyOnMap(Map &map_data);

    void earn_coin(int &val);

    void SetPos(int x,int y) { x_pos_=x;y_pos_=y; };
    float Get_x_pos(){ return x_pos_;};
    float Get_y_pos(){ return y_pos_;};

    void Set_Bulletlist(const vector<BulletObject*>& v) { bulletlist = v;};
    vector<BulletObject*> Get_Bulletlist(){return bulletlist;};

    void SetHealth(int health) { current_health_=health;};
    int GetHealth(){return current_health_;};

    void RenderHealthBar(SDL_Renderer* renderer);

    void CompleteLevel(int &val);
    void SetBoolComplete(bool v) { complete = v; };
    bool GetBoolComplete() { return complete; };

    void Clear();

private:
    float x_val_;
    float y_val_;

    float x_pos_;
    float y_pos_;

    int width_frame_;
    int height_frame_;

    SDL_Rect frame_clip_[8];//1 ảnh 8 frame
    Input input_type_;
    int frame_;//thứ tự của frame trong 8 frame
    int status_;// sang trai hay sang phai

    int on_ground_;

    int map_x_;
    int map_y_;

    int count_coins_;

    vector<BulletObject*> bulletlist;

    int current_health_;

    bool complete;

    bool delay;
};
