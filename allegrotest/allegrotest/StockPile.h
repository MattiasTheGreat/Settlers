#pragma once
#include <queue>
#include "TraversibleNode.h"


class Carrier;
class Order;
class Crossroad;
enum Directions;


class StockPile 
{
public:
	StockPile(Crossroad* crossroad);
	~StockPile();

	Crossroad* location;
	Carrier* carriers[6]; // All carriers connecting to this node. Might be null.
	Order *orders[8]; // The items currently on the crossroad.
	bool space[8]; // Keeps track of which slots are taken by items. False if slot i is empty, true if there is an item.
	bool full; // True whenever there is no space left.
	std::queue<Carrier*>* leaveItemQueue; //Carriers have to stand in line to leave an item. This is done through requestLeaveItem().
	int numberOfItems;
	StockPile* previous;
	bool visited;
	bool pathing;

	bool requestLeaveOrder(Carrier* carrier); // The idea behind this being bool is if factories will inherit and if so enable them to reply false in cases where the wrong item type is being delivered.
	Order *giveOrder(Carrier* pickUpNode);
	void addCarrier(Directions dir, Carrier* carrier);
	bool factory;

	int transportationCost();

	StockPile *getNeighbour(Directions direction);
	Carrier *roadToNeighbour(StockPile* stockPile);
	Directions getDirectionToNeighbour(StockPile* neighbour);

	void createOrder(Order* order);

private:
	void takeItem(); // This is called when an item is to be received from a carrier. Thus it is called on two occasions: when an item is taken from the crossroad (in case it was full), and when someone arrives with an item.
	void processOrder(Order* order);
};

