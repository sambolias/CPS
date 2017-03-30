#include "CPS.h"


double shape::getHeight() const 
{
	return _height;
}
double shape::getWidth() const
{
	return _width;
}

void shape::setHeight(double height)
{
	_height = height;
}
void shape::setWidth(double width)
{
	_width = width;
}

rectangle::rectangle(double width,double height)
{
	setWidth(width);
	setHeight(height);
}

//this probably needs a better place than global
//but it is a nice way of setting postscript strings
//size of replace must be < size of find
void findAndReplace(string & s, string && find, string && replace)
{

	if(replace.length() < find.length())
	{
		string temp;
		temp.resize(find.length(), ' ');

		for(int i = 0; i < replace.length(); i++)
		{
			temp[i] = replace[i];
		}

		replace = temp;
	}

	while(true)
	{
		auto found = s.find(find);

		if(found == string::npos) break;	//find not found
		else
		{
			s.replace(found, find.length(), replace);
		}
	}

}

string circle::getPostScript() const
{
	string ret = R"(
		newpath
		4 inch 5.5 inch RAD inch 0 360 arc
		closepath
	)";
	findAndReplace(ret, "RAD", to_string( (int)getRad() ));

	return ret;
}

string rectangle::getPostScript() const 
{						//setting convention now that inch will needs to be defined in postscript file header
	string ret = R"(
		newpath
		4 inch 5.5 inch moveto
		WIDTH inch 0 rlineto
		0 HEIGHT inch rlineto
		0 WIDTH inch sub 0 rlineto
		closepath
	)";

	findAndReplace(ret, "WIDTH", to_string( (int)getWidth() ));
	findAndReplace(ret, "HEIGHT", to_string( (int)getHeight() ));

	return ret;
}

string polygon::getPostScript() const
{
	

	//

}

rotated::rotated(const shape &s, double rotation) : _rotation(rotation), _postScript(s.getPostScript())
{
	//need to limit to 90 degree intervals...or this could handle all rotations technically

	const double pi = 3.1415926;
	const double radians = _rotation * 2.0 * pi / 360.0;

	_origWidth = s.getWidth();
	_origHeight = s.getHeight();

	setWidth(abs(_origWidth * cos(radians) + _origHeight * sin(radians)));
	setHeight(abs(_origHeight * cos(radians) + _origWidth * sin(radians)));


}

//this is much simpler because you just prefix the rotate command
string rotated::getPostScript() const
{
	string ret;

	ret = to_string( (int)_rotation);

	ret += " rotate \n";
	ret += _postScript;

	return ret;
}

int main()
{
	triangle tri(5, 6.0);
	square sq(5, 6.0);
	std::cout << "triangle number of sides " <<tri.getNumSides() << "\n" << "triangle side length "  <<   tri.getSideLength() << std::endl;
	std::cout << "square number of sides " << sq.getNumSides() << std::endl;

	rectangle rect(40,20);
	cout<< "rectangle dimensions " <<rect.getWidth() << " by " << rect.getHeight() << endl;
	cout<< rect.getPostScript() << endl;

	rotated rot(rect, 270);

	cout<< "rotated dimensions (not set up yet) " << rot.getWidth() << " by " << rot.getHeight() << endl;
	cout<< rot.getPostScript() << endl;

	return 0;
}

double circle::getRad() const
{
	return _radius;
}

double polygon::calcCircumRad()
{
	//side length of an n-sided regular polygon with circumradius cr:
	// L = 2*cr*sin(180/n)
	//because we know l and n, solve that equation for cr: 
	// cr = L/(2*sin(180/n))

	return getSideLength() / (2*sin(180/getNumSides()));
}

double polygon::calcInRad()
{
	//inRad ir given sides, and circumRad cr:
	// ir = cr*sin(pi/n)
	//in the c'tor, calcCircumRad is called first.

	return getCircumRad()*sin(PI/getNumSides());
}

double polygon::calcHeight()
{
	//if number of sides is odd, h = ir + cr
	//if number of sides is even, h = cr*2

	int sides = getNumSides();

	if (sides%2 == 0)	//even # of sides
	{
		return getInRad() + getCircumRad();
	}
	else	//odd # of sides
	{
		return getCircumRad()*2;
	}

}

double polygon::calcWidth()
{
	//for n=3, width is side length
	//starting at n=4, every 4 polygons (8, 12, etc.) have width cr*2
	//starting at n=6, every 4 polygons (10, 14, etc.) have width ir*2

	//for the others, the width is the chord of the circular segment 
	//between (n+1)/2 vertices. The angle for this segment is that
	//number of vertices times 360/n (the angle subtening 1 chord, or side)
	// chord length = 2*cr*sin(360/n*numVertices) 

	int n = getNumSides();

	if (n == 3)
	{
		return getSideLength();
	}

	else if (n % 4 == 0)
	{
		return getCircumRad()*2;
	}

	else if (n % 2 == 0)	//this also implies n % 4 != 0, because of the order
	{
		return getInRad()*2;
	}

	else	//n is odd and != 3
	{
		int numVertices = (n + 1)/2;
		double theta = 360/n * numVertices;
		return 2*getCircumRad()*sin(theta);
	}
	
}

double polygon::calcInnerAngles()
{
	//one of the inner angles of a polygon = (180*(n-2))/n
	int n = getNumSides();
	return 180*(n - 2)/n;
}

double polygon::getInRad()
{
	return _inRad;
}

double polygon::getCircumRad()
{
	return _circumRad;
}

int polygon::getNumSides()
{
	return _numSides;
}

double polygon::getSideLength()
{
	return _sideLength;
}
