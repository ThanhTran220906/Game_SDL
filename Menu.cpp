#include "Menu.h"




titleScreen::titleScreen() {
    pos = 0; // Khởi tạo vị trí chọn

    for (int i = 0; i < n; i++) {
        int x = 800 / 2;
        int y = (640 - ((n - 1) * 100)) / 2 + i * 100;
        textBoxes[i].SetXY(x, y);
    }

    textBoxes[0].SetText("Play");
    textBoxes[1].SetText("Sound: On");
    textBoxes[2].SetText("Quit");
}

titleScreen::~titleScreen() {}

void titleScreen::handleEvents(SDL_Event &event,int x,int y) { // Truyền event từ vòng lặp chính
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
                    if (pos == 0) { Title = false; LevelChoose = true; }
                    if (pos == 1) SoundEnable=!SoundEnable;
                    if (pos == 2) { Title = false; is_quit = true; }
                    break;
                default:
                    break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT) {
                if (pos == 0) { Title = false; LevelChoose = true; }
                if (pos == 1) SoundEnable=!SoundEnable;
                if (pos == 2) { Title = false; is_quit = true; }
            }
            break;
    }
}

void titleScreen::update() {
    if(pos!=last_pos){
        if(SoundEnable) int ret = Mix_PlayChannel(-1, g_sound_menu , 0);
        last_pos=pos;
    }
    for (int i = 0; i < n; i++) {
        textBoxes[i].setColor(TextObject::WHITE_TEXT);
        textBoxes[i].SetSize(24);
        int x = 800 / 2;
        int y = (640 - ((n - 1) * 100)) / 2 + i * 100;
        textBoxes[i].SetXY(x, y);

    }

    textBoxes[pos].setColor(TextObject::YELLOW_TEXT);
    textBoxes[pos].SetSize(42);
    textBoxes[pos].SetXY(400, (640 - ((n - 1) * 100)) / 2 + pos * 100 - 20);
}

void titleScreen::render(SDL_Renderer* des) {
    SDL_SetRenderDrawColor(des, 20, 20, 20, 255);
    SDL_RenderClear(des);
    if(SoundEnable)     textBoxes[1].SetText("Sound: On");
    else     textBoxes[1].SetText("Sound: Off");
    for (int i = 0; i < n; i++) {
        textBoxes[i].loadFromRenderedText(des);
        textBoxes[i].RenderText(des);
    }
    SDL_RenderPresent(des);
}

// ----------------- PAUSE MENU --------------------

pauseMenu::pauseMenu() {
    pos = 0; // Khởi tạo vị trí chọn

    for (int i = 0; i < n; i++) {
        int x = 800 / 2;
        int y = (640 - ((n - 1) * 100)) / 2 + i * 100;
        textBoxes[i].SetXY(x, y);
    }

    textBoxes[0].SetText("Continue");
    textBoxes[1].SetText("Sound: On");
    textBoxes[2].SetText("Title Screen");
    textBoxes[3].SetText("Quit");
}

pauseMenu::~pauseMenu() {}

void pauseMenu::handleEvents(SDL_Event& event,int x,int y) {
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
                    pos = (pos - 1 + n) % n;
                    break;
                case SDLK_DOWN:
                    pos = (pos + 1) % n;
                    break;
                case SDLK_RETURN:
                    if (pos == 0) { Pause = false; gameRunning = true; }
                    if (pos == 1) SoundEnable=!SoundEnable;
                    if (pos == 2) { Pause=false ; Title = true; loading = true; }
                    if (pos == 3) { Pause = false; is_quit = true; }
                    break;
                default:
                    break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT) {
                if (pos == 0) { Pause = false; gameRunning = true; }
                if (pos == 1) SoundEnable=!SoundEnable;
                if (pos == 2) { Pause=false ; Title = true; loading = true; }
                if (pos == 3) { Pause = false; is_quit = true; }
            }
            break;
    }
}

