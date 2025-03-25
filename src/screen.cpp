#include "screen.h"

TTF_Font *Screen::smallFont     = NULL;
TTF_Font *Screen::font          = NULL;
TTF_Font *Screen::largeFont     = NULL;
TTF_Font *Screen::veryLargeFont = NULL;
TTF_Font *Screen::hugeFont      = NULL;

Screen *Screen::instance = NULL;

Screen *Screen::getInstance() {
	if (!instance) {
		instance = new Screen();
	}
	return instance;
}

void Screen::cleanUpInstance() {
	if (smallFont) {
		TTF_CloseFont(smallFont);
		smallFont = NULL;
	}
	if (font) {
		TTF_CloseFont(font);
		font = NULL;
	}
	if (largeFont) {
		TTF_CloseFont(largeFont);
		largeFont = NULL;
	}
	if (veryLargeFont) {
		TTF_CloseFont(veryLargeFont);
		veryLargeFont = NULL;
	}
	if (hugeFont) {
		TTF_CloseFont(hugeFont);
		hugeFont = NULL;
	}
	if (instance) {
		delete instance;
		instance = NULL;
	}
}

Screen::Screen():
	sdlInitErrorOccured(false),
	fullscreen(CommandLineOptions::exists("f","fullscreen")),
	rect_num(0)
{
	// initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL video initialization failed: " << SDL_GetError() << std::endl;
        sdlInitErrorOccured = true;
    }
	if(!sdlInitErrorOccured && TTF_Init() == -1) {
		std::cout << "TTF initialization failed: " << TTF_GetError() << std::endl;
        sdlInitErrorOccured = true;
	}

	SDL_ShowCursor(SDL_DISABLE);

	atexit(SDL_Quit);
	atexit(SDL_CloseAudio);

	if (!sdlInitErrorOccured) {
	screen_surface = SDL_SetVideoMode(640, 480, 24, SDL_HWSURFACE);
		if(screen_surface == 0) {
			std::cout << "Setting video mode failed: " << SDL_GetError() << std::endl;
			sdlInitErrorOccured = true;
		}
	}
	SDL_WM_SetCaption("Pacman", "");
	atexit(Screen::cleanUpInstance);
}

Screen::~Screen() {
	TTF_Quit();
	SDL_Quit();
}

void Screen::AddUpdateRects(int x, int y, int w, int h) {
	if (rect_num >= Constants::MAX_UPDATE_RECTS)
		return;  // prevent index out of bounds problems
	if (x < 0) {
		w += x;
		x = 0;
	}
	if (y < 0) {
		h += y;
		y = 0;
	}
	if (x + w > screen_surface->w)
		w = screen_surface->w - x;
	if (y + h > screen_surface->h)
		h = screen_surface->h - y;
	if (w <= 0 || h <= 0) {
		return;
	}
	rects[rect_num].x = (short int) x;
	rects[rect_num].y = (short int) y;
	rects[rect_num].w = (short int) w;
	rects[rect_num].h = (short int) h;
	rect_num++;
}

void Screen::addTotalUpdateRect() {
	rects[0].x = 0;
	rects[0].y = 0;
	rects[0].w = static_cast<Uint16>(screen_surface->w);  // no scalingFactor as screen_surface already is the total screen surface
	rects[0].h = static_cast<Uint16>(screen_surface->h);
	rect_num = 1;  // all other update rects will be included in this one
}

