#include <SDL.h>
#include <SDL_ttf.h>
#include <cstring>
#include <iostream>

#ifndef INPUTBLOCK_H
#define INPUTBLOCK_H

class InputBlock {
private:
	std::string text;
	TTF_Font *font;
	SDL_Rect sprite, textRect;
	bool isVisible;
	SDL_Color textColor;
	int maxLen;
	bool isFocused;
public:
	InputBlock(TTF_Font *_font, int _x, int _y, SDL_Color _textColor, bool _isVisible, SDL_Renderer *render);
	bool GetIsVisible();
	void Draw(SDL_Renderer *render);
	void SetVisible();
	void DeleteLast();
	void AddChar(char *ch);
	int GetMaxLen();
	void SetFocus();
	void SetDeFocus();
	bool CheckMouse();
	std::string GetText();
	~InputBlock();
};

#endif