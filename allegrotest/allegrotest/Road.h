#pragma once
#include "Item.h"

class Crossroad;

class Road {
public:
	Road();
	int progress;
	Item carried;
	Crossroad *start;
	Crossroad *end;
	bool visited;
	Crossroad *opposite(Crossroad*);
};