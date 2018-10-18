#pragma once
#include <vector>
#include <allegro5/allegro_primitives.h>
#include "Point2D.h"

class Crossroad;
enum itemType {NULLITEM,TREE,PLANKS};

class Item {
public:
	Item();
	itemType type;

	void paintThyself(int GRIDSIZE, Point2D coordinates);
};