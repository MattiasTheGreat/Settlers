#pragma once
#include "Directions.h"

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


	TraversibleNode* getNeighbour(Directions dir);
};

