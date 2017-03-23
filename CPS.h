#include <string>
double  TRISIDES = 3.0;
double SQUARESIDES = 4.0;

class shape
{

private:

	int _height,_width;
	int _x,_y;
	int _inch = 72;

public:

	int getHeight();
	int getWidth();

	int getUnit();

	int getPosX();
	int getPosY();

	virtual ~shape() = default;
};
// triangle do polygon contrustor but do it for 3 sides 
class circle : public shape
{
private : 
	double _radius;

public :
	circle(double rad){
	 _radius = rad;
	}
	double getWidth();
	double getHeight();
};

class polygon : public shape
{
private:
	double _numSides;
	double _sideLength;

public:
	polygon(double numSides, double sideLength){
		_numSides = numSides;
		_sideLength = sideLength;
	};
	double getNumSides();
	double getSideLength();
};

class rectangle : public shape
{
private:
	double _width;
	double _height;
public:
	rectangle(double width,double height)
	{
		_width = width;
		_height = height;
	}
};

class triangle : public polygon
{
private:
	double _numSides;
	double _sideLength;
public:

	triangle(double numSides, double sideLength) : polygon(TRISIDES, sideLength){}

};

class square : public polygon
{

public:
	square(double numSides, double sideLength) : polygon(SQUARESIDES, sideLength){}

};
