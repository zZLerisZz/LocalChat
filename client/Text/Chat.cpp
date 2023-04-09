#include "Chat.h"

std::list<ChatLine> Chat::lines;
std::list<std::pair<std::string, int>> Chat::messagesToCut;
bool Chat::goodQuit = false;

Chat::Chat(Client *_client, TTF_Font *_chatFont, TTF_Font *_infoFont) {
	client = _client;
	chatFont = _chatFont;
	infoFont = _infoFont;
	chatRect.x = chatRect.y = 0;
	chatRect.w = CHATBAR_WIDTH;
	chatRect.h = CHATBAR_HEIGHT;
	interfaceRect.x = CHATBAR_WIDTH;
	interfaceRect.y = 0;
	interfaceRect.w = 1280 - CHATBAR_WIDTH;
	interfaceRect.h = 720;
	messageRect.x = 0;
	messageRect.y = CHATBAR_HEIGHT;
	messageRect.w = CHATBAR_WIDTH;
	messageRect.h = 720 - CHATBAR_HEIGHT;
	isVisible = true;
	move = 0;
}

void Chat::StartChat() {
	ipText = new TextBlock(client->GetIP(), infoFont, CHATBAR_WIDTH, 0, {188, 143, 143});
	portText = new TextBlock(std::to_string(client->GetPort()), infoFont, CHATBAR_WIDTH, INFO_FONT_HEIGHT + 20, {188, 143, 143});
	nickText = new TextBlock(client->GetNickname(), infoFont, CHATBAR_WIDTH, INFO_FONT_HEIGHT * 2 + 40, {188, 143, 143});
	sendButton = new Button("Отправить", infoFont, CHATBAR_WIDTH + 20, CHATBAR_HEIGHT, 2, {0, 0, 0});
	message = new InputMessage(infoFont, 0, CHATBAR_HEIGHT, CHATBAR_WIDTH, 720 - CHATBAR_HEIGHT, {0, 0, 0});
	client->StartClient(&messagesToCut);
	client->SetQuit(&goodQuit);
	goodQuit = false;
	move = 0;
}

Chat::~Chat() {
	delete ipText;
	delete portText;
	delete nickText;
	delete sendButton;
	delete message;
}

void Chat::Update(SDL_Renderer *render, SDL_Event *event) {
	if(!client->GetIsConnected()) {
		isVisible = false;
		return;
	}
	UpdateEvents(event);
	MessageToLines(render);
	Draw(render);
	if(goodQuit) {
		isVisible = false;
		client->QuitServer();
	}
}

void Chat::UpdateEvents(SDL_Event *event) {
	SDL_PollEvent(event);
	switch(event->type) {
	case SDL_MOUSEBUTTONDOWN: {
		if(message->CheckMouse()) {
			message->SetFocus();
		}
		else if(sendButton->CheckMouse()) {
			std::string temp = message->GetText();
			if(CommandsHandler::SetCommand(temp.c_str()) != CHECK && CommandsHandler::SetCommand(temp.c_str()) != DENY && CommandsHandler::SetCommand(temp.c_str()) != ACCEPT && CommandsHandler::SetCommand(temp.c_str()) != QUIT && temp != "") {
				client->SendMessage(temp);
				char timeString[20];
				time_t now = time(0);
				struct tm tstruct = *localtime(&now);
				strftime(timeString, sizeof(timeString), "%d.%m.%Y %X", &tstruct);
				temp = client->GetNickname() + "[" + timeString + "]: " + temp;
				messagesToCut.push_back(std::make_pair(temp, 0));
			}
			else if(CommandsHandler::SetCommand(temp.c_str()) == QUIT) {
				client->QuitServer();
				goodQuit = true;
				messagesToCut.clear();
				lines.clear();
				isVisible = false;
			}
		}
		else {
			message->SetDeFocus();
		}
	} break;
	case SDL_TEXTINPUT: {
		message->AddChar(event->text.text);
	} break;
	case SDL_KEYDOWN: {
		if(event->key.keysym.sym == SDLK_BACKSPACE) {
			message->DeleteLast();
		}
		else if(event->key.keysym.sym == SDLK_RETURN) {
			std::string temp = message->GetText();
			if(CommandsHandler::SetCommand(temp.c_str()) != CHECK && CommandsHandler::SetCommand(temp.c_str()) != DENY && CommandsHandler::SetCommand(temp.c_str()) != ACCEPT && CommandsHandler::SetCommand(temp.c_str()) != QUIT && temp != "") {
				client->SendMessage(temp);
				char timeString[20];
				time_t now = time(0);
				struct tm tstruct = *localtime(&now);
				strftime(timeString, sizeof(timeString), "%d.%m.%Y %X", &tstruct);
				temp = client->GetNickname() + "[" + timeString + "]: " + temp;
				messagesToCut.push_back(std::make_pair(temp, 0));
			}
			else if(CommandsHandler::SetCommand(temp.c_str()) == QUIT) {
				client->QuitServer();
				goodQuit = true;
				messagesToCut.clear();
				lines.clear();
				isVisible = false;
			}
		}
		else if(event->key.keysym.sym == SDLK_DOWN) {
			if(lines.size() - move > LINE_COUNT)
				move++;
		}
		else if(event->key.keysym.sym == SDLK_UP) {
			if(move > 0)
				move--;
		}
	} break;	
	case SDL_QUIT: {
		client->QuitServer();
		isVisible = false;
		goodQuit = false;
	} break;
	}
}