void Screen::addUpdateClipRect() {
	AddUpdateRects(0, 0, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
}

void Screen::Refresh() {
	SDL_UpdateRects(screen_surface, rect_num, rects);
	rect_num = 0;
}

void Screen::draw_dynamic_content(SDL_Surface *surface, int x, int y) {
	SDL_Rect dest;
	dest.x = (short int) x;
	dest.y = (short int) y;
	dest.w = (short int) surface->w;
	dest.h = (short int) surface->h;
	SDL_BlitSurface(surface, NULL, screen_surface, &dest);
	AddUpdateRects(x, y, surface->w + 10, surface->h);
}

void Screen::draw(SDL_Surface* graphic, int offset_x, int offset_y) {
    if (0 == offset_x && 0 == offset_y) {
        SDL_BlitSurface(graphic, NULL, screen_surface, NULL);
    } else {
        SDL_Rect position;
        position.x = (short int) offset_x;
        position.y = (short int) offset_y;
		position.w = (short int) graphic->w;
		position.h = (short int) graphic->h;
		SDL_BlitSurface(graphic, NULL, screen_surface, &position);
    }
}

void Screen::setFullscreen(bool fs) {
    if (fs == fullscreen)
        return;  // the desired mode already has been activated, so do nothing
    SDL_Surface* newScreen;
    if(fs)
        newScreen = SDL_SetVideoMode(640, 480, 24, SDL_HWSURFACE | SDL_FULLSCREEN);
	else
        newScreen = SDL_SetVideoMode(640, 480, 24, SDL_HWSURFACE);
    if (NULL != newScreen) {  // successful? NULL indicates failure
        screen_surface = newScreen;  // take it, but do not dispose of the old screen (says SDL documentation)
        //AddUpdateRects(0, 0, screen_surface->w, screen_surface->h);
		addTotalUpdateRect();

        // no Refresh() here, because at this moment nothing has been drawn to the new screen
        fullscreen = fs;
    } else {
		if (fs) {
			std::cout << "Switching to fullscreen mode failed: " << SDL_GetError() << std::endl;
		} else {
			std::cout << "Switching from fullscreen mode failed: " << SDL_GetError() << std::endl;
		}
	}
}

//void Screen::drawHorizontalLine(int x1, int x2, int y, Uint8 r, Uint8 g, Uint8 b) {
//	if (SDL_MUSTLOCK(this->screen_surface))
//		SDL_LockSurface(this->screen_surface);
//	Uint8* p;
//	for (int i = x1; i <= x2; ++i) {
//		p = (Uint8*) this->screen_surface->pixels + (y * this->screen_surface->pitch) + (i * sizeof(Uint8) * 3);
//#if SDL_BYTEORDER == SDL_BIG_ENDIAN
//		p[0] = r; p[1] = g; p[2] = b;
//#else
//		p[0] = b; p[1] = g; p[2] = r;
//#endif
//	}
//	if (SDL_MUSTLOCK(this->screen_surface))
//		SDL_UnlockSurface(this->screen_surface);
//}
//
//void Screen::drawVerticalLine(int x, int y1, int y2, Uint8 r, Uint8 g, Uint8 b) {
//	if (SDL_MUSTLOCK(this->screen_surface))
//		SDL_LockSurface(this->screen_surface);
//	Uint8* p;
//	for (int i = y1; i <= y2; ++i) {
//		p = (Uint8*) this->screen_surface->pixels + (i * this->screen_surface->pitch) + (x * sizeof(Uint8) * 3);
//#if SDL_BYTEORDER == SDL_BIG_ENDIAN
//		p[0] = r; p[1] = g; p[2] = b;
//#else
//		p[0] = b; p[1] = g; p[2] = r;
//#endif
//	}
//	if (SDL_MUSTLOCK(this->screen_surface))
//		SDL_UnlockSurface(this->screen_surface);
//}
//
//SDL_Surface *Screen::LoadSurface(const char *filename, int transparent_color) {
//	SDL_Surface *surface, *temp;
//	temp = IMG_Load(filename);
//	if(!temp) {
//		printf("Unable to load image: %s\n", IMG_GetError());
//		exit(-1);
//	}
//	if(transparent_color != -1)
//		SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, (Uint32)SDL_MapRGB(temp->format, (uint8_t)transparent_color, (uint8_t)transparent_color, (uint8_t)transparent_color));
//	surface = SDL_DisplayFormat(temp);
//	if(surface == NULL) {
//		printf("Unable to convert image to display format: %s\n", SDL_GetError());
//                exit(EXIT_FAILURE);
//        }
//    SDL_FreeSurface(temp);
//    return surface;
//}

SDL_Surface *Screen::loadImage(const char *filename, int transparent_color) {
	char filePath[256];
	getFilePath(filePath, filename);
	SDL_Surface *surface, *temp;
	temp = IMG_Load(filePath);
	if (!temp) {
		std::cout << "Unable to load image: " << IMG_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	if (transparent_color != -1)
		SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, (Uint32)SDL_MapRGB(temp->format, (uint8_t)transparent_color, (uint8_t)transparent_color, (uint8_t)transparent_color));
	surface = SDL_DisplayFormat(temp);
	if (surface == NULL) {
		std::cout << "Unable to convert image to display format: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	SDL_FreeSurface(temp);
	return surface;
}

TTF_Font *Screen::loadFont(const char *filename, int ptSize) {
	char filePath[256];
	getFilePath(filePath, filename);
	TTF_Font *font = TTF_OpenFont(filePath, ptSize);
	if (!font) {
		std::cout << "Unable to open TTF font: " << TTF_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	return font;
}

SDL_Surface *Screen::getTextSurface(TTF_Font *font, const char *text, SDL_Color color) {
	SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
	if (!surface) {
		std::cout << "Unable to render text \"" << text << "\": " << TTF_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	return surface;
}

void Screen::clear() {
	SDL_Rect rect = {0, 0, 640,480};
	fillRect(&rect, 0, 0, 0);
}

void Screen::fillRect(SDL_Rect *rect, Uint8 r, Uint8 g, Uint8 b) {
	SDL_FillRect(screen_surface, rect, SDL_MapRGB(screen_surface->format, r, g, b));
}

TTF_Font *Screen::getSmallFont() {
	if (!smallFont)
		smallFont = loadFont("fonts/Cheapmot.TTF", 12);
	return smallFont;
}
TTF_Font *Screen::getFont() {
	if (!font)
		font = loadFont("fonts/Cheapmot.TTF", 20);
	return font;
}
TTF_Font *Screen::getLargeFont() {
	if (!largeFont)
		largeFont = loadFont("fonts/Cheapmot.TTF", 24);
	return largeFont;
}
TTF_Font *Screen::getVeryLargeFont() {
	if (!veryLargeFont)
		veryLargeFont = loadFont("fonts/Cheapmot.TTF", 48);
	return veryLargeFont;
}
TTF_Font *Screen::getHugeFont() {
	if (!hugeFont)
		hugeFont = loadFont("fonts/Cheapmot.TTF", 96);
	return hugeFont;
}
