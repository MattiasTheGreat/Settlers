#pragma once
#include <vector>
#include <allegro5/allegro_primitives.h>
#include "Point2D.h"

class Crossroad;
enum itemType {NULLITEM,TREE,PLANKS};

class Item {
public:
	Item(ALLEGRO_COLOR color);
	itemType type;
	ALLEGRO_COLOR color;

	void paintThyself(Point2D coordinates);
};