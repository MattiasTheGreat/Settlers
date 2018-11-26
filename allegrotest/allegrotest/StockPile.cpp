#include "StockPile.h"
#include "Order.h"
#include "StockPile.h"



StockPile::StockPile(Crossroad* crossroad){
	for (int i = 0; i < 8; i++) {
		orders[i] = nullptr;
		space[i] = false;
		if (i < 6) {
			carriers[i] = nullptr;
		}
	}
	factory = false;
	location = crossroad;
	full = false;
	numberOfItems = 0;
	leaveItemQueue = new std::queue<Carrier*>;	
	visited = false;
	pathing = false;
}


StockPile::~StockPile(){
	delete leaveItemQueue;
}

int StockPile::transportationCost() {
	return 1;
}

bool StockPile::requestLeaveOrder(Carrier* carrier) {
	leaveItemQueue->push(carrier);
	fprintf(stderr, "waiting carriers: %i\n", leaveItemQueue->size());
	if (!full) {
		takeItem();
	}
	
	return true;
}

Order* StockPile::exchangeOrder(Carrier* carrier, Order* order) {
	order->moved();
	Order* oldOrder = nullptr;
	StockPile* newOrderNeighbor = carrier->getOpposite(this);
	for (int i = 0; i < 8; ++i) {
		StockPile* oldOrderNext = (space[i] ? orders[i]->nextStockPile() : nullptr);
		if (space[i] && oldOrderNext == newOrderNeighbor) {
			oldOrder = orders[i];
			if (order->nextStockPile() != nullptr) {
				orders[i] = order;
				((Carrier*)roadToNeighbour(order->nextStockPile()))->callForPickUp(this); //This is wrong
			}
			else {
				space[i] = false;
				giveFactoryOrder(order);
			}
			break;
		}
	}
	return oldOrder;
}

Order *StockPile::giveOrder(Carrier* carrier) {
	Order* temp = NULL;
	for (int i = 0; i < 8; ++i) {
		auto neighbor = carrier->getOpposite(this);
		auto next = space[i] ? orders[i]->nextStockPile() : nullptr;
		if (space[i] && next == neighbor) {
			temp = orders[i];
			space[i] = false;
			orders[i] = NULL;
			numberOfItems--;

			if (leaveItemQueue->size() != 0) { // This should take care of any queue that might have built up while we were full.
				takeItem();
			}else
				full = false;
			break;
		}
	}

	return temp;
}

void StockPile::takeItem() {
	Carrier* carrier = leaveItemQueue->front();
	leaveItemQueue->pop();
	fprintf(stderr, "After popping, waiting carriers: %i\n", leaveItemQueue->size());
	Order* order = carrier->dropOffOrder(); 
	processOrder(order);
	
}

void StockPile::processOrder(Order* order) {
	order->moved();
	StockPile* nextStockPile = order->nextStockPile();
	if (nextStockPile != nullptr) {
		((Carrier*)roadToNeighbour(nextStockPile))->callForPickUp(this);
		for (int i = 0; i < 8; ++i) {
			if (!space[i]) {
				orders[i] = order;
				space[i] = true;
				numberOfItems++;
				if (numberOfItems == 8) {
					full = true;
				}
				break;
			}
		}
	}
	else {
		giveFactoryOrder(order);
	}

}

void StockPile::giveFactoryOrder(Order* order) {
	if (!factory) {
		fprintf(stderr, "End goal of order was not factory\n");
	}
	delete order;
}

void StockPile::addCarrier(Directions dir, Carrier* carrier) {
	carriers[dir] = carrier;
}

void StockPile::createOrder(Order* order) {
	auto nextPile = order->nextStockPile();
	auto carrier = roadToNeighbour(nextPile);
	carrier->callForPickUp(this);
	for (int i = 0; i < 8; ++i) {
		if (!space[i]) {
			orders[i] = order;
			space[i] = true;
			numberOfItems++;
			if (numberOfItems == 8) {
				full = true;
			}
			break;
		}
	}
}

StockPile* StockPile::getNeighbour(Directions direction) {
	return this->carriers[direction]->getOpposite(this);
}

Carrier* StockPile::roadToNeighbour(StockPile* neighbour) {
	if(carriers[Directions::EAST] != nullptr)
		if (neighbour == this->getNeighbour(Directions::EAST))
			return this->carriers[Directions::EAST];

	if (carriers[Directions::WEST] != nullptr)
		if (neighbour == this->getNeighbour(Directions::WEST))
			return this->carriers[Directions::WEST];

	if (carriers[Directions::SOUTH_EAST] != nullptr)
		if (neighbour == this->getNeighbour(Directions::SOUTH_EAST))
			return this->carriers[Directions::SOUTH_EAST];

	if (carriers[Directions::NORTH_EAST] != nullptr)
		if (neighbour == this->getNeighbour(Directions::NORTH_EAST))
			return this->carriers[Directions::NORTH_EAST];

	if (carriers[Directions::SOUTH_WEST] != nullptr)
		if (neighbour == this->getNeighbour(Directions::SOUTH_WEST))
			return this->carriers[Directions::SOUTH_WEST];

	if (carriers[Directions::NORTH_WEST] != nullptr)
		if (neighbour == this->getNeighbour(Directions::NORTH_WEST))
			return this->carriers[Directions::NORTH_WEST];

	return NULL;
}

Directions StockPile::getDirectionToNeighbour(StockPile* neighbour) {
	if (neighbour == this->getNeighbour(Directions::EAST))
		return Directions::EAST;

	if (neighbour == this->getNeighbour(Directions::WEST))
		return Directions::WEST;

	if (neighbour == this->getNeighbour(Directions::SOUTH_EAST))
		return Directions::SOUTH_EAST;

	if (neighbour == this->getNeighbour(Directions::NORTH_EAST))
		return Directions::NORTH_EAST;

	if (neighbour == this->getNeighbour(Directions::SOUTH_WEST))
		return Directions::SOUTH_WEST;

	if (neighbour == this->getNeighbour(Directions::NORTH_WEST))
		return Directions::NORTH_WEST;

	fprintf(stderr, "Someone asked for directions to a non-neighbour!\n");
	return EAST; // arbritrary. Should be fine, hopefully never gets here.
}

void StockPile::paintThySelf(Point2D location) {
	int counter = 1;
	for (int i = 0; i < 8; ++i) {
		if (space[i]) {
			Point2D newLocation{location.x, location.y - 4 * counter};
			orders[i]->item->paintThyself(newLocation);
			counter++;
		}
	}
}