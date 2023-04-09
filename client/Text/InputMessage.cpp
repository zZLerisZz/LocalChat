#include "InputMessage.h"

InputMessage::InputMessage(TTF_Font *_font, int _x, int _y, int _w, int _h, SDL_Color _textColor) {
	text = "";
	font = _font;
	sprite.x = _x;
	sprite.y = _y;
	textRect.x = _x;
	textRect.y = _y;
	textColor = _textColor;
	sprite.w = textRect.w = _w;
	sprite.h = textRect.h = _h;
	isFocused = false;
}

InputMessage::~InputMessage() {

}

void InputMessage::Draw(SDL_Renderer *render) {
	std::string res = text;
	if(isFocused)
		res += '|';
	SDL_Surface *textBlockSurface = TTF_RenderUTF8_Blended(font, res.c_str(), textColor);
	SDL_Texture *textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
	SDL_QueryTexture(textBlockTexture, NULL, NULL, &sprite.w, &sprite.h);
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	if(sprite.w > textRect.w) {
		SDL_FreeSurface(textBlockSurface);
		SDL_DestroyTexture(textBlockTexture);
		res = GetShorterText(render);
		textBlockSurface = TTF_RenderUTF8_Blended(font, res.c_str(), textColor);
		textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
		SDL_QueryTexture(textBlockTexture, NULL, NULL, &sprite.w, &sprite.h);
	}
	SDL_RenderFillRect(render, &textRect);
	SDL_RenderFillRect(render, &sprite);
	SDL_RenderCopy(render, textBlockTexture, NULL, &sprite);
	SDL_FreeSurface(textBlockSurface);
	SDL_DestroyTexture(textBlockTexture);
}

void InputMessage::DeleteLast() {
	if(!isFocused)
		return;
	const char *test = text.c_str();
	if(!text.empty())
		if(test[text.size() - 1] < 0)
			if(text.size() > 2)
				text.erase(text.size() - 2);
			else
				text = "";
		else
			if(text.size() > 1)
				text.erase(text.size() - 1);
			else
				text = "";
}

void InputMessage::AddChar(char *ch) {
	if(sprite.w < textRect.w && isFocused) {
		text += ch[0];
		if(ch[0] < 0)
			text += ch[1];
	}
}

bool InputMessage::CheckMouse() {
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	if(mx >= textRect.x && mx <= (textRect.x + textRect.w) &&
		my >= textRect.y && my <= (textRect.y + textRect.h))
		return true;
	return false;
}

void InputMessage::SetFocus() {
	isFocused = true;
}

void InputMessage::SetDeFocus() {
	isFocused = false;
}

std::string InputMessage::GetText() {
	std::string temp = text;
	text = "";
	isFocused = false;
	return temp;
}

std::string InputMessage::GetShorterText(SDL_Renderer *render) {
	std::string temp = text;
	if(isFocused)
		temp += '|';
	SDL_Surface *textBlockSurface = TTF_RenderUTF8_Blended(font, temp.c_str(), textColor);
	SDL_Texture *textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
	SDL_QueryTexture(textBlockTexture, NULL, NULL, &sprite.w, &sprite.h);
	SDL_FreeSurface(textBlockSurface);
	SDL_DestroyTexture(textBlockTexture);
	while(sprite.w > textRect.w) {
		if(temp.c_str()[0] < 0)
			temp.erase(0, 2);
		else
			temp.erase(0, 1);
		textBlockSurface = TTF_RenderUTF8_Blended(font, temp.c_str(), textColor);
		textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
		SDL_QueryTexture(textBlockTexture, NULL, NULL, &sprite.w, &sprite.h);
		SDL_FreeSurface(textBlockSurface);
		SDL_DestroyTexture(textBlockTexture);
	}
	return temp;
}