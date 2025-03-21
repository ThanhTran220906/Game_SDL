#include "MainObject.h"



MainObject::MainObject()
{
    frame_=0;
    x_pos_=SCREEN_WIDTH/2;
    y_pos_=0;
    width_frame_=0;
    height_frame_ =0;
    status_=0;

    input_type_.left_=0;
    input_type_.right_=0;
    input_type_.jump_=0;
    input_type_.run_=0;
    input_type_.shot_=0;
    input_type_.grenade_=0;

    on_ground_=0;

    map_x_=0;
    map_y_=0;
    count_coins_=0;

    current_health_=PLAYER_MAX_HEALTH;

    delay=false;
}

MainObject::~MainObject()
{

}

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
    if(width_frame_>0 && height_frame_>0)//hàm LoadImg chạy thành công
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
    else if(status_==WALK_RIGHT) {
        LoadImg("img//player_right.png",des);
    }
    else if(status_==IDLE_RIGHT) {
        LoadImg("img//idle_right.png",des);
    }
    else if(status_==IDLE_LEFT) {
        LoadImg("img//idle_left.png",des);
    }
    else if(status_==RUN_LEFT) {
        LoadImg("img//p_run_left.png",des);
    }
    else if(status_==RUN_RIGHT) {
        LoadImg("img//p_run_right.png",des);
    }
    else if(status_==SHOT_RIGHT) {
        LoadImg("img//shot_right.png",des);
    }
    else if(status_==SHOT_LEFT) {
        LoadImg("img//shot_left.png",des);
    }
    else if(status_==GRENADE_LEFT) {
        LoadImg("img//grenade_left.png",des);
    }
    else if(status_==GRENADE_RIGHT) {
        LoadImg("img//grenade_right.png",des);
    }

    if(delay) {
        frame_++;
        delay=!delay;
    }
    else {
        delay=!delay;
    }

    frame_=frame_%8;
    if(status_==WALK_LEFT || status_==WALK_RIGHT){
        frame_=frame_%7;
    }
    if (input_type_.shot_==1){
        if(frame_==1 && delay) {
            input_type_.shot_=0;
            BulletObject *bullet_tmp= new BulletObject();
            bullet_tmp->LoadImg("img//bullet.png", des);
            if(status_==SHOT_RIGHT){
                bullet_tmp->CreateBullet(x_pos_ + 30, y_pos_ + 34, map_x_, map_y_);
                bullet_tmp->SetVal(30, 0);
                status_=IDLE_RIGHT;
                LoadImg("img//idle_right.png",des);
            }
            if(status_==SHOT_LEFT){
                bullet_tmp->CreateBullet(x_pos_ + 10, y_pos_ + 34, map_x_, map_y_);
                bullet_tmp->SetVal(-30, 0);
                status_=IDLE_LEFT;
                LoadImg("img//idle_left.png",des);
            }
            bulletlist.push_back(bullet_tmp);

        }
    }
    if (input_type_.grenade_==1){
        if(frame_>=7 && delay) {
            frame_=0;
            input_type_.grenade_=0;
            GrenadeObject *grenade_tmp= new GrenadeObject();
            grenade_tmp->LoadImg("img//grenade.png", des);
            if(status_==GRENADE_RIGHT){
                grenade_tmp->CreateGrenade(x_pos_ + 30, y_pos_ , map_x_, map_y_);
                grenade_tmp->SetVal(30, -20);
                status_=IDLE_RIGHT;
                LoadImg("img//idle_right.png",des);
            }
            if(status_==GRENADE_LEFT){
                grenade_tmp->CreateGrenade(x_pos_ + 10, y_pos_ , map_x_, map_y_);
                grenade_tmp->SetVal(-30, -20);
                status_=IDLE_LEFT;
                LoadImg("img//idle_left.png",des);
            }
            grenadelist.push_back(grenade_tmp);

        }
    }

    rect_.x=x_pos_-map_x_;

    rect_.y=y_pos_-map_y_;


    SDL_Rect* current_clips=&frame_clip_[frame_];

    SDL_Rect renderQuad={rect_.x,rect_.y, width_frame_,height_frame_};

    SDL_RenderCopy(des,p_object_,current_clips,&renderQuad);

    RenderHealthBar(des);

}

