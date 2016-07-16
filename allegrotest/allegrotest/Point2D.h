#pragma once
typedef struct Point2D {
	int x;
	int y;
};

inline
bool operator==(const Point2D& lhs, const Point2D& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}