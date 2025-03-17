#include "Menu.h"

titleScreen::titleScreen() {
    pos = 0; // Khởi tạo vị trí chọn

    for (int i = 0; i < n; i++) {
        int x = 800 / 2;
        int y = (640 - ((n - 1) * 100)) / 2 + i * 100;
        textBoxes[i].SetXY(x, y);
    }

    textBoxes[0].SetText("Play");
    textBoxes[1].SetText("Options");
    textBoxes[2].SetText("Quit");
}

titleScreen::~titleScreen() {}

void titleScreen::handleEvents(SDL_Event &event) { // Truyền event từ vòng lặp chính
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
                    if (pos == 1) std::cout << "Options" << std::endl;
                    if (pos == 2) { Title = false; is_quit = true; }
                    break;
                default:
                    break;
            }
            break;
    }
}

void titleScreen::update() {
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
    textBoxes[1].SetText("Options");
    textBoxes[2].SetText("Title Screen");
    textBoxes[3].SetText("Quit");
}

pauseMenu::~pauseMenu() {}

void pauseMenu::handleEvents(SDL_Event& event) {
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
                    if (pos == 1) std::cout << "Options" << std::endl;
                    if (pos == 2) { Pause=false ; Title = true; loading = true; }
                    if (pos == 3) { Pause = false; is_quit = true; }
                    break;
                default:
                    break;
            }
            break;
    }
}

void pauseMenu::update() {
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

void GameOverMenu::handleEvents(SDL_Event& event, MainObject &player) { // Truyền event từ vòng lặp chính
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
                    if (pos == 0) { GameOver = false; gameRunning = true; player.SetHealth(PLAYER_MAX_HEALTH); player.SetPos(SCREEN_WIDTH/2,0); loading = true; }
                    if (pos == 1) { GameOver = false; Title =true; }
                    if (pos == 2) { GameOver = false; is_quit = true; }
                    break;
                default:
                    break;
            }
            break;
    }
}

void GameOverMenu::update() {
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
