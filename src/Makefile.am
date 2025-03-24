## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS =\
	 -Wall\
	 -g


pacman_SOURCES =  \
	cmdline_options.cpp \
	cmdline_options.h \
	constants.cpp \
	constants.h \
	figur.cpp \
	figur.h \
	game.cpp \
	game.h \
	ghost_figur.cpp \
	ghost_figur.h \
	pacman.cpp \
	pacman.h \
	pacman_figur.cpp \
	pacman_figur.h \
	pille.cpp \
	pille.h \
	labyrinth.h \
	labyrinth.cpp \
	screen.h \
	screen.cpp \
	rail.h \
	rail.cpp \
	sounds.h \
	sounds.cpp \
	platform.h \
	platform.cpp \
	menu_main.h \
	menu_main.cpp \
	menu_options.h \
	menu_options.cpp \
	menu_about.h \
	menu_about.cpp \
	menu.h \
	menu.cpp \
	menu_item.cpp \
	menu_item.h \
	funny_animation.h \
	funny_animation.cpp \
	level.h \
	level.cpp \
	highscore.h \
	highscore.cpp

pacman_CXXFLAGS = -ansi \
	-fno-default-inline \
	-Wall \
	-W \
	-Wpointer-arith \
	-Wcast-qual \
	-Wcast-align \
	-Wconversion \
	-Woverloaded-virtual \
	-Wsign-compare \
	-Wnon-virtual-dtor \
	-Woverloaded-virtual \
	-Wfloat-equal \
	-Wunreachable-code \
	-Wno-reorder \
	-D_GNU_SOURCE

pacman_LDFLAGS = -D_REENTRANT $(SDL_CFLAGS)

pacman_LDADD =  \
	$(SDL_LIBS) \
	-lSDL_mixer \
	-lpthread



pacmanEXTRA_DIST = libsdl \
	sdl-image \
	sdl-ttf \
	sdl-mixer



bin_PROGRAMS =  \
	pacman
