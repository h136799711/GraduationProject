#include "HCDDrawLib.h"

int HCDDrawLib::Scale_Polygon2D_Mat(POLYGON2D_PTR poly, float sx, float sy)
{
	// this function scalesthe local coordinates of the polygon
	
	// test for valid pointer
	if (!poly)
		return(0);
	
	
	MATRIX3X2 ms; // used to hold scaling transform matrix
	HCMath hcmath;
	// initialize the matrix with translation values dx dy
	hcmath.Mat_Init_3X2(&ms,sx,0, 
		0,sy, 
		0, 0); 
	
	
	// loop and scale each point
	for (int curr_vert = 0; curr_vert < poly->num_verts; curr_vert++)
    {
		// scale and store result back
		
		// create a 1x2 matrix to do the transform
		MATRIX1X2 p0 = {poly->vlist[curr_vert].x, poly->vlist[curr_vert].y};
		MATRIX1X2 p1 = {0,0}; // this will hold result
		
		// now scale via a matrix multiply
		hcmath.Mat_Mul_1X2_3X2(&p0, &ms, &p1);
		
		// now copy the result back into vertex
		poly->vlist[curr_vert].x = p1.M[0];
		poly->vlist[curr_vert].y = p1.M[1];
		
    } // end for curr_vert
	
	// return success
	return(1);
	
} // end Scale_Polygon2D_Mat
int HCDDrawLib::Rotate_Polygon2D_Mat(POLYGON2D_PTR poly, int theta)
{
	// this function rotates the local coordinates of the polygon
	
	// test for valid pointer
	if (!poly)
		return(0);
	
	// test for negative rotation angle
	if (theta < 0)
		theta+=360;
	
	MATRIX3X2 mr; // used to hold rotation transform matrix
	HCMath hcmath;
	// initialize the matrix with translation values dx dy
	hcmath.Mat_Init_3X2(&mr,cos_look[theta],sin_look[theta], 
		-sin_look[theta],cos_look[theta], 
		0, 0); 
	
	// loop and rotate each point, very crude, no lookup!!!
	for (int curr_vert = 0; curr_vert < poly->num_verts; curr_vert++)
    {
		// create a 1x2 matrix to do the transform
		MATRIX1X2 p0 = {poly->vlist[curr_vert].x, poly->vlist[curr_vert].y};
		MATRIX1X2 p1 = {0,0}; // this will hold result
		
		// now rotate via a matrix multiply
		hcmath.Mat_Mul_1X2_3X2(&p0, &mr, &p1);
		
		// now copy the result back into vertex
		poly->vlist[curr_vert].x = p1.M[0];
		poly->vlist[curr_vert].y = p1.M[1];
		
    } // end for curr_vert
	
	// return success
	return(1);
	
} // end Rotate_Polygon2D_Mat


int HCDDrawLib::Translate_Polygon2D_Mat(POLYGON2D_PTR poly, int dx, int dy)
{
	// this function translates the center of a polygon by using a matrix multiply
	// on the the center point, this is incredibly inefficient, but for educational purposes
	// if we had an object that wasn't in local coordinates then it would make more sense to
	// use a matrix, but since the origin of the object is at x0,y0 then 2 lines of code can
	// translate, but lets do it the hard way just to see :)
	
	// test for valid pointer
	if (!poly)
		return(0);
	HCMath hcmath;
	MATRIX3X2 mt; // used to hold translation transform matrix
	
	// initialize the matrix with translation values dx dy
	hcmath.Mat_Init_3X2(&mt,1,0, 0,1, dx, dy); 
	
	// create a 1x2 matrix to do the transform
	MATRIX1X2 p0 = {poly->x0, poly->y0};
	MATRIX1X2 p1 = {0,0}; // this will hold result
	
	// now translate via a matrix multiply
	hcmath.Mat_Mul_1X2_3X2(&p0, &mt, &p1);
	
	// now copy the result back into polygon
	poly->x0 = p1.M[0];
	poly->y0 = p1.M[1];
	
	// return success
	return(1);
	
} // end Translate_Polygon2D_Mat
int HCDDrawLib::Draw_Polygon2D(POLYGON2D_PTR poly, UCHAR *vbuffer, int lpitch)
{
	// this function draws a POLYGON2D based on 
	
	// test if the polygon is visible
	if (poly->state)
	{
		// loop thru and draw a line from vertices 1 to n
		for (int index=0; index < poly->num_verts-1; index++)
        {
			// draw line from ith to ith+1 vertex
			Draw_Clip_Line(poly->vlist[index].x+poly->x0, 
				poly->vlist[index].y+poly->y0,
				poly->vlist[index+1].x+poly->x0, 
				poly->vlist[index+1].y+poly->y0,
				poly->color,
				vbuffer, lpitch);
			
        } // end for
		
		// now close up polygon
		// draw line from last vertex to 0th
		Draw_Clip_Line(poly->vlist[0].x+poly->x0, 
			poly->vlist[0].y+poly->y0,
			poly->vlist[index].x+poly->x0, 
			poly->vlist[index].y+poly->y0,
			poly->color,
			vbuffer, lpitch);
		
		// return success
		return(1);
	} // end if
	else 
		return(0);
	
} // end Draw_Polygon2D


int HCDDrawLib::Draw_Polygon2D16(POLYGON2D_PTR poly, UCHAR *vbuffer, int lpitch)
{
	// this function draws a POLYGON2D based on 
	
	// test if the polygon is visible
	if (poly->state)
	{
		// loop thru and draw a line from vertices 1 to n
		for (int index=0; index < poly->num_verts-1; index++)
        {
			// draw line from ith to ith+1 vertex
			Draw_Clip_Line16(poly->vlist[index].x+poly->x0, 
				poly->vlist[index].y+poly->y0,
				poly->vlist[index+1].x+poly->x0, 
				poly->vlist[index+1].y+poly->y0,
				poly->color,
				vbuffer, lpitch);
			
        } // end for
		
		// now close up polygon
		// draw line from last vertex to 0th
		Draw_Clip_Line16(poly->vlist[0].x+poly->x0, 
			poly->vlist[0].y+poly->y0,
			poly->vlist[index].x+poly->x0, 
			poly->vlist[index].y+poly->y0,
			poly->color,
			vbuffer, lpitch);
		
		// return success
		return(1);
	} // end if
	else 
		return(0);
	
} // end Draw_Polygon2D16



int HCDDrawLib::Scale_Polygon2D(POLYGON2D_PTR poly, float sx, float sy)
{
	// this function scalesthe local coordinates of the polygon
	
	// test for valid pointer
	if (!poly)
		return(0);
	
	// loop and scale each point
	for (int curr_vert = 0; curr_vert < poly->num_verts; curr_vert++)
    {
		// scale and store result back
		poly->vlist[curr_vert].x *= sx;
		poly->vlist[curr_vert].y *= sy;
		
    } // end for curr_vert
	
	// return success
	return(1);
	
} // end Scale_Polygon2D

int HCDDrawLib::Rotate_Polygon2D(POLYGON2D_PTR poly, int theta)
{
	// this function rotates the local coordinates of the polygon
	
	// test for valid pointer
	if (!poly)
		return(0);
	
	// test for negative rotation angle
	if (theta < 0)
		theta+=360;

	// loop and rotate each point, very crude, no lookup!!!
	for (int curr_vert = 0; curr_vert < poly->num_verts; curr_vert++)
    {
		
		// perform rotation
		float xr = (float)poly->vlist[curr_vert].x*cos_look[theta] - 
			(float)poly->vlist[curr_vert].y*sin_look[theta];
		
		float yr = (float)poly->vlist[curr_vert].x*sin_look[theta] + 
			(float)poly->vlist[curr_vert].y*cos_look[theta];
		
		// store result back
		poly->vlist[curr_vert].x = xr;
		poly->vlist[curr_vert].y = yr;
		
    } // end for curr_vert
	
	// return success
	return(1);
	
} // end Rotate_Polygon2D
int HCDDrawLib::Translate_Polygon2D(POLYGON2D_PTR poly, int dx, int dy)
{
	// this function translates the center of a polygon
	
	// test for valid pointer
	if (!poly)
		return(0);
	
	// translate
	poly->x0+=dx;
	poly->y0+=dy;
	
	// return success
	return(1);
	
} // end Translate_Polygon2D

