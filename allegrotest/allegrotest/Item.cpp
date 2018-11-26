#include "Item.h"



Item::Item(ALLEGRO_COLOR color){
	this->color = color;
	type = NULLITEM;
}

void Item::paintThyself(Point2D coordinates) {
	al_draw_filled_circle(coordinates.x, coordinates.y, 4, color);
}