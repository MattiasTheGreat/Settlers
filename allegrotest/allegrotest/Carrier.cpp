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
	state = WAITING;

	waitLocation = path->middleNode();
	currentLocation = start;
	currentGoal = waitLocation;
	carrying = false;
	progress = 0;
	returning = false;
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
	
	turnToOtherGoal();
}

void Carrier::turnToOtherGoal() {
	Crossroad* next = waitLocation;
	state = WALKING;
	bool startWaiting = false;
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
			startWaiting = true;
		}
	}
	goToLocation(next);
	if (startWaiting) {
		state = WAITING;
	}
}

void Carrier::tick() {
	if (state != IDLE && state != DROPPING_OFF_ITEM) {
		moveTowardsGoal();
	}
}

void Carrier::arrive() {
	if (carrying) {
		if (pickingUpItemAt(currentGoal)) {
			order = currentGoal->stockPile->exchangeOrder(this, order);
			turnToOtherGoal();
		}
		else {
			state = DROPPING_OFF_ITEM;
			currentGoal->stockPile->requestLeaveOrder(this);
		}
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
		else {
			currentLocation = path->getPreviousNode(currentLocation);
		}
		nextLocation = path->getPreviousNode(currentLocation);
	}
	else if (direction == Travel_Direction::STAY) {

	}
	else
		state = IDLE;
	
	returning = false;

	if (state == WAITING && currentLocation == waitLocation) {
		state = IDLE;
	}
	else if (currentLocation == currentGoal) {
		arrive();
	}
}

Order* Carrier::dropOffOrder() { //TODO (Bug): I only suspect it is here, but somewhere we need to check if we can exchange an item when having been in the waiting to leave item queue. Could also do it in requestPickUp.
	carrying = false;
	auto temp = order;
	order = nullptr;
	turnToOtherGoal();
	return temp;
}

void Carrier::goToLocation(Crossroad* location){ // TODO: Consider if we want to set currentGoal to waitLocation instead of this. Probably doesn't work in all cases though, so might be an obsolete comment
	auto newDirection = path->directionToCrossroad(currentLocation, location);
	if (newDirection == Travel_Direction::UNREACHABLE) {
		state = IDLE;
	}
	else {
		currentGoal = location;
		if (newDirection == Travel_Direction::STAY) {
			
			returning = true;
		}
		else {
			if (state == WAITING) {
				if (direction != newDirection) {
					returning = true;
				}
			}
			else if (newDirection == Travel_Direction::ENDWARD) {
				nextLocation = path->getNextNode(currentLocation);
			}
			else if (newDirection== Travel_Direction::STARTWARD) {
				nextLocation = path->getPreviousNode(currentLocation);
			}
		}
		state = WALKING;
	}
	direction = newDirection;
}

void Carrier::callForPickUp(StockPile* stockPile) {
	bool nothingToDo = (state == WAITING || state == IDLE);
	
	if(nothingToDo)
		goToLocation(stockPile->location);

	if (stockPile == start->stockPile) {
		++itemsAtStart;
	}
	else if(stockPile == end->stockPile){
		++itemsAtEnd;
	}
}

bool Carrier::pickingUpItemAt(Crossroad* crossroad) {
	if (crossroad == end) {
		return itemsAtEnd > 0;
	}
	else if(crossroad == start) {
		return itemsAtStart > 0;
	}
	return false;
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

	Point2D temp{ x,y - 4 };
	if(carrying)
		order->item->paintThyself(temp);
}

