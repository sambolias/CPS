#include <string>

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
	virtual ~shape() = default;
};
// triangle do polygon contrustor but do it for 3 sides 
class circle : public shape
{
private : 
	double _radius;

public :
	circle(double rad)
	{
		_radius = rad;
	}
	double getWidth();
	double getHeight();
};

class polygon : public shape
{

};

class rectangle : public shape
{

};