void HCDDrawLib::Draw_Filled_Polygon2D16(POLYGON2D_PTR poly, UCHAR *_vbuffer, int mempitch)
{
	// this function draws a general n sided polygon 
	
	int ydiff1, ydiff2,         // difference between starting x and ending x
		xdiff1, xdiff2,         // difference between starting y and ending y
		start,                  // starting offset of line between edges
		length,                 // distance from edge 1 to edge 2
		errorterm1, errorterm2, // error terms for edges 1 & 2
		offset1, offset2,       // offset of current pixel in edges 1 & 2
		count1, count2,         // increment count for edges 1 & 2
		xunit1, xunit2;         // unit to advance x offset for edges 1 & 2
	
	
	// recast vbuffer into short version since this is a 16 bit mode
	USHORT *vbuffer = (USHORT *)_vbuffer;
	
	// convert mempitch into WORD or 16bit stride
	mempitch = (mempitch >> 1);
	
	// initialize count of number of edges drawn:
	int edgecount = poly->num_verts-1;
	
	// determine which vertex is at top of polygon:
	
	int firstvert=0;         // start by assuming vertex 0 is at top
	
	int min_y=poly->vlist[0].y; // find y coordinate of vertex 0
	
	for (int index=1; index < poly->num_verts; index++) 
    {  
		// Search thru vertices
		if ((poly->vlist[index].y) < min_y) 
        {  
			// is another vertex higher?
			firstvert=index;                   
			min_y=poly->vlist[index].y;
		} // end if
		
	} // end for index
	
	// finding starting and ending vertices of first two edges:
	int startvert1=firstvert;      // get starting vertex of edge 1
	int startvert2=firstvert;      // get starting vertex of edge 2
	int xstart1=poly->vlist[startvert1].x+poly->x0;
	int ystart1=poly->vlist[startvert1].y+poly->y0;
	int xstart2=poly->vlist[startvert2].x+poly->x0;
	int ystart2=poly->vlist[startvert2].y+poly->y0;
	int endvert1=startvert1-1;           // get ending vertex of edge 1
	
	if (endvert1 < 0) 
		endvert1=poly->num_verts-1;    // check for wrap
	
	int xend1=poly->vlist[endvert1].x+poly->x0;      // get x & y coordinates
	int yend1=poly->vlist[endvert1].y+poly->y0;      // of ending vertices
	int endvert2=startvert2+1;           // get ending vertex of edge 2
	
	if (endvert2==(poly->num_verts)) 
		endvert2=0;  // Check for wrap
	
	int xend2=poly->vlist[endvert2].x+poly->x0;      // get x & y coordinates
	int yend2=poly->vlist[endvert2].y+poly->y0;      // of ending vertices
	
	// draw the polygon:
	
	while (edgecount>0) 
	{    
		// continue drawing until all edges drawn
		offset1=mempitch*ystart1+xstart1;  // offset of edge 1
		offset2=mempitch*ystart2+xstart2;  // offset of edge 2
		
		// initialize error terms
		// for edges 1 & 2
		errorterm1=0;        
		errorterm2=0;           
		
		// get absolute value of
		if ((ydiff1=yend1-ystart1) < 0) 
			ydiff1=-ydiff1;
		
		// x & y lengths of edges
		if ((ydiff2=yend2-ystart2) < 0) 
			ydiff2=-ydiff2; 
		
		if ((xdiff1=xend1-xstart1) < 0) 
		{               
			// get value of length
			xunit1=-1;                    // calculate X increment
			xdiff1=-xdiff1;
		} // end if
		else 
		{
			xunit1=1;
		} // end else
		
		if ((xdiff2=xend2-xstart2) < 0) 
		{
			// Get value of length
			xunit2=-1;                   // calculate X increment
			xdiff2=-xdiff2;
		} // end else
		else 
		{
			xunit2=1;
		} // end else
		
		// choose which of four routines to use
		if (xdiff1 > ydiff1) 
		{    
			// if x length of edge 1 is greater than y length
			if (xdiff2 > ydiff2) 
            {  
				// if X length of edge 2 is greater than y length
				
				// increment edge 1 on X and edge 2 on X:
				count1=xdiff1;    // count for x increment on edge 1
				count2=xdiff2;    // count for x increment on edge 2
				
				while (count1 && count2) 
				{  
					// continue drawing until one edge is done
					// calculate edge 1:
					while ((errorterm1 < xdiff1) && (count1 > 0)) 
					{ 
                        // finished w/edge 1?
						if (count1--) 
						{     
							// count down on edge 1
							offset1+=xunit1;  // increment pixel offset
							xstart1+=xunit1;
						} // end if
						
						errorterm1+=ydiff1; // increment error term
						
						if (errorterm1 < xdiff1) 
						{  // if not more than XDIFF
							vbuffer[offset1]=(USHORT)poly->color; // ...plot a pixel
						} // end if
						
					} // end while
					
					errorterm1-=xdiff1; // if time to increment X, restore error term
					
					// calculate edge 2:
					
					while ((errorterm2 < xdiff2) && (count2 > 0)) 
					{  
                        // finished w/edge 2?
						if (count2--) 
						{     
							// count down on edge 2
							offset2+=xunit2;  // increment pixel offset
							xstart2+=xunit2;
						} // end if
						
						errorterm2+=ydiff2; // increment error term
						
						if (errorterm2 < xdiff2) 
						{  // if not more than XDIFF
							vbuffer[offset2]=(USHORT)poly->color;  // ...plot a pixel
						} // end if
						
					} // end while
					
					errorterm2-=xdiff2; // if time to increment X, restore error term
					
					// draw line from edge 1 to edge 2:
					
					length=offset2-offset1; // determine length of horizontal line
					
					if (length < 0) 
					{ // if negative...
						length=-length;       // make it positive
						start=offset2;        // and set START to edge 2
					} // end if
					else 
						start=offset1;     // else set START to edge 1
					
					for (int index=start; index < start+length+1; index++)
					{  // From edge to edge...
						vbuffer[index]=(USHORT)poly->color;         // ...draw the line
					} // end for index
					
					offset1+=mempitch;           // advance edge 1 offset to next line
					ystart1++;
					offset2+=mempitch;           // advance edge 2 offset to next line
					ystart2++;
					
				} // end if
				
			} // end if
			else 
            {
				// increment edge 1 on X and edge 2 on Y:
				count1=xdiff1;    // count for X increment on edge 1
				count2=ydiff2;    // count for Y increment on edge 2
				
				while (count1 && count2) 
				{  // continue drawing until one edge is done
					// calculate edge 1:
					while ((errorterm1 < xdiff1) && (count1 > 0)) 
					{ // finished w/edge 1?
						if (count1--) 
						{
							// count down on edge 1
							offset1+=xunit1;  // increment pixel offset
							xstart1+=xunit1;
						} // end if
						
						errorterm1+=ydiff1; // increment error term
						
						if (errorterm1 < xdiff1) 
						{  // If not more than XDIFF
							vbuffer[offset1]=(USHORT)poly->color; // ...plot a pixel
						} // end if
						
					} // end while
					
					errorterm1-=xdiff1; // If time to increment X, restore error term
					
					// calculate edge 2:
					errorterm2+=xdiff2; // increment error term
					
                    if (errorterm2 >= ydiff2)  
					{ // if time to increment Y...
						errorterm2-=ydiff2;        // ...restore error term
						offset2+=xunit2;           // ...and advance offset to next pixel
						xstart2+=xunit2;
					} // end if
					
					count2--;
					
					// draw line from edge 1 to edge 2:
					
					length=offset2-offset1; // determine length of horizontal line
					
					if (length < 0)  
					{ // if negative...
						length=-length;       // ...make it positive
						start=offset2;        // and set START to edge 2
					} // end if
					else 
						start=offset1;        // else set START to edge 1
					
					for (int index=start; index < start+length+1; index++)  // from edge to edge
					{
                        vbuffer[index]=(USHORT)poly->color;         // ...draw the line
					} // end for index
					
					offset1+=mempitch;           // advance edge 1 offset to next line
					ystart1++;
					offset2+=mempitch;           // advance edge 2 offset to next line
					ystart2++;
					
				} // end while
			} // end if
		} // end if
		else 
		{
			if (xdiff2 > ydiff2) 
			{
				// increment edge 1 on Y and edge 2 on X:
				
				count1=ydiff1;  // count for Y increment on edge 1
				count2=xdiff2;  // count for X increment on edge 2
				
				while(count1 && count2) 
				{  // continue drawing until one edge is done
					// calculate edge 1:
					
					errorterm1+=xdiff1; // Increment error term
					
					if (errorterm1 >= ydiff1)  
					{  // if time to increment Y...
						errorterm1-=ydiff1;         // ...restore error term
						offset1+=xunit1;            // ...and advance offset to next pixel
						xstart1+=xunit1;
					} // end if
					
					count1--;
					
					// Calculate edge 2:
					
					while ((errorterm2 < xdiff2) && (count2 > 0)) 
					{ // finished w/edge 1?
						if (count2--) 
						{ // count down on edge 2
							offset2+=xunit2;  // increment pixel offset
							xstart2+=xunit2;
						} // end if
						
						errorterm2+=ydiff2; // increment error term
						
						if (errorterm2 < xdiff2) 
						{  // if not more than XDIFF
							vbuffer[offset2]=(USHORT)poly->color; // ...plot a pixel
						} // end if
					} // end while
					
					errorterm2-=xdiff2;  // if time to increment X, restore error term
					
					// draw line from edge 1 to edge 2:
					
					length=offset2-offset1; // determine length of horizontal line
					
					if (length < 0) 
					{    // if negative...
						length=-length;  // ...make it positive
						start=offset2;   // and set START to edge 2
					} // end if
					else 
						start=offset1;  // else set START to edge 1
					
					for (int index=start; index < start+length+1; index++) // from edge to edge...
					{
                        vbuffer[index]=(USHORT)poly->color;      // ...draw the line
					} // end for index
					
					offset1+=mempitch;         // advance edge 1 offset to next line
					ystart1++;
					offset2+=mempitch;         // advance edge 2 offset to next line
					ystart2++;
					
				} // end if
			} // end if
			else 
			{
				// increment edge 1 on Y and edge 2 on Y:
				count1=ydiff1;  // count for Y increment on edge 1
				count2=ydiff2;  // count for Y increment on edge 2
				
				while(count1 && count2) 
				{  
                    // continue drawing until one edge is done
					// calculate edge 1:
					errorterm1+=xdiff1;  // increment error term
					
					if (errorterm1 >= ydiff1)  
					{                           // if time to increment Y
						errorterm1-=ydiff1;         // ...restore error term
						offset1+=xunit1;            // ...and advance offset to next pixel
						xstart1+=xunit1;
					} // end if
					
                    count1--;
					
					// calculate edge 2:
					errorterm2+=xdiff2;            // increment error term
					
					if (errorterm2 >= ydiff2)  
					{                           // if time to increment Y
						errorterm2-=ydiff2;         // ...restore error term
						offset2+=xunit2;            // ...and advance offset to next pixel
						xstart2+=xunit2;
					} // end if
					
					--count2;
					
					// draw line from edge 1 to edge 2:
					
					length=offset2-offset1;  // determine length of horizontal line
					
					if (length < 0) 
					{          
						// if negative...
						length=-length;        // ...make it positive
						start=offset2;         // and set START to edge 2
					} // end if
					else 
						start=offset1;         // else set START to edge 1
					
					for (int index=start; index < start+length+1; index++)   
					{ // from edge to edge
						vbuffer[index]=(USHORT)poly->color;   // ...draw the linee
					} // end for index
					
					offset1+=mempitch;            // advance edge 1 offset to next line
					ystart1++;
					offset2+=mempitch;            // advance edge 2 offset to next line
					ystart2++;
					
				} // end while
				
			} // end else
			
		} // end if
		
		// another edge (at least) is complete. Start next edge, if any.
		if (!count1) 
		{                      // if edge 1 is complete...
			--edgecount;           // decrement the edge count
			startvert1=endvert1;   // make ending vertex into start vertex
			--endvert1;            // and get new ending vertex
			
			if (endvert1 < 0) 
				endvert1=poly->num_verts-1; // check for wrap
			
			xend1=poly->vlist[endvert1].x+poly->x0;  // get x & y of new end vertex
			yend1=poly->vlist[endvert1].y+poly->y0;
		} // end if
		
		if (!count2) 
		{                     // if edge 2 is complete...
			--edgecount;          // decrement the edge count
			startvert2=endvert2;  // make ending vertex into start vertex
			endvert2++;           // and get new ending vertex
			
			if (endvert2==(poly->num_verts)) 
				endvert2=0;                // check for wrap
			
			xend2=poly->vlist[endvert2].x+poly->x0;  // get x & y of new end vertex
			yend2=poly->vlist[endvert2].y+poly->y0;
			
		} // end if
		
	} // end while
	
} // end Draw_Filled_Polygon2D16

