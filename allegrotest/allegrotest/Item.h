#pragma once
#include <vector>
#include <allegro5/allegro_primitives.h>
#include "Point2D.h"

class Crossroad;
enum itemType {NULLITEM,TREE,PLANKS};

class Item {
public:
	Item(std::vector<Crossroad*> path);
	itemType type;
	std::vector<Crossroad*> path; // The path that the item should be moved.
	int currentLocation; // Points to the index in the path member where item currently is.
	void received(); // To be called when a new crossroad receives the object.
	void removeStep();

	Crossroad* nextCrossroad();
	void paintThyself(int GRIDSIZE, Point2D coordinates);
};