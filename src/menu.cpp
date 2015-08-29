#include "menu.h"

Menu::Menu(Screen *screen, const char* title) {
	this->screen = screen;
	textwhite.r = textwhite.g = textwhite.b = 255;
	textgray.r = textgray.g = textgray.b = 192;
	char filePath[256];
	getFilePath(filePath, "fonts/Cheapmot.TTF");
	if(!(font = TTF_OpenFont(filePath, 20)))
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	if (!(smallFont = TTF_OpenFont(filePath, 12)))
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	if (!(largeFont = TTF_OpenFont(filePath, 24)))
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	if (!(veryLargeFont = TTF_OpenFont(filePath, 48)))
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	if (!(hugeFont = TTF_OpenFont(filePath, 96)))
		printf("Unable to open TTF font: %s\n", TTF_GetError());
	menuTitle = TTF_RenderText_Solid(veryLargeFont, title, textwhite);
	back = TTF_RenderText_Solid(largeFont, "back", textwhite);
}
Menu::~Menu() {
	SDL_FreeSurface(menuTitle);
	SDL_FreeSurface(back);
	TTF_CloseFont(font);
	TTF_CloseFont(smallFont);
	TTF_CloseFont(largeFont);
	TTF_CloseFont(veryLargeFont);
	TTF_CloseFont(hugeFont);
}

void Menu::draw() {
	screen->clear();
	this->drawTitle();
	screen->draw(back, 320 - (back->w >> 1), 430 - (back->h >> 1));
	screen->AddUpdateRects(0, 0, 640, 480);
	screen->Refresh();
}

int Menu::show() {
	int event;
	while(!(event = eventloop()))
		SDL_Delay(MIN_FRAME_DURATION);
}

void Menu::drawTitle() {
	screen->draw(menuTitle, 320 - (menuTitle->w >> 1), 50);
}

int Menu::eventloop() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_RETURN)
					return 1;
				if((event.key.keysym.sym == SDLK_q)||(event.key.keysym.sym == SDLK_ESCAPE))
					return 1;
				break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
					return 1;
			break;
		case SDL_QUIT:
				return 1;
		}
	}
	return 0;
}
