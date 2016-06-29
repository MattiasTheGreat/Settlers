#pragma once
#include "Crossroad.h"
#include <vector>
#include <queue>



class Board {
public:
	Board(int xSize, int ySize);
	std::vector<std::vector<Crossroad*>> grid;

	Point2D calculate_closest_node(int x, int y);
	Point2D calculate_closest_coordinate(int x, int y);
	void findPath(Crossroad* start, Crossroad* end, std::queue<Crossroad*>* pathfinder);
	void buildRoad(std::queue<Crossroad*> path);
};