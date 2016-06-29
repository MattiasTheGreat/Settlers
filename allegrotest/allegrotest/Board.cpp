#include "Board.h"
#include "Road.h"

Board::Board(int xSize, int ySize) {
	for (int x = 0; x < xSize; ++x) {
		std::vector<Crossroad*> temp_vect = *(new std::vector<Crossroad*>);
		
		for (int y = 0; y < ySize; ++y) {
			Crossroad* temp = (new Crossroad);
			temp->coordinates.x = x;
			temp->coordinates.y = y;
			temp_vect.push_back(temp);
			
		}
		grid.push_back(temp_vect);
	}
	
	for (int x = 0; x < xSize; ++x) {
		for (int y = 0; y < ySize; ++y) {

			Road *roadEast = new Road;
			grid[x][y]->east = roadEast;
			roadEast->start = grid[x][y];

			if (x == xSize - 1) {
				roadEast->end = grid[0][y];
				grid[0][y]->west = roadEast;
			}
			else {
				roadEast->end = grid[x + 1][y];
				grid[x + 1][y]->west = roadEast;
			}

			Road *roadSouthEast = new Road;
			Road *roadSouthWest = new Road;
			grid[x][y]->southEast = roadSouthEast;
			grid[x][y]->southWest = roadSouthWest;
			roadSouthEast->start = grid[x][y];
			roadSouthWest->start = grid[x][y];

			if (y % 2 == 0) {
				roadSouthEast->end = grid[x][y + 1];
				grid[x][y + 1]->northWest = roadSouthEast;

				if (x == 0) {
					roadSouthWest->end = grid[xSize - 1][y + 1];
					grid[xSize - 1][y + 1]->northEast = roadSouthWest;
				}
				else {
					roadSouthWest->end = grid[x - 1][y + 1];
					grid[x - 1][y + 1]->northEast = roadSouthWest;
				}
			}
			else {
				if (y == ySize - 1) {
					roadSouthEast->end = grid[x][0];
					grid[x][0]->northWest = roadSouthEast;


					if (x == 9) {
						roadSouthWest->end = grid[0][0];
						grid[0][0]->northEast = roadSouthWest;
					}
					else {
						roadSouthWest->end = grid[x + 1][0];
						grid[x + 1][0]->northEast = roadSouthWest;
					}
				}
				else {
					roadSouthEast->end = grid[x][y + 1];
					grid[x][y + 1]->northWest = roadSouthEast;


					if (x == 9) {
						roadSouthWest->end = grid[0][y + 1];
						grid[0][y + 1]->northEast = roadSouthWest;
					}
					else {
						roadSouthWest->end = grid[x + 1][y + 1];
						grid[x + 1][y + 1]->northEast = roadSouthWest;
					}
				}
			}

		}
	}
}

void Board::findPath(Crossroad* start, Crossroad* end, std::queue<Crossroad*>* pathfinder) {

	if (start == end) {
		while (!pathfinder->empty()) {
			pathfinder->front()->visited = false;
			pathfinder->pop();
		}
		pathfinder->push(start);
		pathfinder->push(start->previous);
		
		start->previous = NULL;
	}
	else {

		if (!start->getEastNeighbour()->visited && !start->east->visited) {
			start->getEastNeighbour()->previous = start;
			pathfinder->push(start->getEastNeighbour());
			start->getEastNeighbour()->visited = true;
		}
		if (!start->getWestNeighbour()->visited && !start->west->visited) {
			start->getWestNeighbour()->previous = start;
			pathfinder->push(start->getWestNeighbour());
			start->getWestNeighbour()->visited = true;
		}
		if (!start->getSouthEastNeighbour()->visited && !start->southEast->visited) {
			start->getSouthEastNeighbour()->previous = start;
			pathfinder->push(start->getSouthEastNeighbour());
			start->getSouthEastNeighbour()->visited = true;
		}
		if (!start->getNorthEastNeighbour()->visited && !start->northEast->visited) {
			start->getNorthEastNeighbour()->previous = start;
			pathfinder->push(start->getNorthEastNeighbour());
			start->getNorthEastNeighbour()->visited = true;
		}
		if (!start->getSouthWestNeighbour()->visited && !start->southWest->visited) {
			start->getSouthWestNeighbour()->previous = start;
			pathfinder->push(start->getSouthWestNeighbour());
			start->getSouthWestNeighbour()->visited = true;
		}
		if (!start->getNorthWestNeighbour()->visited && !start->northWest->visited) {
			start->getNorthWestNeighbour()->previous = start;
			pathfinder->push(start->getNorthWestNeighbour());
			start->getNorthWestNeighbour()->visited = true;
		}

		if (pathfinder->empty()) {
			fprintf(stderr, "tomt\n");
			return;
			
		}
		Crossroad* current = pathfinder->front();
		pathfinder->pop();
		findPath(current, end, pathfinder);
		current->visited = false;
		if (current == pathfinder->back()) {
			pathfinder->push(current->previous);
			current->previous = NULL;
		}
	}
}

void Board::buildRoad(std::queue<Crossroad*> path) {
	if (!path.empty()) {
		Crossroad* start = path.front();
		path.pop();
		while (!path.empty()) {
			fprintf(stderr, " first %i %i\n", start->coordinates.x,start->coordinates.y);
			fprintf(stderr, " target %i %i\n", path.front()->coordinates.x, path.front()->coordinates.y);
			start->roadToNeighbour(path.front())->visited = true;
			start = path.front();
			path.pop();
		}
	}
}

Point2D Board::calculate_closest_coordinate(int x, int y) {
	Point2D point;
	int tempy = (y + 20) / 40;
	point.y = tempy * 40;

	int tempx = (y + 20) / 40;
	if (tempx % 2 == 0) {
		point.x = ((int)((x + 20) / 40)) * 40;
	}
	else
		point.x = ((int)(x / 40)) * 40 + 20;
	return point;
}

Point2D Board::calculate_closest_node(int x, int y) {

	Point2D point;
	int tempy = (y + 20) / 40;
	int tempx = (y + 20) / 40;

	point.y = tempy;

	
	if (tempx % 2 == 0) {
		point.x = ((int)((x + 20) / 40)) ;
	}
	else
		point.x = ((int)(x / 40));
	return point;
}