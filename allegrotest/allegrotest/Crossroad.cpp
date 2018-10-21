#include "Crossroad.h"
#include "StockPile.h"

Crossroad::Crossroad() {
	constructed = FREE;
	visited = false;
	distance = 0;
	stockPile = new StockPile(this); //TODO: This might be a bit unnecesary overhead, but works. Might wanna consider when having one is necesary
}

Crossroad* Crossroad::getNeighbour(Directions direction) {
	return this->roads[direction]->opposite(this);
}

Road* Crossroad::roadToNeighbour(Crossroad* neighbour) {
	if (neighbour == this->getNeighbour(Directions::EAST))
		return this->roads[Directions::EAST];

	if (neighbour == this->getNeighbour(Directions::WEST))
		return this->roads[Directions::WEST];

	if (neighbour == this->getNeighbour(Directions::SOUTH_EAST))
		return this->roads[Directions::SOUTH_EAST];

	if (neighbour == this->getNeighbour(Directions::NORTH_EAST))
		return this->roads[Directions::NORTH_EAST];

	if (neighbour == this->getNeighbour(Directions::SOUTH_WEST))
		return this->roads[Directions::SOUTH_WEST];

	if (neighbour == this->getNeighbour(Directions::NORTH_WEST))
		return this->roads[Directions::NORTH_WEST];

	return NULL;
}

Directions Crossroad::getDirectionToNeighbour(Crossroad* neighbour) {
	if (neighbour == this->getNeighbour(Directions::EAST))
		return Directions::EAST;

	if (neighbour == this->getNeighbour(Directions::WEST))
		return Directions::WEST;

	if (neighbour == this->getNeighbour(Directions::SOUTH_EAST))
		return Directions::SOUTH_EAST;

	if (neighbour == this->getNeighbour(Directions::NORTH_EAST))
		return Directions::NORTH_EAST;

	if (neighbour == this->getNeighbour(Directions::SOUTH_WEST))
		return Directions::SOUTH_WEST;

	if (neighbour == this->getNeighbour(Directions::NORTH_WEST))
		return Directions::NORTH_WEST;

	fprintf(stderr, "Someone asked for directions to a non-neighbour!\n");
	return EAST; // arbritrary. Should be fine, hopefully never gets here.
}



void Crossroad::name () {
	fprintf(stderr, "Pos: (%i, %i) \n", this->coordinates.x, this->coordinates.y);
}

void Crossroad::paintThySelf(int GRIDSIZE) {
	ALLEGRO_COLOR color;
	if (pathing)
		color = al_map_rgb(0, 0, 0);
	else if (constructed == FREE)
		color = al_map_rgb(0, 128, 128);
	else if (constructed == BUILDING)
		color = al_map_rgb(128, 128, 0);
	else if (constructed == FLAG)
		color = al_map_rgb(0, 255, 0);
	else
		color = al_map_rgb(255,0,0);
	if (coordinates.y % 2 == 1)
		al_draw_filled_circle(coordinates.x * GRIDSIZE + GRIDSIZE / 2, coordinates.y * GRIDSIZE, 4, color);
	else
		al_draw_filled_circle(coordinates.x * GRIDSIZE, coordinates.y * GRIDSIZE, 4, color);

	for (int i = 0; i < 8; ++i) {

	}
}


double Crossroad::calculateDistance(Crossroad* other) {
	return sqrt(pow(this->coordinates.x - other->coordinates.x, 2) + pow(this->coordinates.y - other->coordinates.y, 2));
}


int Crossroad::transportationCost() {
	if (constructed == FLAG) {
		return stockPile->numberOfItems + 1;
	}
	else
		return 0;
}

bool Crossroad::build(BuildStatus status) {
	switch (status) {
		case FREE:
			constructed = FREE;
			break;
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
			if (getNeighbour(Directions::EAST)->constructed != FLAG &&
				getNeighbour(Directions::WEST)->constructed != FLAG &&
				getNeighbour(Directions::SOUTH_EAST)->constructed != FLAG &&
				getNeighbour(Directions::SOUTH_WEST)->constructed != FLAG &&
				getNeighbour(Directions::NORTH_EAST)->constructed != FLAG &&
				getNeighbour(Directions::NORTH_WEST)->constructed != FLAG &&
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

void Crossroad::printInfo() {
	fprintf(stderr, "%i %i ", this->coordinates.x, this->coordinates.y);
	if (constructed == FLAG)
		fprintf(stderr, "FLAG ");
	if (constructed == ROAD)
		fprintf(stderr, "ROAD ");
	if (constructed == FREE)
		fprintf(stderr, "FREE ");
	if (constructed == BUILDING)
		fprintf(stderr, "BUILDING ");


	fprintf(stderr, "\n");
}

bool Crossroad::operator<(Crossroad* lhs) {
	fprintf(stderr, "%i %i\n", this->distance, lhs->distance);
	return this->distance >= lhs->distance;
}

void Crossroad::builtRoad(Directions dir) {
	//stockPile->addCarrier(dir);
}

void Crossroad::builtRoad(Carrier* carrier) {
	carrier = carrier;
}