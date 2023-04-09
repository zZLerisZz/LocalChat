#include <SDL.h>
#include <SDL_ttf.h>
#include <cstring>
#include <iostream>

#ifndef INPUTMESSAGE_H
#define INPUTMESSAGE_H

class InputMessage {
private:
	std::string text;
	TTF_Font *font;
	SDL_Rect sprite, textRect;
	SDL_Color textColor;
	bool isFocused;
	std::string GetShorterText(SDL_Renderer *render);
public:
	InputMessage(TTF_Font *_font, int _x, int _y, int _w, int _h, SDL_Color _textColor);
	void Draw(SDL_Renderer *render);
	void DeleteLast();
	void AddChar(char *ch);
	void SetFocus();
	void SetDeFocus();
	bool CheckMouse();
	std::string GetText();
	~InputMessage();
};

#endif