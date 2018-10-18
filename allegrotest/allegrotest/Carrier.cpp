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
	waiting = false;
	carrying = false;
	progress = 0;
}


void Carrier::takeOrder() {
	//order = currentGoal->stockPile->giveOrder(pickUpNode); //TODO: uncomment when order passing works
	//if (order != NULL) {
		//carrying = true;
		if (currentGoal == start) {
			currentGoal = end;
		}
		else if (currentGoal == end) {
			currentGoal = start;
		}
	/*}
	else {
		waiting = true;
	}*/
}

void Carrier::tick() {
	if (!waiting) {
		moveTowardsGoal();
	}
}

void Carrier::arrive() {
	if (carrying) {
		currentGoal->stockPile->requestLeaveOrder(this);
	}
	else {
		takeOrder();
	}
}

void Carrier::moveTowardsGoal() {
	if (progress == FINISHED) {
		advanceGoal();
		if (currentLocation == currentGoal) {
			if (currentGoal == end)
				pickUpNode = path->getPreviousNode(end);
			if (currentGoal == start)
				pickUpNode = path->getNextNode(start);
			arrive();
		}
		progress = 0;
	}
	else {
		++progress;
	}
	fprintf(stderr, "Current Progress: %i \n", progress);
}

void Carrier::advanceGoal() {
	if (carrying){
		order->moved(); // Might eventually not want to do this every advanceGoal depending on what path the order is keeping.
	}
	

	if (currentGoal == end) { //The order between these two does funnily enough matter. In the case of a path with the same end and start node, getNextNode will return a next node, while getPreviousNode will return the start node.
		currentLocation = path->getNextNode(currentLocation);
	}
	else if (currentGoal == start) {
		currentLocation = path->getPreviousNode(currentLocation);
	}
	
	else {
		throw std::logic_error("Peddler tried to advance while goal being neither end nor start. Unlogical :P");
	}
}

Order* Carrier::giveOrder() {
	carrying = false;
	auto temp = order;
	takeOrder();
	return temp;
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

	if (currentLocation->coordinates.y % 2 == 1) {
		float x2 = currentLocation->coordinates.x * GRIDSIZE + GRIDSIZE / 2;
		if (nextNode->shifted) {
			float x1 = nextNode->coordinates.x * GRIDSIZE + GRIDSIZE / 2;

			al_draw_line(x1, y1, x2, y2, color, 1);
			al_draw_filled_circle(progress * x1 / FINISHED + (FINISHED - progress) * x2 / FINISHED, progress * y1 / FINISHED + (FINISHED - progress) * y2 / FINISHED, 4, color);
			
		}
		else {		
			float x1 = nextNode->coordinates.x * GRIDSIZE;

			al_draw_line(x1, y1, x2, y2, color, 1);
			al_draw_filled_circle(progress * x1 / FINISHED + (FINISHED - progress) * x2 / FINISHED, progress * y1 / FINISHED + (FINISHED - progress) * y2 / FINISHED, 4, color);
		}
			
	}
	else {
		float x2 = currentLocation->coordinates.x * GRIDSIZE;
		if (nextNode->shifted) {
			float x1 = nextNode->coordinates.x * GRIDSIZE + GRIDSIZE / 2;
			
			al_draw_line(x1, y1, x2, y2, color, 1);
			al_draw_filled_circle(progress * x1 / FINISHED + (FINISHED - progress) * x2 / FINISHED, progress * y1 / FINISHED + (FINISHED - progress) * y2 / FINISHED, 4, color);
		}
			
		else {		
			float x1 = nextNode->coordinates.x * GRIDSIZE;

			al_draw_line(x1, y1, x2, y2, color, 1);
			al_draw_filled_circle(progress * x1 / FINISHED + (FINISHED - progress) * x2 / FINISHED, progress * y1 / FINISHED + (FINISHED - progress) * y2 / FINISHED, 4, color);
		}
			
	}
}

