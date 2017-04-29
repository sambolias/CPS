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

//last global non-test function
//nice way of setting postscript strings
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
		0 0 RAD 0 360 arc
	)";

	findAndReplace(ret, "HALFW", to_string( -getWidth()/2 ));
	findAndReplace(ret, "HALFH", to_string( -getHeight()/2 ));
	findAndReplace(ret, "RAD", to_string( getRad() ));

	return ret;
}

string rectangle::getPostScript() const 
{						
	string ret = R"(
		newpath
		HALFW HALFH moveto
		WIDTH 0 rlineto
		0 HEIGHT rlineto
		0 WIDTH sub 0 rlineto
		closepath
	)";

	findAndReplace(ret, "WIDTH", to_string( getWidth() ));
	findAndReplace(ret, "HEIGHT", to_string( getHeight() ));
	findAndReplace(ret, "HALFW", to_string( -getWidth()/2.0 ));	//centers draw
	findAndReplace(ret, "HALFH", to_string( -getHeight()/2.0 ));

	return ret;
}

string polygon::getPostScript() const
{
	//polygons will start drawing at the bottom, since we know all
	//polygons will have a horizontal bottom edge.
	string ret = R"(
		newpath
		XDRAW YDRAW moveto
		1 1 SIDESMINUSONE{
			SIDELENGTH 0 rlineto
			ROTATIONANGLE rotate
		} for

		closepath
	)";

	double sidesminusone = getNumSides() - 1;
	double rotationangle = 180 - getInnerAngle();

	findAndReplace(ret, "XDRAW", to_string(-getSideLength()/2));	//centering
	findAndReplace(ret, "YDRAW", to_string(-getHeight()/2));
	findAndReplace(ret, "SIDELENGTH", to_string(getSideLength()));
	findAndReplace(ret, "SIDESMINUSONE", to_string(sidesminusone));
	findAndReplace(ret, "ROTATIONANGLE", to_string(rotationangle));

	return ret;
}

