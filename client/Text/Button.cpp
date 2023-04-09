#include "Button.h"

Button::Button(const char* _text, TTF_Font *_font, int _x, int _y, int _thickOfBorder, SDL_Color _textColor) {
	text = _text;
	font = _font;
	border.x = _x;
	border.y = _y;
	thickOfBorder = _thickOfBorder;
	sprite.x = _x + thickOfBorder;
	sprite.y = _y + thickOfBorder;
	textColor = _textColor;
}

void Button::Draw(SDL_Renderer *render) {
	SDL_Surface *textBlockSurface = TTF_RenderUTF8_Blended(font, text.c_str(), textColor);
	SDL_Texture *textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
	SDL_QueryTexture(textBlockTexture, NULL, NULL, &sprite.w, &sprite.h);
	border.w = sprite.w + 2 * thickOfBorder;
	border.h = sprite.h + 2 * thickOfBorder;
	if(!CheckMouse())
		SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	else
		SDL_SetRenderDrawColor(render, 0, 0, 255, 255);
	SDL_RenderFillRect(render, &border);
	SDL_SetRenderDrawColor(render, 128, 128, 128, 255);
	SDL_RenderFillRect(render, &sprite);
	SDL_RenderCopy(render, textBlockTexture, NULL, &sprite);
	SDL_FreeSurface(textBlockSurface);
	SDL_DestroyTexture(textBlockTexture);
}

bool Button::CheckMouse() {
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	if(mx >= border.x && mx <= (border.x + border.w) &&
		my >= border.y && my <= (border.y + border.h))
		return true;
	return false;
}

Button::~Button() {
	
}