#pragma once
#include "Item.h";
#include "TraversibleEdge.h"

class Crossroad;
class Path;
class Order;

class Carrier : TraversibleEdge
{
public:
	Carrier(Path* path);

	Order* order;
	Crossroad* start;
	Crossroad* end;
	Crossroad* currentGoal;
	Crossroad* currentLocation;
	Crossroad* pickUpNode;
	Path* path;

	int length; // Meant to be used for A* pathfinding.
	bool waiting;
	bool carrying;
	int progress;
	const int FINISHED = 30;

	void tick();
	void moveTowardsGoal();
	void advanceGoal();
	void arrive();
	void takeOrder();
	Order* giveOrder(); // This function is never to be called from within the class. It is called by the crossroad when it is ready to take an item.
	void paintThySelf(int GRIDSIZE);
};

