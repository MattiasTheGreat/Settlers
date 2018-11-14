#pragma once
#include "Item.h";
#include "TraversibleEdge.h"
#include "Directions.h"
#include "StockPile.h"

class Crossroad;
template <class T> class Path;
class Order;

class Carrier 
{
public:
	Carrier(Path<Crossroad*>* path);

	

	StockPile* getOpposite(StockPile* node);

	void tick();
	void moveTowardsGoal();
	void advanceGoal();
	void arrive();
	void takeOrder();
	Order* giveOrder(); // This function is never to be called from within the class. It is called by the crossroad when it is ready to take an item.
	void paintThySelf(int GRIDSIZE);
	void goToLocation(Crossroad* location);

	void callForPickUp(StockPile* stockPile);


	

private:
	Order* order;
	Crossroad* start;
	Crossroad* end;
	Crossroad* currentGoal;
	Crossroad* currentLocation;
	Crossroad* waitLocation;
	Crossroad* nextLocation; // Keeping track of the next location to move to allows us to display movement properly.
	Path<Crossroad*>* path;
	Travel_Direction direction;

	bool idle; // When we have nothing to do and have reached the waitingNode this is true.
	int length; // Meant to be used for A* pathfinding.
	bool waiting; // This is true when we have nothing to do.
	bool carrying;
	int progress;
	const int FINISHED = 30;
	bool returning; // In case the carrier should turn around in the middle of two nodes.
	int itemsAtStart;
	int itemsAtEnd;
};