void Chat::Draw(SDL_Renderer *render) {
	SDL_RenderClear(render);
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderFillRect(render, &chatRect);
	DrawLines(render);
	SDL_SetRenderDrawColor(render, 255, 240, 245, 255);
	SDL_RenderFillRect(render, &interfaceRect);
	ipText->Draw(render);
	portText->Draw(render);
	nickText->Draw(render);
	sendButton->Draw(render);
	SDL_SetRenderDrawColor(render, 128, 128, 128, 255);
	SDL_RenderFillRect(render, &messageRect);
	message->Draw(render);
	SDL_RenderPresent(render);
}

void Chat::DrawLines(SDL_Renderer *render) {
	int iter = 0, up = 0;
	for(auto it = lines.begin(); it != lines.end(); it++, iter++) {
		if(iter >= move) {
			it->SetX(0);
			it->SetY((LINE_COUNT - 1 - up++) * CHAT_FONT_HEIGHT);
			it->Draw(render);
		}
	}
}

void Chat::SetVisible() {
	isVisible = true;
}

void Chat::SetInVisible() {
	isVisible = false;
}

bool Chat::GetIsVisible() {
	return isVisible;
}

void Chat::MessageToLines(SDL_Renderer *render) {
	for(auto it = messagesToCut.begin(); it != messagesToCut.end(); it++) {
		std::string temp = "";
		SDL_Rect tmp;
		SDL_Surface *textBlockSurface;
		SDL_Texture *textBlockTexture;
		while(it->first.size() != 0) {
			while(it->first.size() != 0 && tmp.w < CHATBAR_WIDTH) {
				if(it->first.c_str()[0] < 0) {
					temp += it->first.substr(0, 2);
					it->first.erase(0, 2);
				}
				else {
					temp += it->first.substr(0, 1);
					it->first.erase(0, 1);
				}
				textBlockSurface = TTF_RenderUTF8_Blended(chatFont, temp.c_str(), {0, 0, 0});
				textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
				SDL_QueryTexture(textBlockTexture, NULL, NULL, &tmp.w, &tmp.h);
				SDL_FreeSurface(textBlockSurface);
				SDL_DestroyTexture(textBlockTexture);
			}
			/*
			if(tmp.w > CHATBAR_WIDTH) {
				if(temp.c_str()[temp.size() - 1] < 0) {
					std::string buf = temp.substr(0, 2);
					it->first = buf + it->first;
					temp.erase(temp.size() - 2);
				}
				else {
					std::string buf = temp.substr(0, 1);
					it->first = buf + it->first;
					temp.erase(temp.size() - 1);
				}
			}*/
			if(it->second == 0)
				lines.push_front(ChatLine(temp, chatFont, {0, 0, 255}));
			else if(it->second == 1)
				lines.push_front(ChatLine(temp, chatFont, {255, 0, 0}));
			else
				lines.push_front(ChatLine(temp, chatFont, {0, 255, 0}));
			temp = "";
			tmp.w = 0;
		}
	}
	messagesToCut.clear();
}

bool Chat::GetGoodQuit() {
	return goodQuit;
}