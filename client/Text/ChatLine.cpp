#include "ChatLine.h"

ChatLine::ChatLine(std::string _text, TTF_Font *_font, SDL_Color _textColor) {
	text = _text;
	font = _font;
	textColor = _textColor;
}

ChatLine::~ChatLine() {

}

void ChatLine::SetY(int _y) {
	sprite.y = _y;
}

void ChatLine::SetX(int _x) {
	sprite.x = _x;
}

void ChatLine::Draw(SDL_Renderer *render) {
	SDL_Surface *textBlockSurface = TTF_RenderUTF8_Blended(font, text.c_str(), textColor);
	SDL_Texture *textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
	SDL_QueryTexture(textBlockTexture, NULL, NULL, &sprite.w, &sprite.h);
	SDL_RenderFillRect(render, &sprite);
	SDL_RenderCopy(render, textBlockTexture, NULL, &sprite);
	SDL_FreeSurface(textBlockSurface);
	SDL_DestroyTexture(textBlockTexture);
}