#pragma once
#include "Crossroad.h"
#include <queue>

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
	

	void appendPath(std::queue<Crossroad*> appender); // Destructive, so we want a copy of the parameter appender.

private:
	std::vector<Crossroad*>* path;
};

