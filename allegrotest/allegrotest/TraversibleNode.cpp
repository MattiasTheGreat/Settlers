#include "TraversibleNode.h"
#include "TraversibleEdge.h"


TraversibleNode::TraversibleNode()
{
}


TraversibleNode::~TraversibleNode()
{
}

TraversibleNode* TraversibleNode::getNeighbour(Directions dir) {
	return edges[dir]->getOpposite(this);
}

TraversibleEdge* TraversibleNode::roadToNeighbour(TraversibleNode* neighbour) {
	if (neighbour == this->getNeighbour(Directions::EAST))
		return this->edges[Directions::EAST];

	if (neighbour == this->getNeighbour(Directions::WEST))
		return this->edges[Directions::WEST];

	if (neighbour == this->getNeighbour(Directions::SOUTH_EAST))
		return this->edges[Directions::SOUTH_EAST];

	if (neighbour == this->getNeighbour(Directions::NORTH_EAST))
		return this->edges[Directions::NORTH_EAST];

	if (neighbour == this->getNeighbour(Directions::SOUTH_WEST))
		return this->edges[Directions::SOUTH_WEST];

	if (neighbour == this->getNeighbour(Directions::NORTH_WEST))
		return this->edges[Directions::NORTH_WEST];

	return nullptr;
}

Directions TraversibleNode::getDirectionToNeighbour(TraversibleNode* neighbour) {
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