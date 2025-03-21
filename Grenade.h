
#pragma once

#include "CommonFunc.h"
#include "BaseObject.h"


class GrenadeObject : public BaseObject
{
public:
    GrenadeObject();
    ~GrenadeObject();

    void CreateGrenade(int x,int y,int mapx,int mapy);
    void SetVal(int x,int y);
    bool Get_is_move(){ return is_move; };
    int Get_x_pos() {return x_pos;};
    int Get_y_pos() {return y_pos;};
    void CheckToMap(Map map_data);
    void HandleGrenadeMove(int mapx,int mapy);

private:
    int x_pos;
    int y_pos;
    int x_val;
    int y_val;
    bool is_move;
};
//explode
class Explode : public BaseObject
{
public:
    Explode();
    ~Explode();
    bool LoadImg(string path,SDL_Renderer *screen);
    void CreateExplode(int x,int y,int mapx,int mapy);
    void Show(SDL_Renderer *des,int mapx,int mapy);
    bool Get_delete(){ return delete_; };
    int Get_frame(){ return frame; };

private:
    int x_pos;
    int y_pos;
    int frame;
    bool explode;
    int width_frame;
    int height_frame;
    bool delay;
    bool delete_;

    SDL_Rect frame_clip[8];
};
