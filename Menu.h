
#pragma once

#include "CommonFunc.h"
#include "TextObject.h"
#include "MainObject.h"


//game



class titleScreen {
private:

	int pos ;
	int last_pos;
	int n ;
	TextObject textBoxes[3];
	BaseObject background;
public:
	titleScreen();
	~titleScreen();

	void update();
	void render(SDL_Renderer *des);
	void handleEvents(SDL_Event &event,int x,int y);


};

class pauseMenu {
private:
	int pos ;
	int last_pos ;
	int n ;
	bool loading ;

	TextObject textBoxes[4];
public:
	pauseMenu();
	~pauseMenu();

	void update();
	void render(SDL_Renderer *des);
	void handleEvents(SDL_Event &event,int x,int y);
	bool GetBoolLoading() { return loading; };
    void SetBoolLoading(bool v) { loading = v; };

};


class GameOverMenu {
private:

	int pos ;
	int last_pos ;
	int n ;
	TextObject textBoxes[3];
    bool loading;
    BaseObject background;
public:
	GameOverMenu();
	~GameOverMenu();

	void update();
	void render(SDL_Renderer *des);
	void handleEvents(SDL_Event &event, MainObject &player,int x,int y);
    bool GetBoolLoading() { return loading; };
    void SetBoolLoading(bool v) { loading = v; };

};