void HCDDrawLib::Draw_Filled_Polygon2D(POLYGON2D_PTR poly, UCHAR *vbuffer, int mempitch)
{
	// this function draws a general n sided polygon 
	
	int ydiff1, ydiff2,         // difference between starting x and ending x
		xdiff1, xdiff2,         // difference between starting y and ending y
		start,                  // starting offset of line between edges
		length,                 // distance from edge 1 to edge 2
		errorterm1, errorterm2, // error terms for edges 1 & 2
		offset1, offset2,       // offset of current pixel in edges 1 & 2
		count1, count2,         // increment count for edges 1 & 2
		xunit1, xunit2;         // unit to advance x offset for edges 1 & 2
	
	// initialize count of number of edges drawn:
	int edgecount = poly->num_verts-1;
	
	// determine which vertex is at top of polygon:
	
	int firstvert=0;         // start by assuming vertex 0 is at top
	
	int min_y=poly->vlist[0].y; // find y coordinate of vertex 0
	
	for (int index=1; index < poly->num_verts; index++) 
    {  
		// Search thru vertices
		if ((poly->vlist[index].y) < min_y) 
        {  
			// is another vertex higher?
			firstvert=index;                   
			min_y=poly->vlist[index].y;
		} // end if
		
	} // end for index
	
	// finding starting and ending vertices of first two edges:
	int startvert1=firstvert;      // get starting vertex of edge 1
	int startvert2=firstvert;      // get starting vertex of edge 2
	int xstart1=poly->vlist[startvert1].x+poly->x0;
	int ystart1=poly->vlist[startvert1].y+poly->y0;
	int xstart2=poly->vlist[startvert2].x+poly->x0;
	int ystart2=poly->vlist[startvert2].y+poly->y0;
	int endvert1=startvert1-1;           // get ending vertex of edge 1
	
	if (endvert1 < 0) 
		endvert1=poly->num_verts-1;    // check for wrap
	
	int xend1=poly->vlist[endvert1].x+poly->x0;      // get x & y coordinates
	int yend1=poly->vlist[endvert1].y+poly->y0;      // of ending vertices
	int endvert2=startvert2+1;           // get ending vertex of edge 2
	
	if (endvert2==(poly->num_verts)) 
		endvert2=0;  // Check for wrap
	
	int xend2=poly->vlist[endvert2].x+poly->x0;      // get x & y coordinates
	int yend2=poly->vlist[endvert2].y+poly->y0;      // of ending vertices
	
	// draw the polygon:
	
	while (edgecount>0) 
	{    
		// continue drawing until all edges drawn
		offset1=mempitch*ystart1+xstart1;  // offset of edge 1
		offset2=mempitch*ystart2+xstart2;  // offset of edge 2
		
		// initialize error terms
		// for edges 1 & 2
		errorterm1=0;        
		errorterm2=0;           
		
		// get absolute value of
		if ((ydiff1=yend1-ystart1) < 0) 
			ydiff1=-ydiff1;
		
		// x & y lengths of edges
		if ((ydiff2=yend2-ystart2) < 0) 
			ydiff2=-ydiff2; 
		
		if ((xdiff1=xend1-xstart1) < 0) 
		{               
			// get value of length
			xunit1=-1;                    // calculate X increment
			xdiff1=-xdiff1;
		} // end if
		else 
		{
			xunit1=1;
		} // end else
		
		if ((xdiff2=xend2-xstart2) < 0) 
		{
			// Get value of length
			xunit2=-1;                   // calculate X increment
			xdiff2=-xdiff2;
		} // end else
		else 
		{
			xunit2=1;
		} // end else
		
		// choose which of four routines to use
		if (xdiff1 > ydiff1) 
		{    
			// if x length of edge 1 is greater than y length
			if (xdiff2 > ydiff2) 
            {  
				// if X length of edge 2 is greater than y length
				
				// increment edge 1 on X and edge 2 on X:
				count1=xdiff1;    // count for x increment on edge 1
				count2=xdiff2;    // count for x increment on edge 2
				
				while (count1 && count2) 
				{  
					// continue drawing until one edge is done
					// calculate edge 1:
					while ((errorterm1 < xdiff1) && (count1 > 0)) 
					{ 
                        // finished w/edge 1?
						if (count1--) 
						{     
							// count down on edge 1
							offset1+=xunit1;  // increment pixel offset
							xstart1+=xunit1;
						} // end if
						
						errorterm1+=ydiff1; // increment error term
						
						if (errorterm1 < xdiff1) 
						{  // if not more than XDIFF
							vbuffer[offset1]=(UCHAR)poly->color; // ...plot a pixel
						} // end if
						
					} // end while
					
					errorterm1-=xdiff1; // if time to increment X, restore error term
					
					// calculate edge 2:
					
					while ((errorterm2 < xdiff2) && (count2 > 0)) 
					{  
                        // finished w/edge 2?
						if (count2--) 
						{     
							// count down on edge 2
							offset2+=xunit2;  // increment pixel offset
							xstart2+=xunit2;
						} // end if
						
						errorterm2+=ydiff2; // increment error term
						
						if (errorterm2 < xdiff2) 
						{  // if not more than XDIFF
							vbuffer[offset2]=(UCHAR)poly->color;  // ...plot a pixel
						} // end if
						
					} // end while
					
					errorterm2-=xdiff2; // if time to increment X, restore error term
					
					// draw line from edge 1 to edge 2:
					
					length=offset2-offset1; // determine length of horizontal line
					
					if (length < 0) 
					{ // if negative...
						length=-length;       // make it positive
						start=offset2;        // and set START to edge 2
					} // end if
					else 
						start=offset1;     // else set START to edge 1
					
					for (int index=start; index < start+length+1; index++)
					{  // From edge to edge...
						vbuffer[index]=(UCHAR)poly->color;         // ...draw the line
					} // end for index
					
					offset1+=mempitch;           // advance edge 1 offset to next line
					ystart1++;
					offset2+=mempitch;           // advance edge 2 offset to next line
					ystart2++;
					
				} // end if
				
			} // end if
			else 
            {
				// increment edge 1 on X and edge 2 on Y:
				count1=xdiff1;    // count for X increment on edge 1
				count2=ydiff2;    // count for Y increment on edge 2
				
				while (count1 && count2) 
				{  // continue drawing until one edge is done
					// calculate edge 1:
					while ((errorterm1 < xdiff1) && (count1 > 0)) 
					{ // finished w/edge 1?
						if (count1--) 
						{
							// count down on edge 1
							offset1+=xunit1;  // increment pixel offset
							xstart1+=xunit1;
						} // end if
						
						errorterm1+=ydiff1; // increment error term
						
						if (errorterm1 < xdiff1) 
						{  // If not more than XDIFF
							vbuffer[offset1]=(UCHAR)poly->color; // ...plot a pixel
						} // end if
						
					} // end while
					
					errorterm1-=xdiff1; // If time to increment X, restore error term
					
					// calculate edge 2:
					errorterm2+=xdiff2; // increment error term
					
                    if (errorterm2 >= ydiff2)  
					{ // if time to increment Y...
						errorterm2-=ydiff2;        // ...restore error term
						offset2+=xunit2;           // ...and advance offset to next pixel
						xstart2+=xunit2;
					} // end if
					
					count2--;
					
					// draw line from edge 1 to edge 2:
					
					length=offset2-offset1; // determine length of horizontal line
					
					if (length < 0)  
					{ // if negative...
						length=-length;       // ...make it positive
						start=offset2;        // and set START to edge 2
					} // end if
					else 
						start=offset1;        // else set START to edge 1
					
					for (int index=start; index < start+length+1; index++)  // from edge to edge
					{
                        vbuffer[index]=(UCHAR)poly->color;         // ...draw the line
					} // end for index
					
					offset1+=mempitch;           // advance edge 1 offset to next line
					ystart1++;
					offset2+=mempitch;           // advance edge 2 offset to next line
					ystart2++;
					
				} // end while
			} // end if
		} // end if
		else 
		{
			if (xdiff2 > ydiff2) 
			{
				// increment edge 1 on Y and edge 2 on X:
				
				count1=ydiff1;  // count for Y increment on edge 1
				count2=xdiff2;  // count for X increment on edge 2
				
				while(count1 && count2) 
				{  // continue drawing until one edge is done
					// calculate edge 1:
					
					errorterm1+=xdiff1; // Increment error term
					
					if (errorterm1 >= ydiff1)  
					{  // if time to increment Y...
						errorterm1-=ydiff1;         // ...restore error term
						offset1+=xunit1;            // ...and advance offset to next pixel
						xstart1+=xunit1;
					} // end if
					
					count1--;
					
					// Calculate edge 2:
					
					while ((errorterm2 < xdiff2) && (count2 > 0)) 
					{ // finished w/edge 1?
						if (count2--) 
						{ // count down on edge 2
							offset2+=xunit2;  // increment pixel offset
							xstart2+=xunit2;
						} // end if
						
						errorterm2+=ydiff2; // increment error term
						
						if (errorterm2 < xdiff2) 
						{  // if not more than XDIFF
							vbuffer[offset2]=(UCHAR)poly->color; // ...plot a pixel
						} // end if
					} // end while
					
					errorterm2-=xdiff2;  // if time to increment X, restore error term
					
					// draw line from edge 1 to edge 2:
					
					length=offset2-offset1; // determine length of horizontal line
					
					if (length < 0) 
					{    // if negative...
						length=-length;  // ...make it positive
						start=offset2;   // and set START to edge 2
					} // end if
					else 
						start=offset1;  // else set START to edge 1
					
					for (int index=start; index < start+length+1; index++) // from edge to edge...
					{
                        vbuffer[index]=(UCHAR)poly->color;      // ...draw the line
					} // end for index
					
					offset1+=mempitch;         // advance edge 1 offset to next line
					ystart1++;
					offset2+=mempitch;         // advance edge 2 offset to next line
					ystart2++;
					
				} // end if
			} // end if
			else 
			{
				// increment edge 1 on Y and edge 2 on Y:
				count1=ydiff1;  // count for Y increment on edge 1
				count2=ydiff2;  // count for Y increment on edge 2
				
				while(count1 && count2) 
				{  
                    // continue drawing until one edge is done
					// calculate edge 1:
					errorterm1+=xdiff1;  // increment error term
					
					if (errorterm1 >= ydiff1)  
					{                           // if time to increment Y
						errorterm1-=ydiff1;         // ...restore error term
						offset1+=xunit1;            // ...and advance offset to next pixel
						xstart1+=xunit1;
					} // end if
					
                    count1--;
					
					// calculate edge 2:
					errorterm2+=xdiff2;            // increment error term
					
					if (errorterm2 >= ydiff2)  
					{                           // if time to increment Y
						errorterm2-=ydiff2;         // ...restore error term
						offset2+=xunit2;            // ...and advance offset to next pixel
						xstart2+=xunit2;
					} // end if
					
					--count2;
					
					// draw line from edge 1 to edge 2:
					
					length=offset2-offset1;  // determine length of horizontal line
					
					if (length < 0) 
					{          
						// if negative...
						length=-length;        // ...make it positive
						start=offset2;         // and set START to edge 2
					} // end if
					else 
						start=offset1;         // else set START to edge 1
					
					for (int index=start; index < start+length+1; index++)   
					{ // from edge to edge
						vbuffer[index]=(UCHAR)poly->color;   // ...draw the linee
					} // end for index
					
					offset1+=mempitch;            // advance edge 1 offset to next line
					ystart1++;
					offset2+=mempitch;            // advance edge 2 offset to next line
					ystart2++;
					
				} // end while
				
			} // end else
			
		} // end if
		
		// another edge (at least) is complete. Start next edge, if any.
		if (!count1) 
		{                      // if edge 1 is complete...
			--edgecount;           // decrement the edge count
			startvert1=endvert1;   // make ending vertex into start vertex
			--endvert1;            // and get new ending vertex
			
			if (endvert1 < 0) 
				endvert1=poly->num_verts-1; // check for wrap
			
			xend1=poly->vlist[endvert1].x+poly->x0;  // get x & y of new end vertex
			yend1=poly->vlist[endvert1].y+poly->y0;
		} // end if
		
		if (!count2) 
		{                     // if edge 2 is complete...
			--edgecount;          // decrement the edge count
			startvert2=endvert2;  // make ending vertex into start vertex
			endvert2++;           // and get new ending vertex
			
			if (endvert2==(poly->num_verts)) 
				endvert2=0;                // check for wrap
			
			xend2=poly->vlist[endvert2].x+poly->x0;  // get x & y of new end vertex
			yend2=poly->vlist[endvert2].y+poly->y0;
			
		} // end if
		
	} // end while
	
} // end Draw_Filled_Polygon2D

