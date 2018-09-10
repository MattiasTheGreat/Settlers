#include "Carrier.h"
#include "Crossroad.h"
#include "Path.h"



Carrier::Carrier(Path* path)
{
	this->path = path;
	this->item = NULL;
	itemNumberToGet = -1;
}


void Carrier::takeItem() {
	item = currentGoal->giveItem(itemNumberToGet);
	carrying = true;
}

void Carrier::tick() {
	if (!waiting) {
		moveTowardsGoal();
	}
}

void Carrier::arrive() {
	if (carrying) {
		currentGoal->requestLeaveItem(this);
	}
	else {

	}
}

void Carrier::moveTowardsGoal() {

}

Item* Carrier::giveItem() {
	return item;
}

