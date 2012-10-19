#include "HCPhysical.h"


int HCPhysical::Find_Bounding_Box_Poly2D(POLYGON2D_PTR poly, 
										 float &min_x, float &max_x, 
										 float &min_y, float &max_y)
{
	// this function finds the bounding box of a 2D polygon 
	// and returns the values in the sent vars
	
	// is this poly valid?
	if (poly->num_verts == 0)
		return(0);
	
	// initialize output vars (note they are pointers)
	// also note that the algorithm assumes local coordinates
	// that is, the poly verts are relative to 0,0
	max_x = max_y = min_x = min_y = 0;
	
	// process each vertex
	for (int index=0; index < poly->num_verts; index++)
    {
		// update vars - run min/max seek
		if (poly->vlist[index].x > max_x)
			max_x = poly->vlist[index].x;
		
		if (poly->vlist[index].x < min_x)
			min_x = poly->vlist[index].x;
		
		if (poly->vlist[index].y > max_y)
			max_y = poly->vlist[index].y;
		
		if (poly->vlist[index].y < min_y)
			min_y = poly->vlist[index].y;
		
	} // end for index
	
	// return success
	return(1);
	
} // end Find_Bounding_Box_Poly2D
int HCPhysical::Collision_Test(int x1, int y1, int w1, int h1, 
							   int x2, int y2, int w2, int h2) 
{
	// this function tests if the two rects overlap
	
	// get the radi of each rect
	int width1  = (w1>>1) - (w1>>3);
	int height1 = (h1>>1) - (h1>>3);
	
	int width2  = (w2>>1) - (w2>>3);
	int height2 = (h2>>1) - (h2>>3);
	
	// compute center of each rect
	int cx1 = x1 + width1;
	int cy1 = y1 + height1;
	
	int cx2 = x2 + width2;
	int cy2 = y2 + height2;
	
	// compute deltas
	int dx = abs(cx2 - cx1);
	int dy = abs(cy2 - cy1);
	
	// test if rects overlap
	if (dx < (width1+width2) && dy < (height1+height2))
		return(1);
	else
		// else no collision
		return(0);
	
} // end Collision_Test
