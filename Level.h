
#pragma once

#include "CommonFunc.h"
#include "TextObject.h"
#include "MainObject.h"

#define TOTAL_LEVELS 6


class Level {
private:
	int pos ;
	int last_pos ;
	int n = 6;
	TextObject textBoxes[6];
	string file_map;
	bool loading;
	int CompleteLevel[TOTAL_LEVELS];
public:
	Level();
	~Level();

	void update();
	void render(SDL_Renderer *des);
	void handleEvents(SDL_Event &event, MainObject &player,int x,int y);
    bool GetBoolLoading() { return loading; };
    void SetBoolLoading(bool v) { loading = v; };
    string GetFilemap() { return file_map; };
    void SaveLevel();
};
