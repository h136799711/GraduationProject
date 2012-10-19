#include "HCUtility.h"

int Color_Scan(int x1, int y1, int x2, int y2,UCHAR scan_start, UCHAR scan_end,
               UCHAR *scan_buffer, int scan_lpitch,int screen_width,int screen_height)
{
	// this function implements a crude collision technique
	// based on scanning for a range of colors within a rectangle
	
	// clip rectangle
	
	// x coords first    
	if (x1 >= screen_width)
		x1=screen_width-1;
	else
		if (x1 < 0)
			x1=0;
		
		if (x2 >= screen_width)
			x2=screen_width-1;
		else
			if (x2 < 0)
				x2=0;
			
			// now y-coords
			if (y1 >= screen_height)
				y1=screen_height-1;
			else
				if (y1 < 0)
					y1=0;
				
				if (y2 >= screen_height)
					y2=screen_height-1;
				else
					if (y2 < 0)
						y2=0;
					
					// scan the region
					scan_buffer +=y1*scan_lpitch;
					
					for (int scan_y=y1; scan_y<=y2; scan_y++)
					{
						for (int scan_x=x1; scan_x<=x2; scan_x++)
						{
							if (scan_buffer[scan_x] >= scan_start && scan_buffer[scan_x] <= scan_end )
								return(1);
						} // end for x
						
						// move down a line
						scan_buffer+=scan_lpitch;
						
					} // end for y
					
					// return failure
					return(0);
					
} // end Color_Scan

////////////////////////////////////////////////////////////////

int Color_Scan16(int x1, int y1, int x2, int y2, 
				 USHORT scan_start, USHORT scan_end,
				 UCHAR *scan_buffer, int scan_lpitch,
				 int screen_width,int screen_height)
{
	// this function implements a crude collision technique
	// based on scanning for a range of colors within a rectangle
	// this is the 16-bit version, thus the interpretation of scan_start
	// and end are different, they are they EXACT RGB values you are looking
	// for, thus you can test for 2 values at most, else make them equal to
	// test for one value
	USHORT *scan_buffer2 = (USHORT *)scan_buffer;
	
	// convert number of bytes per line to number of 16-bit shorts
	scan_lpitch = (scan_lpitch >> 1);
	
	// clip rectangle
	
	// x coords first    
	if (x1 >= screen_width)
		x1=screen_width-1;
	else
		if (x1 < 0)
			x1=0;
		
		if (x2 >= screen_width)
			x2=screen_width-1;
		else
			if (x2 < 0)
				x2=0;
			
			// now y-coords
			if (y1 >= screen_height)
				y1=screen_height-1;
			else
				if (y1 < 0)
					y1=0;
				
				if (y2 >= screen_height)
					y2=screen_height-1;
				else
					if (y2 < 0)
						y2=0;
					
					// scan the region
					scan_buffer2 +=y1*scan_lpitch;
					
					for (int scan_y=y1; scan_y<=y2; scan_y++)
					{
						for (int scan_x=x1; scan_x<=x2; scan_x++)
						{
							if (scan_buffer2[scan_x] == scan_start || scan_buffer2[scan_x] == scan_end )
								return(1);
						} // end for x
						
						// move down a line
						scan_buffer2+=scan_lpitch;
						
					} // end for y
					
					// return failure
					return(0);
					
} // end Color_Scan16



DWORD Wait_Clock(DWORD count)
{
	// this function is used to wait for a specific number of clicks
	// since the call to Start_Clock
	
	while((Get_Clock() - start_clock_count) < count);
	return(Get_Clock());
	
} // end Wait_Clock


DWORD Start_Clock(void)
{
	// this function starts the clock, that is, saves the current
	// count, use in conjunction with Wait_Clock()
	
	return(start_clock_count = Get_Clock());
	
} // end Start_Clock

DWORD Get_Clock(void)
{
	// this function returns the current tick count
	
	// return time
	return(GetTickCount());
	
} // end Get_Clock



void Mem_Set_QUAD(void *dest, UINT data, int count)
{
	// this function fills or sets unsigned 32-bit aligned memory
	// count is number of quads
	
	_asm 
    { 
		mov edi, dest   ; edi points to destination memory
			mov ecx, count  ; number of 32-bit words to move
			mov eax, data   ; 32-bit data
			rep stosd       ; move data
    } // end asm
	
} // end Mem_Set_QUAD

void Mem_Set_WORD(void *dest,USHORT data,int count)
{
	_asm
	{
		mov edi,dest;edi points to destination memory
			mov ecx,count;number of 16-bit words to move
			mov ax,data;16-bit data
			rep stosw;move data
			
	}//end asm
}


USHORT RGB16Bit565(int r,int g,int b)
{
	r>>=3;g>>=2;b>>=3;
	
	return (_RGB16BIT565((r),(g),(b)));
}


USHORT RGB16Bit555(int r, int g, int b)
{
	// this function simply builds a 5.5.5 format 16 bit pixel
	// assumes input is RGB 0-255 each channel
	r>>=3; g>>=3; b>>=3;
	return(_RGB16BIT555((r),(g),(b)));
	
} // end RGB16Bit555
