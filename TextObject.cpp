#include "TextObject.h"
#include <SDL_ttf.h>

TextObject::TextObject()
{
    text_color_ = {255, 255, 255}; // Màu mặc định là trắng
    x_ = 0;
    y_ = 0;
    size_ = 24;
    font_ = "font//OpenSans-Regular.ttf";
    texture_ = NULL; // Khởi tạo con trỏ tránh lỗi
}

TextObject::~TextObject()
{
    Free();
}

bool TextObject::loadFromRenderedText(SDL_Renderer* screen)
{
    Free(); // Giải phóng texture cũ trước khi tạo mới

    TTF_Font* gFont = TTF_OpenFont(font_.c_str(), size_);
    if (gFont == NULL) {
        printf("Khong the tai font! Loi: %s\n", TTF_GetError());
        return false;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, str_val_.c_str(), text_color_);
    if (textSurface == NULL) {
        printf("Khong the tao surfacce tu text! Loi: %s\n", TTF_GetError());
        TTF_CloseFont(gFont);
        return false;
    }

    texture_ = SDL_CreateTextureFromSurface(screen, textSurface);
    if (texture_ == NULL) {
        printf("Khong the tao texture tu surface! Loi: %s\n", SDL_GetError());
    } else {
        width_ = textSurface->w;
        height_ = textSurface->h;
    }

    SDL_FreeSurface(textSurface);
    TTF_CloseFont(gFont); // Giải phóng font sau khi tạo xong

    return texture_ != NULL;
}

void TextObject::Free()
{
    if (texture_ != NULL) {
        SDL_DestroyTexture(texture_);
        texture_ = NULL;
    }
}

void TextObject::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    text_color_ = {red, green, blue};
}

void TextObject::setColor(int type)
{
    switch (type) {
        case RED_TEXT:
            text_color_ = {255, 0, 0}; break;
        case WHITE_TEXT:
            text_color_ = {255, 255, 255}; break;
        case BLACK_TEXT:
            text_color_ = {0, 0, 0}; break;
        case YELLOW_TEXT:
            text_color_ = {255, 255, 0}; break;
        case GREEN_TEXT:
            text_color_ = {0, 255, 0}; break;
        default:
            text_color_ = {255, 255, 255}; // Mặc định là trắng
    }
}

void TextObject::RenderText(SDL_Renderer* screen)
{
    if (texture_ == NULL) {
        printf("Loi: Texture khong ton tai! Van ban: %s\n", str_val_.c_str());
        return;
    }

    SDL_Rect renderQuad = {x_, y_, width_, height_};
    if (SDL_RenderCopy(screen, texture_, NULL, &renderQuad) != 0) {
        printf("Loi: Khong the ve texture! SDL_Error: %s\n", SDL_GetError());
    }
}