void pauseMenu::update() {
    if(pos!=last_pos){
        if(SoundEnable) int ret = Mix_PlayChannel(-1, g_sound_menu , 0);
        last_pos=pos;
    }
    for (int i = 0; i < n; i++) {
        textBoxes[i].setColor(TextObject::WHITE_TEXT);
        textBoxes[i].SetSize(24);
        int x = 800 / 2;
        int y = (640 - ((n - 1) * 100)) / 2 + i * 100;
        textBoxes[i].SetXY(x, y);

    }

    textBoxes[pos].setColor(TextObject::YELLOW_TEXT);
    textBoxes[pos].SetSize(42);
    textBoxes[pos].SetXY(400, (640 - ((n - 1) * 100)) / 2 + pos * 100 - 20);
}


void pauseMenu::render(SDL_Renderer* des) {
    SDL_SetRenderDrawColor(des, 20, 20, 20, 255);
    SDL_RenderClear(des);

    for (int i = 0; i < n; i++) {
        textBoxes[i].loadFromRenderedText(des);
        textBoxes[i].RenderText(des);
    }
    if(SoundEnable)     textBoxes[1].SetText("Sound: On");
    else     textBoxes[1].SetText("Sound: Off");
    SDL_RenderPresent(des);
}

//------------------GameOverMenu-----------------------

GameOverMenu::GameOverMenu() {
    pos = 0; // Khởi tạo vị trí chọn

    for (int i = 0; i < n; i++) {
        int x = 800 / 2;
        int y = (640 - ((n - 1) * 100)) / 2 + i * 100;
        textBoxes[i].SetXY(x, y);
    }

    textBoxes[0].SetText("Play Again");
    textBoxes[1].SetText("Title Screen");
    textBoxes[2].SetText("Quit");
}

GameOverMenu::~GameOverMenu() {}

void GameOverMenu::handleEvents(SDL_Event& event, MainObject &player,int x,int y) { // Truyền event từ vòng lặp chính
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
                    if (pos == 0) { GameOver = false; gameRunning = true; player.SetHealth(PLAYER_MAX_HEALTH); player.SetPos(SCREEN_WIDTH/2,0); player.Set_Coin(0); loading = true; }
                    if (pos == 1) { GameOver = false; Title =true; }
                    if (pos == 2) { GameOver = false; is_quit = true; }
                    break;
                default:
                    break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT) {
                if (pos == 0) { GameOver = false; gameRunning = true; player.SetHealth(PLAYER_MAX_HEALTH); player.SetPos(SCREEN_WIDTH/2,0); player.Set_Coin(0); loading = true; }
                if (pos == 1) { GameOver = false; Title =true; }
                if (pos == 2) { GameOver = false; is_quit = true; }
            }
            break;
    }
}

void GameOverMenu::update() {
    if(pos!=last_pos){
        int ret = Mix_PlayChannel(-1, g_sound_menu , 0);
        last_pos=pos;
    }
    for (int i = 0; i < n; i++) {
        textBoxes[i].setColor(TextObject::WHITE_TEXT);
        textBoxes[i].SetSize(24);
        int x = 800 / 2;
        int y = (640 - ((n - 1) * 100)) / 2 + i * 100;
        textBoxes[i].SetXY(x, y);

    }

    textBoxes[pos].setColor(TextObject::YELLOW_TEXT);
    textBoxes[pos].SetSize(42);
    textBoxes[pos].SetXY(400, (640 - ((n - 1) * 100)) / 2 + pos * 100 - 20);
}

void GameOverMenu::render(SDL_Renderer* des) {
    SDL_SetRenderDrawColor(des, 20, 20, 20, 255);
    SDL_RenderClear(des);

    for (int i = 0; i < n; i++) {
        textBoxes[i].loadFromRenderedText(des);
        textBoxes[i].RenderText(des);
    }
    SDL_RenderPresent(des);
}