void HCDDrawLib::Draw_Bottom_TriFP(int x1,int y1, 
								   int x2,int y2, 
								   int x3,int y3,
								   int color,
								   UCHAR *dest_buffer, int mempitch)
{
	
	// this function draws a triangle that has a flat bottom using fixed point math
	
	int dx_right,    // the dx/dy ratio of the right edge of line
		dx_left,     // the dx/dy ratio of the left edge of line
		xs,xe,       // the starting and ending points of the edges
		height;      // the height of the triangle
	
	int temp_x,        // used during sorting as temps
		temp_y,
		right,         // used by clipping
		left;
	
	UCHAR  *dest_addr;
	
	if (y1==y2 || y1==y3)
		return;
	
	// test order of x1 and x2
	if (x3 < x2)
	{
		temp_x = x2;
		x2     = x3;
		x3     = temp_x;
		
	} // end if swap
	
	// compute delta's
	height = y3-y1;
	
	dx_left  = ((x2-x1)<<FIXP16_SHIFT)/height;
	dx_right = ((x3-x1)<<FIXP16_SHIFT)/height;
	
	// set starting points
	xs = (x1<<FIXP16_SHIFT);
	xe = (x1<<FIXP16_SHIFT); 
	
	// perform y clipping
	if (y1<min_clip_y)
	{
		// compute new xs and ys
		xs = xs+dx_left*(-y1+min_clip_y);
		xe = xe+dx_right*(-y1+min_clip_y);
		
		// reset y1
		y1=min_clip_y;
		
	} // end if top is off screen
	
	if (y3>max_clip_y)
		y3=max_clip_y;
	
	// compute starting address in video memory
	dest_addr = dest_buffer+y1*mempitch;
	
	// test if x clipping is needed
	if (x1>=min_clip_x && x1<=max_clip_x &&
		x2>=min_clip_x && x2<=max_clip_x &&
		x3>=min_clip_x && x3<=max_clip_x)
    {
		// draw the triangle
		for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=mempitch)
        {
			memset((UCHAR *)dest_addr+((xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT),
                color, (((xe-xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT)+1));
			
			// adjust starting point and ending point
			xs+=dx_left;
			xe+=dx_right;
			
        } // end for
		
    } // end if no x clipping needed
	else
	{
		// clip x axis with slower version
		
		// draw the triangle
		for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=mempitch)
		{
			// do x clip
			left  = ((xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT);
			right = ((xe+FIXP16_ROUND_UP)>>FIXP16_SHIFT);
			
			// adjust starting point and ending point
			xs+=dx_left;
			xe+=dx_right;
			
			// clip line
			if (left < min_clip_x)
			{
				left = min_clip_x;
				
				if (right < min_clip_x)
					continue;
			}
			
			if (right > max_clip_x)
			{
				right = max_clip_x;
				
				if (left > max_clip_x)
					continue;
			}
			
			memset((UCHAR *)dest_addr+left,
				color, (right-left+1));
			
		} // end for
		
	} // end else x clipping needed
	
} // end Draw_Bottom_TriFP

void HCDDrawLib::Draw_Top_TriFP(int x1,int y1,
								int x2,int y2, 
								int x3,int y3,
								int color, 
								UCHAR *dest_buffer, int mempitch)
{
	// this function draws a triangle that has a flat top using fixed point math
	
	int dx_right,    // the dx/dy ratio of the right edge of line
		dx_left,     // the dx/dy ratio of the left edge of line
		xs,xe,       // the starting and ending points of the edges
		height;      // the height of the triangle
	
	int temp_x,        // used during sorting as temps
		temp_y,
		right,         // used by clipping
		left;
	
	UCHAR  *dest_addr;
	
	// test for degenerate
	if (y1==y3 || y2==y3)
		return;
	
	// test order of x1 and x2
	if (x2 < x1)
	{
		temp_x = x2;
		x2     = x1;
		x1     = temp_x;
	} // end if swap
	
	// compute delta's
	height = y3-y1;
	
	dx_left  = ((x3-x1)<<FIXP16_SHIFT)/height;
	dx_right = ((x3-x2)<<FIXP16_SHIFT)/height;
	
	// set starting points
	xs = (x1<<FIXP16_SHIFT);
	xe = (x2<<FIXP16_SHIFT);
	
	// perform y clipping
	if (y1<min_clip_y)
	{
		// compute new xs and ys
		xs = xs+dx_left*(-y1+min_clip_y);
		xe = xe+dx_right*(-y1+min_clip_y);
		
		// reset y1
		y1=min_clip_y;
		
	} // end if top is off screen
	
	if (y3>max_clip_y)
		y3=max_clip_y;
	
	// compute starting address in video memory
	dest_addr = dest_buffer+y1*mempitch;
	
	// test if x clipping is needed
	if (x1>=min_clip_x && x1<=max_clip_x &&
		x2>=min_clip_x && x2<=max_clip_x &&
		x3>=min_clip_x && x3<=max_clip_x)
    {
		// draw the triangle
		for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=mempitch)
        {
			memset((UCHAR *)dest_addr+((xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT),
				color, (((xe-xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT)+1));
			
			// adjust starting point and ending point
			xs+=dx_left;
			xe+=dx_right;
			
        } // end for
		
    } // end if no x clipping needed
	else
	{
		// clip x axis with slower version
		
		// draw the triangle
		for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=mempitch)
		{
			// do x clip
			left  = ((xs+FIXP16_ROUND_UP)>>16);
			right = ((xe+FIXP16_ROUND_UP)>>16);
			
			// adjust starting point and ending point
			xs+=dx_left;
			xe+=dx_right;
			
			// clip line
			if (left < min_clip_x)
			{
				left = min_clip_x;
				
				if (right < min_clip_x)
					continue;
			}
			
			if (right > max_clip_x)
			{
				right = max_clip_x;
				
				if (left > max_clip_x)
					continue;
			}
			
			memset((UCHAR  *)dest_addr+(unsigned int)left,
				color,(unsigned int)(right-left+1));
			
		} // end for
		
	} // end else x clipping needed
	
} // end Draw_Top_TriFP



void HCDDrawLib::Draw_QuadFP_2D(int x0,int y0,
								int x1,int y1,
								int x2,int y2,
								int x3, int y3,
								int color,
								UCHAR *dest_buffer, int mempitch)
{
	// this function draws a 2D quadrilateral
	
	// simply call the triangle function 2x, let it do all the work
	Draw_TriangleFP_2D(x0,y0,x1,y1,x3,y3,color,dest_buffer,mempitch);
	Draw_TriangleFP_2D(x1,y1,x2,y2,x3,y3,color,dest_buffer,mempitch);
	
} // end Draw_QuadFP_2D

void HCDDrawLib::Draw_Triangle_2D16(int x1,int y1,
									int x2,int y2,
									int x3,int y3,
									int color,
									UCHAR *dest_buffer, int mempitch)
{
	// this function draws a triangle on the destination buffer
	// it decomposes all triangles into a pair of flat top, flat bottom
	
	
	int temp_x, // used for sorting
		temp_y,
		new_x;
	
	// test for h lines and v lines
	if ((x1==x2 && x2==x3)  ||  (y1==y2 && y2==y3))
		return;
	
	// sort p1,p2,p3 in ascending y order
	if (y2<y1)
	{
		temp_x = x2;
		temp_y = y2;
		x2     = x1;
		y2     = y1;
		x1     = temp_x;
		y1     = temp_y;
	} // end if
	
	// now we know that p1 and p2 are in order
	if (y3<y1)
	{
		temp_x = x3;
		temp_y = y3;
		x3     = x1;
		y3     = y1;
		x1     = temp_x;
		y1     = temp_y;
	} // end if
	
	// finally test y3 against y2
	if (y3<y2)
	{
		temp_x = x3;
		temp_y = y3;
		x3     = x2;
		y3     = y2;
		x2     = temp_x;
		y2     = temp_y;
		
	} // end if
	
	// do trivial rejection tests for clipping
	if ( y3<min_clip_y || y1>max_clip_y ||
		(x1<min_clip_x && x2<min_clip_x && x3<min_clip_x) ||
		(x1>max_clip_x && x2>max_clip_x && x3>max_clip_x) )
		return;
	
	// test if top of triangle is flat
	if (y1==y2)
	{
		Draw_Top_Tri16(x1,y1,x2,y2,x3,y3,color, dest_buffer, mempitch);
	} // end if
	else
		if (y2==y3)
		{
			Draw_Bottom_Tri16(x1,y1,x2,y2,x3,y3,color, dest_buffer, mempitch);
		} // end if bottom is flat
		else
		{
			// general triangle that's needs to be broken up along long edge
			new_x = x1 + (int)(0.5+(float)(y2-y1)*(float)(x3-x1)/(float)(y3-y1));
			
			// draw each sub-triangle
			Draw_Bottom_Tri16(x1,y1,new_x,y2,x2,y2,color, dest_buffer, mempitch);
			Draw_Top_Tri16(x2,y2,new_x,y2,x3,y3,color, dest_buffer, mempitch);
			
		} // end else
		
} // end Draw_Triangle_2D16

void HCDDrawLib::Draw_Triangle_2D(int x1,int y1,
								  int x2,int y2,
								  int x3,int y3,
								  int color,
								  UCHAR *dest_buffer, int mempitch)
{
	// this function draws a triangle on the destination buffer
	// it decomposes all triangles into a pair of flat top, flat bottom
	
	int temp_x, // used for sorting
		temp_y,
		new_x;
	
	// test for h lines and v lines
	if ((x1==x2 && x2==x3)  ||  (y1==y2 && y2==y3))
		return;
	
	// sort p1,p2,p3 in ascending y order
	if (y2<y1)
	{
		temp_x = x2;
		temp_y = y2;
		x2     = x1;
		y2     = y1;
		x1     = temp_x;
		y1     = temp_y;
	} // end if
	
	// now we know that p1 and p2 are in order
	if (y3<y1)
	{
		temp_x = x3;
		temp_y = y3;
		x3     = x1;
		y3     = y1;
		x1     = temp_x;
		y1     = temp_y;
	} // end if
	
	// finally test y3 against y2
	if (y3<y2)
	{
		temp_x = x3;
		temp_y = y3;
		x3     = x2;
		y3     = y2;
		x2     = temp_x;
		y2     = temp_y;
		
	} // end if
	
	// do trivial rejection tests for clipping
	if ( y3<min_clip_y || y1>max_clip_y ||
		(x1<min_clip_x && x2<min_clip_x && x3<min_clip_x) ||
		(x1>max_clip_x && x2>max_clip_x && x3>max_clip_x) )
		return;
	
	// test if top of triangle is flat
	if (y1==y2)
	{
		Draw_Top_Tri(x1,y1,x2,y2,x3,y3,color, dest_buffer, mempitch);
	} // end if
	else
		if (y2==y3)
		{
			Draw_Bottom_Tri(x1,y1,x2,y2,x3,y3,color, dest_buffer, mempitch);
		} // end if bottom is flat
		else
		{
			// general triangle that's needs to be broken up along long edge
			new_x = x1 + (int)(0.5+(float)(y2-y1)*(float)(x3-x1)/(float)(y3-y1));
			
			// draw each sub-triangle
			Draw_Bottom_Tri(x1,y1,new_x,y2,x2,y2,color, dest_buffer, mempitch);
			Draw_Top_Tri(x2,y2,new_x,y2,x3,y3,color, dest_buffer, mempitch);
			
		} // end else
		
} // end Draw_Triangle_2D


void HCDDrawLib::Draw_TriangleFP_2D(int x1,int y1,
									int x2,int y2,
									int x3,int y3,
									int color,
									UCHAR *dest_buffer, int mempitch)
{
	// this function draws a triangle on the destination buffer using fixed point
	// it decomposes all triangles into a pair of flat top, flat bottom
	
	int temp_x, // used for sorting
		temp_y,
		new_x;
	
	// test for h lines and v lines
	if ((x1==x2 && x2==x3)  ||  (y1==y2 && y2==y3))
		return;
	
	// sort p1,p2,p3 in ascending y order
	if (y2<y1)
	{
		temp_x = x2;
		temp_y = y2;
		x2     = x1;
		y2     = y1;
		x1     = temp_x;
		y1     = temp_y;
	} // end if
	
	// now we know that p1 and p2 are in order
	if (y3<y1)
	{
		temp_x = x3;
		temp_y = y3;
		x3     = x1;
		y3     = y1;
		x1     = temp_x;
		y1     = temp_y;
	} // end if
	
	// finally test y3 against y2
	if (y3<y2)
	{
		temp_x = x3;
		temp_y = y3;
		x3     = x2;
		y3     = y2;
		x2     = temp_x;
		y2     = temp_y;
		
	} // end if
	
	// do trivial rejection tests for clipping
	if ( y3<min_clip_y || y1>max_clip_y ||
		(x1<min_clip_x && x2<min_clip_x && x3<min_clip_x) ||
		(x1>max_clip_x && x2>max_clip_x && x3>max_clip_x) )
		return;
	
	// test if top of triangle is flat
	if (y1==y2)
	{
		Draw_Top_TriFP(x1,y1,x2,y2,x3,y3,color, dest_buffer, mempitch);
	} // end if
	else
		if (y2==y3)
		{
			Draw_Bottom_TriFP(x1,y1,x2,y2,x3,y3,color, dest_buffer, mempitch);
		} // end if bottom is flat
		else
		{
			// general triangle that's needs to be broken up along long edge
			new_x = x1 + (int)(0.5+(float)(y2-y1)*(float)(x3-x1)/(float)(y3-y1));
			
			// draw each sub-triangle
			Draw_Bottom_TriFP(x1,y1,new_x,y2,x2,y2,color, dest_buffer, mempitch);
			Draw_Top_TriFP(x2,y2,new_x,y2,x3,y3,color, dest_buffer, mempitch);
			
		} // end else
		
} // end Draw_TriangleFP_2D
void HCDDrawLib::Draw_Bottom_Tri16(int x1,int y1, 
								   int x2,int y2, 
								   int x3,int y3,
								   int color,
								   UCHAR *_dest_buffer, int mempitch)
{
	// this function draws a triangle that has a flat bottom
	
	float dx_right,    // the dx/dy ratio of the right edge of line
		dx_left,     // the dx/dy ratio of the left edge of line
		xs,xe,       // the starting and ending points of the edges
		height;      // the height of the triangle
	
	int temp_x,        // used during sorting as temps
		temp_y,
		right,         // used by clipping
		left;
	
	// cast dest buffer to ushort
	USHORT *dest_buffer = (USHORT *)_dest_buffer;
	
	// destination address of next scanline
	USHORT  *dest_addr = NULL;
	
	// recompute mempitch in 16-bit words
	mempitch = (mempitch >> 1);
	
	// test order of x1 and x2
	if (x3 < x2)
	{
		temp_x = x2;
		x2     = x3;
		x3     = temp_x;
	} // end if swap
	
	// compute delta's
	height = y3-y1;
	
	dx_left  = (x2-x1)/height;
	dx_right = (x3-x1)/height;
	
	// set starting points
	xs = (float)x1;
	xe = (float)x1; // +(float)0.5;
	
	// perform y clipping
	if (y1<min_clip_y)
	{
		// compute new xs and ys
		xs = xs+dx_left*(float)(-y1+min_clip_y);
		xe = xe+dx_right*(float)(-y1+min_clip_y);
		
		// reset y1
		y1=min_clip_y;
		
	} // end if top is off screen
	
	if (y3>max_clip_y)
		y3=max_clip_y;
	
	// compute starting address in video memory
	dest_addr = dest_buffer+y1*mempitch;
	
	// test if x clipping is needed
	if (x1>=min_clip_x && x1<=max_clip_x &&
		x2>=min_clip_x && x2<=max_clip_x &&
		x3>=min_clip_x && x3<=max_clip_x)
    {
		// draw the triangle
		for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=mempitch)
        {
			// draw the line
			Mem_Set_WORD(dest_addr+(unsigned int)xs,color,(unsigned int)(xe-xs+1));
			
			// adjust starting point and ending point
			xs+=dx_left;
			xe+=dx_right;
			
        } // end for
		
    } // end if no x clipping needed
	else
	{
		// clip x axis with slower version
		
		// draw the triangle
		for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=mempitch)
		{
			// do x clip
			left  = (int)xs;
			right = (int)xe;
			
			// adjust starting point and ending point
			xs+=dx_left;
			xe+=dx_right;
			
			// clip line
			if (left < min_clip_x)
			{
				left = min_clip_x;
				
				if (right < min_clip_x)
					continue;
			}
			
			if (right > max_clip_x)
			{
				right = max_clip_x;
				
				if (left > max_clip_x)
					continue;
			}
			// draw the line
			Mem_Set_WORD(dest_addr+(unsigned int)left,color,(unsigned int)(right-left+1));
			
		} // end for
		
	} // end else x clipping needed
	
} // end Draw_Bottom_Tri16

