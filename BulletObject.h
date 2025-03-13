
#pragma once

#include "CommonFunc.h"
#include "BaseObject.h"

#define BULLET_BORDER 800

class BulletObject : public BaseObject
{
public:
    BulletObject();
    ~BulletObject();

    void CreateBullet(int x,int y,int mapx,int mapy);
    int Get_xpos(){ return x_pos; };
    int Get_ypos(){ return y_pos; };
    void SetVal(int x,int y);
    bool Get_is_move(){ return is_move; }
    void Set_is_move(bool ismove) { is_move=ismove; };
    void CheckToMap(Map map_data);

    void HandleBulletMove(int mapx,int mapy);
private:
    int x_pos;
    int y_pos;
    int x_val;
    int y_val;
    int x_border;
    int y_border;
    bool is_move;

};
