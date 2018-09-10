#pragma once
#include "Item.h"
#include "BuildStatus.h"
#include "Point2D.h"
#include "Directions.h"
#include <stdlib.h>
#include "Road.h"
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "Carrier.h"
#include <queue>

class Road;

class Crossroad {
public:
	Crossroad();

	BuildStatus constructed; // Defines what is currently on this node, a building, road, flag etc.
	bool visited; // To be used for pathfinding.
	bool pathing; // Not sure..
	bool shifted; // Defines wether this node is to be shifted on paint or not. The shift is to make the grid seem hexagonal.
	Point2D coordinates; // Where the node is in the grid
	Road *roads[6]; // All roads connecting to this node.
	Carrier* carriers[6]; // All carriers connecting to this node. Might be null.
	Item *items[8]; // The items currently on the crossroad.
	bool space[8]; // Keeps track of which slots are taken by items. False if slot i is empty, true if there is an item.
	bool full; // True whenever there is no space left.
	std::queue<Carrier*>* leaveItemQueue; //Carriers have to stand in line to leave an item. Thisis done through requestLeaveItem().
	int numberOfItems;
	Crossroad *previous;
	double distance;
	
	bool operator<(Crossroad* lhs);

	void name();
	void printInfo();
	
	Crossroad *getNeighbour(Directions direction);
	Road *roadToNeighbour(Crossroad*);

	void requestLeaveItem(Carrier* carrier);
	Item *giveItem(int i);

	int transportationCost();
	double calculateDistance(Crossroad* other);

	bool build(BuildStatus status);
	void paintThySelf(int GRIDSIZE);

private:
	void getItem();
	
};