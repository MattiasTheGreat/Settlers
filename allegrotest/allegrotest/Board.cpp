#include "Board.h"
#include "Road.h"
#include "fpsCounter.h"	//BEAR
#include <assert.h>
#include <random>
#include <thread>
#include <allegro5/allegro_primitives.h>
#include "Directions.h"


Board::Board(int xSize, int ySize) {
	assert(ySize % 2 == 0);

	buildingRoad = false;
	running = true;
	mouse = { -1, -1 };
	clicked = { -1,-1 };
	this->xSize = xSize;
	this-> ySize = ySize;
	carriers = new std::vector<Carrier*>;

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
			grid[x][y]->roads[Directions::EAST] = roadEast;
			roadEast->start = grid[x][y];
			if (x == xSize - 1) {
				roadEast->end = grid[0][y];
				grid[0][y]->roads[Directions::WEST] = roadEast;
			}
			else {
				roadEast->end = grid[x + 1][y];
				grid[x + 1][y]->roads[Directions::WEST] = roadEast;
			}
			if (rand() % 5 == 0)
				grid[x][y]->constructed = BUILDING;
			
			Road *roadSouthEast = new Road;
			Road *roadSouthWest = new Road;
			grid[x][y]->roads[Directions::SOUTH_EAST] = roadSouthEast;
			grid[x][y]->roads[Directions::SOUTH_WEST] = roadSouthWest;
			roadSouthEast->start = grid[x][y];
			roadSouthWest->start = grid[x][y];

			if (y % 2 == 0) {
				grid[x][y]->shifted = false;
				roadSouthEast->end = grid[x][y + 1];
				grid[x][y + 1]->roads[Directions::NORTH_WEST] = roadSouthEast;

				if (x == 0) {
					roadSouthWest->end = grid[xSize - 1][y + 1];
					grid[xSize - 1][y + 1]->roads[Directions::NORTH_EAST] = roadSouthWest;
				}
				else {
					roadSouthWest->end = grid[x - 1][y + 1];
					grid[x - 1][y + 1]->roads[Directions::NORTH_EAST] = roadSouthWest;
				}
			}
			else {
				grid[x][y]->shifted = true;
				if (y == ySize - 1) {
					roadSouthEast->end = grid[x][0];
					grid[x][0]->roads[Directions::NORTH_WEST] = roadSouthEast;


					if (x == xSize - 1) {
						roadSouthWest->end = grid[0][0];
						grid[0][0]->roads[Directions::NORTH_EAST] = roadSouthWest;
					}
					else {
						roadSouthWest->end = grid[x + 1][0];
						grid[x + 1][0]->roads[Directions::NORTH_EAST] = roadSouthWest;
					}
				}
				else {
					roadSouthEast->end = grid[x][y + 1];
					grid[x][y + 1]->roads[Directions::NORTH_WEST] = roadSouthEast;


					if (x == xSize - 1) {
						roadSouthWest->end = grid[0][y + 1];
						grid[0][y + 1]->roads[Directions::NORTH_EAST] = roadSouthWest;
					}
					else {
						roadSouthWest->end = grid[x + 1][y + 1];
						grid[x + 1][y + 1]->roads[Directions::NORTH_EAST] = roadSouthWest;
					}
				}
			}

		}
	}
}

void Board::addCarrier(Carrier* carrier) {
	carriers->push_back(carrier);
}

void Board::removeCarrier(Carrier* carrier) {
	//carriers->remove(carrier);
}

void Board::removeRoad(Crossroad* node) {
	node->printInfo();
	if (node->constructed != FLAG) {
		node->constructed = FREE;
		fprintf(stderr, "second\n");
		Road* east = node->roadToNeighbour(node->getNeighbour(Directions::EAST));
		if (east->built == ROAD) {
			east->built = FREE;
			removeRoad(node->getNeighbour(Directions::EAST));
		}
		Road* southEast = node->roadToNeighbour(node->getNeighbour(Directions::SOUTH_EAST));
		if (southEast->built == ROAD) {
			southEast->built = FREE;
			removeRoad(node->getNeighbour(Directions::SOUTH_EAST));
		}
		Road* southWest = node->roadToNeighbour(node->getNeighbour(Directions::SOUTH_WEST));
		if (southWest->built == ROAD) {
			southWest->built = FREE;
			removeRoad(node->getNeighbour(Directions::SOUTH_WEST));
		}
		Road* west = node->roadToNeighbour(node->getNeighbour(Directions::WEST));
		if (west->built == ROAD) {
			west->built = FREE;
			removeRoad(node->getNeighbour(Directions::WEST));
		}

		Road* northEast = node->roadToNeighbour(node->getNeighbour(Directions::NORTH_EAST));
		if (northEast->built == ROAD) {
			northEast->built = FREE;
			removeRoad(node->getNeighbour(Directions::NORTH_EAST));
		}
		Road* northWest = node->roadToNeighbour(node->getNeighbour(Directions::NORTH_WEST));
		if (northWest->built == ROAD) {
			northWest->built = FREE;
			removeRoad(node->getNeighbour(Directions::NORTH_WEST));
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
		int return_value = breadthFirst(start, end, pathfinder, pathType);

		
		/*
		// for a-starish
		std::priority_queue<Crossroad*, std::vector<Crossroad*>,test> priority;
		int return_value = aStarish(start, end, &priority, pathType);
		*/
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

//Not tested since change to roads being a vector instead of six different variables.
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

		const Directions directions[6] = { NORTH_WEST, NORTH_EAST, EAST, SOUTH_EAST, SOUTH_WEST, WEST };
		for (int i = 0; i < 6; ++i) {
			Crossroad* node = start->getNeighbour(directions[i]);

			if (pathCriteria(pathType, start->roads[directions[i]], end, node)) {
				node->previous = start;
				node->distance = node->calculateDistance(end);
				pathfinder->push(node);
				node->visited = true;
			}
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
		const Directions directions[6] = {NORTH_WEST, NORTH_EAST, EAST, SOUTH_EAST, SOUTH_WEST, WEST };
		for (int i = 0; i < 6; ++i) {
			Crossroad *node = start->getNeighbour(directions[i]);
			if (pathCriteria(pathType, start->roads[i], end, node)) {
				//fprintf(stderr, "E\n"); //DEBUG
				//start->east->built = ROAD; //DEBUG
				node->previous = start;
				pathfinder->push(node);
				node->visited = true;
			}
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

void Board::tick(int GRIDSIZE) {
	paintThySelf(GRIDSIZE);
	for (int i = 0; i < carriers->size(); ++i) {
		carriers->at(i)->tick();
	}
}

void Board::paintThySelf(int GRIDSIZE) {
	al_clear_to_color(al_map_rgb(0, 0, 0));
	for (int x = 0; x < xSize; ++x) {
		for (int y = 0; y < ySize; ++y) {

			if ((x != xSize - 1 && x != 0 && y != ySize - 1)) {
				grid[x][y]->roads[Directions::EAST]->paintThySelf(GRIDSIZE);
				grid[x][y]->roads[Directions::SOUTH_EAST]->paintThySelf(GRIDSIZE);
				grid[x][y]->roads[Directions::SOUTH_WEST]->paintThySelf(GRIDSIZE);
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