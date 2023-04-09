#include "Application.h"
#include <iostream>

using namespace std;

void Application::UpdateServer() {
	ser->CheckConnections();
}

Server* Application::ser = new Server();

Application::Application() {
	isWorking = true;
	Init();
	info1 = new TextBlock("IP: ", font, 0, 0, {75, 0, 130}, false);
	info2 = new TextBlock("Порт: ", font, 0, info1->GetHeight(renderer), {75, 0, 130}, false);
	info3 = new TextBlock("Количество пользователей: ", font, 0, info1->GetHeight(renderer) + info2->GetHeight(renderer), {75, 0, 130}, false);
	info4 = new TextBlock("Файл с логами: ", font, 0, info1->GetHeight(renderer) + info2->GetHeight(renderer) + info3->GetHeight(renderer), {75, 0, 130}, false);
	info5 = new TextBlock("О логах: ", font, 0, info1->GetHeight(renderer) + info2->GetHeight(renderer) + info3->GetHeight(renderer) + info4->GetHeight(renderer), {75, 0, 130}, false);
	inputText = new TextBlock("IP: ", font, 0, 140, {75, 0, 130}, true);
	switchButton = new Button("Выключить логи", font, 0, info1->GetHeight(renderer) + info2->GetHeight(renderer) + info3->GetHeight(renderer) + info4->GetHeight(renderer) + info5->GetHeight(renderer), 2, {0, 0, 0}, false);
	ipInput = new InputBlock(font, inputText->GetLen(renderer), 140, {255, 255, 255}, true, renderer);
	int xBut = ipInput->GetMaxLen() + 100;
	inputButton = new Button("Подтвердить", font, xBut, 138, 2, {0, 0, 0}, true);
}

Application::~Application() {
	DeInit();
	delete info1;
	delete info2;
	delete info3;
	delete info4;
	delete info5;
	delete switchButton;
	delete inputText;
	delete ipInput;
	delete inputButton;
}

void Application::Init(){
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error with SDL init\n" << SDL_GetError() << endl;
		return;
	}
	window = SDL_CreateWindow("Server Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
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
	font = TTF_OpenFont("../Src/timesnewromanpsmt.ttf", 40);
}

void Application::DeInit() {
	if(renderer)
		SDL_DestroyRenderer(renderer);
	if(window)
		SDL_DestroyWindow(window);
	if(font)
		TTF_CloseFont(font);
	SDL_Quit();
	TTF_Quit();
}

void Application::Update() {
	while(isWorking) {
		UpdateEvents();
		SDL_RenderClear(renderer);
		if(info1->GetIsVisible())
			info1->Draw(renderer, ser->GetIP());
		if(info2->GetIsVisible())
			info2->Draw(renderer, ser->GetPort());
		if(info3->GetIsVisible())
			info3->Draw(renderer, ser->GetUserCount());
		if(info4->GetIsVisible())
			info4->Draw(renderer, ser->GetLogFile());
		if(info5->GetIsVisible())
			info5->Draw(renderer, ser->GetIsWriting());
		if(switchButton->GetIsVisible())
			switchButton->Draw(renderer);
		if(inputText->GetIsVisible())
			inputText->Draw(renderer);
		if(ipInput->GetIsVisible())
			ipInput->Draw(renderer);
		if(inputButton->GetIsVisible())
			inputButton->Draw(renderer);
		SDL_SetRenderDrawColor(renderer, 222, 184, 135, 255);
		SDL_RenderPresent(renderer);
		if(!ser->GetSearching() && ser->GetIsWorking())
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&UpdateServer, NULL, NULL, NULL);
		if(ser->GetIsWorking())
			ser->Update();
	}
}

void Application::UpdateEvents() {
	SDL_PollEvent(&event);
	switch(event.type) {
	case SDL_MOUSEBUTTONDOWN: {
		if(switchButton->GetIsVisible() && switchButton->CheckMouse()) {
			ser->SwitchLog();
			if(ser->GetIsLogWriting())
				switchButton->TextUpdate("Выключить логи");
			else
				switchButton->TextUpdate("Включить логи");
		}
		if(inputButton->GetIsVisible() && inputButton->CheckMouse()) {
			ser->StartServer(ipInput->GetText());
			if(ser->GetIsWorking()) {
				ipInput->SetVisible();
				inputButton->SetVisible();
				inputText->SetVisible();
				info1->SetVisible();
				info2->SetVisible();
				info3->SetVisible();
				info4->SetVisible();
				info5->SetVisible();
				switchButton->SetVisible();
			}
		}
		if(ipInput->CheckMouse() && ipInput->GetIsVisible())
			ipInput->SetFocus();
		else if(ipInput->GetIsVisible())
			ipInput->SetDeFocus();
	} break;
	case SDL_KEYDOWN: {
		if(event.key.keysym.sym == SDLK_BACKSPACE && ipInput->GetIsVisible())
			ipInput->DeleteLast();
	} break;
	case SDL_TEXTINPUT: {
		if(ipInput->GetIsVisible())
			ipInput->AddChar(event.text.text);
	} break;
	case SDL_QUIT: {
		ser->SendQuit();
		isWorking = false;
	} break;
	}
}