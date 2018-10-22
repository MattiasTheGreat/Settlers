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
	factory = false;
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
		takeItem();		
	}
	return true;
}

Order *StockPile::giveOrder(Carrier* carrier) {
	Order* temp = NULL;
	for (int i = 0; i < 8; ++i) {
		if (space[i] && orders[i]->nextStockPile() == carrier->getOpposite(this)) {
			temp = orders[i];
			space[i] = false;
			orders[i] = NULL;
			numberOfItems--;

			if (leaveItemQueue->size() != 0) { // This should take care of any queue that might have built up while we were full.
				takeItem();
			}
			full = false;
			break;
		}
	}

	return temp;
}

void StockPile::takeItem() {
	Order* order = leaveItemQueue->front()->giveOrder();
	leaveItemQueue->pop();
	processOrder(order);
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

void StockPile::processOrder(Order* order) {
	order->moved();
	if (!factory) {
		//((Carrier*)roadToNeighbour(order->nextStockPile()))->callForPickUp(this);
	}
}

void StockPile::addCarrier(Directions dir, Carrier* carrier) {
	carriers[dir] = carrier;
}

void StockPile::createOrder(Order* order) {
	//((Carrier*)roadToNeighbour(order->nextStockPile()))->callForPickUp(this);
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

