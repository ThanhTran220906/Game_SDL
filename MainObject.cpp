
#include "MainObject.h"
#include <bits/stdc++.h>


MainObject::MainObject()
{
    frame_=0;
    x_pos_=SCREEN_WIDTH/2;
    y_pos_=0;
    x_val_=0;
    y_val_=0;
    width_frame_=0;
    height_frame_ =0;
    status_=-1;
    input_type_.left_=0;
    input_type_.right_=0;
    input_type_.jump_=0;
    input_type_.up_=0;
    input_type_.down_=0;

    on_ground=0;

    map_x_=0;

    count_coins=0;
}

MainObject::~MainObject(){}

bool MainObject::LoadImg(string path,SDL_Renderer *screen)
{
    bool ret=BaseObject::LoadImg(path,screen);//kế thừa từ baseobject
    if(ret==true)
    {
        width_frame_=rect_.w/8;
        height_frame_=rect_.h;
    }

    return ret;
}


void MainObject::set_clips()
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

void MainObject::Show(SDL_Renderer *des)
{
    if(status_==WALK_LEFT){
        LoadImg("img//player_left.png",des);
    }
    else {
        LoadImg("img//player_right.png",des);
    }

    if(input_type_.left_==1||input_type_.right_==1){
        frame_++;
        if(frame_>=8) frame_=0;

    }
    else frame_=0;

    rect_.x=x_pos_-map_x_;
    rect_.y=y_pos_;

    SDL_Rect* current_clips=&frame_clip_[frame_];

    SDL_Rect renderQuad={rect_.x,rect_.y, width_frame_,height_frame_};

    SDL_RenderCopy(des,p_object_,current_clips,&renderQuad);

}

void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer *screen)
{
    if(events.type==SDL_KEYDOWN){
        switch(events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                status_=WALK_RIGHT;
                input_type_.right_=1;
                input_type_.left_=0;
            }
            break;
        case SDLK_LEFT:
            {
                status_=WALK_LEFT;
                input_type_.left_=1;
                input_type_.right_=0;
            }
            break;
        }
    }
    else if(events.type==SDL_KEYUP){
        switch(events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                input_type_.right_=0;
                input_type_.left_=0;
            }
            break;
        case SDLK_LEFT:
            {
                input_type_.left_=0;
                input_type_.right_=0;
            }
            break;
        }
    }
    if(events.type==SDL_KEYDOWN){
        if(events.key.keysym.sym==SDLK_SPACE){
            input_type_.jump_=1;
        }
    }

}

void MainObject::DoPlayer(Map &map_data)
{
    x_val_=0;
    y_val_+=GRAVITY;

    if(y_val_>=MAX_FALL_SPEED) y_val_=MAX_FALL_SPEED;

    if(input_type_.left_==1) x_val_-=PLAYER_SPEED;

    if(input_type_.right_==1) x_val_+=PLAYER_SPEED;

    if(input_type_.jump_==1) {
        if(on_ground!=0){
            y_val_=-JUMP_VAL;
            on_ground--;
        }
        input_type_.jump_=0;
    }


    CheckToMap(map_data);
    CenterUntinyOnMap(map_data);
}

void MainObject::CenterUntinyOnMap(Map &map_data)
{
    map_data.start_x_=x_pos_-(SCREEN_WIDTH/2);
    if(map_data.start_x_<0) map_data.start_x_=0;
    else if(map_data.start_x_+SCREEN_WIDTH>map_data.max_x_) map_data.start_x_=map_data.max_x_-SCREEN_WIDTH-1;
}

void MainObject::CheckToMap(Map &map_data)
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
            if(earn_coin(map_data.tile[y1][x2],map_data.tile[y2][x2])){;}
            else if(map_data.tile[y1][x2]!=BLANK_TILE || map_data.tile[y2][x2]!=BLANK_TILE){
                x_pos_ = x2 * TILE_SIZE;
                x_pos_-=width_frame_+1;
                x_val_ = 0;
            }
        }
        if(x_val_<0){

            if(earn_coin(map_data.tile[y1][x1],map_data.tile[y2][x1])){;}
            else if(map_data.tile[y1][x1]!=BLANK_TILE || map_data.tile[y2][x1]!=BLANK_TILE){
                x_pos_ = (x1 + 1) * TILE_SIZE;
                x_val_ = 0;
            }
        }
    }
    //check vertical
    int width_min =min(width_frame_,TILE_SIZE);
    x1=(x_pos_)/TILE_SIZE;
    x2=(x_pos_+width_min)/TILE_SIZE;

    y1=(y_pos_+y_val_)/TILE_SIZE;
    y2=(y_pos_+y_val_+height_frame_-1)/TILE_SIZE;

    if(on_ground==0||on_ground==1) frame_=0;
    if(x1>=0 && x2<MAX_MAP_X && y1>=0 && y2<MAX_MAP_Y){
        if(y_val_>0){// down
            if(earn_coin(map_data.tile[y2][x1],map_data.tile[y2][x2])){;}
            else if(map_data.tile[y2][x1]!=BLANK_TILE || map_data.tile[y2][x2]!=BLANK_TILE){
                y_pos_ = y2 * TILE_SIZE;
                y_pos_ -= height_frame_;
                y_val_ = 0;
                on_ground=2;
            }
        }
        if(y_val_<0){
            if(earn_coin(map_data.tile[y1][x1],map_data.tile[y1][x2])){;}
            else if(map_data.tile[y1][x1]!=BLANK_TILE || map_data.tile[y1][x2]!=BLANK_TILE){
                y_pos_ = (y1 + 1) * TILE_SIZE;
                y_val_ = 0;
            }
        }
    }


    x_pos_+=x_val_;
    y_pos_+=y_val_;

    if(x_pos_<0) x_pos_=0;
    if(x_pos_+width_frame_>map_data.max_x_) x_pos_=map_data.max_x_-width_frame_-1;

    if(y_pos_>SCREEN_WIDTH) y_pos_=0;
}

bool MainObject::earn_coin(int &val1,int &val2)
{
    if(val1==COIN_TILE)
    {
        val1=BLANK_TILE;
        count_coins++;
        return true;
    }
    if(val2==COIN_TILE)
    {
        val1=BLANK_TILE;
        count_coins++;
        return true;
    }
    return false;
}//xu li xong nhat dong xu


















