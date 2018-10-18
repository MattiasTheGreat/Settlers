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