#include "Crossroad.h"
#include <stdlib.h>
#include "Road.h"
#include <stdio.h>
#include <allegro5/allegro_primitives.h>

Crossroad::Crossroad() {
	
	for (int i = 0; i < 8; i++) {
		/*item *a;
		a = (item*) malloc(sizeof(item));
		a->type = itemType::null;
		items[i] = a;*/
		items[i] = nullptr;
		space[i] = false;
	}
	constructed = FREE;
	visited = false;
	numberOfItems = 2;
}

Crossroad* Crossroad::getEastNeighbour() {
	return this->east->opposite(this);
}

Crossroad* Crossroad::getWestNeighbour() {
	return this->west->opposite(this);
}

Crossroad* Crossroad::getSouthEastNeighbour() {
	return this->southEast->opposite(this);
}

Crossroad* Crossroad::getSouthWestNeighbour() {
	return this->southWest->opposite(this);
}

Crossroad* Crossroad::getNorthEastNeighbour() {
	return this->northEast->opposite(this);
}

Crossroad* Crossroad::getNorthWestNeighbour() {
	return this->northWest->opposite(this);
}

Road* Crossroad::roadToNeighbour(Crossroad* neighbour) {
	if (neighbour == this->getEastNeighbour())
		return this->east;
	if (neighbour == this->getWestNeighbour())
		return this->west;
	if (neighbour == this->getSouthEastNeighbour())
		return this->southEast;
	if (neighbour == this->getNorthEastNeighbour())
		return this->northEast;
	if (neighbour == this->getSouthWestNeighbour())
		return this->southWest;
	if (neighbour == this->getNorthWestNeighbour())
		return this->northWest;
	return NULL;
}

void Crossroad::name () {
	fprintf(stderr, "Pos: (%i, %i) \n", this->coordinates.x, this->coordinates.y);
}

void Crossroad::paintThySelf( int GRIDSIZE) {
	ALLEGRO_COLOR color;
	if (constructed == FREE)
		color = al_map_rgb(0, 128, 128);
	else if (constructed == BUILDING)
		color = al_map_rgb(128, 128, 0);
	else if (constructed == FLAG)
		color = al_map_rgb(255, 0, 0);
	else
		color = al_map_rgb(128, 0, 128);
	if (coordinates.y % 2 == 1)
		al_draw_filled_circle(coordinates.x * GRIDSIZE + GRIDSIZE / 2, coordinates.y * GRIDSIZE, 4, color);
	else
		al_draw_filled_circle(coordinates.x * GRIDSIZE, coordinates.y * GRIDSIZE, 4, color);
}

int Crossroad::transportationCost() {
	return numberOfItems;
}

bool Crossroad::build(BuildStatus status) {
	switch (status) {
		case ROAD:
			if (constructed == FREE) {
				constructed = ROAD;
				return true;
			}
			break;
		case BUILDING:
			constructed = BUILDING;
			break;
		case FLAG:
			if (getEastNeighbour()->constructed != FLAG &&
				getWestNeighbour()->constructed != FLAG &&
				getSouthEastNeighbour()->constructed != FLAG &&
				getSouthWestNeighbour()->constructed != FLAG &&
				getNorthEastNeighbour()->constructed != FLAG &&
				getNorthWestNeighbour()->constructed != FLAG &&
				(constructed == FREE || constructed == FLAG || constructed == ROAD)) 
			{
				constructed = FLAG;
				return true;
			}
			else
				return false;
			break;
	}
	return false;
}