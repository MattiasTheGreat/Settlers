#include "Carrier.h"
#include "Crossroad.h"
#include "Path.h"
#include "StockPile.h"
#include <stdexcpt.h>



Carrier::Carrier(Path<Crossroad*>* path)
{
	this->path = path;
	start = path->getStart();
	end = path->getEnd();
	nextLocation = path->getNextNode(start);
	order = NULL;
	length = path->length();

	waitLocation = path->middleNode();
	currentLocation = start;
	currentGoal = waitLocation;
	waiting = true;
	carrying = false;
	progress = 0;
	returning = false;
	idle = false;
	direction = Travel_Direction::ENDWARD;
	itemsAtEnd = 0;
	itemsAtStart = 0;

	Crossroad* startNeighbour = path->getNextNode(start);
	Directions carrierIndex = start->getDirectionToNeighbour(startNeighbour);
	start->stockPile->addCarrier(carrierIndex, this);

	Crossroad* endNeighbour = path->getPreviousNode(end);
	carrierIndex = end->getDirectionToNeighbour(startNeighbour);
	end->stockPile->addCarrier(carrierIndex, this);
}


void Carrier::takeOrder() {
	order = currentGoal->stockPile->giveOrder(this); 
	Crossroad* next = waitLocation;
	bool temp = false;
	if (order != NULL) {
		carrying = true;
		if (currentGoal == start) {
			--itemsAtStart;
			next = end;
		}
		else if (currentGoal == end) {
			--itemsAtEnd;
			next = start;
		}
	}
	else {
		if (itemsAtEnd != 0) {
			next = end;
		}
		else if (itemsAtStart != 0) {
			next = start;
		}
		else {
			temp = true;
		}
	}
	goToLocation(next);
	if (temp) {
		waiting = true;
	}
}

void Carrier::tick() {
	if (!idle) {
		moveTowardsGoal();
	}
}

void Carrier::arrive() {
	if (carrying) {
		currentGoal->stockPile->requestLeaveOrder(this);
	}
	else { // If we're not carrying we don't need to wait until we've let go of the current order. 
		takeOrder();
	}
}

void Carrier::moveTowardsGoal() {
	if (progress == FINISHED || (progress == 0 && returning)) {
		advanceGoal();
		progress = 0;
	}
	else {
		if (returning) {
			--progress;
		}
		else {
			++progress;
		}
		
	}
}

void Carrier::advanceGoal() {

	if (direction == Travel_Direction::ENDWARD) {
		if (returning) {
			returning = false;
		}
		else {
			currentLocation = path->getNextNode(currentLocation);
		}
		nextLocation = path->getNextNode(currentLocation);
	}
	else if (direction == Travel_Direction::STARTWARD) {
		if (returning) {
			returning = false;
		}
		else{
			currentLocation = path->getPreviousNode(currentLocation);
		}
		nextLocation = path->getPreviousNode(currentLocation);
	}
	else if (direction == Travel_Direction::STAY) {
		
	}
	else
		idle = true;
	
	returning = false;

	if (waiting && currentLocation == waitLocation) {
		idle = true;
	}
	else if (currentLocation == currentGoal) {
	}
}

Order* Carrier::giveOrder() {
	carrying = false;
	auto temp = order;
	takeOrder();
	return temp;
}

void Carrier::goToLocation(Crossroad* location){ // TODO: Consider if we want to set currentGoal to waitLocation instead of this. Probably doesn't work in all cases though, so might be an obsolete comment
	auto temp = path->directionToCrossroad(currentLocation, location);
	if (temp == Travel_Direction::UNREACHABLE) {
		idle = true;
	}
	else {
		currentGoal = location;
		if (temp == Travel_Direction::STAY) {
			returning = true;
		}
		else {
			if (waiting) {
				if (direction != temp) {
					returning = true;
				}
			}
			else if (temp == Travel_Direction::ENDWARD) {
				nextLocation = path->getNextNode(currentLocation);
			}
			else if (temp== Travel_Direction::STARTWARD) {
				nextLocation = path->getPreviousNode(currentLocation);
			}
		}
	}
	direction = temp;
}

void Carrier::callForPickUp(StockPile* stockPile) {
	bool nothingToDo = waiting || idle;
	
	if(nothingToDo)
		goToLocation(stockPile->location);
	
	idle = false;
	waiting = false;

	if (stockPile == start->stockPile) {
		++itemsAtStart;
	}
	else if(stockPile == end->stockPile){
		++itemsAtEnd;

	}
}

StockPile* Carrier::getOpposite(StockPile* node) {
	if (node->location == start) {
		return end->stockPile;
	}
	else if (node->location == end) {
		return start->stockPile;
	}
	return nullptr;
}

void Carrier::paintThySelf(int GRIDSIZE) {
	ALLEGRO_COLOR color = al_map_rgb(125, 125, 255);
	if (carrying) {
		color = al_map_rgb(128, 0, 64);
	}
	
	float y1 = nextLocation->coordinates.y * GRIDSIZE;
	float y2 = currentLocation->coordinates.y * GRIDSIZE;
	float x1 = 0;
	float x2 = 0;
	float x = 0;
	float y = 0;

	if (currentLocation->shifted) {
		x2 = currentLocation->coordinates.x * GRIDSIZE + GRIDSIZE / 2;
		if (nextLocation->shifted) {
			x1 = nextLocation->coordinates.x * GRIDSIZE + GRIDSIZE / 2;
		}
		else {		
			x1 = nextLocation->coordinates.x * GRIDSIZE;
		}
			
	}
	else {
		x2 = currentLocation->coordinates.x * GRIDSIZE;
		if (nextLocation->shifted) {
			x1 = nextLocation->coordinates.x * GRIDSIZE + GRIDSIZE / 2;
		}		
		else {		
			x1 = nextLocation->coordinates.x * GRIDSIZE;
		}
			
	}
	x = progress * x1 / FINISHED + (FINISHED - progress) * x2 / FINISHED;
	y = progress * y1 / FINISHED + (FINISHED - progress) * y2 / FINISHED;
	al_draw_filled_circle(x, y, 4, color);
}

