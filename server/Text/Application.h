#include <SDL.h>
#include <SDL_ttf.h>
#include "Server.h"
#include "TextBlock.h"
#include "Button.h"
#include "InputBlock.h"

#ifndef APPLICATION_H
#define APPLICATION_H

#define WIDTH 640
#define HEIGHT 480

class Application {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	void Init();
	void DeInit();
	TTF_Font *font;
	static Server* ser;
	TextBlock *info1, *info2, *info3, *info4, *info5, *inputText;
	Button *switchButton, *inputButton;
	InputBlock *ipInput;
	bool isWorking;
	void UpdateEvents();
	static void UpdateServer();
public:
	Application();
	~Application();
	void Update();
};

#endif