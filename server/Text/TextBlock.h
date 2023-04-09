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
	bool isVisible;
	SDL_Color textColor;
public:
	TextBlock(const char *_text, TTF_Font *_font, int _x, int _y, SDL_Color _textColor, bool _isVisible);
	void Draw(SDL_Renderer *render);
	void Draw(SDL_Renderer *render, std::string add);
	void Draw(SDL_Renderer *render, unsigned short add);
	void Draw(SDL_Renderer *render, unsigned int add);
	void Draw(SDL_Renderer *render, const char *add);
	int GetLen(SDL_Renderer *render);
	int GetHeight(SDL_Renderer *render);
	bool GetIsVisible();
	void SetVisible();
	~TextBlock();
};

#endif