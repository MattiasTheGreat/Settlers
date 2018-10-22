#pragma once
#include "Directions.h"
#include <stdio.h>

class TraversibleEdge;

class TraversibleNode
{
public:
	TraversibleNode();
	~TraversibleNode();

	TraversibleEdge* edges[6];
	bool pathing;
	bool visited;
	TraversibleNode* previous;

	virtual int transportationCost() = 0;

	TraversibleEdge *roadToNeighbour(TraversibleNode* neighbour);
	Directions getDirectionToNeighbour(TraversibleNode* neighbour);

	TraversibleNode* getNeighbour(Directions dir);
};

