#pragma once
#include "Item.h"
#include "BuildStatus.h"
#include "TraversibleEdge.h"

class Crossroad;

class Road : TraversibleEdge {
	friend class Crossroad;

public:
	Road();
	int progress;
	Item* carried;
	Crossroad *start;
	Crossroad *end;
	BuildStatus built;

	void paintThySelf(int GRIDSIZE);

private:
	Crossroad *opposite(Crossroad*);
};