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

int main(int argc, char **argv) {

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *keyboard_event_queue = NULL;
	ALLEGRO_EVENT_QUEUE *display_event_queue = NULL;
	ALLEGRO_EVENT_QUEUE *mouse_event_queue = NULL;
	
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

	keyboard_event_queue = al_create_event_queue();
	if (!keyboard_event_queue) {
		fprintf(stderr, "failed to create keyboard_event_queue!\n");
		al_destroy_display(display);
		return -1;
	}

	display_event_queue = al_create_event_queue();
	if (!display_event_queue) {
		fprintf(stderr, "failed to create display_event_queue!\n");
		al_destroy_display(display);
		return -1;
	}

	mouse_event_queue = al_create_event_queue();
	if (!mouse_event_queue) {
		fprintf(stderr, "failed to create display_event_queue!\n");
		al_destroy_display(display);
		return -1;
	}

	al_init_primitives_addon();

	al_init_font_addon(); // initialize the font addon
	al_init_ttf_addon();// initialize the ttf (True Type Font) addon
	al_register_event_source(display_event_queue, al_get_display_event_source(display));
	al_register_event_source(keyboard_event_queue, al_get_keyboard_event_source());
	al_register_event_source(mouse_event_queue, al_get_mouse_event_source());

	ALLEGRO_FONT *font = al_load_ttf_font("pirulen.ttf", 8, 0);

	if (!font) {
		fprintf(stderr, "Could not load 'pirulen.ttf'.\n");
		return -1;
	}

	
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	const int GRIDSIZE = 30;
	const int YSIZE = 20; //jämnt tal!
	const int XSIZE = 20;
	Board board(XSIZE,YSIZE);
	std::queue<Crossroad*> path;
	bool tillsvidare = false;
	int mouse_x = 0;
	int mouse_y = 0;
	Point2D clicked;
	clicked.x = -1;
	clicked.y = -1;
	bool test = false;
	PathType pathType = ROAD_PATH;
	while (true){
		ALLEGRO_EVENT key_ev;
		ALLEGRO_EVENT disp_ev;
		ALLEGRO_EVENT mouse_ev;
		
		al_get_next_event(keyboard_event_queue, &key_ev);
		al_get_next_event(display_event_queue, &disp_ev);
		al_get_next_event(mouse_event_queue, &mouse_ev);

		al_clear_to_color(al_map_rgb(0, 0, 0));

		for (int x = 0; x < XSIZE; ++x) {
			for (int y = 0; y < YSIZE; ++y) {
				
				if ((x != XSIZE - 1 && x != 0 && y != YSIZE - 1)) {
					board.grid[x][y]->east->paintThySelf(GRIDSIZE);
					board.grid[x][y]->southEast->paintThySelf(GRIDSIZE);
					board.grid[x][y]->southWest->paintThySelf(GRIDSIZE);
					/*
					if (y % 2 == 1) {
						al_draw_line(board.grid[x][y]->coordinates.x * GRIDSIZE + GRIDSIZE/2, board.grid[x][y]->coordinates.y * GRIDSIZE, board.grid[x][y]->getEastNeighbour()->coordinates.x * GRIDSIZE + GRIDSIZE/2, board.grid[x][y]->getEastNeighbour()->coordinates.y * GRIDSIZE, (board.grid[x][y]->east->built ? al_map_rgb(255, 0, 0) : al_map_rgb(0, 0, 255)), 1);
						al_draw_line(board.grid[x][y]->coordinates.x * GRIDSIZE + GRIDSIZE/2, board.grid[x][y]->coordinates.y * GRIDSIZE, board.grid[x][y]->getSouthEastNeighbour()->coordinates.x * GRIDSIZE, board.grid[x][y]->getSouthEastNeighbour()->coordinates.y * GRIDSIZE, (board.grid[x][y]->southEast->built ? al_map_rgb(255, 0, 0) : al_map_rgb(0,0,255)), 1);
						al_draw_line(board.grid[x][y]->coordinates.x * GRIDSIZE + GRIDSIZE/2, board.grid[x][y]->coordinates.y * GRIDSIZE, board.grid[x][y]->getSouthWestNeighbour()->coordinates.x * GRIDSIZE, board.grid[x][y]->getSouthWestNeighbour()->coordinates.y * GRIDSIZE, (board.grid[x][y]->southWest->built ? al_map_rgb(255, 0, 0) : al_map_rgb(0, 0, 255)), 1);
					}
					else {
						al_draw_line(board.grid[x][y]->coordinates.x * GRIDSIZE, board.grid[x][y]->coordinates.y * GRIDSIZE, board.grid[x][y]->getEastNeighbour()->coordinates.x * GRIDSIZE, board.grid[x][y]->getEastNeighbour()->coordinates.y * GRIDSIZE,(board.grid[x][y]->east-> built ? al_map_rgb(255, 0, 0) : al_map_rgb(0,0,255)), 1);
						al_draw_line(board.grid[x][y]->coordinates.x * GRIDSIZE, board.grid[x][y]->coordinates.y * GRIDSIZE, board.grid[x][y]->getSouthEastNeighbour()->coordinates.x * GRIDSIZE + GRIDSIZE/2, board.grid[x][y]->getSouthEastNeighbour()->coordinates.y * GRIDSIZE, (board.grid[x][y]->southEast->built ? al_map_rgb(255, 0, 0) : al_map_rgb(0, 0, 255)), 1);
						al_draw_line(board.grid[x][y]->coordinates.x * GRIDSIZE, board.grid[x][y]->coordinates.y * GRIDSIZE, board.grid[x][y]->getSouthWestNeighbour()->coordinates.x * GRIDSIZE + GRIDSIZE/2, board.grid[x][y]->getSouthWestNeighbour()->coordinates.y * GRIDSIZE, (board.grid[x][y]->southWest->built ? al_map_rgb(255, 0, 0) : al_map_rgb(0, 0, 255)), 1);
					}*/
				}
				board.grid[x][y]->paintThySelf(GRIDSIZE);
				/*
				if (y % 2 == 1)
					al_draw_filled_circle(x * GRIDSIZE + GRIDSIZE / 2, y * GRIDSIZE, 4,(board.grid[x][y]->constructed == EMPTY ? al_map_rgb(0, 128, 128) : al_map_rgb(128, 128, 0)));
				else
					al_draw_filled_circle(x * GRIDSIZE, y * GRIDSIZE, 4, (board.grid[x][y]->constructed == EMPTY ? al_map_rgb(0, 128, 128) : al_map_rgb(128, 128, 0)));
				*/
			}
		}
		
		/*
		if (key_ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			al_draw_filled_circle(500,400, 2, al_map_rgb(128, 128, 0));
		}*/
		if (key_ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			break;
		
		if (key_ev.keyboard.keycode == ALLEGRO_KEY_A && !tillsvidare) {
			tillsvidare = true;
			for (int x = 0; x < XSIZE; ++x)
				for (int y = 0; y < YSIZE; ++y)
					fprintf(stderr, "%i %i %i %i\n", x, y, board.grid[x][y]->visited, board.grid[x][y]->previous);
		}
		if (key_ev.keyboard.keycode == ALLEGRO_KEY_S)
			tillsvidare = false;

		if (key_ev.keyboard.keycode == ALLEGRO_KEY_W)
			pathType = FREE_PATH;

		if (key_ev.keyboard.keycode == ALLEGRO_KEY_Q)
			pathType = ROAD_PATH;

		if (disp_ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
			break;
		if (mouse_ev.type == ALLEGRO_EVENT_MOUSE_AXES || mouse_ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
			Point2D temp = board.calculate_closest_coordinate(mouse_ev.mouse.x, mouse_ev.mouse.y,GRIDSIZE);
			mouse_x = temp.x;
			mouse_y = temp.y;
		}

		if (key_ev.keyboard.keycode == ALLEGRO_KEY_R) {
			for (int x = 0; x < XSIZE; ++x)
				for (int y = 0; y < YSIZE; ++y) {
					board.grid[x][y]->visited = false;
					board.grid[x][y]->previous = NULL;
				}
		}

		al_draw_filled_circle(mouse_x, mouse_y, 4, al_map_rgb(255, 255, 255));

		if (mouse_ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && !test) {
			test = true;
			
			if (clicked.x == -1) {
				clicked = board.calculate_closest_node(mouse_ev.mouse.x,mouse_ev.mouse.y,GRIDSIZE);
				fprintf(stderr, "%i %i %i\n", clicked.x, clicked.y, board.grid[clicked.x][clicked.y]->visited);
			}
			else {
				
				Point2D temp = board.calculate_closest_node(mouse_ev.mouse.x, mouse_ev.mouse.y,GRIDSIZE);
				fprintf(stderr, "%i %i %i\n", temp.x, temp.y, board.grid[temp.x][temp.y]->visited);
				std::queue<Crossroad*> paths;
				board.findPath(board.grid[clicked.x][clicked.y], board.grid[temp.x][temp.y], &paths, pathType);

				if(!paths.empty())
					board.buildRoad(paths);
				clicked.x = -1;
			}
			
		}
		if (mouse_ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			test = false;
		}
		al_flip_display();
	}

	al_destroy_display(display);
	al_destroy_event_queue(display_event_queue);
	al_destroy_event_queue(keyboard_event_queue);

	return 0;
}

