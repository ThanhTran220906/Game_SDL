
#include "BulletObject.h"

BulletObject::BulletObject()
{
    x_pos=0;
    y_pos=0;
    x_val=0;
    y_val=0;
    x_border=0;
    y_border=0;
    is_move=false;
}

BulletObject::~BulletObject()
{
    Free();
}

void BulletObject::CreateBullet(int x,int y,int mapx,int mapy)
{
    is_move=true;
    x_pos=x;
    y_pos=y;
    rect_.x=x_pos-mapx;
    rect_.y=y_pos-mapy;

}

void BulletObject::SetVal(int x,int y)
{
    x_val=x;
    y_val=y;
    x_border=x_pos+x/abs(x)*BULLET_BORDER;
    y_border=y_pos+y/abs(y)*BULLET_BORDER;
}

void BulletObject::CheckToMap(Map map_data)
{
    int x1=0;
    int x2=0;

    int y1=0;
    int y2=0;


    //check horizontal
    x1 = (x_pos + x_val) / TILE_SIZE;
    x2 = (x_pos + x_val + rect_.w - 1) / TILE_SIZE;

    y1=(y_pos)/TILE_SIZE;
    y2=(y_pos+rect_.h-1)/TILE_SIZE;

    if(x1>=0 && x2<MAX_MAP_X && y1>=0 && y2<MAX_MAP_Y){
        if(x_val>0){// right
            if(map_data.tile[y1][x2]!=BLANK_TILE || map_data.tile[y2][x2]!=BLANK_TILE||x_pos>x_border){
                is_move=false;
            }
        }
        if(x_val<0){
            if(map_data.tile[y1][x1]!=BLANK_TILE || map_data.tile[y2][x1]!=BLANK_TILE||x_pos<x_border){
                is_move=false;
            }
        }
    }
    //check vertical
    x1=(x_pos)/TILE_SIZE;
    x2=(x_pos+rect_.w)/TILE_SIZE;

    y1=(y_pos+y_val)/TILE_SIZE;
    y2=(y_pos+y_val+rect_.h-1)/TILE_SIZE;


    if(x1>=0 && x2<MAX_MAP_X && y1>=0 && y2<MAX_MAP_Y){
        if(y_val>0){// down
            if(map_data.tile[y2][x1]!=BLANK_TILE || map_data.tile[y2][x2]!=BLANK_TILE||y_pos>y_border){
                is_move=false;
            }
        }
        if(y_val<0){
            if(map_data.tile[y1][x1]!=BLANK_TILE || map_data.tile[y1][x2]!=BLANK_TILE ||y_pos<y_border){
                is_move=false;
            }
        }
    }

}

void BulletObject::HandleBulletMove(int mapx,int mapy)
{
    x_pos+=x_val;
    y_pos+=y_val;
    rect_.x=x_pos-mapx;
    rect_.y=y_pos-mapy;
}
