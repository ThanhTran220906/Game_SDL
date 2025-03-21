
#include "Level.h"

Level::Level() {
    pos = 0;
    last_pos = 0;
    file_map="";
    loading = false;

    for (int i = 0; i < n; i++) {
        int x = 800 / 2;
        int y = (640 - ((n - 1) * 100)) / 2 + i * 100;
        textBoxes[i].SetXY(x, y);
    }

    textBoxes[0].SetText("Level 1");
    textBoxes[1].SetText("Level 2");
    textBoxes[2].SetText("Level 3");
    textBoxes[3].SetText("Level 4");
    textBoxes[4].SetText("Level 5");
    textBoxes[5].SetText("Level 6");
}

Level::~Level()
{

}

void Level::handleEvents(SDL_Event &event,MainObject &player,int x,int y) { // Truyền event từ vòng lặp chính
    for (int i = 0; i < n; i++) {
        if(x>400 && x<600 && y>(640 - ((n - 1) * 100)) / 2 + i * 100 && y< (640 - ((n - 1) * 100)) / 2 + i * 100+70) pos=i;
    }

    switch (event.type) {
        case SDL_QUIT:
            is_quit = true;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    pos = (pos - 1 + n) % n; // Di chuyển lên
                    break;
                case SDLK_DOWN:
                    pos = (pos + 1) % n; // Di chuyển xuống
                    break;
                case SDLK_RETURN:
                    if (pos == 0) file_map="map//map01.dat";
                    if (pos == 1) file_map="map//map02.dat";
                    if (pos == 2) file_map="map//map03.dat";
                    if (pos == 3) file_map="map//map04.dat";
                    if (pos == 4) file_map="map//map05.dat";
                    if (pos == 5) file_map="map//map06.dat";
                    loading = true;
                    LevelChoose = false;
                    gameRunning = true;
                    player.SetHealth(PLAYER_MAX_HEALTH);
                    player.SetPos(SCREEN_WIDTH/2,0);
                    player.Set_Coin(0);
                    break;
                default:
                    break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT) {
                if (pos == 0) file_map="map//map01.dat";
                    if (pos == 1) file_map="map//map02.dat";
                    if (pos == 2) file_map="map//map03.dat";
                    if (pos == 3) file_map="map//map04.dat";
                    if (pos == 4) file_map="map//map05.dat";
                    if (pos == 5) file_map="map//map06.dat";
                    loading = true;
                    LevelChoose = false;
                    gameRunning = true;
                    player.SetHealth(PLAYER_MAX_HEALTH);
                    player.SetPos(SCREEN_WIDTH/2,0);
                    player.Set_Coin(0);
                }
            break;
    }
}

void Level::update() {
        if(pos!=last_pos){
        if(SoundEnable) int ret = Mix_PlayChannel(-1, g_sound_menu , 0);
        last_pos=pos;
    }
    std::ifstream file("map//save.txt");
    if (!file) {
        return;
    }
    for(int i=0;i<TOTAL_LEVELS;i++){
        file>>CompleteLevel[i];
    }
    file.close();
    for (int i = 0; i < n; i++) {
        if(CompleteLevel[i]==0) textBoxes[i].setColor(TextObject::RED_TEXT);
        if(CompleteLevel[i]==1) textBoxes[i].setColor(TextObject::GREEN_TEXT);
        textBoxes[i].SetSize(24);
        textBoxes[i].SetXY(800 / 2, (640 - ((n - 1) * 100)) / 2 + i * 100);

    }

    textBoxes[pos].setColor(TextObject::YELLOW_TEXT);
    textBoxes[pos].SetSize(42);
    textBoxes[pos].SetXY(400, (640 - ((n - 1) * 100)) / 2 + pos * 100 - 20);
}

void Level::render(SDL_Renderer* des) {
    SDL_SetRenderDrawColor(des, 20, 20, 20, 255);
    SDL_RenderClear(des);

    for (int i = 0; i < n; i++) {
        textBoxes[i].loadFromRenderedText(des);
        textBoxes[i].RenderText(des);
    }
    SDL_RenderPresent(des);
}

void Level::SaveLevel()
{
    CompleteLevel[pos] =1;
    std::ofstream file("map//save.txt");
    if (!file) {
        return;
    }
    for(int i=0;i<TOTAL_LEVELS;i++){
        file<<CompleteLevel[i]<<" ";
    }
    file.close();
}
