#include "Game.h"


bool InitData() {
    bool success = true;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("RUN TO SURVICE",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (g_window == NULL) {
        success = false;
    }
    else {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL) {
            success = false;
        }
        else {
            SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
            if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) success = false;
        }
    }
    if (TTF_Init() == -1) {
        return false;
    }
    // Mở audio với tần số 22050 Hz, định dạng mặc định, 2 kênh (stereo), buffer 4096
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        return false;
    }
    g_sound_bullet = Mix_LoadWAV("mixer//shot.wav");
    g_sound_explode = Mix_LoadWAV("mixer//explode.wav");
    g_sound_jump = Mix_LoadWAV("mixer//jump.wav");
    g_sound_menu = Mix_LoadWAV("mixer//menu.wav");
    g_sound_coin = Mix_LoadWAV("mixer//coin.wav");
    g_sound_hurt = Mix_LoadWAV("mixer//hurt.wav");

    return success;
}

void close() {

    SDL_DestroyRenderer(g_screen);
    SDL_DestroyWindow(g_window);

    Mix_FreeChunk(g_sound_bullet);
    Mix_FreeChunk(g_sound_explode);
    Mix_FreeChunk(g_sound_jump);
    Mix_FreeChunk(g_sound_coin);
    Mix_FreeChunk(g_sound_menu);
    Mix_FreeChunk(g_sound_hurt);

    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

}

int main(int argc, char* argv[]) {
    //init
    if (!InitData()) return -1;

    Game game;
    game.Init();

    //game loop
    while (!is_quit) {
        while (SDL_PollEvent(&g_event)) {
            //quit
            if (g_event.type == SDL_QUIT) {
                is_quit = true;
            }
            //xu li game
            game.HandleSystem();
        }
        game.RenderSystem();
        if (gameRunning){
            game.Logic();
        }
    }
    game.~Game();
    close();
}
