#pragma once
#include "Item.h";
#include "TraversibleEdge.h"
#include "Directions.h"

class Crossroad;
class Path;
class Order;

class Carrier : TraversibleEdge
{
public:
	Carrier(Path* path);

	


	void tick();
	void moveTowardsGoal();
	void advanceGoal();
	void arrive();
	void takeOrder();
	Order* giveOrder(); // This function is never to be called from within the class. It is called by the crossroad when it is ready to take an item.
	void paintThySelf(int GRIDSIZE);
	void goToIdle();
	

private:
	Order* order;
	Crossroad* start;
	Crossroad* end;
	Crossroad* currentGoal;
	Crossroad* currentLocation;
	Crossroad* waitLocation;
	Path* path;
	Travel_Direction direction;

	bool idle;
	int length; // Meant to be used for A* pathfinding.
	bool waiting;
	bool carrying;
	int progress;
	const int FINISHED = 30;
	bool returning; // In case the carrier should turn around in the middle of two nodes.
};