rotated::rotated(const shape &s, double rotation) : _rotation(rotation), _postScript(s.getPostScript())
{
	//need to limit to 90 degree intervals...or this could handle all rotations technically

	//const double pi = 3.1415926;
	const double radians = _rotation * 2.0 * PI / 360.0;

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

spacer::spacer(double height, double width)
{
	setHeight(height);
	setWidth(width);
}

string spacer::getPostScript() const
{
	return "";
}
//scaled ctor takes a shape and double of scale you would like to apply to shape
//sets height and width to the new scaled size of shape

scaled::scaled(const shape & s, double xScale, double yScale) : _postScript(s.getPostScript())
{
	// get original height and width to multiply by x and y scale values
	_origHeight = s.getHeight();
	_origWidth = s.getWidth();
	// save to private data members of xScale and yScale used in getPostScript
	_xScale = xScale;
	_yScale = yScale;
	//set the width and height of this new shape to original values times the scales of x and y 
	setWidth(_origWidth * xScale);
	setHeight(_origHeight *  yScale);

}
// gets the postscript of the new scaled shape
string scaled::getPostScript() const
{
	string ret = "";
	ret += to_string((double)_xScale);
	ret += " ";
	ret += to_string((double)_yScale);
	ret += " scale \n";
	ret += _postScript;

	return ret;
}

circle::circle(double rad)
{
	_radius = rad;
	setWidth(rad*2);
	setHeight(rad*2);
}

double circle::getRad() const
{
	return _radius;
}

double polygon::calcCircumRad()
{
	//side length of an n-sided regular polygon with circumradius cr:
	// L = 2*cr*sin(180/n)
	//because we know L and n, solve that equation for cr: 
	// cr = L/(2*sin(180/n))

	return getSideLength() / (2*sin(180/getNumSides() * PI/180));
}

double polygon::calcInRad()
{
	//inRad ir given sides, and circumRad cr:
	// ir = cr*cos(pi/n)
	//in the c'tor, calcCircumRad is called first.
	//this formula was already in radians

	return getCircumRad()*cos(PI/getNumSides());
}

double polygon::calcHeight()
{
	//note: the following statements assume flat side on the bottom
	//if number of sides is odd, h = ir + cr
	//if number of sides is even, h = ir*2

	int sides = getNumSides();

	if (sides%2 == 0)	//even # of sides
	{
		return getInRad()*2;		
	}
	else	//odd # of sides
	{
		return getInRad() + getCircumRad();
	}

}

double polygon::calcWidth()
{
	//note: the following statements assume flat side on the bottom

	//for n=3, width is side length
	//starting at n=4, every 4 polygons (8, 12, etc.) have width ir*2
	//starting at n=6, every 4 polygons (10, 14, etc.) have width cr*2
	
	//for the odds, the width is the chord of the circular segment 
	//between (n+1)/2 vertices. The angle for this segment is that
	//number-1 sides times 360/n (the angle subtending a side)
	// chord length = 2*cr*sin(360/n*(numVertices-1)/2) = width

	int n = getNumSides();

	if (n == 3)
	{
		return getSideLength();
	}

	else if (n % 4 == 0)
	{
		return getInRad()*2;
	}

	else if (n % 2 == 0)	//also implies n % 4 != 0
	{
		return getCircumRad()*2;
	}

	else	//n is odd and != 3
	{
		int numVertices = (n + 1)/2;
		double theta = 360/n * (numVertices-1);
		return 2*getCircumRad()*sin(theta/2 * PI/180);
	}
	
}

polygon::polygon(double numSides, double sideLength)
{
	_numSides = (int)numSides;
	_sideLength = sideLength;
	_circumRad = calcCircumRad();	//radius of circle containing each vertex
	_inRad = calcInRad();			//radius of circle tangent to each side
	_innerAngle = calcInnerAngle();	//inner angle at each vertex
	setHeight(calcHeight());
	setWidth(calcWidth());
}

double polygon::calcInnerAngle()
{
	//one of the inner angles of a polygon = (180*(n-2))/n
	int n = getNumSides();
	return 180*(n - 2)/n;
}

double polygon::getInRad() const
{
	return _inRad;
}

double polygon::getCircumRad() const
{
	return _circumRad;
}

double polygon::getInnerAngle() const
{
	return _innerAngle;
}

int polygon::getNumSides() const
{
	return _numSides;
}

double polygon::getSideLength() const
{
	return _sideLength;
}

//another global test function that needs moved eventually
string shape::draw(const shape &s, int x, int y)
{
	string ret;

	ret += "gsave \n";
	ret += to_string(  x) + "  " + to_string( y) + "  translate\n"; // translate might want to be moveto
	ret += s.getPostScript();
	ret += "\n stroke \n grestore \n";

	return ret;
}

mandelbrot::mandelbrot(int width, int height )
{
	int ht = height;
	int wid = width;

	for(int y=0; y < ht; y+=1)
	{
		for(int x=0; x<wid; x+=1)
		{
			//this code is taken from Dr. Lawlor's assembly 301 class
			//the math is still a bit beyond me
	
			float fx = x*(1.0 / wid), fy = y*(1.0 / ht);
			float scale = 1.0; // amount of the mandelbrot set to draw
			fx *= scale; fy *= scale;

			float ci = fy, cr = fx; // complex constant: x,y coordinates
			float zi = ci, zr = cr; // complex number to iterate
			int iter;
			for (iter = 0; iter<100; iter++) 
			{
				if (zi*zi + zr*zr>4.0) break; // number too big--stop iterating
											  // z = z*z + c
				float zr_new = zr*zr - zi*zi + cr;
				float zi_new = 2 * zr*zi + ci;
				zr = zr_new; zi = zi_new;
			}

			////////////////////////////////////////////////////////////

			int r = zr/1.0;
			int g = zi/2.0;
			int b = 0;

			pixel p(r, g, b);

			_postScript += draw(p, x, y);
		
		}
	}
}

string mandelbrot::getPostScript() const
{
	return _postScript;
}

pixel::pixel(double r, double g, double b)
{
	//too many squares causes stack overflow
	//polygon temp(4,2);
	circle temp(1);
	_postScript = temp.getPostScript();
	_postScript += "\n" + to_string(r) + " " + to_string(g) + " " + to_string(b) + " setrgbcolor\n";
	_postScript += "\nfill\n";
}

string pixel::getPostScript() const
{
	return _postScript;
}

string vertical::vertStackOdd(const vector<shared_ptr<shape>> &shapes, int offset)
{
	string ret;


	int mid = shapes.size()/2;

	int upOffset = shapes[mid]->getHeight()/2.0, dnOffset = shapes[mid]->getHeight()/2.0;

	//draw middle shape
	ret = draw(*shapes[mid], 0, offset);

	//draw surrounding shapes
	for(int i = 1; i <= mid; i++)
	{

		upOffset += shapes[mid+i]->getHeight()/2.0;
		dnOffset += shapes[mid-i]->getHeight()/2.0;

		ret += draw(*shapes[mid+i],0, upOffset+offset);
		ret += draw(*shapes[mid-i], 0,-dnOffset+offset);		

		upOffset += shapes[mid+i]->getHeight()/2.0;
		dnOffset += shapes[mid-i]->getHeight()/2.0;

	}

	return ret;
}


vertical::vertical(initializer_list<shared_ptr<shape>> shapes)
{
	double maxWidth = 0, sumHeight = 0;

	for(auto s : shapes)
	{
		
		sumHeight += s->getHeight();
		if(s->getWidth() > maxWidth)
			maxWidth = s->getWidth();
	}

	setWidth(maxWidth);
	setHeight(sumHeight);




	if(shapes.size()%2==0)	//even number of shapes
	{
		//split an even in half to get 2 odds

		int lt=0,rt=0;

		auto midPtr = shapes.begin()+ shapes.size()/2;


		vector<shared_ptr<shape>> lefts(shapes.begin(), midPtr ),
  						 rights(midPtr, shapes.end());

  		for(int i = 0; i < (int)lefts.size()/2 + 1 ; i++)
  		{
  			rt += rights[i]->getHeight();
  			lt -= lefts[lefts.size()-i-1]->getHeight();

  			//bad form
  			if(i == lefts.size()/2)
  			{
  				rt -= rights[i]->getHeight()/2;
  				lt += lefts[lefts.size()-i-1]->getHeight()/2;
  			}
  		}

		_postScript = vertStackOdd(lefts, lt) + vertStackOdd(rights, rt);

	}
	else
	{

		vector<shared_ptr<shape>> list(shapes.begin(), shapes.end());

		_postScript = vertStackOdd(list, 0);
	}


}

string horizontal::horStackOdd(const vector<shared_ptr<shape>> &shapes, int offset)
{
	string ret;


	int mid = shapes.size()/2;

	int rtOffset = shapes[mid]->getWidth()/2.0, ltOffset = shapes[mid]->getWidth()/2.0;

	//draw middle shape
	ret = draw(*shapes[mid], offset, 0);

	//draw surrounding shapes
	for(int i = 1; i <= mid; i++)
	{

		rtOffset += shapes[mid+i]->getWidth()/2.0;
		ltOffset += shapes[mid-i]->getWidth()/2.0;

		ret += draw(*shapes[mid+i], rtOffset+offset, 0);
		ret += draw(*shapes[mid-i], -ltOffset+offset, 0);		

		rtOffset += shapes[mid+i]->getWidth()/2.0;
		ltOffset += shapes[mid-i]->getWidth()/2.0;

	}

	return ret;
}

horizontal::horizontal(initializer_list<shared_ptr<shape>> shapes)
{
	double sumWidth = 0, maxHeight = 0;

	for(auto s : shapes)
	{
		
		sumWidth += s->getWidth();
		if(s->getHeight() > maxHeight)
			maxHeight = s->getHeight();
	}

	setWidth(sumWidth);
	setHeight(maxHeight);




	if(shapes.size()%2==0)	//even number of shapes
	{
		//split an even in half to get 2 odds

		int lt=0,rt=0;

		auto midPtr = shapes.begin()+ shapes.size()/2;


		vector<shared_ptr<shape>> lefts(shapes.begin(), midPtr ),
  						 rights(midPtr, shapes.end());

  		for(int i = 0; i < (int)lefts.size()/2 + 1 ; i++)
  		{
  			rt += rights[i]->getWidth();
  			lt -= lefts[lefts.size()-i-1]->getWidth();

  			//bad form
  			if(i == lefts.size()/2)
  			{
  				rt -= rights[i]->getWidth()/2;
  				lt += lefts[lefts.size()-i-1]->getWidth()/2;
  			}
  		}

		_postScript = horStackOdd(lefts, lt) + horStackOdd(rights, rt);

	}
	else
	{

		vector<shared_ptr<shape>> list(shapes.begin(), shapes.end());

		_postScript = horStackOdd(list, 0);
	}


}

layered::layered(initializer_list<shared_ptr<shape>> shapes)
{
	for (auto i : shapes) // go through all shapes find newHighest width and height of shapes set it to layered width or height
	{
		// get the biggest box made with the height and width of all the shapes
		// this is needed to get a center point from which everything will be drawn around
		if (getWidth() < i->getWidth())
		{
			setWidth(i->getWidth()); // get biggest height too
		}
		if (getHeight() < i->getHeight())
		{
			setHeight(i->getHeight());
		}
		else {} // if not new tallest then go to next one
	}
	vector<shared_ptr<shape>> vecShapes(shapes.begin(), shapes.end());
	string postSript;
	double xCenterCord = getWidth() / 2.0; // we only need to go half way to reach the edge since were in the center
	xCenterCord += 144;
	double yCenterCord = getHeight() / 2.0;// do this for y cord as well
	yCenterCord += 144;

	for (int i = 0; i < vecShapes.size(); ++i)
	{
		yCenterCord -= vecShapes[i]->getHeight() / 2.0;
		xCenterCord -= vecShapes[i]->getWidth() / 2.0;
	//	postSript += draw(*vecShapes[i], xCenterCord, yCenterCord); // these are the maxWidths and maxHeights of all the shapes in the list
		postSript += draw(*vecShapes[i], 0,0);	//check out my test with rects and circle, this way they are all centered
	}
	_postScript = postSript;
}

string horizontal::getPostScript() const
{
	return _postScript;
}

string vertical::getPostScript() const
{
	return _postScript;
}
string layered::getPostScript() const
{
	return _postScript;
}
void page::drawTo(const shape &s, int x, int y)
{
	_postScript += "gsave \n";
	_postScript += to_string( x) + " " + to_string( y) + " translate\n";
	_postScript += s.getPostScript();
	_postScript += "\n stroke \n grestore \n";
}

string page::getPostScript() 
{ 
	return _postScript;
}

void output::addPage(const page &p)
{
	pages.push_back(p);
}

void output::outputFile(string fname)
{
	ofstream ofs(fname);
	ofs << "%%PS-Adobe-2.0" << "\n"; 
	ofs << "%%Pages: " << pages.size() << "\n"; // sets up amount of pages total
	ofs<<"%1 \n /inch {72 mul} def \n";
	int pageNum = 1;
	for(auto i : pages)
	{
		ofs << "%%Page: " << pageNum << " " << pageNum << "\n"; // as each object puts ps it will make a new corresponding page
		ofs<<i.getPostScript();
		ofs<<" \n showpage \n";
		++pageNum;
	}

	ofs.close();
	cout<<"Output Successful\n";
}

void testShapes(void)
{
	rectangle rec1(30, 20);
	square sq1(20, 20);
	circle circ1(20);
	triangle tri(3, 20);
	polygon penta(5, 30);
	//draw all standard shapes
	page stdShapes;
	stdShapes.drawTo(rec1, 40, 40);
	stdShapes.drawTo(sq1, 100, 100);
	stdShapes.drawTo(circ1, 200, 200);
	stdShapes.drawTo(tri, 250, 200);
	stdShapes.drawTo(penta, 144, 144);

	rectangle rect(40, 20);
	rotated rot(tri, 270);
	rotated rotPent(penta, 15);
	scaled sca(rect, 3, 2);
	scaled scaCirc(circ1, 2, 1);

	// scaled and rotated shapes
	page scaledRotatedShapes;
	scaledRotatedShapes.drawTo(rot, 60, 60);
	scaledRotatedShapes.drawTo(tri, 144, 144);
	scaledRotatedShapes.drawTo(scaCirc, 180, 180);
	scaledRotatedShapes.drawTo(sca, 220, 100);
	scaledRotatedShapes.drawTo(rotPent, 220, 40);

	auto a = make_shared<rectangle>(40, 20);
	auto b = make_shared<rotated>(rect, 220);
	auto c = make_shared<rectangle>(40, 20);
	auto d = make_shared<circle>(20);
	auto s = make_shared<spacer>(40, 40);
	auto p = make_shared<polygon>(5, 30);
	// vertical, horizontal and layered objects
	vertical vert{ b,a,c,s,d,p,d };
	horizontal hor{ b,a,d,p,d };

	//heavy compound shape example
	auto la = make_shared<layered>(initializer_list<shared_ptr<shape>>{b, d});
	auto v2 = make_shared<vertical>(initializer_list<shared_ptr<shape>>{b, d, la});
	auto sl = make_shared<scaled>(rotPent, 3, 3);
	auto hl = make_shared<layered>(initializer_list<shared_ptr<shape>>{d,sl,v2});
	auto compound = make_shared<horizontal>(initializer_list<shared_ptr<shape>>{a,p,hl});
	rotated r(*compound, 45);
	

	auto triLay = make_shared<triangle>(40, 50);
	auto squareLay = make_shared<square>(20, 20);
	auto circleLay = make_shared<circle>(20);
	layered lay{ triLay,squareLay,circleLay };

	//page to hold all of our compounded shapes test
	page compundedShapes;
	compundedShapes.drawTo(lay, 144, 144);
	compundedShapes.drawTo(vert, 60, 140);
	compundedShapes.drawTo(hor, 120, 320);
	compundedShapes.drawTo(r, 150, 500);

	//ctor args are width and height
	//if this causes a stack overflow
	//on other machines make it smaller
	mandelbrot man(72*8.5, 72*11);
	page customShape;
	customShape.drawTo(man, 0, 0);

	output of;
	of.addPage(stdShapes);
	of.addPage(scaledRotatedShapes);
	of.addPage(compundedShapes);
	of.addPage(customShape);
	of.outputFile("test.ps");
}

// this is our testing that we did as we worked through this project
// Just a history of how we were continuously checking that output was 
// from the functions and classes.
void developmentTest(void)
{
	cout << "//////////" << "\n" << "layered postScript" << endl;
	auto triLay = make_shared<triangle>(40, 20);
	auto squareLay = make_shared<square>(20, 20);
	auto squar = make_shared<square>(20, 20);
	cout << triLay->getPostScript();
	layered lay{ triLay,squareLay };
	cout << lay.getPostScript() << endl;
	cout << "end of layerd input" << endl;

	rectangle rectScale(20, 20);
	cout << "we are goign to scale this rectangle" << endl;
	cout << " current dimensions are: " << rectScale.getWidth() << " by " << rectScale.getHeight() << endl;
	scaled sca(rectScale, 2.0, 1.0);

	cout << "new dimensions are: " << sca.getWidth() << " by " << sca.getHeight() << endl;
	cout << sca.getPostScript() << endl;

	rectangle rect(40, 20);
	cout << "rectangle dimensions " << rect.getWidth() << " by " << rect.getHeight() << endl;
	cout << rect.getPostScript() << endl;

	rotated rot(rect, 270);

	cout << "rotated dimensions " << rot.getWidth() << " by " << rot.getHeight() << endl;
	cout << rot.getPostScript() << endl;

	cout << "Testing vertical stack\n";

	auto a = make_shared<rectangle>(40, 20);
	auto b = make_shared<rotated>(rect, 220);
	auto c = make_shared<rectangle>(40, 20);
	auto d = make_shared<circle>(20);
	auto s = make_shared<spacer>(40, 40);
	auto p = make_shared<polygon>(5, 30);
	auto la = make_shared<layered>(initializer_list<shared_ptr<shape>>{a, b, d});
	auto v2 = make_shared<vertical>(initializer_list<shared_ptr<shape>>{a, b, d, la, p});

	vertical vert{ b,a,c,s,d,p,la };
	cout << vert.getPostScript() << endl;

	horizontal hor{ b,a,c,d,d,v2 };

	layered l{ a, b, d };

	pixel pix(.5, .5, .5);

	page test;
	test.drawTo(pix, 20, 20);
	test.drawTo(rect, 4 * 72, 2 * 72);
	test.drawTo(rot, 4 * 72, 4 * 72);
	test.drawTo(vert, 2 * 72, 2 * 72);

	page test2;
	test2.drawTo(lay, 4 * 72, 6 * 72);
	test2.drawTo(l, 2 * 72, 7 * 72);
	test2.drawTo(hor, 2 * 72, 4 * 72);
	cout << "\ndraw shapes to page \n";
	//cout<<test.getPostScript() <<endl;


	mandelbrot mandle(72*8.5, 72*11);

	page test3;
	test3.drawTo(mandle, 0, 0);

	output of;
	of.addPage(test);
	of.addPage(test2);
	of.addPage(test3);
	cout << "testing file output \n";
	of.outputFile("test.ps");
}

int main()
{
	testShapes();
	//developmentTest();
	return 0;
}
