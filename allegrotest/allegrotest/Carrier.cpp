#include "Carrier.h"
#include "Crossroad.h"
#include "Path.h"
#include "StockPile.h"
#include <stdexcpt.h>



Carrier::Carrier(Path* path)
{
	this->path = path;
	this->start = path->getStart();
	this->end = path->getEnd();
	this->order = NULL;
	length = path->length();

	//TODO: This is temp initialization parameters, should be changed later probably
	currentLocation = start;
	currentGoal = end;
	waitLocation = end;
	waiting = true;
	carrying = false;
	progress = 1;
	returning = false;
	idle = false;
	direction = Travel_Direction::ENDWARD;
}


void Carrier::takeOrder() {
	//order = currentGoal->stockPile->giveOrder(pickUpNode); //TODO: uncomment when order passing works
	//if (order != NULL) {
		//carrying = true;
		if (currentGoal == start) {
			currentGoal = end;
			direction = Travel_Direction::ENDWARD;
		}
		else if (currentGoal == end) {
			currentGoal = start;
			direction = Travel_Direction::STARTWARD;
		}
	/*}
	else {
		waiting = true;
	}*/
}

void Carrier::tick() {
	if (!idle) {
		moveTowardsGoal();
	}
}

void Carrier::arrive() {
	if (waiting) {

	}
	else if (carrying) {
		currentGoal->stockPile->requestLeaveOrder(this);
	}
	else {
		takeOrder();
	}
}

void Carrier::moveTowardsGoal() {
	if (progress == FINISHED || progress == 0) {
		advanceGoal();
		if (currentLocation == currentGoal) {
			arrive();
		}
		progress = 1;
	}
	else {
		if (returning) {
			--progress;
		}
		else {
			++progress;
		}
		
	}
	fprintf(stderr, "Current Progress: %i \n", progress);
}

void Carrier::advanceGoal() {
	if (carrying){
		order->moved(); // Might eventually not want to do this every advanceGoal depending on what path the order is keeping (Crossroads or Carriers).
	}

	if (direction == Travel_Direction::ENDWARD) {
		currentLocation = path->getNextNode(currentLocation);
	}
	else if (direction == Travel_Direction::STARTWARD) {
		currentLocation = path->getPreviousNode(currentLocation);
	}
	else {
		idle = true;
	}
	if (waiting & currentLocation == waitLocation) {
		idle = true;
	}
}

Order* Carrier::giveOrder() {
	carrying = false;
	auto temp = order;
	takeOrder();
	return temp;
}

void Carrier::goToIdle(){ //TODO: Consider if we want to set currentGoal to waitLocation instead of this...
	direction = path->directionToCrossroad(currentLocation, waitLocation);
	if (direction = Travel_Direction::STAY) {
		returning = true;
	}
	if (direction = Travel_Direction::UNREACHABLE) {
		idle = true;
	}
}

void Carrier::paintThySelf(int GRIDSIZE) {
	ALLEGRO_COLOR color = al_map_rgb(125, 125, 255);
	Crossroad* nextNode = currentLocation; //This is ugly, but will probably be fixed if i ever make a direction variable to solve all the currentGoal == end || start if-else things

	if (currentGoal == end) {
		nextNode = path->getNextNode(currentLocation);
	}
	else if (currentGoal == start) {
		nextNode = path->getPreviousNode(currentLocation);
	}

	
	float y1 = nextNode->coordinates.y * GRIDSIZE;
	float y2 = currentLocation->coordinates.y * GRIDSIZE;
	float x1 = 0;
	float x2 = 0;
	float x = 0;
	float y = 0;

	if (currentLocation->shifted) {
		x2 = currentLocation->coordinates.x * GRIDSIZE + GRIDSIZE / 2;
		if (nextNode->shifted) {
			x1 = nextNode->coordinates.x * GRIDSIZE + GRIDSIZE / 2;
		}
		else {		
			x1 = nextNode->coordinates.x * GRIDSIZE;
		}
			
	}
	else {
		x2 = currentLocation->coordinates.x * GRIDSIZE;
		if (nextNode->shifted) {
			x1 = nextNode->coordinates.x * GRIDSIZE + GRIDSIZE / 2;
		}		
		else {		
			x1 = nextNode->coordinates.x * GRIDSIZE;
		}
			
	}
	x = progress * x1 / FINISHED + (FINISHED - progress) * x2 / FINISHED;
	y = progress * y1 / FINISHED + (FINISHED - progress) * y2 / FINISHED;
	al_draw_filled_circle(x, y, 4, color);
}

