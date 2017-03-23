#include "CPS.h"
#include <iostream>
int shape::getHeight()
{
	return _height;
}
int shape::getWidth()
{
	return _width;
}

int shape::getPosX()
{
	return _x;
}
int shape::getPosY()
{
	return _y;
}

int shape::getUnit()
{
	return _inch;
}

int main()
{
	triangle tri(5, 6.0);
	square sq(5, 6.0);
	std::cout << "triangle number of sides " <<tri.getNumSides() << "\n" << "triangle side length "  <<   tri.getSideLength() << std::endl;
	std::cout << "square number of sides " << sq.getNumSides() << std::endl;
	return 0;
}


double circle::getWidth()
{
	return 2*_radius;
}

double circle::getHeight()
{
	return getWidth();
}

double polygon::getNumSides()
{
	return _numSides;
}

double polygon::getSideLength()
{
	return _sideLength;
}
