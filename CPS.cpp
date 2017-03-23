#include "CPS.h"
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

shape::~shape()
{

}



int main()
{

	return 0;
}

double circle::getRadius()
{
	return _radius;
}

double circle::getWidth()
{
	return 2*_radius;
}

double circle::getHeight()
{
	return getWidth();
}
