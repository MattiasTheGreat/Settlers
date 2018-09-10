#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


#include <stack>
#include "Crossroad.h"
#include "Road.h"
#include <vector>
#include <queue>
#include <sstream>
#include "board.h"
#include <thread>
#include "InputHandler.h"
#include <time.h>

//bear
#include <iostream>
#include <Windows.h>

int main(int argc, char **argv) {

	ALLEGRO_DISPLAY *display = NULL;
	
	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	if (!al_install_mouse()) {
		fprintf(stderr, "failed to initialize the mouse!\n");
		return -1;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return -1;
	}

	display = al_create_display(640, 480);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	al_init_primitives_addon();
	al_init_font_addon(); // initialize the font addon
	al_init_ttf_addon();// initialize the ttf (True Type Font) addon

	ALLEGRO_FONT *font = al_load_ttf_font("pirulen.ttf", 8, 0);
	if (!font) {
		fprintf(stderr, "Could not load 'pirulen.ttf'.\n");
		return -1;
	}
	
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	
	const int GRIDSIZE = 30;
	const int YSIZE = 30; //jämnt tal!
	const int XSIZE = 30;
	Board board(XSIZE,YSIZE);

	if (!InputHandler::init(&board, GRIDSIZE, display)) {
		al_destroy_display(display);
		return -1;
	}

	clock_t previous_time = clock();
	while (board.isGameRunning()){
		//Only update the screen at 60 fps.
		if ( (1000 * (clock() - previous_time)) / CLOCKS_PER_SEC > 60) {
			previous_time = clock();
			board.tick(GRIDSIZE);
		}
		InputHandler::handleInput();
		al_flip_display();
	}

	al_destroy_display(display);
	
	return 0;
}

