#include "Board.h"
#include "Road.h"
#include "fpsCounter.h"	//BEAR
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

void Board::removeRoad(Crossroad* node) {
	node->printInfo();
	if (node->constructed != FLAG) {
		node->constructed = FREE;
		fprintf(stderr, "second\n");
		Road* east = node->roadToNeighbour(node->getEastNeighbour());
		if (east->built == ROAD) {
			east->built = FREE;
			removeRoad(node->getEastNeighbour());
		}
		Road* southEast = node->roadToNeighbour(node->getSouthEastNeighbour());
		if (southEast->built == ROAD) {
			southEast->built = FREE;
			removeRoad(node->getSouthEastNeighbour());
		}
		Road* southWest = node->roadToNeighbour(node->getSouthWestNeighbour());
		if (southWest->built == ROAD) {
			southWest->built = FREE;
			removeRoad(node->getSouthWestNeighbour());
		}
		Road* west = node->roadToNeighbour(node->getWestNeighbour());
		if (west->built == ROAD) {
			west->built = FREE;
			removeRoad(node->getWestNeighbour());
		}

		Road* northEast = node->roadToNeighbour(node->getNorthEastNeighbour());
		if (northEast->built == ROAD) {
			northEast->built = FREE;
			removeRoad(node->getNorthEastNeighbour());
		}
		Road* northWest = node->roadToNeighbour(node->getNorthWestNeighbour());
		if (northWest->built == ROAD) {
			northWest->built = FREE;
			removeRoad(node->getNorthWestNeighbour());
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
				return destination->constructed != BUILDING;
				break;
			case ITEM_PATH:
				return way->built == ROAD;
				break;
			default:
				return false;
		}
	}
	return false;
	
}



int Board::findPath(Crossroad* start, Crossroad* end, std::queue<Crossroad*>* pathfinder, PathType pathType) {

	if ((end->constructed == FREE || end->constructed == FLAG) && (start->constructed == FLAG || (start->constructed == ROAD && buildingRoad == true))) {

		clock_t start_time = clock();
		//int return_value = breadthFirst(start, end, pathfinder, pathType);

		

		std::priority_queue<Crossroad*, std::vector<Crossroad*>,test> priority;
		int return_value = aStarish(start, end, &priority, pathType);
		fprintf(stderr, "ms %i \n", timeDiff_ms(start_time));

		if (end->previous != NULL) {
			Crossroad* current = end;
			while (current != start) {
				pathfinder->push(current);
				Crossroad* temp = current->previous;
				current->previous = NULL;
				current = temp;
			}
			pathfinder->push(start);
		}
		return return_value;

	}

	else
		return -1;

}

