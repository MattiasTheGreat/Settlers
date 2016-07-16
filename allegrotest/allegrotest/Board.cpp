#include "Board.h"
#include "Road.h"
#include <assert.h>
#include <random>
#include <thread>
#include <allegro5/allegro_primitives.h>


Board::Board(int xSize, int ySize) {
	assert(ySize % 2 == 0);

	buildingRoad = false;
	running = true;
	mouse = { -1, -1 };
	clicked = { -1,-1 };
	this->xSize = xSize;
	this-> ySize = ySize;

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
			if (rand() % 5 == 2)
				grid[x][y]->constructed = BUILDING;
			
			Road *roadSouthEast = new Road;
			Road *roadSouthWest = new Road;
			grid[x][y]->southEast = roadSouthEast;
			grid[x][y]->southWest = roadSouthWest;
			roadSouthEast->start = grid[x][y];
			roadSouthWest->start = grid[x][y];

			if (y % 2 == 0) {
				grid[x][y]->shifted = false;
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
				grid[x][y]->shifted = true;
				if (y == ySize - 1) {
					roadSouthEast->end = grid[x][0];
					grid[x][0]->northWest = roadSouthEast;


					if (x == xSize - 1) {
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


					if (x == xSize - 1) {
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

bool Board::pathCriteria(PathType type, Road* way, Crossroad* end, Crossroad* destination) {
	if (!destination->visited) {
		switch (type) {
			case ROAD_PATH:
				if (destination->constructed == FLAG) {
					return end == destination;
				}
				else if (destination->constructed == FREE)
					return way->built == FREE;// && destination->constructed == FREE;
				else
					return false;
				break;
			case FREE_PATH:
				return !destination->visited && destination->constructed != BUILDING;
				break;
			case ITEM_PATH:
				return !destination->visited && way->built;
				break;
			default:
				return false;
		}
	}
	return false;
	
}

int Board::findPath(Crossroad* start, Crossroad* end, std::queue<Crossroad*>* pathfinder, PathType pathType) {
	if ((end->constructed == FREE || end->constructed == FLAG) && (start->constructed == FLAG || start->constructed == FREE || (start->constructed == ROAD && buildingRoad == true)) )
		return breadthFirst(start, end, pathfinder, pathType);
	else
		return -1;

}

int Board::breadthFirst(Crossroad* start, Crossroad* end, std::queue<Crossroad*>* pathfinder, PathType pathType) {
	//system("pause"); //DEBUG
	if (start == end) {
		while (!pathfinder->empty()) {
			pathfinder->front()->visited = false;
			pathfinder->front()->previous = NULL;
			pathfinder->pop();
		}
		if (start->previous != NULL) {
			pathfinder->push(start);
			pathfinder->push(start->previous);
		}
		start->previous = NULL;
		return start->transportationCost();
	}

	else {

		if (pathCriteria(pathType, start->east, end, start->getEastNeighbour())) {
			//fprintf(stderr, "E\n"); //DEBUG
			//start->east->built = ROAD; //DEBUG
			start->getEastNeighbour()->previous = start;
			pathfinder->push(start->getEastNeighbour());
			start->getEastNeighbour()->visited = true;
		}
		if (pathCriteria(pathType, start->southEast, end, start->getSouthEastNeighbour())) {
			//start->southEast->built = ROAD; //DEBUG
			//fprintf(stderr, "SE\n"); //DEBUG
			start->getSouthEastNeighbour()->previous = start;
			pathfinder->push(start->getSouthEastNeighbour());
			start->getSouthEastNeighbour()->visited = true;
		}
		if (pathCriteria(pathType, start->southWest, end, start->getSouthWestNeighbour())) {
			//start->southWest->built = ROAD; //DEBUG
			//fprintf(stderr, "SW\n"); //DEBUG
			start->getSouthWestNeighbour()->previous = start;
			pathfinder->push(start->getSouthWestNeighbour());
			start->getSouthWestNeighbour()->visited = true;
		}
		if (pathCriteria(pathType, start->west, end, start->getWestNeighbour())) {
			//start->west->built = ROAD; //DEBUG
			//fprintf(stderr, "W\n"); //DEBUG
			start->getWestNeighbour()->previous = start;
			pathfinder->push(start->getWestNeighbour());
			start->getWestNeighbour()->visited = true;
		}
		if (pathCriteria(pathType, start->northWest, end, start->getNorthWestNeighbour())) {
			//start->northWest->built = ROAD; //DEBUG
			//fprintf(stderr, "NW\n"); //DEBUG
			start->getNorthWestNeighbour()->previous = start;
			pathfinder->push(start->getNorthWestNeighbour());
			start->getNorthWestNeighbour()->visited = true;
		}
		if (pathCriteria(pathType, start->northEast, end, start->getNorthEastNeighbour())) {
			//start->northEast->built = ROAD; //DEBUG
			//fprintf(stderr, "NE\n"); //DEBUG
			start->getNorthEastNeighbour()->previous = start;
			pathfinder->push(start->getNorthEastNeighbour());
			start->getNorthEastNeighbour()->visited = true;
		}
		
		

		if (pathfinder->empty()) {
			return -1;
		}

		paintThySelf(30);
		al_flip_display();
		Crossroad* current = pathfinder->front();
		pathfinder->pop();

		int returnvalue = breadthFirst(current, end, pathfinder, pathType);

		if (!pathfinder->empty()) {	
			if (current == pathfinder->back()) {
				pathfinder->push(current->previous);
				returnvalue += current->transportationCost();
			}
		}
		current->visited = false;
		current->previous = NULL;
		return returnvalue;
	}
}

void Board::buildRoad(std::queue<Crossroad*> path) {
	if (!path.empty()) {
		
		Crossroad* start = path.front();
		path.pop();
		start->build(BuildStatus::ROAD);
		while (!path.empty()) {
			start->roadToNeighbour(path.front())->built = PORTAL;
			start = path.front();
			start->build(BuildStatus::ROAD);
			path.pop();
		}
	}
}

Point2D Board::calculate_closest_coordinate(int x, int y, int gridSize) {
	Point2D point;
	int tempy = (y + gridSize/2) / gridSize;
	point.y = tempy * gridSize;

	int tempx = (y + gridSize/2) / gridSize;
	if (tempx % 2 == 0) {
		point.x = ((int)((x + gridSize/2) / gridSize)) * gridSize;
	}
	else
		point.x = ((int)(x / gridSize)) * gridSize + gridSize/2;
	return point;
}

Point2D Board::calculate_closest_node(int x, int y, int gridSize) {

	Point2D point;
	int tempy = (y + gridSize/2) / gridSize;
	int tempx = (y + gridSize/2) / gridSize;

	point.y = tempy;

	
	if (tempx % 2 == 0) {
		point.x = ((int)((x + gridSize/2) / gridSize)) ;
	}
	else
		point.x = ((int)(x / gridSize));
	return point;
}

void Board::paintThySelf(int GRIDSIZE) {
	al_clear_to_color(al_map_rgb(0, 0, 0));
	for (int x = 0; x < xSize; ++x) {
		for (int y = 0; y < ySize; ++y) {

			if ((x != xSize - 1 && x != 0 && y != ySize - 1)) {
				grid[x][y]->east->paintThySelf(GRIDSIZE);
				grid[x][y]->southEast->paintThySelf(GRIDSIZE);
				grid[x][y]->southWest->paintThySelf(GRIDSIZE);
			}
			grid[x][y]->paintThySelf(GRIDSIZE);
		}
	}
	al_draw_filled_circle(mouse.x, mouse.y, 4, al_map_rgb(255, 255, 255));
	if (clicked.x != -1) 
		al_draw_filled_circle(clicked.x*30 + (grid[clicked.x][clicked.y]->shifted ? 15:0), clicked.y*30, 6, al_map_rgb(255, 69, 0));
}

bool Board::isGameRunning() {
	return running;
}

void Board::setGameRunning(bool running) {
	this->running = running;
}