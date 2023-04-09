#include "TextBlock.h"

TextBlock::TextBlock(const char *_text, TTF_Font *_font, int _x, int _y, SDL_Color _textColor, bool _isVisible) {
	text = _text;
	font = _font;
	sprite.x = _x;
	sprite.y = _y;
	textColor = _textColor;
	isVisible = _isVisible;
}

void TextBlock::Draw(SDL_Renderer *render) {
	SDL_Surface *textBlockSurface = TTF_RenderUTF8_Blended(font, text.c_str(), textColor);
	SDL_Texture *textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
	SDL_QueryTexture(textBlockTexture, NULL, NULL, &sprite.w, &sprite.h);
	SDL_RenderFillRect(render, &sprite);
	SDL_RenderCopy(render, textBlockTexture, NULL, &sprite);
	SDL_FreeSurface(textBlockSurface);
	SDL_DestroyTexture(textBlockTexture);
}

void TextBlock::Draw(SDL_Renderer *render, std::string add) {
	std::string res = text + add;
	SDL_Surface *textBlockSurface = TTF_RenderUTF8_Blended(font, res.c_str(), textColor);
	SDL_Texture *textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
	SDL_QueryTexture(textBlockTexture, NULL, NULL, &sprite.w, &sprite.h);
	SDL_RenderFillRect(render, &sprite);
	SDL_RenderCopy(render, textBlockTexture, NULL, &sprite);
	SDL_FreeSurface(textBlockSurface);
	SDL_DestroyTexture(textBlockTexture);
}

void TextBlock::Draw(SDL_Renderer *render, unsigned short add) {
	std::string res = text + std::to_string(add);
	SDL_Surface *textBlockSurface = TTF_RenderUTF8_Blended(font, res.c_str(), textColor);
	SDL_Texture *textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
	SDL_QueryTexture(textBlockTexture, NULL, NULL, &sprite.w, &sprite.h);
	SDL_RenderFillRect(render, &sprite);
	SDL_RenderCopy(render, textBlockTexture, NULL, &sprite);
	SDL_FreeSurface(textBlockSurface);
	SDL_DestroyTexture(textBlockTexture);
}

void TextBlock::Draw(SDL_Renderer *render, unsigned int add) {
	std::string res = text + std::to_string(add);
	SDL_Surface *textBlockSurface = TTF_RenderUTF8_Blended(font, res.c_str(), textColor);
	SDL_Texture *textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
	SDL_QueryTexture(textBlockTexture, NULL, NULL, &sprite.w, &sprite.h);
	SDL_RenderFillRect(render, &sprite);
	SDL_RenderCopy(render, textBlockTexture, NULL, &sprite);
	SDL_FreeSurface(textBlockSurface);
	SDL_DestroyTexture(textBlockTexture);
}

void TextBlock::Draw(SDL_Renderer *render, const char* add) {
	std::string res = text + add;
	SDL_Surface *textBlockSurface = TTF_RenderUTF8_Blended(font, res.c_str(), textColor);
	SDL_Texture *textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
	SDL_QueryTexture(textBlockTexture, NULL, NULL, &sprite.w, &sprite.h);
	SDL_RenderFillRect(render, &sprite);
	SDL_RenderCopy(render, textBlockTexture, NULL, &sprite);
	SDL_FreeSurface(textBlockSurface);
	SDL_DestroyTexture(textBlockTexture);
}

int TextBlock::GetLen(SDL_Renderer *render) {
	SDL_Surface *textBlockSurface = TTF_RenderUTF8_Blended(font, text.c_str(), textColor);
	SDL_Texture *textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
	SDL_QueryTexture(textBlockTexture, NULL, NULL, &sprite.w, &sprite.h);
	SDL_FreeSurface(textBlockSurface);
	SDL_DestroyTexture(textBlockTexture);
	return sprite.w;
}

int TextBlock::GetHeight(SDL_Renderer *render) {
	SDL_Surface *textBlockSurface = TTF_RenderUTF8_Blended(font, text.c_str(), textColor);
	SDL_Texture *textBlockTexture = SDL_CreateTextureFromSurface(render, textBlockSurface);
	SDL_QueryTexture(textBlockTexture, NULL, NULL, &sprite.w, &sprite.h);
	SDL_FreeSurface(textBlockSurface);
	SDL_DestroyTexture(textBlockTexture);
	return sprite.h;
}

TextBlock::~TextBlock() {

}

bool TextBlock::GetIsVisible() {
	return isVisible;
}

void TextBlock::SetVisible() {
	isVisible = !isVisible;
}