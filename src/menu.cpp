#include "menu.h"

Menu::Menu(const char* title):
	selection(0) {
//	this->screen = Screen::getInstance();
//	if(title)
//		menuTitle = TTF_RenderText_Solid(Screen::getVeryLargeFont(), title, Constants::WHITE_COLOR);
	menuTitle = title ? Screen::getTextSurface(Screen::getVeryLargeFont(), title, Constants::WHITE_COLOR) : NULL;
}
Menu::~Menu() {
	SDL_FreeSurface(menuTitle);
	for(unsigned int i = 0; i < menuItems.size(); ++i)
		delete menuItems.at(i);
}

void Menu::draw(bool updateAll) {
//	screen->clear();
//	this->drawTitle();
//	this->drawMenuItems();
//	if(updateAll)
//		screen->AddUpdateRects(0, 0, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
//	screen->Refresh();
	if (updateAll) {
		Screen::getInstance()->clear();
		drawTitle();
		drawMenuItems();
		Screen::getInstance()->addUpdateClipRect();
	} else {
		drawMenuItems();
	}
	Screen::getInstance()->Refresh();
}

void Menu::drawMenuItems() {
	const int vertical_pad = 35;
//	unsigned int i;
//	for(i = 0; i < menuItems.size(); ++i) {
//		if(selection == i)
//			menuItems.at(i)->setSelectMenuItem(true);
//		else
//			menuItems.at(i)->setSelectMenuItem(false);
//		screen->draw(menuItems.at(i)->getCurrentMenuItem(), 320 - (menuItems.at(i)->getCurrentMenuItem()->w >> 1), (430 - (i)*vertical_pad) - (menuItems.at(i)->getCurrentMenuItem()->h >> 1));
//	}
//	screen->AddUpdateRects(0, (430 - (i)*vertical_pad),
//	                       Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT-(430 - (i)*vertical_pad));
	SDL_Rect rect;
	rect.x = 0;
	rect.w = Constants::WINDOW_WIDTH;
	//rect.y = 430 - (int)menuItems.size()*vertical_pad;
	rect.y = static_cast<Sint16>(430 - static_cast<int>(menuItems.size()) *vertical_pad);
	//rect.h = Constants::WINDOW_HEIGHT - rect.y;
	rect.h = static_cast<Uint16>(std::max(0, Constants::WINDOW_HEIGHT - rect.y));
	Screen::getInstance()->fillRect(&rect, 0, 0, 0);
	for(uint8_t i = 0; i < menuItems.size(); ++i) {
		menuItems.at(i)->setSelectMenuItem(selection == i);
		Screen::getInstance()->draw(menuItems.at(i)->getCurrentMenuItem(), 320 - (menuItems.at(i)->getCurrentMenuItem()->w >> 1), (430 - (i)*vertical_pad) - (menuItems.at(i)->getCurrentMenuItem()->h >> 1));
	}
	Screen::getInstance()->AddUpdateRects(rect.x, rect.y, rect.w, rect.h);
}

int Menu::show() {
	draw();
	int event;
	while(!(event = eventloop())) {
		SDL_Delay(Constants::MIN_FRAME_DURATION);
	}
	return event;
}

void Menu::drawTitle() {
	if(menuTitle)
		Screen::getInstance()->draw_dynamic_content(menuTitle, 320 - (menuTitle->w >> 1), 50);
}

void Menu::addMenuItem(const char* menuItem, const char* menuItemAlt) {
	menuItems.push_back(new MenuItem(menuItem, menuItemAlt));
	menuItems.back()->setXPosition(320 - (menuItems.back()->getCurrentMenuItem()->w >> 1));
	menuItems.back()->setYPosition(430 - (((int)menuItems.size()-1)*35) - (menuItems.back()->getCurrentMenuItem()->h >> 1));
}

int Menu::eventloop() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_RETURN)
				return handleSelection();
			else if(event.key.keysym.sym == SDLK_UP)
				menuItemUp();
			else if(event.key.keysym.sym == SDLK_DOWN)
				menuItemDown();
			else if(event.key.keysym.sym == SDLK_f) {
				Screen::getInstance()->toggleFullscreen();
				updateMenuItemNames();
				this->draw();
			}
			else if(event.key.keysym.sym == SDLK_s) {
				Sounds::getInstance()->toggleEnabled();
				updateMenuItemNames();
				this->draw();
			}
			else if(event.key.keysym.sym == SDLK_m) {
				Sounds::getInstance()->toggleMusicEnabled();
				updateMenuItemNames();
				this->draw();
			}
			else if((event.key.keysym.sym == SDLK_q)||(event.key.keysym.sym == SDLK_ESCAPE))
				return 2;
			break;
		case SDL_MOUSEMOTION:
			for(unsigned int i = 0; i < menuItems.size(); ++i) {
				if(menuItems.at(i)->getXPosition() <= event.motion.x && event.motion.x <= menuItems.at(i)->getXPosition()+menuItems.at(i)->getCurrentMenuItem()->w && menuItems.at(i)->getYPosition() <= event.motion.y && event.motion.y <= menuItems.at(i)->getYPosition()+menuItems.at(i)->getCurrentMenuItem()->h) {
					//menuItemSelect(i);
					menuItemSelect(static_cast<uint8_t>(i));
					break;
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				if(menuItems.at(selection)->getXPosition() <= event.motion.x &&
					event.motion.x <= menuItems.at(selection)->getXPosition()+menuItems.at(selection)->getCurrentMenuItem()->w &&
					menuItems.at(selection)->getYPosition() <= event.motion.y &&
					event.motion.y <= menuItems.at(selection)->getYPosition()+menuItems.at(selection)->getCurrentMenuItem()->h)
				{
					return handleSelection();
				}
			}
			break;
		case SDL_QUIT:
				return 2;
		}
		if (event.type == SDL_ACTIVEEVENT) {
			if (event.active.state & SDL_APPACTIVE || event.active.state & SDL_APPINPUTFOCUS) {
				Screen::getInstance()->addTotalUpdateRect();
				Screen::getInstance()->Refresh();
			}
		} else if (event.type == SDL_VIDEOEXPOSE) {
			Screen::getInstance()->addTotalUpdateRect();
			Screen::getInstance()->Refresh();
		}
	}
	return 0;
}

void Menu::menuItemDown() {
	selection = (uint8_t) (selection - 1 + (uint8_t)menuItems.size()) % (uint8_t)menuItems.size();
	draw(false);
}

void Menu::menuItemUp() {
	selection = (uint8_t) (selection + 1) % (uint8_t)menuItems.size();
	draw(false);
}

void Menu::menuItemSelect(uint8_t selection) {
	if (this->selection != selection) {
		this->selection = selection;
		draw(false);
	}
}

MenuItem* Menu::getSelectedMenuItem() {
	return menuItems.at(selection);
}

int Menu::handleSelection() {
	return 0;
}

void Menu::updateMenuItemNames() {}
