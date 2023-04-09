#include "InputBlock.h"

InputBlock::InputBlock(TTF_Font *_font, int _x, int _y, SDL_Color _textColor, SDL_Renderer *render) {
	text = "";
	font = _font;
	sprite.x = _x;
	sprite.y = _y;
	textRect.x = _x;
	textRect.y = _y;
	textColor = _textColor;
	std::string temp = "255.255.255.255|";
	SDL_Surface *textBlockSurface = TTF_RenderUTF8_Blended(font, temp.c_str(), textColor);
	SDL_Texture *textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
	SDL_QueryTexture(textBlockTexture, NULL, NULL, &textRect.w, &textRect.h);
	sprite.w = textRect.w;
	sprite.h = textRect.h;
	SDL_FreeSurface(textBlockSurface);
	SDL_DestroyTexture(textBlockTexture);
	maxLen = textRect.w;
	isFocused = false;
}

InputBlock::~InputBlock() {

}

int InputBlock::GetMaxLen() {
	return maxLen;
}

void InputBlock::Draw(SDL_Renderer *render) {
	std::string res = text;
	if(isFocused)
		res += '|';
	SDL_Surface *textBlockSurface = TTF_RenderUTF8_Blended(font, res.c_str(), textColor);
	SDL_Texture *textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
	SDL_QueryTexture(textBlockTexture, NULL, NULL, &sprite.w, &sprite.h);
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	SDL_RenderFillRect(render, &textRect);
	SDL_RenderFillRect(render, &sprite);
	SDL_RenderCopy(render, textBlockTexture, NULL, &sprite);
	SDL_FreeSurface(textBlockSurface);
	SDL_DestroyTexture(textBlockTexture);
}

void InputBlock::DeleteLast() {
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

void InputBlock::AddChar(char *ch) {
	if(sprite.w < textRect.w && isFocused) {
		text += ch[0];
		if(ch[0] < 0)
			text += ch[1];
	}
}

bool InputBlock::CheckMouse() {
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	if(mx >= textRect.x && mx <= (textRect.x + textRect.w) &&
		my >= textRect.y && my <= (textRect.y + textRect.h))
		return true;
	return false;
}

void InputBlock::SetFocus() {
	isFocused = true;
}

void InputBlock::SetDeFocus() {
	isFocused = false;
}

std::string InputBlock::GetText() {
	return text;
}