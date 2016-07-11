#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include "Board.h"
#include "Crossroad.h"


class InputHandler {
public:
	~InputHandler();
	static Point2D clicked;
	static Board *board;
	static int GRIDSIZE;
	static bool mouseButtonDown;
	static PathType pathType;
	static bool onePulse;
	static ALLEGRO_EVENT_QUEUE *keyboard_event_queue;
	static ALLEGRO_EVENT_QUEUE *display_event_queue;
	static ALLEGRO_EVENT_QUEUE *mouse_event_queue;

	static bool init(Board *board, int gridsize, ALLEGRO_DISPLAY *display);
	static void handleInput();

private:
	static void handleMouse(ALLEGRO_EVENT event);
	static void handleKeyboard(ALLEGRO_EVENT event);
	static void handleDisplay(ALLEGRO_EVENT event);

private:

};