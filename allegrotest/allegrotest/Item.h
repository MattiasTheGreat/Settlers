#pragma once
enum itemType {NULLITEM,TREE,PLANKS};

class Item {
public:

	Item();
	itemType type;
	int path;
	void removeStep();

};