int Board::aStarish(Crossroad* start, Crossroad* end, std::priority_queue<Crossroad*, std::vector<Crossroad*>, test>* pathfinder, PathType pathType) {

	fprintf(stderr, "distance %i \n", (int)(start->calculateDistance(end)));

	if (start == end) { 
		while (!pathfinder->empty()) {
			Crossroad* temp = pathfinder->top();
			temp->visited = false;
			temp->previous = NULL;
			pathfinder->pop();
		}
		//start->previous = NULL;
		start->previous->pathing = true;
		return start->transportationCost();
	}

	else {

		Crossroad* east = start->getEastNeighbour();
		if (pathCriteria(pathType, start->east, end, east)) {
			/*start->east->built = ROAD; //DEBUG
			system("pause"); //DEBUG
			paintThySelf(30); //DEBUG 30 is current gridsize
			al_flip_display(); //DEBUG*/
			//fprintf(stderr, "E\n"); //DEBUG
			
			east->previous = start;
			east->distance = east->calculateDistance(end);
			pathfinder->push(east);
			east->visited = true;
		}
		Crossroad* southEast = start->getSouthEastNeighbour();
		if (pathCriteria(pathType, start->southEast, end, southEast)) {
			/*start->southEast->built = ROAD; //DEBUG
			system("pause"); //DEBUG
			paintThySelf(30); //DEBUG 30 is current gridsize
			al_flip_display(); //DEBUG*/
			//fprintf(stderr, "SE\n"); //DEBUG
			southEast->previous = start;
			southEast->distance = southEast->calculateDistance(end);
			pathfinder->push(southEast);
			southEast->visited = true;
		}
		Crossroad* southWest = start->getSouthWestNeighbour();
		if (pathCriteria(pathType, start->southWest, end,southWest)) {
			/*start->southWest->built = ROAD; //DEBUG
			system("pause"); //DEBUG
			paintThySelf(30); //DEBUG 30 is current gridsize
			al_flip_display(); //DEBUG*/
			//fprintf(stderr, "SW\n"); //DEBUG
			southWest->previous = start;
			southWest->distance = southWest->calculateDistance(end);
			pathfinder->push(southWest);
			southWest->visited = true;
		}
		Crossroad* west = start->getWestNeighbour();
		if (pathCriteria(pathType, start->west, end, west)) {
			/*start->west->built = ROAD; //DEBUG
			system("pause"); //DEBUG
			paintThySelf(30); //DEBUG 30 is current gridsize
			al_flip_display(); //DEBUG*/
			//fprintf(stderr, "W\n"); //DEBUG
			west->previous = start;
			west->distance = west->calculateDistance(end);
			pathfinder->push(west);
			west->visited = true;
		}
		Crossroad* northWest = start->getNorthWestNeighbour();
		if (pathCriteria(pathType, start->northWest, end, northWest)) {
			/*start->northWest->built = ROAD; //DEBUG
			system("pause"); //DEBUG
			paintThySelf(30); //DEBUG 30 is current gridsize
			al_flip_display(); //DEBUG*/
			//fprintf(stderr, "NW\n"); //DEBUG
			northWest->previous = start;
			northWest->distance = northWest->calculateDistance(end);
			pathfinder->push(northWest);
			northWest->visited = true;
		}
		Crossroad* northEast = start->getNorthEastNeighbour();
		if (pathCriteria(pathType, start->northEast, end, northEast)) {
			/*start->northEast->built = ROAD; //DEBUG
			system("pause"); //DEBUG
			paintThySelf(30); //DEBUG 30 is current gridsize
			al_flip_display(); //DEBUG*/
			//fprintf(stderr, "NE\n"); //DEBUG
			northEast->previous = start;
			northEast->distance = northEast->calculateDistance(end);
			pathfinder->push(northEast);
			northEast->visited = true;
		}



		if (pathfinder->empty()) {
			return -1;
		}

		//paintThySelf(30); //DEBUG
		//al_flip_display(); //DEGUB
		Crossroad* current = pathfinder->top();
		pathfinder->pop();

		int returnvalue = aStarish(current, end, pathfinder, pathType);

		if (start->pathing) {
			start->pathing = false;
			if (start->previous != NULL)
				start->previous->pathing = true;
		}
		else
			start->previous = NULL;

		current->visited = false;
		return returnvalue;
	}
}

int Board::breadthFirst(Crossroad* start, Crossroad* end, std::queue<Crossroad*>* pathfinder, PathType pathType) {
	/*system("pause"); //DEBUG
	paintThySelf(30); //DEBUG
	al_flip_display(); //DEBUG*/

	fprintf(stderr, "distance %i \n", (int) (start->calculateDistance(end)));

	if (start == end) {
		while (!pathfinder->empty()) {
			Crossroad* temp = pathfinder->front();
			temp->visited = false;
			temp->previous = NULL;

			pathfinder->pop();
		}
		//start->previous = NULL;
		start->previous->pathing = true;
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

		//paintThySelf(30); //DEBUG
		//al_flip_display(); //DEGUB
		Crossroad* current = pathfinder->front();
		pathfinder->pop();

		int returnvalue = breadthFirst(current, end, pathfinder, pathType);

		if (start->pathing) {
			start->pathing = false;
			if(start->previous != NULL)
				start->previous->pathing = true;
		}
		else 
			start->previous = NULL;

		current->visited = false;
		return returnvalue;
	}
}

void Board::buildRoad(std::queue<Crossroad*> path) {
	if (!path.empty()) {
		
		Crossroad* start = path.front();
		path.pop();
		start->build(BuildStatus::ROAD);
		while (!path.empty()) {
			start->roadToNeighbour(path.front())->built = ROAD;
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

void Board::deleteClicked(){
	if (grid[clicked.x][clicked.y]->constructed == FLAG) {
		grid[clicked.x][clicked.y]->build(FREE);
		removeRoad(grid[clicked.x][clicked.y]);
	}
	else if (grid[clicked.x][clicked.y]->constructed == ROAD) {
		removeRoad(grid[clicked.x][clicked.y]);
	}

}