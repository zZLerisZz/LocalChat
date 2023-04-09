#include <SDL.h>
#include <SDL_ttf.h>
#include <cstring>
#include <iostream>

#ifndef CLIENTLINE_H
#define CLIENTLINE_H

class ChatLine {
private:
	std::string text;
	TTF_Font *font;
	SDL_Rect sprite;
	SDL_Color textColor;
public:
	ChatLine(std::string _text, TTF_Font *_font, SDL_Color _textColor);
	void Draw(SDL_Renderer *render);
	void SetY(int _y);
	void SetX(int _x);
	~ChatLine();
};

#endif