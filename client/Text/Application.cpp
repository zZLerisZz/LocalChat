#include "Application.h"

using namespace std;

Application::Application() {
	isWorking = true;
	Init();
	client = new Client();
	inputMenu = new InputMenu(client, bigfont, renderer);
	chat = new Chat(client, smallfont, bigfont);
}

Application::~Application() {
	DeInit();
	delete client;
	delete chat;
}

void Application::Init(){
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error with SDL init\n" << SDL_GetError() << endl;
		return;
	}
	window = SDL_CreateWindow("Client Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if(!window) {
		cout << "Error window creation\n" << SDL_GetError() << endl;
		return;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(!renderer) {
		cout << "Error with render" << SDL_GetError() << endl;
		return;
	}
	if(TTF_Init() < 0) {
		cout << "Error with SDL_ttf init\n" << TTF_GetError() << endl;
		return;
	}
	bigfont = TTF_OpenFont("../Src/timesnewromanpsmt.ttf", 40);
	smallfont = TTF_OpenFont("../Src/timesnewromanpsmt.ttf", 20);
}

void Application::DeInit() {
	if(renderer)
		SDL_DestroyRenderer(renderer);
	if(window)
		SDL_DestroyWindow(window);
	if(bigfont)
		TTF_CloseFont(bigfont);
	if(smallfont)
		TTF_CloseFont(smallfont);
	SDL_Quit();
	TTF_Quit();
}

void Application::Update() {
	while(inputMenu->GetIsVisible() || chat->GetIsVisible()) {
		do {
			inputMenu->SetVisible(chat->GetIsVisible());
			chat->SetVisible();
			while(inputMenu->GetIsVisible())
				inputMenu->Update(renderer, &event);
			chat->StartChat();
			while(chat->GetIsVisible())
				chat->Update(renderer, &event);
		} while(chat->GetGoodQuit());
	}
}