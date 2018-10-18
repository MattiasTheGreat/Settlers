#pragma once

class TraversibleNode;

class TraversibleEdge
{
public:
	TraversibleEdge();
	~TraversibleEdge();

	TraversibleNode* start;
	TraversibleNode* end;

	TraversibleNode* getOpposite(TraversibleNode* node);

};

