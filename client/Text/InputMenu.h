#include <SDL.h>
#include <SDL_ttf.h>
#include "Client.h"
#include "InputBlock.h"
#include "TextBlock.h"
#include "Button.h"

#ifndef INPUTMENU_H
#define INPUTMENU_H

class InputMenu {
private:
	bool isVisible;
	TTF_Font *font;
	Client *client;
	TextBlock *infoText, *ipText, *portText, *nickText;
	InputBlock *ipInput, *portInput, *nickInput;
	Button *acceptButton;
	void Draw(SDL_Renderer *render);
	void UpdateEvents(SDL_Event *event);
public:
	InputMenu(Client *client, TTF_Font *_font, SDL_Renderer *render);
	void Update(SDL_Renderer *render, SDL_Event *event);
	bool GetIsVisible();
	void SetVisible(bool fl);
	~InputMenu();
};

#endif