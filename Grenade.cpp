

#include "Grenade.h"

GrenadeObject::GrenadeObject()
{
    x_pos=0;
    y_pos=0;
    x_val=0;
    y_val=0;
    is_move=false;
}

GrenadeObject::~GrenadeObject()
{

}

void GrenadeObject::CreateGrenade(int x,int y,int mapx,int mapy)
{
    is_move=true;
    x_pos=x;
    y_pos=y;
    rect_.x=x_pos-mapx;
    rect_.y=y_pos-mapy;

}

void GrenadeObject::SetVal(int x,int y)
{
    x_val=x;
    y_val=y;

}

void GrenadeObject::CheckToMap(Map map_data)
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
            if(map_data.tile[y1][x2]!=BLANK_TILE || map_data.tile[y2][x2]!=BLANK_TILE){
                is_move=false;
            }
        }
        if(x_val<0){
            if(map_data.tile[y1][x1]!=BLANK_TILE || map_data.tile[y2][x1]!=BLANK_TILE){
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
            if(map_data.tile[y2][x1]!=BLANK_TILE || map_data.tile[y2][x2]!=BLANK_TILE){
                is_move=false;
            }
        }
        if(y_val<0){
            if(map_data.tile[y1][x1]!=BLANK_TILE || map_data.tile[y1][x2]!=BLANK_TILE){
                is_move=false;
            }
        }
    }
    if(y_pos>SCREEN_HEIGHT|| x_pos<0 || x_pos>MAX_MAP_X*TILE_SIZE) is_move=false;

}

void GrenadeObject::HandleGrenadeMove(int mapx,int mapy)
{
    x_pos+=x_val;
    y_val+=3;
    y_pos+=y_val;
    rect_.x=x_pos-mapx;
    rect_.y=y_pos-mapy;
}



//exolode
Explode::Explode()
{
    x_pos=0;
    y_pos=0;
    frame=0;
    explode=0;
    width_frame=0;
    height_frame=0;
    delay=false;
    delete_=false;
}

Explode::~Explode()
{

}

bool Explode::LoadImg(string path,SDL_Renderer *screen)
{
    bool ret=BaseObject::LoadImg(path,screen);//kế thừa từ baseobject
    if(ret==true)
    {
        width_frame=rect_.w/6;
        height_frame=rect_.h;
    }
    if(width_frame>0 && height_frame>0)//hàm LoadImg chạy thành công
    {
        for(int i=0;i<6;i++){
            frame_clip[i].x=i*width_frame;
            frame_clip[i].y=0;
            frame_clip[i].w=width_frame;
            frame_clip[i].h=height_frame;
        }
    }
    return ret;
}
void Explode::Show(SDL_Renderer *des,int mapx,int mapy)
{
    if(delay) {
        frame++;
        delay=!delay;
    }
    else delay=!delay;
    if(frame>=5 && delay) delete_=true;
    frame=frame%6;

    rect_.x=x_pos-mapx;
    rect_.y=y_pos-mapy;

    SDL_Rect* current_clips=&frame_clip[frame];

    SDL_Rect renderQuad={rect_.x,rect_.y, width_frame,height_frame};

    SDL_RenderCopy(des,p_object_,current_clips,&renderQuad);


}

void Explode::CreateExplode(int x,int y,int mapx,int mapy)
{
    x_pos=x;
    y_pos=y;
    rect_.x=x_pos-mapx;
    rect_.y=y_pos-mapy;
}



