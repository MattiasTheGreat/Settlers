#include "Order.h"



Order::Order(Path<StockPile*>* path, Item* item)
{
	this->path = path;
	this->item = item;
	nextLocation = path->length() - 2; //2 because -1 would give the node its currently at. It should not be possible to create paths with a single element, in which case this is safe.
}

void Order::moved() {
	removeStep();
}

void Order::removeStep()
{
	nextLocation--;
}

StockPile* Order::nextStockPile() {
	if (nextLocation >= 0) {
		return path->getNode(nextLocation);
	}
	else {
		return nullptr;
	}
}


Order::~Order()
{
}
