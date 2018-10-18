#include "Path.h"



Path::Path()
{
	path = new std::vector<Crossroad*>;
	
}

Path::~Path() {
	delete path;
}

void Path::appendPath(std::queue<Crossroad*> appender) {
	while(!appender.empty()){
		path->push_back(appender.front());
		appender.pop();
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