void MainObject::HandleInputAction(SDL_Event events)
{
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(input_type_.shot_==1||input_type_.grenade_==1){ return;}
    if (events.type == SDL_KEYDOWN){
        if(events.key.keysym.sym == SDLK_w){
            input_type_.jump_=1;
        }
    }
    if((events.type ==SDL_KEYDOWN && events.key.keysym.sym==SDLK_a)||(state[SDL_SCANCODE_A])){
        input_type_.left_ =1;
        input_type_.right_ =0;
        status_=WALK_LEFT;
    }
    if((events.type ==SDL_KEYDOWN && events.key.keysym.sym==SDLK_d)||(state[SDL_SCANCODE_D])){
        input_type_.left_=0;
        input_type_.right_=1;
        status_=WALK_RIGHT;
    }
    if (events.type == SDL_KEYUP) {
        if(events.key.keysym.sym == SDLK_a){
            input_type_.left_=0;
            input_type_.right_=0;
            status_=IDLE_LEFT;
        }
        if(events.key.keysym.sym == SDLK_d) {
            input_type_.left_=0;
            input_type_.right_=0;
            status_=IDLE_RIGHT;
        }
    }
    if((input_type_.left_ ||input_type_.right_)){
        if ( (events.type ==SDL_KEYDOWN && events.key.keysym.sym==SDLK_LSHIFT) || (state[SDL_SCANCODE_LSHIFT]) ){
            input_type_.run_=1;
            if(input_type_.left_==1) status_= RUN_LEFT;
            if(input_type_.right_==1) status_= RUN_RIGHT;
        }
        if(events.type == SDL_KEYUP){
            if(events.key.keysym.sym == SDLK_LSHIFT){
                input_type_.run_=0;
                if(input_type_.left_==1) status_= WALK_LEFT;
                if(input_type_.right_==1) status_= WALK_RIGHT;

            }
        }
    }
    else input_type_.run_=0;


    if(events.type == SDL_MOUSEBUTTONDOWN ){
        if(on_ground_==2 && events.button.button == SDL_BUTTON_LEFT){
            Clear();
            input_type_.shot_=1;
            frame_=0;
            if(SoundEnable) int ret = Mix_PlayChannel(-1, g_sound_bullet , 0);
            if (status_ == WALK_RIGHT || status_ == RUN_RIGHT || status_ == IDLE_RIGHT) {
                status_=SHOT_RIGHT;
            }
            if (status_ == WALK_LEFT || status_ == RUN_LEFT || status_ ==IDLE_LEFT) {
                status_=SHOT_LEFT;
            }
        }
    }
    if(events.type == SDL_MOUSEBUTTONDOWN ){
        if(on_ground_==2 && events.button.button == SDL_BUTTON_RIGHT){
            Clear();
            input_type_.grenade_=1;
            frame_=0;
            if (status_ == WALK_RIGHT || status_ == RUN_RIGHT || status_ == IDLE_RIGHT) {
                status_=GRENADE_RIGHT;
            }
            if (status_ == WALK_LEFT || status_ == RUN_LEFT || status_ ==IDLE_LEFT) {
                status_=GRENADE_LEFT;
            }
        }
    }


}
void MainObject::DoPlayer(Map &map_data)
{
    x_val_=0;
    y_val_+=GRAVITY;

    if(y_val_>=MAX_FALL_SPEED) y_val_=MAX_FALL_SPEED;

    if(input_type_.left_==1) x_val_=(input_type_.run_==1) ? -PLAYER_SPEED*1.5:-PLAYER_SPEED ;

    if(input_type_.right_==1) x_val_=(input_type_.run_==1) ? +PLAYER_SPEED*1.5:+PLAYER_SPEED ;

    if(input_type_.jump_==1) {
        if(on_ground_!=0){
            if(SoundEnable) int ret = Mix_PlayChannel(-1, g_sound_jump , 0);
            y_val_=-JUMP_VAL;
            on_ground_--;
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
    if(map_data.start_x_+SCREEN_WIDTH>map_data.max_x_-64) map_data.start_x_=map_data.max_x_-SCREEN_WIDTH-64;
}

void MainObject::CheckToMap(Map &map_data)
{
    //earn coin
    int x1 = (x_pos_  + x_val_ -1) / TILE_SIZE;
    int x2 = (x_pos_  + width_frame_ + x_val_ +1) / TILE_SIZE;
    int y1 = (y_pos_ + y_val_ -1)/TILE_SIZE;
    int y2 = (y_pos_ + height_frame_ + y_val_ +1) /TILE_SIZE;
    int y3 = (y1 + y2)/2;
    int x3 = (x1 + x2)/2;

    for (int y : {y1, y2, y3})
        for (int x : {x1, x2, x3})
        {
            earn_coin(map_data.tile[y][x]);
            CompleteLevel(map_data.tile[y][x]);
        }
    //thêm cơ chế đổi đạn vào đây

    //check horizontal
    int height_min =min(height_frame_,TILE_SIZE);
    x1 = (x_pos_ + x_val_) / TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

    y1=(y_pos_)/TILE_SIZE;
    y2=(y_pos_+height_min-1)/TILE_SIZE;

    if(x1>=0 && x2<MAX_MAP_X && y1>=0 && y2<MAX_MAP_Y){
        if(x_val_>0){// right
            if(map_data.tile[y1][x2]!=BLANK_TILE || map_data.tile[y2][x2]!=BLANK_TILE){
                x_pos_ = x2 * TILE_SIZE;
                x_pos_-=width_frame_+1;
                x_val_ = 0;
            }
        }
        if(x_val_<0){
            if(map_data.tile[y1][x1]!=BLANK_TILE || map_data.tile[y2][x1]!=BLANK_TILE){
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

    if(x1>=0 && x2<MAX_MAP_X && y1>=0 && y2<MAX_MAP_Y){

        if(y_val_>0){// down
            if(map_data.tile[y2][x1]!=BLANK_TILE || map_data.tile[y2][x2]!=BLANK_TILE){
                y_pos_ = y2 * TILE_SIZE;
                y_pos_ -= height_frame_;
                y_val_ = 0;
                on_ground_=2;
            }
        }
        if(y_val_<0){
            if(map_data.tile[y1][x1]!=BLANK_TILE || map_data.tile[y1][x2]!=BLANK_TILE){
                y_pos_ = (y1 + 1) * TILE_SIZE;
                y_val_ = 0;
            }
        }
    }
    x_pos_+=x_val_;
    y_pos_+=y_val_;

    if(x_pos_<0) x_pos_=0;
    if(x_pos_+width_frame_>map_data.max_x_) x_pos_=map_data.max_x_-width_frame_-1;

    if(y_pos_>SCREEN_HEIGHT+100) current_health_=0;
}

void MainObject::earn_coin(int &val)
{
    if(val==COIN_TILE){
        if(SoundEnable) int ret = Mix_PlayChannel(-1, g_sound_coin , 0);
        count_coins_++;
        val=0;
    }
}

void MainObject::RenderHealthBar(SDL_Renderer* renderer) {
    int bar_width = 50;  // Độ dài thanh máu
    int bar_height = 5;  // Độ cao thanh máu
    int x = rect_.x  ;  // Vị trí ngang theo tọa độ của Threat
    int y = rect_.y - 10;  // Đặt thanh máu ngay trên đầu Threat

    // Viền đen của thanh máu
    SDL_Rect border = {x, y, bar_width, bar_height};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Màu đen
    SDL_RenderFillRect(renderer, &border);

    // Thanh máu (màu đỏ)
    float health_ratio = (float)current_health_ / PLAYER_MAX_HEALTH;
    SDL_Rect health = {x, y, (int)(bar_width * health_ratio), bar_height};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Màu đỏ
    SDL_RenderFillRect(renderer, &health);
}

void MainObject::CompleteLevel(int &val)
{
    if(val==11){
        complete=true;
    }
}

void MainObject::Clear()
{
    input_type_.left_=0;
    input_type_.right_=0;
    input_type_.jump_=0;
    input_type_.run_=0;
    input_type_.shot_=0;
    input_type_.grenade_=0;
}