void HCDDrawLib::Draw_Top_Tri16(int x1,int y1, 
								int x2,int y2, 
								int x3,int y3,
								int color, 
								UCHAR *_dest_buffer, int mempitch)
{
	// this function draws a triangle that has a flat top
	
	float dx_right,    // the dx/dy ratio of the right edge of line
		dx_left,     // the dx/dy ratio of the left edge of line
		xs,xe,       // the starting and ending points of the edges
		height;      // the height of the triangle
	
	int temp_x,        // used during sorting as temps
		temp_y,
		right,         // used by clipping
		left;
	
	// cast dest buffer to ushort
	USHORT *dest_buffer = (USHORT *)_dest_buffer;
	
	// destination address of next scanline
	USHORT  *dest_addr = NULL;
	
	// recompute mempitch in 16-bit words
	mempitch = (mempitch >> 1);
	
	// test order of x1 and x2
	if (x2 < x1)
	{
		temp_x = x2;
		x2     = x1;
		x1     = temp_x;
	} // end if swap
	
	// compute delta's
	height = y3-y1;
	
	dx_left  = (x3-x1)/height;
	dx_right = (x3-x2)/height;
	
	// set starting points
	xs = (float)x1;
	xe = (float)x2+(float)0.5;
	
	// perform y clipping
	if (y1 < min_clip_y)
	{
		// compute new xs and ys
		xs = xs+dx_left*(float)(-y1+min_clip_y);
		xe = xe+dx_right*(float)(-y1+min_clip_y);
		
		// reset y1
		y1=min_clip_y;
		
	} // end if top is off screen
	
	if (y3>max_clip_y)
		y3=max_clip_y;
	
	// compute starting address in video memory
	dest_addr = dest_buffer+y1*mempitch;
	
	// test if x clipping is needed
	if (x1>=min_clip_x && x1<=max_clip_x &&
		x2>=min_clip_x && x2<=max_clip_x &&
		x3>=min_clip_x && x3<=max_clip_x)
    {
		// draw the triangle
		for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=mempitch)
        {
			// draw the line
			Mem_Set_WORD(dest_addr+(unsigned int)xs,color,(unsigned int)(xe-xs+1));
			
			// adjust starting point and ending point
			xs+=dx_left;
			xe+=dx_right;
			
        } // end for
		
    } // end if no x clipping needed
	else
	{
		// clip x axis with slower version
		
		// draw the triangle
		for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=mempitch)
		{
			// do x clip
			left  = (int)xs;
			right = (int)xe;
			
			// adjust starting point and ending point
			xs+=dx_left;
			xe+=dx_right;
			
			// clip line
			if (left < min_clip_x)
			{
				left = min_clip_x;
				
				if (right < min_clip_x)
					continue;
			}
			
			if (right > max_clip_x)
			{
				right = max_clip_x;
				
				if (left > max_clip_x)
					continue;
			}
			
			// draw the line
			Mem_Set_WORD(dest_addr+(unsigned int)left,color,(unsigned int)(right-left+1));
			
		} // end for
		
	} // end else x clipping needed
	
} // end Draw_Top_Tri16

