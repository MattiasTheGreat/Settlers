#include "Crossroad.h"

Crossroad::Crossroad() {
	for (int i = 0; i < 8; i++) {
		items[i] = nullptr;
		space[i] = false;
	}
	full = false;
	constructed = FREE;
	visited = false;
	numberOfItems = 0;
	distance = 0;
	leaveItemQueue = new std::queue<Carrier*>;
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

void Crossroad::requestLeaveItem(Carrier* carrier) {
	leaveItemQueue->push(carrier);
	if (!full) {
		getItem();
	}
}

Item *Crossroad::giveItem(int i) {
	Item* temp = items[i];
	space[i] = false;
	items[i] = NULL;
	numberOfItems--;
	if (leaveItemQueue->size() != 0){ //This should be efficient enough. There should only be someone in the waiting line when the crossroad has been full.
		getItem();
	}
	full = false;
	return temp;
}

void Crossroad::getItem() {
	//Both these checks are more efficient if we do in the two cases where this function is called: when someone wants to leave an item, and when an item is taken from a full crossroad. But perhaps it might be safer to do it all here
	//if (!full) { 
		//if (leaveItemQueue->size() != 0) {
			Item* item = leaveItemQueue->front()->giveItem();
			leaveItemQueue->pop();
			for (int i = 0; i < 8; ++i) {
				if (space[i]) {
					items[i] = item;
					space[i] = true;
					item->received();
					numberOfItems++;
					if (numberOfItems == 8) {
						full = true;
					}
					//return true;
				}
			}
		//}
	//}
	//return false;
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
	return numberOfItems + 1;
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

