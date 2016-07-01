#include "Road.h"
#include "Item.h"
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include "Crossroad.h"

Road::Road() {
	built = FREE;
	progress = -1;
}

Crossroad *Road::opposite(Crossroad *base) {
	if (end == base)
		return start;
	else if (start == base)
		return end;
	else {
		fprintf(stderr, "error");
		return end;
		
	}

}

void Road::paintThySelf(int GRIDSIZE) {
	ALLEGRO_COLOR color;
	if (built)
		color = al_map_rgb(255, 0, 0);
	else
		color = al_map_rgb(0, 0, 255);
	if (start->coordinates.y % 2 == 1) {
		if(end->shifted)
			al_draw_line(start->coordinates.x * GRIDSIZE + GRIDSIZE / 2, start->coordinates.y * GRIDSIZE, end->coordinates.x * GRIDSIZE + GRIDSIZE / 2, end->coordinates.y * GRIDSIZE, color, 1);
		else
			al_draw_line(start->coordinates.x * GRIDSIZE + GRIDSIZE / 2, start->coordinates.y * GRIDSIZE, end->coordinates.x * GRIDSIZE, end->coordinates.y * GRIDSIZE, color, 1);
	}
	else {
		if (end->shifted)
			al_draw_line(start->coordinates.x * GRIDSIZE, start->coordinates.y * GRIDSIZE, end->coordinates.x * GRIDSIZE + GRIDSIZE / 2, end->coordinates.y * GRIDSIZE, color, 1);
		else
			al_draw_line(start->coordinates.x * GRIDSIZE, start->coordinates.y * GRIDSIZE, end->coordinates.x * GRIDSIZE, end->coordinates.y * GRIDSIZE, color, 1);
	}
}