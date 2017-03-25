#ifndef CPS_H_INCLUDED
#define CPS_H_INCLUDED


#include <string>
using std::string;
using std::to_string;
#include <iostream>
using std::cout;
using std::endl;
#include <initializer_list>
using std::initializer_list;

double  TRISIDES = 3.0;
double SQUARESIDES = 4.0;

class shape
{

private:

	double _height,_width;

protected:

	void setHeight(double height);
	void setWidth(double width);

public:

	int getHeight();
	int getWidth();
	virtual string getPostScript(){}	//this needs to be =0 once derived classes all define it
										//shape can be purely abstract class
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

//this is what i had in mind
//it would be nice to have the ps file output set up 
//for further testing
class rectangle : public shape
{

public:
	rectangle(double width, double height);

	virtual string getPostScript() override;
	
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

//i don't think there is any reason to have a compound shapes base class

class rotated : public shape
{
private:

	double _rotation;
	shape subject;	//doing it this way all shapes need copy ctor

public:

	//rotated(const shape &s, double rotation);

	//not sure how to make postscript here
	//i think each postscript output needs to omit the moveto
	//so that rotate can be done after
	//might be easiest to just make file output functions
	//to do some testing and find out how to get base classes set up properly
};


#endif