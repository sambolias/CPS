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

const double  TRISIDES = 3.0;
const double SQUARESIDES = 4.0;

const double PI = 4*atan(1);	//calculates pi to the machine's precision

class shape
{

private:

	double _height,_width;

protected:

	void setHeight(double height);
	void setWidth(double width);

public:

	virtual double getHeight() const;
	virtual double getWidth() const;
	virtual string getPostScript() const {}	//this needs to be =0 once derived classes all define it
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
	 setWidth(rad*2);
	 setHeight(rad*2);
	}
	double getRad() const;

	string getPostScript() const override;
};

class polygon : public shape
{
private:
	int _numSides;
	double _sideLength;
	double _circumRad, _inRad, _innerAngle;

public:
	polygon(double numSides, double sideLength){
		_numSides = (int)numSides;
		_sideLength = sideLength;
		_circumRad = calcCircumRad();	//radius of circle touching each vertex
		_inRad = calcInRad();	//radius of circle tangent to each side
		_innerAngle = calcInnerAngle();	//inner angle at each vertex
		setHeight(calcHeight());
		setWidth(calcWidth());
	};

	double calcCircumRad();
	double calcInRad();

	double calcHeight();
	double calcWidth();
	double calcInnerAngle();

	int getNumSides() const;
	double getSideLength() const; 
	double getInnerAngle() const;

	double getCircumRad() const;
	double getInRad() const;

	

	string getPostScript() const override;
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

	string getPostScript() const override;
	
};

class triangle : public polygon
{
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