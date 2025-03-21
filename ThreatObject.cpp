
#include "ThreatObject.h"
#include <bits/stdc++.h>




ThreatObject::ThreatObject()
{
    frame_=0;
    x_pos_=0;
    y_pos_=0;
    x_val_=0;
    y_val_=0;

    width_frame_=0;
    height_frame_ =0;
    status_=WALK_RIGHT;

    map_x_=0;
    map_y_=0;

    current_health=THREAT_MAX_HEALTH;
    move_to_player=false;
    is_attack=false;
    run_attack=false;
    delay=0;
}

ThreatObject::~ThreatObject()
{

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
    else if(status_==WALK_RIGHT) {
        LoadImg("img//threat_right.png",des);
    }
    else if(status_==ATTACK_LEFT){
        LoadImg("img//attack_left.png",des);
    }
    else if(status_==ATTACK_RIGHT){
        LoadImg("img//attack_right.png",des);
    }
    else if(status_==RUN_RIGHT){
        LoadImg("img//run_right.png",des);
    }
    else if(status_==RUN_LEFT){
        LoadImg("img//run_left.png",des);
    }
    else if(status_==RUN_ATTACK_RIGHT){
        LoadImg("img//run_attack_right.png",des);
    }
    else if(status_==RUN_ATTACK_LEFT){
        LoadImg("img//run_attack_left.png",des);
    }

    if(delay) {
        frame_++;
        delay=!delay;
    }
    else delay=!delay;

    frame_=frame_%8;


    rect_.x=x_pos_-map_x_;

    rect_.y=y_pos_-map_y_;


    SDL_Rect* current_clips=&frame_clip_[frame_];

    SDL_Rect renderQuad={rect_.x,rect_.y, width_frame_,height_frame_};

    SDL_RenderCopy(des,p_object_,current_clips,&renderQuad);

    // Vẽ thanh máu
    RenderHealthBar(des);

}

void ThreatObject::Do_Threat(Map &map_data, MainObject &player)
{
    int dx = player.Get_x_pos() - x_pos_;
    int dy = player.Get_y_pos() - y_pos_;
    if(!run_attack && !is_attack){
        if(dx*dx+dy*dy<25000 && dx*dx+dy*dy>23000 && abs(dy)<30){
            run_attack=true; frame_=0;
            x_val_ = THREAT_VAL * ((dx > 0) ? 1 : -1) * 3; // Tăng tốc nhưng không nhảy
            status_ = (x_val_ < 0) ? RUN_ATTACK_LEFT : RUN_ATTACK_RIGHT;
        }
        else if(dx*dx+dy*dy<2500) {
            is_attack=true; frame_=0;
            status_ = (dx<0) ? ATTACK_LEFT : ATTACK_RIGHT;
        }
        else if(dx*dx+dy*dy<90000) move_to_player=true;
        else move_to_player=false;
    }
    if(run_attack){
        Run_attack(player, map_data);
    }
    else if(is_attack){
        Attack_player(player);
    }
    else if(move_to_player){
        MovetoPlayer(player,map_data);
    }
    else AutoMoveThreat(map_data);
}

void ThreatObject::AutoMoveThreat(Map &map_data)
{
    if(status_==RUN_LEFT||status_==RUN_ATTACK_LEFT||status_==ATTACK_LEFT) status_=WALK_LEFT;
    else if(status_==RUN_RIGHT||status_==RUN_ATTACK_RIGHT||status_==ATTACK_RIGHT) status_=WALK_RIGHT;
    x_val_ = (status_ == WALK_LEFT) ? -THREAT_VAL : THREAT_VAL;

    int height_min =min(height_frame_,TILE_SIZE);
    int x1 = (x_pos_ + x_val_) / TILE_SIZE;
    int x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;
    int y1=(y_pos_)/TILE_SIZE;
    int y2=(y_pos_+height_min-1)/TILE_SIZE;

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

    x_pos_ += x_val_;
}


void ThreatObject::MovetoPlayer(MainObject player, Map map_data)
{
    int dx = player.Get_x_pos() - x_pos_;
    x_val_ = THREAT_VAL * ((dx > 0) ? 1 : -1) * 1.5; // Di chuyển nhanh hơn nhưng không nhảy

    status_ = (x_val_ < 0) ? RUN_LEFT : RUN_RIGHT;

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

    x_pos_ += x_val_;
}

void ThreatObject::Run_attack(MainObject &player, Map map_data)
{
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

    if(frame_==6 && delay){
        SDL_Rect r=player.GetRect();
        if((r.x>rect_.x  &&  r.x<rect_.x+width_frame_ &&  r.y>rect_.y  &&  r.y<rect_.y+height_frame_)
           || (r.x+r.w/8>rect_.x && r.x+r.w/8<rect_.x+width_frame_ &&  r.y>rect_.y  &&  r.y<rect_.y+height_frame_)
           || (r.x>rect_.x  &&  r.x<rect_.x+width_frame_ &&  r.y+r.h >rect_.y  &&  r.y+r.h <rect_.y+height_frame_)
           || (r.x+r.w/8>rect_.x && r.x+r.w/8<rect_.x+width_frame_&& r.y+r.h >rect_.y  &&  r.y+r.h <rect_.y+height_frame_)
           || (r.x>rect_.x  &&  r.x<rect_.x+width_frame_ &&  r.y+r.h/2 >rect_.y  &&  r.y+r.h/2 <rect_.y+height_frame_)
           || (r.x+r.w/8>rect_.x && r.x+r.w/8<rect_.x+width_frame_&& r.y+r.h/2 >rect_.y  &&  r.y+r.h/2 <rect_.y+height_frame_)
           )
        {
            if(SoundEnable) int ret = Mix_PlayChannel(-1, g_sound_hurt , 0);
            int health_player=player.GetHealth();
            health_player--;
            player.SetHealth(health_player);
        }
    }
    x_pos_ += x_val_;
    if(frame_>=7) {
        run_attack=false;
        frame_=0;
        status_ = (status_==RUN_ATTACK_LEFT) ? WALK_LEFT : WALK_RIGHT;

    }
}

