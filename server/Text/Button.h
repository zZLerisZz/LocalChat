#include <SDL.h>
#include <SDL_ttf.h>
#include <cstring>
#include <iostream>

#ifndef BUTTON_H
#define BUTTON_H

class Button {
private:
	std::string text;
	TTF_Font *font;
	SDL_Rect sprite, border;
	int thickOfBorder;
	bool isVisible;
	SDL_Color textColor;
public:
	Button(const char* _text, TTF_Font *_font, int _x, int _y, int _thickOfBorder, SDL_Color _textColor, bool _isVisible);
	bool GetIsVisible();
	void Draw(SDL_Renderer *render);
	void TextUpdate(const char *upd);
	bool CheckMouse();
	void SetVisible();
	~Button();
};

#endif