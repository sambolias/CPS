#ifndef CPS_H_INCLUDED
#define CPS_H_INCLUDED

#include <cmath>
using std::sin;
using std::cos;
using std::abs;
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

	double getHeight() const;
	double getWidth() const;
	virtual string getPostScript() const {}	//this needs to be =0 once derived classes all define it
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
	double getWidth() const;
	double getHeight() const;
	double getRad() const;

	virtual string getPostScript() const override;
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

//this is what i have in mind
//see rotate 
//for other multishape classes use translate before rotates
//as far as I can tell this should work
//test with ps files, now is the time to find errors
class rectangle : public shape
{

public:
	rectangle(double width, double height);

	virtual string getPostScript() const override;
	
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

	double _origWidth, _origHeight;
	double _rotation;
	string _postScript;	

public:

	rotated(const shape &s, double rotation);

	virtual string getPostScript() const override;

};


#endif