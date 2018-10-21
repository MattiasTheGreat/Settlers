#pragma once
#include "Crossroad.h"
#include <queue>
#include <stack>
#include "Directions.h"

class Path
{
public:
	Path();
	~Path();

	Crossroad* getNode(int index);
	Crossroad* getStart();
	Crossroad* getEnd();
	Crossroad* getNextNode(Crossroad* crossroad);
	Crossroad* getPreviousNode(Crossroad* crossroad);
	int length();
	

	void addToPath(std::queue<Crossroad*> appender, bool continueBuilding); // Destructive, so we want a copy of the parameter appender.
	Travel_Direction directionToCrossroad(Crossroad* current, Crossroad* goal);

private:
	std::vector<Crossroad*>* path;
};