void HCDDrawLib::Draw_Bottom_Tri(int x1,int y1, 
								 int x2,int y2, 
								 int x3,int y3,
								 int color,
								 UCHAR *dest_buffer, int mempitch)
{
	// this function draws a triangle that has a flat bottom
	
	float dx_right,    // the dx/dy ratio of the right edge of line
		dx_left,     // the dx/dy ratio of the left edge of line
		xs,xe,       // the starting and ending points of the edges
		height;      // the height of the triangle
	
	int temp_x,        // used during sorting as temps
		temp_y,
		right,         // used by clipping
		left;
	
	// destination address of next scanline
	UCHAR  *dest_addr;
	
	// test order of x1 and x2
	if (x3 < x2)
	{
		temp_x = x2;
		x2     = x3;
		x3     = temp_x;
	} // end if swap
	
	// compute delta's
	height = y3-y1;
	
	dx_left  = (x2-x1)/height;
	dx_right = (x3-x1)/height;
	
	// set starting points
	xs = (float)x1;
	xe = (float)x1; // +(float)0.5;
	
	// perform y clipping
	if (y1<min_clip_y)
	{
		// compute new xs and ys
		xs = xs+dx_left*(float)(-y1+min_clip_y);
		xe = xe+dx_right*(float)(-y1+min_clip_y);
		
		// reset y1
		y1=min_clip_y;
		
	} // end if top is off screen
	
	if (y3>max_clip_y)
		y3=max_clip_y;
	
	// compute starting address in video memory
	dest_addr = dest_buffer+y1*mempitch;
	
	// test if x clipping is needed
	if (x1>=min_clip_x && x1<=max_clip_x &&
		x2>=min_clip_x && x2<=max_clip_x &&
		x3>=min_clip_x && x3<=max_clip_x)
    {
		// draw the triangle
		for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=mempitch)
        {
			memset((UCHAR  *)dest_addr+(unsigned int)xs,
                color,(unsigned int)(xe-xs+1));
			
			// adjust starting point and ending point
			xs+=dx_left;
			xe+=dx_right;
			
        } // end for
		
    } // end if no x clipping needed
	else
	{
		// clip x axis with slower version
		
		// draw the triangle
		
		for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=mempitch)
		{
			// do x clip
			left  = (int)xs;
			right = (int)xe;
			
			// adjust starting point and ending point
			xs+=dx_left;
			xe+=dx_right;
			
			// clip line
			if (left < min_clip_x)
			{
				left = min_clip_x;
				
				if (right < min_clip_x)
					continue;
			}
			
			if (right > max_clip_x)
			{
				right = max_clip_x;
				
				if (left > max_clip_x)
					continue;
			}
			
			memset((UCHAR  *)dest_addr+(unsigned int)left,
				color,(unsigned int)(right-left+1));
			
		} // end for
		
	} // end else x clipping needed
	
} // end Draw_Bottom_Tri


void HCDDrawLib::Draw_Top_Tri(int x1,int y1, 
							  int x2,int y2, 
							  int x3,int y3,
							  int color, 
							  UCHAR *dest_buffer, int mempitch)
{
	// this function draws a triangle that has a flat top
	
	float dx_right,    // the dx/dy ratio of the right edge of line
		dx_left,     // the dx/dy ratio of the left edge of line
		xs,xe,       // the starting and ending points of the edges
		height;      // the height of the triangle
	
	int temp_x,        // used during sorting as temps
		temp_y,
		right,         // used by clipping
		left;
	
	// destination address of next scanline
	UCHAR  *dest_addr = NULL;
	
	// test order of x1 and x2
	if (x2 < x1)
	{
		temp_x = x2;
		x2     = x1;
		x1     = temp_x;
	} // end if swap
	
	// compute delta's
	height = y3-y1;
	
	dx_left  = (x3-x1)/height;
	dx_right = (x3-x2)/height;
	
	// set starting points
	xs = (float)x1;
	xe = (float)x2+(float)0.5;
	
	// perform y clipping
	if (y1 < min_clip_y)
	{
		// compute new xs and ys
		xs = xs+dx_left*(float)(-y1+min_clip_y);
		xe = xe+dx_right*(float)(-y1+min_clip_y);
		
		// reset y1
		y1=min_clip_y;
		
	} // end if top is off screen
	
	if (y3>max_clip_y)
		y3=max_clip_y;
	
	// compute starting address in video memory
	dest_addr = dest_buffer+y1*mempitch;
	
	// test if x clipping is needed
	if (x1>=min_clip_x && x1<=max_clip_x &&
		x2>=min_clip_x && x2<=max_clip_x &&
		x3>=min_clip_x && x3<=max_clip_x)
    {
		// draw the triangle
		for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=mempitch)
        {
			memset((UCHAR *)dest_addr+(unsigned int)xs,
                color,(unsigned int)(xe-xs+1));
			
			// adjust starting point and ending point
			xs+=dx_left;
			xe+=dx_right;
			
        } // end for
		
    } // end if no x clipping needed
	else
	{
		// clip x axis with slower version
		
		// draw the triangle
		for (temp_y=y1; temp_y<=y3; temp_y++,dest_addr+=mempitch)
		{
			// do x clip
			left  = (int)xs;
			right = (int)xe;
			
			// adjust starting point and ending point
			xs+=dx_left;
			xe+=dx_right;
			
			// clip line
			if (left < min_clip_x)
			{
				left = min_clip_x;
				
				if (right < min_clip_x)
					continue;
			}
			
			if (right > max_clip_x)
			{
				right = max_clip_x;
				
				if (left > max_clip_x)
					continue;
			}
			
			memset((UCHAR  *)dest_addr+(unsigned int)left,
				color,(unsigned int)(right-left+1));
			
		} // end for
		
	} // end else x clipping needed
	
} // end Draw_Top_Tri

