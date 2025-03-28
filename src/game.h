#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "constants.h"
#include "pacman_figur.h"
#include "ghost_figur.h"
#include "highscore.h"

class Game {
	public:
		static Game *getInstance();
		static void cleanUpInstance();

		void start();
		void checkMusic();
		void startHuntingMode();
		void sleep(int ms);  // sleep for a short time - should be used after a ghost or a fruit has been eaten (display score, sleep shortly, remove score)
		bool isGameOver();

	private:
		Game();
		~Game();
		static Game *instance;

		void init();

		bool gameOver;
		void setGameOver(bool gameOver);

		bool stopMoving;
		void stop(bool stop);  // stop all figures

		bool refreshGhosts;
		bool completeRedraw;

		bool pause;

		int startOffset;
		Uint32 currentTicks;

		int cnt_sleep;

		int cnt_hunting_mode;
		void stopHuntingMode();

		void preselectDirection(int keycode);
		void togglePause();
		bool eventloop();

		int deltaT;
		void updateDelayTime();

		int animationCounter;
		void handleAnimations();

		void handleStartOffset();
		void handleHuntingMode();
		void handleSleep();
		void handleFruit();

		int currentScore;
		bool checkLastPillEaten();
		void checkGhostTouched();
		void checkScoreForExtraLife();
		void checkedMove();
		void checkedRedraw();
		void checkGameOver();

		SDL_Surface *scoreLabel, *levelLabel;
};

#endif
