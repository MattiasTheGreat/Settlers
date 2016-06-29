#include "Crossroad.h"
#include <stdlib.h>
#include "Road.h"
#include <stdio.h>


Crossroad::Crossroad() {
	
	for (int i = 0; i < 8; i++) {
		/*item *a;
		a = (item*) malloc(sizeof(item));
		a->type = itemType::null;
		items[i] = a;*/
		items[i] = nullptr;

	}
	visited = false;
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