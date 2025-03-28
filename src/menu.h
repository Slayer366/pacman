#ifndef MENU_H
#define MENU_H
#include "platform.h"
#include <SDL/SDL_ttf.h>
#include <vector>
#include "menu_item.h"
#include "sounds.h"
#include "constants.h"

using namespace std;

class Menu {
	public:
		Menu(const char* title = NULL);
		virtual ~Menu();
		virtual void draw(bool updateAll = true);
		virtual int show();
		virtual void drawTitle();
		void addMenuItem(const char* menuItem, const char* menuItemAlt = NULL);
		void menuItemUp();
		void menuItemDown();
		void menuItemSelect(uint8_t selection);
		MenuItem* getSelectedMenuItem();
	protected:
		//static const int MIN_FRAME_DURATION = 30;
		uint8_t selection;
		vector<MenuItem*> menuItems;
		//Screen *screen;
		SDL_Surface *menuTitle;
		virtual int eventloop();
		void drawMenuItems();
		virtual int handleSelection();
		virtual void updateMenuItemNames();
};
#endif
