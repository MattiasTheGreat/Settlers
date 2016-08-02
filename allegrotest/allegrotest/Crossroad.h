#pragma once
#include "Item.h"
#include "BuildStatus.h"
#include "Point2D.h"

class Road;

class Crossroad {
public:
	Crossroad();

	BuildStatus constructed;
	bool visited;
	bool pathing;
	bool shifted;
	Point2D coordinates;
	Road *northEast;
	Road *northWest;
	Road *east;
	Road *southEast;
	Road *southWest;
	Road *west;
	Item *items[8];
	bool space[8];
	int numberOfItems;
	Crossroad *previous;
	double distance;
	
	bool operator<(Crossroad* lhs);

	void name();
	void printInfo();

	Crossroad *getSouthEastNeighbour();
	Crossroad *getSouthWestNeighbour();
	Crossroad *getWestNeighbour();
	Crossroad *getEastNeighbour();
	Crossroad *getNorthEastNeighbour();
	Crossroad *getNorthWestNeighbour();

	Road *roadToNeighbour(Crossroad*);

	int transportationCost();
	double calculateDistance(Crossroad* other);

	bool build(BuildStatus status);
	void paintThySelf(int GRIDSIZE);

	
};