void ThreatObject::Attack_player(MainObject &player)
{
    if(frame_==3 && delay){
        SDL_Rect r=player.GetRect();
        if((r.x>rect_.x  &&  r.x<rect_.x+width_frame_ &&  r.y>rect_.y  &&  r.y<rect_.y+height_frame_)
           || (r.x+r.w/8>rect_.x && r.x+r.w/8<rect_.x+width_frame_ &&  r.y>rect_.y  &&  r.y<rect_.y+height_frame_)
           || (r.x>rect_.x  &&  r.x<rect_.x+width_frame_ &&  r.y+r.h >rect_.y  &&  r.y+r.h <rect_.y+height_frame_)
           || (r.x+r.w/8>rect_.x && r.x+r.w/8<rect_.x+width_frame_&& r.y+r.h >rect_.y  &&  r.y+r.h <rect_.y+height_frame_)
           || (r.x>rect_.x  &&  r.x<rect_.x+width_frame_ &&  r.y+r.h/2 >rect_.y  &&  r.y+r.h/2 <rect_.y+height_frame_)
           || (r.x+r.w/8>rect_.x && r.x+r.w/8<rect_.x+width_frame_&& r.y+r.h/2 >rect_.y  &&  r.y+r.h/2 <rect_.y+height_frame_)
           )
        {
            if(SoundEnable) int ret = Mix_PlayChannel(-1, g_sound_hurt , 0);
            int health_player=player.GetHealth();
            health_player--;
            player.SetHealth(health_player);
        }
    }
    if(frame_>=6&& !delay) {
        is_attack=false;
        frame_=0;
        status_ = (status_==ATTACK_LEFT) ? WALK_LEFT : WALK_RIGHT;
    }
}




void ThreatObject::Bullet_to_threat(vector <BulletObject*> &bulletlist, vector <Explode*> explodelist)
{
    for(int i=0;i<bulletlist.size();i++){
        if(bulletlist[i]->Get_is_move()){
        SDL_Rect r=bulletlist[i]->GetRect();
        if((r.x>rect_.x  &&  r.x<rect_.x+width_frame_ &&  r.y>rect_.y  &&  r.y<rect_.y+height_frame_)
           || (r.x+r.w>rect_.x && r.x+r.w<rect_.x+width_frame_ &&  r.y>rect_.y  &&  r.y<rect_.y+height_frame_)
           || (r.x>rect_.x  &&  r.x<rect_.x+width_frame_ &&  r.y+r.h >rect_.y  &&  r.y+r.h <rect_.y+height_frame_)
           || (r.x+r.w>rect_.x && r.x+r.w<rect_.x+width_frame_&& r.y+r.h >rect_.y  &&  r.y+r.h <rect_.y+height_frame_))
            {
                bulletlist[i]->Set_is_move(false);
                current_health--;
            }
        }
    }
    for(int i=0;i<explodelist.size();i++){
        if(explodelist[i]->Get_frame()==4){
            SDL_Rect r=explodelist[i]->GetRect();
        if((r.x>rect_.x  &&  r.x<rect_.x+width_frame_ &&  r.y>rect_.y  &&  r.y<rect_.y+height_frame_)
           || (r.x+r.w/6>rect_.x && r.x+r.w/6<rect_.x+width_frame_ &&  r.y>rect_.y  &&  r.y<rect_.y+height_frame_)
           || (r.x>rect_.x  &&  r.x<rect_.x+width_frame_ &&  r.y+r.h >rect_.y  &&  r.y+r.h <rect_.y+height_frame_)
           || (r.x+r.w/6>rect_.x && r.x+r.w/6<rect_.x+width_frame_&& r.y+r.h >rect_.y  &&  r.y+r.h <rect_.y+height_frame_))
            {
                current_health--;
            }
        }
    }
}

void ThreatObject::RenderHealthBar(SDL_Renderer* renderer) {
    int bar_width = 50;  // Độ dài thanh máu
    int bar_height = 5;  // Độ cao thanh máu
    int x = rect_.x + 20;  // Vị trí ngang theo tọa độ của Threat
    int y = rect_.y - 10;  // Đặt thanh máu ngay trên đầu Threat

    // Viền đen của thanh máu
    SDL_Rect border = {x, y, bar_width, bar_height};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Màu đen
    SDL_RenderFillRect(renderer, &border);

    // Thanh máu (màu đỏ)
    float health_ratio = (float)current_health / THREAT_MAX_HEALTH;
    SDL_Rect health = {x, y, (int)(bar_width * health_ratio), bar_height};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Màu đỏ
    SDL_RenderFillRect(renderer, &health);
}









