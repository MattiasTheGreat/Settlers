#pragma once
#include "Item.h"
#include "BuildStatus.h"
#include "Point2D.h"
#include "Directions.h"
#include <stdlib.h>
#include "Road.h"
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include "Carrier.h"
#include <queue>
#include "Order.h"
#include "TraversibleNode.h"

class Road;
class StockPile;



class Crossroad  {
public:
	Crossroad();

	BuildStatus constructed; // Defines what is currently on this node, a building, road, flag etc.
	bool visited; // To be used for pathfinding.
	bool pathing; // Not sure..
	bool shifted; // Defines wether this node is to be shifted on paint or not. The shift is to make the grid seem hexagonal.
	Point2D coordinates; // Where the node is in the grid
	Road *roads[6]; // All roads connecting to this node.
	Carrier* carrier;

	Crossroad *previous;
	double distance;
	StockPile* stockPile;
	
	bool operator<(Crossroad* lhs);

	void name();
	void printInfo();
	
	Crossroad *getNeighbour(Directions direction);
	Road *roadToNeighbour(Crossroad* road);
	Directions getDirectionToNeighbour(Crossroad* neighbour);

	int transportationCost();
	int estimateDistance(Crossroad* other);

	bool build(BuildStatus status);
	void paintThySelf(int GRIDSIZE);

	void builtRoad(Directions dir);
	void builtRoad(Carrier* carrier);

};