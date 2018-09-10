#include "Path.h"



Path::Path()
{
	path = new std::queue<Crossroad*>;
	
}

Path::~Path() {
	delete path;
}

void Path::appendQueue(std::queue<Crossroad*> appender) {
	while(!appender.empty()){
		path->push(appender.front());
		appender.pop();
	}
}