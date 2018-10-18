#pragma once
#include "Item.h"

class Crossroad;

class Order
{
public:
	Order(std::vector<Crossroad*> path, Item* item);
	~Order();

	Item* item;
	std::vector<Crossroad*> path; // The path that the item should be moved along.
	int nextLocation; // Points to the index in the path member where item currently is.

	void moved(); // To be called when a new crossroad receives the object.
	void removeStep();
	Crossroad* nextCrossroad();

};

