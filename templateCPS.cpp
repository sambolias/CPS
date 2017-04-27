#include "cps.h"
/*
	description of what I did
	took the variables of maxHeight and maxWidht
	sumWidht... etc and put them into the begining of the funciton to
	assign them their proper values then found the sum and max heights and widths
	that I would need later on. Layered really find its new center with 
	layWidth and layHeight and draw like usual

	for horizontal and vertical i split it up into even and odd for the first if conditions
	then inside of even I ran through the for loop like we did for vertical and horizontal stack
	but as i assigned new variable i did a check to see what type we had and then acted accordingly

	Last was the odd and I used the functions we had previous for stackingOdd vertically and horizontally
	but since I ordered them already I could send them off after a quick type check and set postcript variable equal
	to what is returned from those functions following that the last line i set the private data member of shape of 
	_postscript = postscript
*/
template<class T>//h == horizontal v == vertical l == layered
T::T stackHorVert(initializer_list<shared_ptr<shape>> shapes, char type)
{
	int sumWidth = 0;
	int sumHeight = 0;
	int maxHeight,maxWidth;

	for (auto i : shapes) // go through all shapes find newHighest width and height of shapes set it to layered width or height
	{
		// get the biggest box made with the height and width of all the shapes
		// this is needed to get a center point from which everything will be drawn around
		if (getWidth() < i->getWidth())
		{
			maxWidth = i->getWidth(); // get biggest height too
		}
		if (getHeight() < i->getHeight())
		{
			maxHeight = i->getHeight();
		}
		else {
			
		} //
		sumWidth += i->getWidth();
		sumHeight += i->getHeight();
	}
	vector<shared_ptr<shape>> vecShapes(shapes.begin(), shapes.end());


	//layered
	if(type == "l") // layered postscript find the middle
	{
		double xCenterCord = layWidth / 2.0; // we only need to go half way to reach the edge since were in the center
		double yCenterCord = layHeight / 2.0;// do this for y cord as well
		for (int i = 0; i < vecShapes.size(); ++i)
		{
		yCenterCord -= vecShapes[i]->getHeight() / 2.0;
		xCenterCord -= vecShapes[i]->getWidth() / 2.0;
	
		_postSript += draw(*vecShapes[i], 0,0);	//
		}

	}
	
	if(shapes.size()%2==0)	//even number of shapes
		{
		//split an even in half to get 2 odds

			int lt=0,rt=0;

			auto midPtr = shapes.begin()+ shapes.size()/2;


			vector<shared_ptr<shape>> lefts(shapes.begin(), midPtr ),
  						 rights(midPtr, shapes.end());

  			for(int i = 0; i < (int)lefts.size()/2 + 1 ; i++)
  			{
  				if(type == "v")
  				{
  				rt += rights[i]->getHeight();
  				lt -= lefts[lefts.size()-i-1]->getHeight();

  			
  				if(i == lefts.size()/2)
  				{
  					rt -= rights[i]->getHeight()/2;
  					lt += lefts[lefts.size()-i-1]->getHeight()/2;
  				}
  				}
  				if(type == "h")
  				{
  				
  				rt += rights[i]->getWidth();
  				lt -= lefts[lefts.size()-i-1]->getWidth();

  				
  				if(i == lefts.size()/2)
  				{
  					rt -= rights[i]->getWidth()/2;
  					lt += lefts[lefts.size()-i-1]->getWidth()/2;
  				}
  				}
  				
  			}

		}
	else
	{

		if(type== "h") // horizontal odd stack
		{
			vector<shared_ptr<shape>> list(shapes.begin(), shapes.end());
			postScript = horStackOdd(list, 0);
		}
		else // vertical odd stack
		{

			vector<shared_ptr<shape>> list(shapes.begin(), shapes.end());

			postScript = vertStackOdd(list, 0);
		}
		
	}
	_postScript = postSript;
}
