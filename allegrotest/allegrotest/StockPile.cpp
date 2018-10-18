#include "StockPile.h"
#include "Order.h"
#include "Crossroad.h"



StockPile::StockPile(Crossroad* crossroad)
{
	for (int i = 0; i < 8; i++) {
		orders[i] = nullptr;
		space[i] = false;
		if (i < 6) {
			carriers[i] = nullptr;
		}
	}
	location = crossroad;
	full = false;
	numberOfItems = 0;
	leaveItemQueue = new std::queue<Carrier*>;
	
	
}


StockPile::~StockPile()
{
	delete leaveItemQueue;
}


bool StockPile::requestLeaveOrder(Carrier* carrier) {
	leaveItemQueue->push(carrier);
	if (!full) {
		getItem();		
	}
	return true;
}

Order *StockPile::giveOrder(Crossroad* pickUpNode) {
	Order* temp = NULL;
	for (int i = 0; i < 8; ++i) {
		if (space[i] & orders[i]->nextCrossroad() == pickUpNode) {
			temp = orders[i];
			space[i] = false;
			orders[i] = NULL;
			numberOfItems--;

			if (leaveItemQueue->size() != 0) { // This should take care of any line that might have built up while we were full.
				getItem();
			}
			full = false;
			break;
		}
	}

	return temp;
}

void StockPile::getItem() {
	Order* order = leaveItemQueue->front()->giveOrder();
	leaveItemQueue->pop();
	for (int i = 0; i < 8; ++i) {
		if (space[i]) {
			orders[i] = order;
			space[i] = true;
			numberOfItems++;
			if (numberOfItems == 8) {
				full = true;
			}
		}
	}
}

void StockPile::addCarrier(Directions dir, Carrier* carrier) {
	carriers[dir] = carrier;
}