#include "Inputhandler.h"
#include <allegro5\allegro_primitives.h>


Point2D InputHandler::clicked;
Board *InputHandler::board;
int InputHandler::GRIDSIZE;
bool InputHandler::mouseButtonDown;
PathType InputHandler::pathType;
bool InputHandler::onePulse;
ALLEGRO_EVENT_QUEUE *InputHandler::keyboard_event_queue;
ALLEGRO_EVENT_QUEUE *InputHandler::display_event_queue;
ALLEGRO_EVENT_QUEUE *InputHandler::mouse_event_queue;

bool InputHandler::init(Board *gameBoard, int gridsize, ALLEGRO_DISPLAY *display) {
	InputHandler::board = gameBoard;
	InputHandler::GRIDSIZE = gridsize;
	InputHandler::clicked = { -1, -1 };
	InputHandler::mouseButtonDown = false;
	InputHandler::pathType = ROAD_PATH;

	keyboard_event_queue = al_create_event_queue();
	if (!keyboard_event_queue) {
		fprintf(stderr, "failed to create keyboard_event_queue!\n");
		return false;
	}
	/*al_destroy_event_queue(display_event_queue);
	al_destroy_event_queue(keyboard_event_queue);
	al_destroy_event_queue(mouse_event_queue);*/
	display_event_queue = al_create_event_queue();
	if (!display_event_queue) {
		al_destroy_event_queue(keyboard_event_queue);
		fprintf(stderr, "failed to create display_event_queue!\n");
		return false;
	}
	
	mouse_event_queue = al_create_event_queue();
	if (!mouse_event_queue) {
		al_destroy_event_queue(display_event_queue);
		al_destroy_event_queue(keyboard_event_queue);
		fprintf(stderr, "failed to create display_event_queue!\n");
		return false;
	}
	al_register_event_source(display_event_queue, al_get_display_event_source(display));
	al_register_event_source(keyboard_event_queue, al_get_keyboard_event_source());
	al_register_event_source(mouse_event_queue, al_get_mouse_event_source());
	return true;
}

void InputHandler::handleInput() {
	ALLEGRO_EVENT disp_ev;
	ALLEGRO_EVENT mouse_ev;
	ALLEGRO_EVENT key_ev;
	al_get_next_event(mouse_event_queue, &mouse_ev);
	al_get_next_event(keyboard_event_queue, &key_ev);
	al_get_next_event(display_event_queue, &disp_ev);
	handleMouse(mouse_ev);
	handleDisplay(disp_ev);
	handleKeyboard(key_ev);
}

void InputHandler::handleMouse(ALLEGRO_EVENT event) {
	//fprintf(stderr, "%i %i %i\n", event.mouse.x, event.mouse.y, board->grid[1][1]->visited);
	//al_draw_filled_circle(10, 10, 40, al_map_rgb(255, 255, 255));
	if (event.type == ALLEGRO_EVENT_MOUSE_AXES || event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
		board->mouse = board->calculate_closest_coordinate(event.mouse.x, event.mouse.y, GRIDSIZE);
	}
	
	
	if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && !mouseButtonDown) {
		mouseButtonDown = true;

		if (clicked.x == -1) {
			clicked = board->calculate_closest_node(event.mouse.x, event.mouse.y, GRIDSIZE);
			fprintf(stderr, "%i %i %i\n", clicked.x, clicked.y, board->grid[clicked.x][clicked.y]->visited);
		}
		else {

			Point2D temp = board->calculate_closest_node(event.mouse.x, event.mouse.y, GRIDSIZE);
			fprintf(stderr, "%i %i %i\n", temp.x, temp.y, board->grid[temp.x][temp.y]->visited);
			std::queue<Crossroad*> paths;
			board->findPath(board->grid[clicked.x][clicked.y], board->grid[temp.x][temp.y], &paths, pathType);

			if (!paths.empty())
				board->buildRoad(paths);
			clicked.x = -1;
		}

	}
	if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
		mouseButtonDown = false;
	}
}

void InputHandler::handleKeyboard(ALLEGRO_EVENT event) {

	/*
	if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
	al_draw_filled_circle(500,400, 2, al_map_rgb(128, 128, 0));
	}*/

	if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
		board->setGameRunning(false);

	if (event.keyboard.keycode == ALLEGRO_KEY_A && !onePulse) {
		onePulse = true;
		for (int x = 0; x < board->xSize; ++x)
			for (int y = 0; y < board->ySize; ++y)
				fprintf(stderr, "%i %i %i %i\n", x, y, board->grid[x][y]->visited, board->grid[x][y]->previous);
	}

	if (event.keyboard.keycode == ALLEGRO_KEY_S)
		onePulse = false;

	if (event.keyboard.keycode == ALLEGRO_KEY_W)
		pathType = FREE_PATH;

	if (event.keyboard.keycode == ALLEGRO_KEY_Q)
		pathType = ROAD_PATH;

	if (event.keyboard.keycode == ALLEGRO_KEY_R) {
		for (int x = 0; x < board->xSize; ++x)
			for (int y = 0; y < board->ySize; ++y) {
				board->grid[x][y]->visited = false;
				board->grid[x][y]->previous = NULL;
			}
	}
}

void InputHandler::handleDisplay(ALLEGRO_EVENT event) {
	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		board->setGameRunning(false);
}

InputHandler::~InputHandler() {
	if(display_event_queue)
		al_destroy_event_queue(display_event_queue);
	if (keyboard_event_queue)
		al_destroy_event_queue(keyboard_event_queue);
	if (mouse_event_queue)
		al_destroy_event_queue(mouse_event_queue);
}