int HCDDrawLib::Draw_Text_GDI(char *text, int x,int y,COLORREF color, LPDIRECTDRAWSURFACE7 lpdds)
{
	// this function draws the sent text on the sent surface 
	// using color index as the color in the palette
	
	HDC xdc; // the working dc
	
	// get the dc from surface
	if (FAILED(lpdds->GetDC(&xdc)))
		return(0);
	
	// set the colors for the text up
	SetTextColor(xdc,color);
	
	// set background mode to transparent so black isn't copied
	SetBkMode(xdc, TRANSPARENT);
	
	// draw the text a
	TextOut(xdc,x,y,text,strlen(text));
	
	// release the dc
	lpdds->ReleaseDC(xdc);
	
	// return success
	return(1);
} // end Draw_Text_GDI

///////////////////////////////////////////////////////////

int HCDDrawLib::Draw_Text_GDI(char *text, int x,int y,int color, LPDIRECTDRAWSURFACE7 lpdds)
{
	// this function draws the sent text on the sent surface 
	// using color index as the color in the palette
	
	HDC xdc; // the working dc
	
	// get the dc from surface
	if (FAILED(lpdds->GetDC(&xdc)))
		return(0);
	
	// set the colors for the text up
	SetTextColor(xdc,color);
	
	// set background mode to transparent so black isn't copied
	SetBkMode(xdc, TRANSPARENT);
	
	// draw the text a
	TextOut(xdc,x,y,text,strlen(text));
	
	// release the dc
	lpdds->ReleaseDC(xdc);
	
	// return success
	return(1);
} // end Draw_Text_GDI


///////////////////////////////////////////////////////////   

int HCDDrawLib::Draw_Rectangle(int x1, int y1, int x2, int y2, int color,
							   LPDIRECTDRAWSURFACE7 lpdds)
{
	// this function uses directdraw to draw a filled rectangle
	
	DDBLTFX ddbltfx; // this contains the DDBLTFX structure
	RECT fill_area;  // this contains the destination rectangle
	
	// clear out the structure and set the size field 
	DDRAW_INIT_STRUCT(ddbltfx);
	
	// set the dwfillcolor field to the desired color
	ddbltfx.dwFillColor = color; 
	
	// fill in the destination rectangle data (your data)
	fill_area.top    = y1;
	fill_area.left   = x1;
	fill_area.bottom = y2;
	fill_area.right  = x2;
	
	// ready to blt to surface, in this case blt to primary
	lpdds->Blt(&fill_area, // ptr to dest rectangle
		NULL,       // ptr to source surface, NA            
		NULL,       // ptr to source rectangle, NA
		DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
		&ddbltfx);  // ptr to DDBLTFX structure
	
	// return success
	return(1);
	
} // end Draw_Rectangle


int HCDDrawLib::Draw_Pixel16(int x, int y,int color,
							 UCHAR *video_buffer, int lpitch)
{
	// this function plots a single pixel at x,y with color
	
	((USHORT *)video_buffer)[x + y*(lpitch >> 1)] = color;
	
	// return success
	return(1);
	
} // end Draw_Pixel16


int HCDDrawLib::Draw_Pixel(int x, int y,int color,
						   UCHAR *video_buffer, int lpitch)
{
	// this function plots a single pixel at x,y with color
	
	video_buffer[x + y*lpitch] = color;
	
	// return success
	return(1);
	
} // end Draw_Pixel

int HCDDrawLib::Draw_Line16(int x0, int y0, // starting position 
							int x1, int y1, // ending position
							int color,     // color index
							UCHAR *vb_start, int lpitch) // video buffer and memory pitch
{
	// this function draws a line from xo,yo to x1,y1 using differential error
	// terms (based on Bresenahams work)
	
	int dx,             // difference in x's
		dy,             // difference in y's
		dx2,            // dx,dy * 2
		dy2, 
		x_inc,          // amount in pixel space to move during drawing
		y_inc,          // amount in pixel space to move during drawing
		error,          // the discriminant i.e. error i.e. decision variable
		index;          // used for looping
	
	int lpitch_2 = lpitch >> 1; // USHORT strided lpitch
	
	// pre-compute first pixel address in video buffer based on 16bit data
	USHORT *vb_start2 = (USHORT *)vb_start + x0 + y0*lpitch_2;
	
	// compute horizontal and vertical deltas
	dx = x1-x0;
	dy = y1-y0;
	
	// test which direction the line is going in i.e. slope angle
	if (dx>=0)
	{
		x_inc = 1;
		
	} // end if line is moving right
	else
	{
		x_inc = -1;
		dx    = -dx;  // need absolute value
		
	} // end else moving left
	
	// test y component of slope
	
	if (dy>=0)
	{
		y_inc = lpitch_2;
	} // end if line is moving down
	else
	{
		y_inc = -lpitch_2;
		dy    = -dy;  // need absolute value
		
	} // end else moving up
	
	// compute (dx,dy) * 2
	dx2 = dx << 1;
	dy2 = dy << 1;
	
	// now based on which delta is greater we can draw the line
	if (dx > dy)
	{
		// initialize error term
		error = dy2 - dx; 
		
		// draw the line
		for (index=0; index <= dx; index++)
		{
			// set the pixel
			*vb_start2 = (USHORT)color;
			
			// test if error has overflowed
			if (error >= 0) 
			{
				error-=dx2;
				
				// move to next line
				vb_start2+=y_inc;
				
			} // end if error overflowed
			
			// adjust the error term
			error+=dy2;
			
			// move to the next pixel
			vb_start2+=x_inc;
			
		} // end for
		
	} // end if |slope| <= 1
	else
	{
		// initialize error term
		error = dx2 - dy; 
		
		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			*vb_start2 = (USHORT)color;
			
			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;
				
				// move to next line
				vb_start2+=x_inc;
				
			} // end if error overflowed
			
			// adjust the error term
			error+=dx2;
			
			// move to the next pixel
			vb_start2+=y_inc;
			
		} // end for
		
	} // end else |slope| > 1
	
	// return success
	return(1);
	
} // end Draw_Line16

int HCDDrawLib::Draw_Line(int x0, int y0, // starting position 
						  int x1, int y1, // ending position
						  int color,     // color index
						  UCHAR *vb_start, int lpitch) // video buffer and memory pitch
{
	// this function draws a line from xo,yo to x1,y1 using differential error
	// terms (based on Bresenahams work)
	
	int dx,             // difference in x's
		dy,             // difference in y's
		dx2,            // dx,dy * 2
		dy2, 
		x_inc,          // amount in pixel space to move during drawing
		y_inc,          // amount in pixel space to move during drawing
		error,          // the discriminant i.e. error i.e. decision variable
		index;          // used for looping
	
	// pre-compute first pixel address in video buffer
	vb_start = vb_start + x0 + y0*lpitch;
	
	// compute horizontal and vertical deltas
	dx = x1-x0;
	dy = y1-y0;
	
	// test which direction the line is going in i.e. slope angle
	if (dx>=0)
	{
		x_inc = 1;
		
	} // end if line is moving right
	else
	{
		x_inc = -1;
		dx    = -dx;  // need absolute value
		
	} // end else moving left
	
	// test y component of slope
	
	if (dy>=0)
	{
		y_inc = lpitch;
	} // end if line is moving down
	else
	{
		y_inc = -lpitch;
		dy    = -dy;  // need absolute value
		
	} // end else moving up
	
	// compute (dx,dy) * 2
	dx2 = dx << 1;
	dy2 = dy << 1;
	
	// now based on which delta is greater we can draw the line
	if (dx > dy)
	{
		// initialize error term
		error = dy2 - dx; 
		
		// draw the line
		for (index=0; index <= dx; index++)
		{
			// set the pixel
			*vb_start = color;
			
			// test if error has overflowed
			if (error >= 0) 
			{
				error-=dx2;
				
				// move to next line
				vb_start+=y_inc;
				
			} // end if error overflowed
			
			// adjust the error term
			error+=dy2;
			
			// move to the next pixel
			vb_start+=x_inc;
			
		} // end for
		
	} // end if |slope| <= 1
	else
	{
		// initialize error term
		error = dx2 - dy; 
		
		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			*vb_start = color;
			
			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;
				
				// move to next line
				vb_start+=x_inc;
				
			} // end if error overflowed
			
			// adjust the error term
			error+=dx2;
			
			// move to the next pixel
			vb_start+=y_inc;
			
		} // end for
		
	} // end else |slope| > 1
	
	// return success
	return(1);
	
} // end Draw_Line

