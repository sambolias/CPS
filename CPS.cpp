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

double circle::getWidth() const
{
	return 2*_radius;
}

double circle::getHeight() const
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
