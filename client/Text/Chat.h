#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <cstring>
#include <list>
#include <ctime>
#include "Client.h"
#include "Button.h"
#include "TextBlock.h"
#include "InputMessage.h"
#include "ChatLine.h"

#ifndef CHAT_H
#define CHAT_H

#define CHAT_FONT_HEIGHT 23
#define INFO_FONT_HEIGHT 45
#define CHATBAR_HEIGHT 667
#define CHATBAR_WIDTH 1024
#define LINE_COUNT 29

class Chat {
private:
	TTF_Font *chatFont, *infoFont;
	Client *client;
	SDL_Rect chatRect, interfaceRect, messageRect;
	TextBlock *ipText, *portText, *nickText;
	Button *sendButton;
	InputMessage *message;
	bool isVisible;
	static bool goodQuit;
	static std::list<ChatLine> lines;
	static std::list<std::pair<std::string, int>> messagesToCut;
	int move;
	void UpdateEvents(SDL_Event *event);
	void Draw(SDL_Renderer *render);
	void DrawLines(SDL_Renderer *render);
	void MessageToLines(SDL_Renderer *render);
public:
	Chat(Client *_client, TTF_Font *_chatFont, TTF_Font *_infoFont);
	void StartChat();
	void Update(SDL_Renderer *render, SDL_Event *event);
	void SetVisible();
	void SetInVisible();
	bool GetIsVisible();
	bool GetGoodQuit();
	~Chat();
};

#endif