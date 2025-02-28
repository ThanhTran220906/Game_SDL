
#pragma once

#include "CommonFunc.h"
#include "BaseObject.h"

#define GRAVITY 1.2
#define MAX_FALL_SPEED 15
#define PLAYER_SPEED 40//10
#define JUMP_VAL 15

#define COME_BACK_TIME 1000

class MainObject : public BaseObject
{
public:
    MainObject();
    ~MainObject();

    enum WalkType
    {
        WALK_RIGHT=0,
        WALK_LEFT=1,
    };

    bool LoadImg(string path,SDL_Renderer *screen);
    void Show(SDL_Renderer *des);
    void HandleInputAction(SDL_Event events,SDL_Renderer *screen);
    void set_clips();

    void DoPlayer(Map &map_data);
    void CheckToMap(Map &map_data);

    void SetMapX(int x){ map_x_=x;}
    void CenterUntinyOnMap(Map &map_data);

    bool earn_coin(int &val1,int &val2);

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

    int on_ground;

    int map_x_;

    int count_coins;

};
