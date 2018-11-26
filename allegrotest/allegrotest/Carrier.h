#pragma once
#include "Item.h";
#include "TraversibleEdge.h"
#include "Directions.h"
#include "StockPile.h"

class Crossroad;
template <class T> class Path;
class Order;

enum CarrierState {	WAITING, // We have no tasks to do and are not carrying, but have yet to return to the waitLocation.
					DROPPING_OFF_ITEM, //We are waiting to drop off an item.
					IDLE, //We have nothing to do and are at the waitLocation.
					WALKING //We have stuff to do and are going somewhere.
};

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
	Order* dropOffOrder(); // This function is never to be called from within the class. It is called by the crossroad when it is ready to take an item.
	void paintThySelf(int GRIDSIZE);
	void goToLocation(Crossroad* location);

	void turnToOtherGoal();

	void callForPickUp(StockPile* stockPile);

	bool pickingUpItemAt(Crossroad* crossroad);
	

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

	CarrierState state;
	int length; // Meant to be used for A* pathfinding.
	bool carrying;
	int progress;
	const int FINISHED = 30;
	bool returning; // In case the carrier should turn around in the middle of two nodes.
	int itemsAtStart;
	int itemsAtEnd;
};

