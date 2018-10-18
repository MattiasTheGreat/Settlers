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


/*TODO:
Ongoing -------------------------------------------------------------------------------------------------------------

Common pathfinders:
	-TraversibleEdge and TraversibleNode now exists as baseclasses for pathfinding. Should be inherited by different levels of abstraction that we can pathfind on. Pathfinders doesnt' know of this yet though.

Item passing:
	-How to know which item to pick up at a crossroads. Currently based on previous node stuff, should be able to change to carrier. Feels more sound. ?= If there is no item to pick up giveItem in crossroads should return null.

When building a road and it goes to an existing flag it should change select as is done, but should finish the current road and create a carrier.

Eventuall improvements ----------------------------------------------------------------------------------------------

Board is too broad, it does to much. Suggestions so far for refactoring are:
	-The heuristics. The heuristics could be a class of its own, and the board only has a method to search given a heuristic.

Expand the heuristics so that they can take a vector as goal, allowing for searching for the closest of a number of nodes.

A lot (most?) of the vectors etc containing items should be priority vectors with my own comparator that puts items with higher priority first. I will also have to implement a item priority.
	
Items are not possible to pass properly yet. To allow this we need:
	-Some kind of order class that encapsulates items.
	-A Human abstract class of which a subclass will be carrier (or peddler) that actually carries stuff on the connections. Other subclasses are warriors, and perhaps all different proffesions if they should interact with the map.
	-A way to notify a peddler of that an item needs to be picked up. Think there is a start of this but that was long ago and might be depreciated.
	-Carrier interface. Might be nice if I want donkeys as in the original game
	-Since all peddlers are supposed to tick, they might wanna enter and leave a active list if they are not idle, so as to not tick unnecesary idle peddlers.
	-An iterator for peddlers that takes an argument would be cool. The argument being current goal: startNode, endNode or idleNode, and you can always just increment the iterator with the end goal you want.

Probably also want to think about potential memory leaks in my queues. The thing I see now is that the Path class takes a copy of a queue, meaning that it should be deleted somewhere after being passed to path. (not vberified)

ALso a lot of things in my classes are public and are being accessed form the outside in a manner that they probably should not. Probably not a problem at the moment, but its not pretty. Setters/getters...

*/
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

	//TODO: For some reason this doesn't initialize. Don't care about it now though.
	al_init_font_addon(); // initialize the font addon
	/*al_init_ttf_addon();// initialize the ttf (True Type Font) addon

	ALLEGRO_FONT *font = al_load_ttf_font("pirulen.ttf", 8, 0);
	if (!font) {
		fprintf(stderr, "Could not load 'pirulen.ttf'.\n");
		return -1;
	}*/
	
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	
	const int GRIDSIZE = 30;
	const int YSIZE = 30; //j�mnt tal!
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

