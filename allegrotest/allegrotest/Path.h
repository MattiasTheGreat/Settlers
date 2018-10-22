#pragma once
#include "Crossroad.h"
#include <queue>
#include <stack>
#include "Directions.h"


template <class T>
class Path
{
public:
	Path();
	~Path();

	T getNode(int index);
	T getStart();
	T getEnd();
	T getNextNode(T node);
	T getPreviousNode(T node);
	int length();
	

	void addToPath(std::queue<T> appender, bool continueBuilding); // Destructive, so we want a copy of the parameter appender.
	Travel_Direction directionToCrossroad(T current, T goal);
	T middleNode();

private:
	std::vector<T>* path;
};


#include "Path.cpp"
