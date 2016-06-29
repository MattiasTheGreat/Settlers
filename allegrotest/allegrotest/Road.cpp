#include "Road.h"
#include "Item.h"
#include <stdio.h>

Road::Road() {
	visited = false;
	progress = -1;
}

Crossroad *Road::opposite(Crossroad *base) {
	if (this->end == base)
		return this->start;
	else if (this->start == base)
		return this->end;
	else {
		fprintf(stderr, "error");
		return this->end;
		
	}
}