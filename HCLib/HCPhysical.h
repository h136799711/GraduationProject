#ifndef H_HCPHYSICAL_INCLUDED
#define H_HCPHYSICAL_INCLUDED

#include "HCDDrawLib.h"

class HCPhysical
{
public:
	/*
		Åö×²¼ì²â
	*/
	int Collision_Test(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2);
		
		
	// collision detection functions
	int Find_Bounding_Box_Poly2D(POLYGON2D_PTR poly, 
								 float &min_x, float &max_x, 
								 float &min_y, float &max_y);

};

#endif
