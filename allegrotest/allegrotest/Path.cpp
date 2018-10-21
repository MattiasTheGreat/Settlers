#include "Path.h"



Path::Path()
{
	path = new std::vector<Crossroad*>;
	
}

Path::~Path() {
	delete path;
}

void Path::addToPath(std::queue<Crossroad*> appender, bool continueBuilding) {
	std::stack<Crossroad*> intermediary = std::stack<Crossroad*>();

	while (!appender.empty()) {
		intermediary.push(appender.front());
		appender.pop();
	}

	if(continueBuilding)
		intermediary.pop(); //The pathbuilder always adds the start node which is already in the path as it was added as the ending node before, so we need to remove it as to not have it twice in a row. Might want to consider if appender should be vector

	while(!intermediary.empty()){
		path->push_back(intermediary.top());
		intermediary.pop();
	}
}

Crossroad* Path::getNode(int index) {
	return path->at(index); // TODO: might want to make this access safer
}

Crossroad* Path::getStart() {
	return path->front();
}

Crossroad* Path::getEnd() {
	return path->back();
}

Crossroad* Path::getNextNode(Crossroad* crossroad) {
	for (int i = 0; i < path->size(); ++i) {
		if (path->at(i) == crossroad) {
			if (i == path->size() - 1) {
				return crossroad;
			}
			else
				return path->at(i + 1);
		}
	}
	return NULL;
}
Crossroad* Path::getPreviousNode(Crossroad* crossroad) {
	for (int i = 0; i < path->size(); ++i) {
		if (path->at(i) == crossroad) {
			if (i == 0) {
				return crossroad;
			}
			else
				return path->at(i - 1);
		}
	 }
	return NULL;
}

int Path::length() {
	return path->size();
}

Travel_Direction Path::directionToCrossroad( Crossroad* current, Crossroad* goal) {
	Travel_Direction direction = Travel_Direction::STARTWARD;
	int stop = path->size();

	for (int i = 0; i < stop; ++i) {
		if (direction == Travel_Direction::STAY) {
			direction = Travel_Direction::ENDWARD;
		}

		if (current == path->at(i)) {
			direction = Travel_Direction::STAY;
		}

		if (goal == path->at(i)) {
			return direction;
		}
	}

	return Travel_Direction::UNREACHABLE;
}