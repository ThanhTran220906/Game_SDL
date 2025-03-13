
#include "ThreatObject.h"
#include <bits/stdc++.h>




ThreatObject::ThreatObject()
{
    frame_=0;
    x_pos_=0;
    y_pos_=0;
    width_frame_=0;
    height_frame_ =0;
    status_=WALK_LEFT;

    map_x_=0;
    map_y_=0;

    health=1;
    near_player=false;
}

ThreatObject::~ThreatObject()
{
    Free();
}

bool ThreatObject::LoadImg(string path,SDL_Renderer *screen)
{
    bool ret=BaseObject::LoadImg(path,screen);//kế thừa từ baseobject
    if(ret==true)
    {
        width_frame_=rect_.w/8;
        height_frame_=rect_.h;
    }

    return ret;
}


void ThreatObject::set_clips()
{
    if(width_frame_>0&&height_frame_>0)//hàm LoadImg chạy thành công
    {
        for(int i=0;i<8;i++){
            frame_clip_[i].x=i*width_frame_;
            frame_clip_[i].y=0;
            frame_clip_[i].w=width_frame_;
            frame_clip_[i].h=height_frame_;
        }
    }
}

void ThreatObject::Show(SDL_Renderer *des)
{
    if(status_==WALK_LEFT){
        LoadImg("img//threat_left.png",des);
    }
    else {
        LoadImg("img//threat_right.png",des);
    }

        frame_++;
        if(frame_>=8) frame_=0;

    rect_.x=x_pos_-map_x_;

    rect_.y=y_pos_-map_y_;


    SDL_Rect* current_clips=&frame_clip_[frame_];

    SDL_Rect renderQuad={rect_.x,rect_.y, width_frame_,height_frame_};

    SDL_RenderCopy(des,p_object_,current_clips,&renderQuad);


}



void ThreatObject::AutoMoveThreat(Map &map_data)
{   if(near_player==false){
        x_val_=0;

        if(status_==WALK_LEFT) x_val_-=THREAT_VAL;

        if(status_==WALK_RIGHT) x_val_+=THREAT_VAL;

        CheckToMap(map_data);
    }
}

void ThreatObject::CheckToMap(Map &map_data)
{
    int x1=0;
    int x2=0;

    int y1=0;
    int y2=0;

    //check horizontal
    int height_min =min(height_frame_,TILE_SIZE);
    x1 = (x_pos_ + x_val_) / TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

    y1=(y_pos_)/TILE_SIZE;
    y2=(y_pos_+height_min-1)/TILE_SIZE;

    if(x1>=0 && x2<MAX_MAP_X && y1>=0 && y2<MAX_MAP_Y){

        if(x_val_>0){// right
            if(map_data.tile[y1][x2]!=BLANK_TILE || map_data.tile[y2][x2]!=BLANK_TILE||map_data.tile[y2+1][x2]==BLANK_TILE){
                status_=WALK_LEFT;
                x_val_=0;
            }
        }
        if(x_val_<0){
            if(map_data.tile[y1][x1]!=BLANK_TILE || map_data.tile[y2][x1]!=BLANK_TILE||map_data.tile[y2+1][x1]==BLANK_TILE){
                status_=WALK_RIGHT;
                x_val_=0;
            }
        }
    }

    x_pos_+=x_val_;
    y_pos_+=y_val_;

}
void ThreatObject::MovetoPlayer(MainObject player, Map map_data)
{
    if(sqrt(pow(player.Get_x_pos()-x_pos_,2)+pow(player.Get_y_pos()-y_pos_,2))<300){
        near_player=true;
        x_val_=THREAT_VAL*(player.Get_x_pos()-x_pos_)/abs(player.Get_x_pos()-x_pos_);
        if(x_val_<0) status_=WALK_LEFT;
        else status_=WALK_RIGHT;
        int height_min =min(height_frame_,TILE_SIZE);
        int x1 = (x_pos_ + x_val_) / TILE_SIZE;
        int x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

        int y1=(y_pos_)/TILE_SIZE;
        int y2=(y_pos_+height_min-1)/TILE_SIZE;

        if(x1>=0 && x2<MAX_MAP_X && y1>=0 && y2<MAX_MAP_Y){

            if(x_val_>0){// right
                if(map_data.tile[y1][x2]!=BLANK_TILE || map_data.tile[y2][x2]!=BLANK_TILE||map_data.tile[y2+1][x2]==BLANK_TILE){
                    x_val_=0;
                }
            }
            if(x_val_<0){
                if(map_data.tile[y1][x1]!=BLANK_TILE || map_data.tile[y2][x1]!=BLANK_TILE||map_data.tile[y2+1][x1]==BLANK_TILE){
                    x_val_=0;
                }
            }
        }
    x_pos_+=x_val_;

    }
    else {near_player=false;}
}

void ThreatObject::Bullet_to_threat()
{
    for(int i=0;i<bulletlist.size();i++){
        SDL_Rect r=bulletlist[i]->GetRect();
        if((r.x>rect_.x  &&  r.x<rect_.x+width_frame_ &&  r.y>rect_.y  &&  r.y<rect_.y+height_frame_)
           || (r.x+r.w>rect_.x && r.x+r.w<rect_.x+width_frame_ &&  r.y>rect_.y  &&  r.y<rect_.y+height_frame_)
           || (r.x>rect_.x  &&  r.x<rect_.x+width_frame_ &&  r.y+r.h >rect_.y  &&  r.y+r.h <rect_.y+height_frame_)
           || (r.x+r.w>rect_.x && r.x+r.w<rect_.x+width_frame_&& r.y+r.h >rect_.y  &&  r.y+r.h <rect_.y+height_frame_))
            {
                bulletlist[i]->Set_is_move(false);
                health--;
            }
    }
}












