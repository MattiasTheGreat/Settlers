#pragma once
#include "Crossroad.h"
#include <vector>
#include <vector>
#include "Carrier.h"
#include "StockPile.h"

/*The different path types:
 *ROAD_PATH: Can only go through crossroads where nothing is built.
 *ITEM_PATH: Can only go through already existing paths to a flag.
 *FREE_PATH: Doesn't care if there is roads or flags. Can still not go through obstacles though.
*/
enum PathType { ROAD_PATH, ITEM_PATH, FREE_PATH };
struct test;

extern int GRID_X_SIZE; // Value set in Crossroad.cpp
extern int GRID_Y_SIZE; // Value set in Crossroad.cpp


class Board {
public:
	Board();

	bool buildingRoad;
	Point2D mouse; // The node which is currently closest to the mouse. Will be highlighted.
	Point2D selected; // The currently targeted crossroad on the map. Value of -1 means nothing targeted atm.
	bool roadBuilding = true; // Temp variable to test item creating
	
	std::vector<std::vector<Crossroad*>> grid;

	void addCarrier(Carrier* carrier);
	void removeCarrier(Carrier* carrier);

	Point2D calculate_closest_node(int x, int y, int gridSize);
	Point2D calculate_closest_coordinate(int x, int y, int gridSize);

	bool isGameRunning(); // Is used to turn of the game when the cross button in the window is pressed.
	void setGameRunning(bool running);

	// Give unto me an empty queue and i shall place in it a Path. Give unto Me a Criteria to specify the Path thou Desire and that Boon shalt be yours, should the place for it to Exist be in this World. 
	int findPath(Crossroad* start, Crossroad* end, std::queue<Crossroad*>* pathfinder, PathType criteria);
	int findPath(StockPile* start, StockPile* end, std::queue<StockPile*>* pathfinder, PathType pathType);
	
	void buildRoad(std::queue<Crossroad*> path);

	bool pathCriteria(PathType type, Road* road, Crossroad* start, Crossroad* destination);
	bool pathCriteria(PathType type, Carrier* road, StockPile* start, StockPile* destination);

	void removeRoad(Crossroad* node);

	void deleteClicked();

	void tick(int GRIDSIZE);
	void paintThySelf(int GRIDSIZE);

	
	int breadthFirst(Crossroad* start, Crossroad* end, std::queue<Crossroad*>* pathfinder, PathType criteria);
	int breadthFirst(StockPile* start, StockPile* end, std::queue<StockPile*>* pathfinder, PathType criteria);
private:
	std::vector<Carrier*>* carriers;

	int aStarish(Crossroad* start, Crossroad* end, std::priority_queue<Crossroad*, std::vector<Crossroad*>, test>* pathfinder, PathType criteria);

	
	bool running;
};

struct test {
	bool operator()(const Crossroad* lhs, const Crossroad* rhs) {
		return lhs->distance > rhs->distance;
	}
};