#pragma once
#include "Item.h"

enum roadType {FREE, DISABLED, ROADie};

class Crossroad;

class Road {
	friend class Crossroad;

public:
	Road();
	int progress;
	Item carried;
	Crossroad *start;
	Crossroad *end;
	bool built;

	void paintThySelf(int GRIDSIZE);

private:
	Crossroad *opposite(Crossroad*);
};