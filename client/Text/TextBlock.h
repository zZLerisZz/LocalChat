#include <SDL.h>
#include <SDL_ttf.h>
#include <cstring>
#include <iostream>

#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

class TextBlock {
private:
	std::string text;
	TTF_Font *font;
	SDL_Rect sprite;
	SDL_Color textColor;
public:
	TextBlock(std::string _text, TTF_Font *_font, int _x, int _y, SDL_Color _textColor);
	TextBlock(const char *_text, TTF_Font *_font, int _x, int _y, SDL_Color _textColor);
	void Draw(SDL_Renderer *render);
	int GetLen(SDL_Renderer *render);
	int GetHeight(SDL_Renderer *render);
	void UpdateText(const char *upd);
	~TextBlock();
};

#endif