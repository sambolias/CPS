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
#include <vector>
using std::vector;
#include <fstream>
using std::ofstream;
#include <memory>
using std::shared_ptr;
using std::make_shared;

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
	virtual string getPostScript() const { return ""; }	//this needs to be =0 once derived classes all define it, or the string
	virtual ~shape() = default;				//and function could be defined here for less repetition
};

class spacer : public shape
{
public:
	spacer(double height, double width) 
	{	
		setHeight(height);
		setWidth(width);
	}

	virtual string getPostScript() const override {return "";}	//this could also be left out if we use only one getPostScript fn
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


class pixel : public shape
{

private:
	string _postScript;

public:
	pixel(double r, double g, double b) 
	{
		//too many squares causes stack overflow
		//polygon temp(4,2);
		circle temp(1);
		_postScript = temp.getPostScript();
		_postScript += "\n" + to_string(r) + " " + to_string(g) + " " + to_string(b) + " setrgbcolor\n";
		_postScript += "\nfill\n";
	}

	string getPostScript() const override
	{
		return _postScript;
	}

};

class mandelbrot : public shape
{

private:

	string _postScript;

public:

	mandelbrot();

	string getPostScript() const override
	{
		return _postScript;
	}

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

	triangle(double numSides, double sideLength) : polygon(TRISIDES, sideLength) {};
};

class square : public polygon
{

public:
	square(double numSides, double sideLength) : polygon(SQUARESIDES, sideLength){}

};

//compound shapes

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
class scaled : public shape
{
private: 

	double _origWidth, _origHeight;
	double _xScale, _yScale;
	string _postScript;

public:

	scaled(const shape & s, double xScale, double yScale); // takes 2 scales one for x one for y

	virtual string getPostScript() const override;

};
class vertical : public shape
{

private:

	string _postScript;

public:

	vertical(initializer_list<shared_ptr<shape>> shapes);

	virtual string getPostScript() const override;

};

class horizontal : public shape
{

private:

	string _postScript;

public:

	horizontal(initializer_list<shared_ptr<shape>> shapes);

	virtual string getPostScript() const override;

};

class layered : public shape
{
private:
	string _postScript;
public:
	layered(initializer_list<shared_ptr<shape>> shapes);
	
	virtual string getPostScript() const override;
};

//file output interface

class page
{

private:

	string _postScript;

public:


	void drawTo(const shape &s, int x, int y);
	string getPostScript();
};

class output
{

private:

	string _postScript;
	vector<page> pages;

public:

	void addPage(const page &p);

	void outputFile(string fname);

};

#endif