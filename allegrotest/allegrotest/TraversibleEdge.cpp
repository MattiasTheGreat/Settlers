#include "TraversibleEdge.h"



TraversibleEdge::TraversibleEdge()
{
}


TraversibleEdge::~TraversibleEdge()
{
}


TraversibleNode* TraversibleEdge::getOpposite(TraversibleNode* node) {
	if (node == start) {
		return end;
	}
	else if (node == end) {
		return start;
	}
	return nullptr;
}