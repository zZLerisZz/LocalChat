#include "InputMenu.h"

InputMenu::InputMenu(Client *_client, TTF_Font *_font, SDL_Renderer *render) {
	isVisible = true;
	font = _font;
	client = _client;
	infoText = new TextBlock("Введите данные", font, 0, 0, {248, 248, 255});
	ipText = new TextBlock("IP: ", font, 0, infoText->GetHeight(render) + 20, {0, 255, 0});
	portText = new TextBlock("Порт: ", font, 0, infoText->GetHeight(render) + ipText->GetHeight(render) + 40, {0, 255, 0});
	nickText = new TextBlock("Никнейм: ", font, 0, infoText->GetHeight(render) + ipText->GetHeight(render) + portText->GetHeight(render) + 60, {0, 255, 0});
	ipInput = new InputBlock(font, ipText->GetLen(render), infoText->GetHeight(render) + 20, {0, 0, 0}, render);
	portInput = new InputBlock(font, portText->GetLen(render), infoText->GetHeight(render) + ipText->GetHeight(render) + 40, {0, 0, 0}, render);
	nickInput = new InputBlock(font, nickText->GetLen(render), infoText->GetHeight(render) + ipText->GetHeight(render) + portText->GetHeight(render) + 60, {0, 0, 0}, render);
	acceptButton = new Button("Подключиться", font, 0, infoText->GetHeight(render) + ipText->GetHeight(render) + portText->GetHeight(render) + nickText->GetHeight(render) + 80, 2, {0, 0, 0});
}

InputMenu::~InputMenu() {
	delete infoText;
	delete ipText;
	delete portText;
	delete nickText;
	delete ipInput;
	delete portInput;
	delete nickInput;
	delete acceptButton;
}

bool InputMenu::GetIsVisible() {
	return isVisible;
}

void InputMenu::Update(SDL_Renderer *render, SDL_Event *event) {
	UpdateEvents(event);
	Draw(render);
}

void InputMenu::UpdateEvents(SDL_Event *event) {
	SDL_PollEvent(event);
	switch(event->type) {
	case SDL_MOUSEBUTTONDOWN: {
		if(ipInput->CheckMouse()) {
			portInput->SetDeFocus();
			nickInput->SetDeFocus();
			ipInput->SetFocus();
		}
		else if(portInput->CheckMouse()) {
			ipInput->SetDeFocus();
			nickInput->SetDeFocus();
			portInput->SetFocus();
		}
		else if(nickInput->CheckMouse()) {
			ipInput->SetDeFocus();
			portInput->SetDeFocus();
			nickInput->SetFocus();
		}
		else if(acceptButton->CheckMouse()) {
			ipInput->SetDeFocus();
			portInput->SetDeFocus();
			nickInput->SetDeFocus();
			int res = client->TryToConnect(ipInput->GetText(), static_cast<unsigned short>(std::strtoul(portInput->GetText().c_str(), NULL, 0)), nickInput->GetText());
			if(client->GetIsConnected())
				isVisible = false;
			else if(res == 1)
				infoText->UpdateText("Неверный IP или порт");
			else if(res == 2)
				infoText->UpdateText("Неверный никнейм");
		}
		else {
			ipInput->SetDeFocus();
			portInput->SetDeFocus();
			nickInput->SetDeFocus();
		}
	} break;
	case SDL_KEYDOWN: {
		if(event->key.keysym.sym == SDLK_BACKSPACE) {
			ipInput->DeleteLast();
			portInput->DeleteLast();
			nickInput->DeleteLast();
		}
	} break;
	case SDL_TEXTINPUT: {
		ipInput->AddChar(event->text.text);
		portInput->AddChar(event->text.text);
		nickInput->AddChar(event->text.text);
	} break;
	case SDL_QUIT: {
		isVisible = false;
	} break;
	}
}

void InputMenu::Draw(SDL_Renderer *render) {
	SDL_RenderClear(render);
	infoText->Draw(render);
	ipText->Draw(render);
	ipInput->Draw(render);
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	portText->Draw(render);
	portInput->Draw(render);
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	nickText->Draw(render);
	nickInput->Draw(render);
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	acceptButton->Draw(render);
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderPresent(render);
}

void InputMenu::SetVisible(bool fl) {
	if(!fl)
		infoText->UpdateText("Вы были отключены");
	isVisible = true;
}