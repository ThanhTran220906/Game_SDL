

#pragma once

#include "CommonFunc.h"

class TextObject
{
public:
    TextObject();
    ~TextObject();

    enum TextColor
    {
        RED_TEXT  = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,
        YELLOW_TEXT = 3,
        GREEN_TEXT =4,
    };

    bool LoadFromFile(string path);
    bool loadFromRenderedText( SDL_Renderer* screen);
    void Free();

    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void setColor(int type);

    void RenderText(SDL_Renderer* screen);

    void SetXY(int x,int y){ x_=x;y_=y; };

    int getWidth() const {return width_;}
    int getHeight() const {return height_;}
    void SetText(const string& text) {str_val_ = text;}
    string GetText() const {return str_val_;}

    void SetSize(int x) {size_=x;};
    void SetFont(string s) { font_=s; };

private:
    TTF_Font* gFont;
    string str_val_;
    SDL_Color text_color_;
    SDL_Texture* texture_;
    int width_;
    int height_;
    int x_;
    int y_;
    int size_;
    string font_;
};


