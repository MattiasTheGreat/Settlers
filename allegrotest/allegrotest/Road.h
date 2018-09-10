#pragma once
#include "Item.h"
#include "BuildStatus.h"

class Crossroad;

class Road {
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