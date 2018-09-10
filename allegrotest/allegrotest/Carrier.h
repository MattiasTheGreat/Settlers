#pragma once
#include "Item.h";


class Crossroad;
class Path;

class Carrier
{
public:
	Carrier(Path* path);

	Item* item;
	Crossroad* start;
	Crossroad* end;
	Crossroad* currentGoal;
	Crossroad* currentLocation;
	Path* path;
	int itemNumberToGet;
	bool waiting;
	bool carrying;

	void tick();
	void moveTowardsGoal();
	void arrive();
	void takeItem();
	Item* giveItem();
};

