#include "Item.h"



Item::Item(){
	type = NULLITEM;
}

void Item::paintThyself(int GRIDSIZE, Point2D coordinates) {
	ALLEGRO_COLOR color = al_map_rgb(0, 128, 128);
	if (coordinates.y % 2 == 1)
		al_draw_filled_circle(coordinates.x * GRIDSIZE + GRIDSIZE / 2, coordinates.y * GRIDSIZE, 4, color);
	else
		al_draw_filled_circle(coordinates.x * GRIDSIZE, coordinates.y * GRIDSIZE, 4, color);
}