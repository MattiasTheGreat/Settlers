#pragma once
#include "Crossroad.h"
#include <queue>

class Path
{
public:
	Path();
	~Path();

	std::queue<Crossroad*>* path;

	void appendQueue(std::queue<Crossroad*> appender); // Destructive, so we want a copy of appender.

};

