#pragma once
enum itemType {nullItem,tree,planks};

class Item {
public:

	Item();
	itemType type;
	int path;
	void removeStep();

};