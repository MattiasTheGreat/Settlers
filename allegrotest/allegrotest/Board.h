#pragma once
#include "Crossroad.h"
#include <vector>
#include <queue>

enum PathType { ROAD_PATH, ITEM_PATH, FREE_PATH };

class Board {
public:
	Board(int xSize, int ySize);

	bool buildingRoad;
	int xSize;
	int ySize;
	Point2D mouse;
	Point2D clicked;
	
	std::vector<std::vector<Crossroad*>> grid;

	Point2D calculate_closest_node(int x, int y, int gridSize);
	Point2D calculate_closest_coordinate(int x, int y, int gridSize);

	bool isGameRunning();
	void setGameRunning(bool running);

	//Give unto me an empty queue and i shall place in it a Path. Give unto me a Criteria to specify what Path thou Desire and that boon shalt be yours, should the place for it to exist be in this World. 
	int findPath(Crossroad* start, Crossroad* end, std::queue<Crossroad*>* pathfinder, PathType criteria);
	
	void buildRoad(std::queue<Crossroad*> path);

	bool pathCriteria(PathType type, Road* road, Crossroad* start, Crossroad* destination);

	void paintThySelf(int GRIDSIZE);
private:
	int breadthFirst(Crossroad* start, Crossroad* end, std::queue<Crossroad*>* pathfinder, PathType criteria);
	bool running;
};