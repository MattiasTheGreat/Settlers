#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>



class Path;
class Board;
class Crossroad;
enum PathType; // Comes from board.h

class InputHandler {
public:
	~InputHandler();
	static Board *board;
	static int GRIDSIZE;
	static bool mouseButtonDown;
	static PathType pathType; //Specifies what path is currently being built. Mainly for testing purposes: testing how well the pathfinding algorithm works with different criteria.
	static bool onePulse; //Used for the keyboard prompt to hinder pressing 'a' to write to prompt multiple times at once.
	static ALLEGRO_EVENT_QUEUE *keyboard_event_queue;
	static ALLEGRO_EVENT_QUEUE *display_event_queue;
	static ALLEGRO_EVENT_QUEUE *mouse_event_queue;
	static Path* currentPath;

	static bool init(Board *board, int gridsize, ALLEGRO_DISPLAY *display);
	static void handleInput();

private:
	static void handleMouse(ALLEGRO_EVENT event);
	static void handleKeyboard(ALLEGRO_EVENT event);
	static void handleDisplay(ALLEGRO_EVENT event);

private:

};