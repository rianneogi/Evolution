#include "WeightedRandom.h"

struct Vector2I
{
	int x;
	int y;
	Vector2I() : x(0),y(0) {};
	Vector2I(int _x, int _y) : x(_x), y(_y){};
};

int calculateELO(int elo1, int elo2, int res, int k_factor);