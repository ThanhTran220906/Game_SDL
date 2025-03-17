
#pragma once

#include "CommonFunc.h"
#include "TextObject.h"
#include "MainObject.h"


//game



struct titleScreen {
	int pos = 0;
	int n = 3;
	TextObject textBoxes[3];

	titleScreen();
	~titleScreen();

	void update();
	void render(SDL_Renderer *des);
	void handleEvents(SDL_Event &event);


};

struct pauseMenu {
	int pos = 0;
	int n = 4;
	bool loading = false;

	TextObject textBoxes[4];

	pauseMenu();
	~pauseMenu();

	void update();
	void render(SDL_Renderer *des);
	void handleEvents(SDL_Event &event);
	bool GetBoolLoading() { return loading; };
    void SetBoolLoading(bool v) { loading = v; };

};


struct GameOverMenu {
	int pos = 0;
	int n = 3;
	TextObject textBoxes[3];
    bool loading = false;

	GameOverMenu();
	~GameOverMenu();

	void update();
	void render(SDL_Renderer *des);
	void handleEvents(SDL_Event &event, MainObject &player);
    bool GetBoolLoading() { return loading; };
    void SetBoolLoading(bool v) { loading = v; };

};
