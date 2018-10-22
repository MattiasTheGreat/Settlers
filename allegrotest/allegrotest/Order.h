#pragma once
#include "Item.h"
#include "Path.h"

class Crossroad;

class Order
{
public:
	Order(Path<StockPile*>* path, Item* item);
	~Order();

	Item* item;
	Path<StockPile*>* path; // The path that the item should be moved along.
	int nextLocation; // Points to the index in the path member where item currently is.

	void moved(); // To be called when a new crossroad receives the object.
	void removeStep();
	StockPile* nextStockPile();

};

