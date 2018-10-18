#include "Order.h"



Order::Order(std::vector<Crossroad*> path, Item* item)
{
	this->path = path;
	this->item = item;
	nextLocation = path.size() - 2; //2 because -1 would give the node its currently at. It should not be possible to create paths with a single element, in which case this is safe.
}

void Order::moved() {
	removeStep();
}

void Order::removeStep()
{
	nextLocation--;
}

Crossroad* Order::nextCrossroad() {
	return path.at(nextLocation);
}


Order::~Order()
{
}
