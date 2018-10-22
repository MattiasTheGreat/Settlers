#include "Path.h"
#include "Crossroad.h"


template <class T>
Path<T>::Path()
{
	path = new std::vector<T>;
	
}

template <class T>
Path<T>::~Path() {
	delete path;
}

template <class T>
void Path<T>::addToPath(std::queue<T> appender, bool continueBuilding) {
	std::stack<T> intermediary = std::stack<T>();

	while (!appender.empty()) {
		intermediary.push(appender.front());
		appender.pop();
	}

	if(continueBuilding)
		intermediary.pop(); //The pathbuilder always adds the start node which is already in the path as it was added as the ending node before, so we need to remove it as to not have it twice in a row. TODO: Might want to consider if appender should be vector

	while(!intermediary.empty()){
		path->push_back(intermediary.top());
		intermediary.pop();
	}
}

template <class T>
T Path<T>::getNode(int index) {
	return path->at(index); // TODO: might want to make this access safer
}

template <class T>
T Path<T>::getStart() {
	return path->front();
}

template <class T>
T Path<T>::getEnd() {
	return path->back();
}

template <class T>
T Path<T>::getNextNode(T node) {
	for (int i = 0; i < path->size(); ++i) {
		if (path->at(i) == node) {
			if (i == path->size() - 1) {
				return node;
			}
			else
				return path->at(i + 1);
		}
	}
	return NULL;
}

template <class T>
T Path<T>::getPreviousNode(T node) {
	for (int i = 0; i < path->size(); ++i) {
		if (path->at(i) == node) {
			if (i == 0) {
				return node;
			}
			else
				return path->at(i - 1);
		}
	 }
	return NULL;
}

template <class T>
int Path<T>::length() {
	return path->size();
}

template <class T>
Travel_Direction Path<T>::directionToCrossroad(T current, T goal) {
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

template <class T>
T Path<T>::middleNode() {
	return path->at(path->size() / 2);
}