int HCDDrawLib::Draw_Clip_Line(int x0,int y0, int x1, int y1, int color, 
							   UCHAR *dest_buffer, int lpitch)
{
	// this function draws a wireframe triangle
	
	int cxs, cys,
		cxe, cye;
	
	// clip and draw each line
	cxs = x0;
	cys = y0;
	cxe = x1;
	cye = y1;
	
	// clip the line
	if (Clip_Line(cxs,cys,cxe,cye))
		Draw_Line(cxs, cys, cxe,cye,color,dest_buffer,lpitch);
	
	// return success
	return(1);
	
} // end Draw_Clip_Line

int HCDDrawLib::Draw_Clip_Line16(int x0,int y0, int x1, int y1, int color, 
								 UCHAR *dest_buffer, int lpitch)
{
	// this function draws a clipped line
	
	int cxs, cys,
		cxe, cye;
	
	// clip and draw each line
	cxs = x0;
	cys = y0;
	cxe = x1;
	cye = y1;
	
	// clip the line
	if (Clip_Line(cxs,cys,cxe,cye))
		Draw_Line16(cxs, cys, cxe,cye,color,dest_buffer,lpitch);
	
	// return success
	return(1);
	
} // end Draw_Clip_Line16


int HCDDrawLib::Clip_Line(int &x1,int &y1,int &x2, int &y2)
{
	// this function clips the sent line using the globally defined clipping
	// region
	
	// internal clipping codes
#define CLIP_CODE_C  0x0000
#define CLIP_CODE_N  0x0008
#define CLIP_CODE_S  0x0004
#define CLIP_CODE_E  0x0002
#define CLIP_CODE_W  0x0001
	
#define CLIP_CODE_NE 0x000a
#define CLIP_CODE_SE 0x0006
#define CLIP_CODE_NW 0x0009 
#define CLIP_CODE_SW 0x0005
	
	int xc1=x1, 
		yc1=y1, 
		xc2=x2, 
		yc2=y2;
	
	int p1_code=0, 
		p2_code=0;
	
	// determine codes for p1 and p2
	if (y1 < min_clip_y)
		p1_code|=CLIP_CODE_N;
	else
		if (y1 > max_clip_y)
			p1_code|=CLIP_CODE_S;
		
		if (x1 < min_clip_x)
			p1_code|=CLIP_CODE_W;
		else
			if (x1 > max_clip_x)
				p1_code|=CLIP_CODE_E;
			
			if (y2 < min_clip_y)
				p2_code|=CLIP_CODE_N;
			else
				if (y2 > max_clip_y)
					p2_code|=CLIP_CODE_S;
				
				if (x2 < min_clip_x)
					p2_code|=CLIP_CODE_W;
				else
					if (x2 > max_clip_x)
						p2_code|=CLIP_CODE_E;
					
					// try and trivially reject
					if ((p1_code & p2_code)) 
						return(0);
					
					// test for totally visible, if so leave points untouched
					if (p1_code==0 && p2_code==0)
						return(1);
					
					// determine end clip point for p1
					switch(p1_code)
					{
					case CLIP_CODE_C: break;
						
					case CLIP_CODE_N:
						{
							yc1 = min_clip_y;
							xc1 = x1 + 0.5+(min_clip_y-y1)*(x2-x1)/(y2-y1);
						} break;
					case CLIP_CODE_S:
						{
							yc1 = max_clip_y;
							xc1 = x1 + 0.5+(max_clip_y-y1)*(x2-x1)/(y2-y1);
						} break;
						
					case CLIP_CODE_W:
						{
							xc1 = min_clip_x;
							yc1 = y1 + 0.5+(min_clip_x-x1)*(y2-y1)/(x2-x1);
						} break;
						
					case CLIP_CODE_E:
						{
							xc1 = max_clip_x;
							yc1 = y1 + 0.5+(max_clip_x-x1)*(y2-y1)/(x2-x1);
						} break;
						
						// these cases are more complex, must compute 2 intersections
					case CLIP_CODE_NE:
						{
							// north hline intersection
							yc1 = min_clip_y;
							xc1 = x1 + 0.5+(min_clip_y-y1)*(x2-x1)/(y2-y1);
							
							// test if intersection is valid, of so then done, else compute next
							if (xc1 < min_clip_x || xc1 > max_clip_x)
							{
								// east vline intersection
								xc1 = max_clip_x;
								yc1 = y1 + 0.5+(max_clip_x-x1)*(y2-y1)/(x2-x1);
							} // end if
							
						} break;
						
					case CLIP_CODE_SE:
						{
							// south hline intersection
							yc1 = max_clip_y;
							xc1 = x1 + 0.5+(max_clip_y-y1)*(x2-x1)/(y2-y1);	
							
							// test if intersection is valid, of so then done, else compute next
							if (xc1 < min_clip_x || xc1 > max_clip_x)
							{
								// east vline intersection
								xc1 = max_clip_x;
								yc1 = y1 + 0.5+(max_clip_x-x1)*(y2-y1)/(x2-x1);
							} // end if
							
						} break;
						
					case CLIP_CODE_NW: 
						{
							// north hline intersection
							yc1 = min_clip_y;
							xc1 = x1 + 0.5+(min_clip_y-y1)*(x2-x1)/(y2-y1);
							
							// test if intersection is valid, of so then done, else compute next
							if (xc1 < min_clip_x || xc1 > max_clip_x)
							{
								xc1 = min_clip_x;
								yc1 = y1 + 0.5+(min_clip_x-x1)*(y2-y1)/(x2-x1);	
							} // end if
							
						} break;
						
					case CLIP_CODE_SW:
						{
							// south hline intersection
							yc1 = max_clip_y;
							xc1 = x1 + 0.5+(max_clip_y-y1)*(x2-x1)/(y2-y1);	
							
							// test if intersection is valid, of so then done, else compute next
							if (xc1 < min_clip_x || xc1 > max_clip_x)
							{
								xc1 = min_clip_x;
								yc1 = y1 + 0.5+(min_clip_x-x1)*(y2-y1)/(x2-x1);	
							} // end if
							
						} break;
						
					default:break;
						
					} // end switch
					
					// determine clip point for p2
					switch(p2_code)
					{
					case CLIP_CODE_C: break;
						
					case CLIP_CODE_N:
						{
							yc2 = min_clip_y;
							xc2 = x2 + (min_clip_y-y2)*(x1-x2)/(y1-y2);
						} break;
						
					case CLIP_CODE_S:
						{
							yc2 = max_clip_y;
							xc2 = x2 + (max_clip_y-y2)*(x1-x2)/(y1-y2);
						} break;
						
					case CLIP_CODE_W:
						{
							xc2 = min_clip_x;
							yc2 = y2 + (min_clip_x-x2)*(y1-y2)/(x1-x2);
						} break;
						
					case CLIP_CODE_E:
						{
							xc2 = max_clip_x;
							yc2 = y2 + (max_clip_x-x2)*(y1-y2)/(x1-x2);
						} break;
						
						// these cases are more complex, must compute 2 intersections
					case CLIP_CODE_NE:
						{
							// north hline intersection
							yc2 = min_clip_y;
							xc2 = x2 + 0.5+(min_clip_y-y2)*(x1-x2)/(y1-y2);
							
							// test if intersection is valid, of so then done, else compute next
							if (xc2 < min_clip_x || xc2 > max_clip_x)
							{
								// east vline intersection
								xc2 = max_clip_x;
								yc2 = y2 + 0.5+(max_clip_x-x2)*(y1-y2)/(x1-x2);
							} // end if
							
						} break;
						
					case CLIP_CODE_SE:
						{
							// south hline intersection
							yc2 = max_clip_y;
							xc2 = x2 + 0.5+(max_clip_y-y2)*(x1-x2)/(y1-y2);	
							
							// test if intersection is valid, of so then done, else compute next
							if (xc2 < min_clip_x || xc2 > max_clip_x)
							{
								// east vline intersection
								xc2 = max_clip_x;
								yc2 = y2 + 0.5+(max_clip_x-x2)*(y1-y2)/(x1-x2);
							} // end if
							
						} break;
						
					case CLIP_CODE_NW: 
						{
							// north hline intersection
							yc2 = min_clip_y;
							xc2 = x2 + 0.5+(min_clip_y-y2)*(x1-x2)/(y1-y2);
							
							// test if intersection is valid, of so then done, else compute next
							if (xc2 < min_clip_x || xc2 > max_clip_x)
							{
								xc2 = min_clip_x;
								yc2 = y2 + 0.5+(min_clip_x-x2)*(y1-y2)/(x1-x2);	
							} // end if
							
						} break;
						
					case CLIP_CODE_SW:
						{
							// south hline intersection
							yc2 = max_clip_y;
							xc2 = x2 + 0.5+(max_clip_y-y2)*(x1-x2)/(y1-y2);	
							
							// test if intersection is valid, of so then done, else compute next
							if (xc2 < min_clip_x || xc2 > max_clip_x)
							{
								xc2 = min_clip_x;
								yc2 = y2 + 0.5+(min_clip_x-x2)*(y1-y2)/(x1-x2);	
							} // end if
							
						} break;
						
					default:break;
						
					} // end switch
					
					// do bounds check
					if ((xc1 < min_clip_x) || (xc1 > max_clip_x) ||
						(yc1 < min_clip_y) || (yc1 > max_clip_y) ||
						(xc2 < min_clip_x) || (xc2 > max_clip_x) ||
						(yc2 < min_clip_y) || (yc2 > max_clip_y) )
					{
						return(0);
					} // end if
					
					// store vars back
					x1 = xc1;
					y1 = yc1;
					x2 = xc2;
					y2 = yc2;
					
					return(1);
					
} // end Clip_Line