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

	int getPosX();
	int getPosY();

	int getUnit();
	virtual ~shape() = default;

};

class circle : public shape
{

};

class polygon : public shape
{

};

class rectangle : public shape
{

};

