#include <SDL.h>
#include <SDL_ttf.h>
#include "Client.h"
#include "InputMenu.h"
#include "Chat.h"

#ifndef APPLICATION_H
#define APPLICATION_H

#define WIDTH 1280
#define HEIGHT 720

class Application {
private:
	Client *client;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	void Init();
	void DeInit();
	TTF_Font *bigfont, *smallfont;
	bool isWorking;
	InputMenu *inputMenu;
	Chat *chat;
public:
	Application();
	~Application();
	void Update();
};

#endif