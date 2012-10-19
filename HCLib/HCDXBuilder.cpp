#include "HCDXBuilder.h"

void HCDXBuilder::VLine(int y1,int y2,int x,int color,UCHAR *vbuffer, int lpitch)
{
	// draw a vertical line, note that a memset function can no longer be
	// used since the pixel addresses are no longer contiguous in memory
	// note that the end points of the line must be on the screen
	
	UCHAR *start_offset; // starting memory offset of line
	
	int index, // loop index
		temp;  // used for temporary storage during swap
	
	
	// perform trivial rejections
	if (x > max_clip_x || x < min_clip_x)
		return;
	
	// make sure y2 > y1
	if (y1>y2)
	{
		temp = y1;
		y1   = y2;
		y2   = temp;
	} // end swap
	
	// perform trivial rejections
	if (y1 > max_clip_y || y2 < min_clip_y)
		return;
	
	// now clip
	y1 = ((y1 < min_clip_y) ? min_clip_y : y1);
	y2 = ((y2 > max_clip_y) ? max_clip_y : y2);
	
	// compute starting position
	start_offset = vbuffer + (y1*lpitch) + x;
	
	// draw line one pixel at a time
	for (index=0; index<=y2-y1; index++)
    {
		// set the pixel
		*start_offset = (UCHAR)color;
		
		// move downward to next line
		start_offset+=lpitch;
		
    } // end for index
	
} // end VLine

void HCDXBuilder::HLine(int x1,int x2,int y,int color, UCHAR *vbuffer, int lpitch)
{
	// draw a horizontal line using the memset function
	
	int temp; // used for temporary storage during endpoint swap
	
	// perform trivial rejections
	if (y > max_clip_y || y < min_clip_y)
		return;
	
	// sort x1 and x2, so that x2 > x1
	if (x1>x2)
	{
		temp = x1;
		x1   = x2;
		x2   = temp;
	} // end swap
	
	// perform trivial rejections
	if (x1 > max_clip_x || x2 < min_clip_x)
		return;
	
	// now clip
	x1 = ((x1 < min_clip_x) ? min_clip_x : x1);
	x2 = ((x2 > max_clip_x) ? max_clip_x : x2);
	
	// draw the row of pixels
	memset((UCHAR *)(vbuffer+(y*lpitch)+x1),
		(UCHAR)color,x2-x1+1);
	
} // end HLine

void HCDXBuilder::VLine16(int y1,int y2,int x,int color,UCHAR *vbuffer, int lpitch)
{
	// draw a vertical line, note that a memset function can no longer be
	// used since the pixel addresses are no longer contiguous in memory
	// note that the end points of the line must be on the screen
	
	USHORT *start_offset; // starting memory offset of line
	
	int index, // loop index
		temp;  // used for temporary storage during swap
	
	// convert lpitch to number of words
	lpitch = lpitch >> 1;
	
	// perform trivial rejections
	if (x > max_clip_x || x < min_clip_x)
		return;
	
	// make sure y2 > y1
	if (y1>y2)
	{
		temp = y1;
		y1   = y2;
		y2   = temp;
	} // end swap
	
	// perform trivial rejections
	if (y1 > max_clip_y || y2 < min_clip_y)
		return;
	
	// now clip
	y1 = ((y1 < min_clip_y) ? min_clip_y : y1);
	y2 = ((y2 > max_clip_y) ? max_clip_y : y2);
	
	// compute starting position
	start_offset = (USHORT *)vbuffer + (y1*lpitch) + x;
	
	// draw line one pixel at a time
	for (index=0; index<=y2-y1; index++)
    {
		// set the pixel
		*start_offset = color;
		
		// move downward to next line
		start_offset+=lpitch;
		
    } // end for index
	
} // end VLine16


void HCDXBuilder::HLine16(int x1,int x2,int y,int color, UCHAR *vbuffer, int lpitch)
{
	// draw a horizontal line using the memset function
	
	int temp; // used for temporary storage during endpoint swap
	
	USHORT *vbuffer2 = (USHORT *)vbuffer; // short pointer to buffer
	
	// convert pitch to words
	lpitch = lpitch >> 1;
	
	// perform trivial rejections
	if (y > max_clip_y || y < min_clip_y)
		return;
	
	// sort x1 and x2, so that x2 > x1
	if (x1>x2)
	{
		temp = x1;
		x1   = x2;
		x2   = temp;
	} // end swap
	
	// perform trivial rejections
	if (x1 > max_clip_x || x2 < min_clip_x)
		return;
	
	// now clip
	x1 = ((x1 < min_clip_x) ? min_clip_x : x1);
	x2 = ((x2 > max_clip_x) ? max_clip_x : x2);
	
	// draw the row of pixels
	Mem_Set_WORD((vbuffer2+(y*lpitch)+x1), color,x2-x1+1);
	
} // end HLine16



void HCDXBuilder::Screen_Transitions(int effect, UCHAR *vbuffer, int lpitch)
{
	// this function can be called to perform a myraid of screen transitions
	// to the destination buffer, make sure to save and restore the palette
	// when performing color transitions in 8-bit modes
	
	int pal_reg;         // used as loop counter
	int index;           // used as loop counter
	int red,green,blue;           // used in fad algorithm
	
	PALETTEENTRY color;              // temporary color
	PALETTEENTRY work_palette[MAX_COLORS_PALETTE];  // used as a working palette
//	PALETTEENTRY work_color;         // used in color algorithms
	
	// test which screen effect is being selected
	switch(effect)
	{
	case SCREEN_DARKNESS:
		{
			// fade to black
			
			for (index=0; index<80; index++)
			{
				// get the palette 
				Save_Palette(work_palette);
				
				// process each color
				for (pal_reg=1; pal_reg<MAX_COLORS_PALETTE; pal_reg++)
				{
					// get the entry data
					color = work_palette[pal_reg];
					
					// test if this color register is already black
					if (color.peRed > 4) color.peRed-=3;
					else
						color.peRed = 0;
					
					if (color.peGreen > 4) color.peGreen-=3;
					else
						color.peGreen = 0;
					
					if (color.peBlue  > 4) color.peBlue-=3;
					else
						color.peBlue = 0;
					
					// set the color to a diminished intensity
					work_palette[pal_reg] = color;
					
				} // end for pal_reg
				
				// write the palette back out
				Set_Palette(work_palette);
				
				// wait a bit
				
				Start_Clock(); Wait_Clock(12);
				
			} // end for index
			
		} break;
		
	case SCREEN_WHITENESS:
		{
			// fade to white
			for (index=0; index<64; index++)
			{
				// get the palette 
				Save_Palette(work_palette);
				
				// loop thru all palette registers
				for (pal_reg=0; pal_reg < MAX_COLORS_PALETTE; pal_reg++)
				{
					// get the entry data
					color = work_palette[pal_reg];
					
					// make 32 bit copy of color
					red   = color.peRed;
					green = color.peGreen;
					blue  = color.peBlue; 
					
					if ((red+=4) >=255)
						red=255;
					
					if ((green+=4) >=255)
						green=255;
					
					if ((blue+=4) >=255)
						blue=255;
					
					// store colors back
					color.peRed   = red;
					color.peGreen = green;
					color.peBlue  = blue;
					
					// set the color to a diminished intensity
					work_palette[pal_reg] = color;
					
				} // end for pal_reg
				
				// write the palette back out
				Set_Palette(work_palette);
				
				// wait a bit
				
				Start_Clock(); Wait_Clock(12);
				
			} // end for index
			
		} break;
		
	case SCREEN_REDNESS:
		{
			// fade to red
			
			for (index=0; index<64; index++)
			{
				// get the palette 
				Save_Palette(work_palette);
				
				// loop thru all palette registers
				for (pal_reg=0; pal_reg < MAX_COLORS_PALETTE; pal_reg++)
				{
					// get the entry data
					color = work_palette[pal_reg];
					
					// make 32 bit copy of color
					red   = color.peRed;
					green = color.peGreen;
					blue  = color.peBlue; 
					
					if ((red+=6) >=255)
						red=255; 
					
					if ((green-=4) < 0)
						green=0;
					
					if ((blue-=4) < 0)
						blue=0;
					
					// store colors back
					color.peRed   = red;
					color.peGreen = green;
					color.peBlue  = blue;
					
					// set the color to a diminished intensity
					work_palette[pal_reg] = color;
					
				} // end for pal_reg
				
				// write the palette back out
				Set_Palette(work_palette);
				
				// wait a bit
				
				Start_Clock(); Wait_Clock(12);
				
			} // end for index
			
		} break;
		
	case SCREEN_BLUENESS:
		{
			// fade to blue
			
			for (index=0; index<64; index++)
			{
				// get the palette 
				Save_Palette(work_palette);
				
				// loop thru all palette registers
				for (pal_reg=0; pal_reg < MAX_COLORS_PALETTE; pal_reg++)
				{
					// get the entry data
					color = work_palette[pal_reg];
					
					// make 32 bit copy of color
					red   = color.peRed;
					green = color.peGreen;
					blue  = color.peBlue; 
					
					if ((red-=4) < 0)
						red=0;
					
					if ((green-=4) < 0)
						green=0;
					
					if ((blue+=6) >=255)
						blue=255;
					
					// store colors back
					color.peRed   = red;
					color.peGreen = green;
					color.peBlue  = blue;
					
					// set the color to a diminished intensity
					work_palette[pal_reg] = color;
					
				} // end for pal_reg
				
				// write the palette back out
				Set_Palette(work_palette);
				
				// wait a bit
				
				Start_Clock(); Wait_Clock(12);
				
			} // end for index
			
		} break;
		
	case SCREEN_GREENNESS:
		{
			// fade to green
			for (index=0; index<64; index++)
			{
				// get the palette 
				Save_Palette(work_palette);
				
				// loop thru all palette registers
				for (pal_reg=0; pal_reg < MAX_COLORS_PALETTE; pal_reg++)
				{
					// get the entry data
					color = work_palette[pal_reg];                  
					
					// make 32 bit copy of color
					red   = color.peRed;
					green = color.peGreen;
					blue  = color.peBlue; 
					
					if ((red-=4) < 0)
						red=0;
					
					if ((green+=6) >=255)
						green=255;
					
					if ((blue-=4) < 0)
						blue=0;
					
					// store colors back
					color.peRed   = red;
					color.peGreen = green;
					color.peBlue  = blue;
					
					// set the color to a diminished intensity
					work_palette[pal_reg] = color; 
					
				} // end for pal_reg
				
				// write the palette back out
				Set_Palette(work_palette);
				
				// wait a bit
				
				Start_Clock(); Wait_Clock(12);
				
				
			} // end for index
			
		} break;
		
	case SCREEN_SWIPE_X:
		{
			// do a screen wipe from right to left, left to right
			for (index=0; index < (screen_width/2); index+=2)
			{
				// use this as a 1/70th of second time delay
				
				Start_Clock(); Wait_Clock(12);
				
				// test screen depth
				if (screen_bpp==8)
				{    
					// draw two vertical lines at opposite ends of the screen
					VLine(0,(screen_height-1),(screen_width-1)-index,0,vbuffer,lpitch);
					VLine(0,(screen_height-1),index,0,vbuffer,lpitch);
					VLine(0,(screen_height-1),(screen_width-1)-(index+1),0,vbuffer,lpitch);
					VLine(0,(screen_height-1),index+1,0,vbuffer,lpitch);
				} // end if 8-bit mode
				else
					if (screen_bpp==16)
					{    
						// 16-bit mode draw two vertical lines at opposite ends of the screen
						VLine16(0,(screen_height-1),(screen_width-1)-index,0,vbuffer,lpitch);
						VLine16(0,(screen_height-1),index,0,vbuffer,lpitch);
						VLine16(0,(screen_height-1),(screen_width-1)-(index+1),0,vbuffer,lpitch);
						VLine16(0,(screen_height-1),index+1,0,vbuffer,lpitch);
					} // end if 16-bit mode
					
					
			} // end for index
			
		} break;
		
	case SCREEN_SWIPE_Y:
		{
			// do a screen wipe from top to bottom, bottom to top
			for (index=0; index < (screen_height/2); index+=2)
			{
				// use this as a 1/70th of second time delay
				
				Start_Clock(); Wait_Clock(12);
				
				// test screen depth             
				if (screen_bpp==8)
				{
					// draw two horizontal lines at opposite ends of the screen
					HLine(0,(screen_width-1),(screen_height-1)-index,0,vbuffer,lpitch);
					HLine(0,(screen_width-1),index,0,vbuffer,lpitch);
					HLine(0,(screen_width-1),(screen_height-1)-(index+1),0,vbuffer,lpitch);
					HLine(0,(screen_width-1),index+1,0,vbuffer,lpitch);
				} // end if 8-bit mode
				else 
					if (screen_bpp==16)
					{
						// draw two horizontal lines at opposite ends of the screen
						HLine16(0,(screen_width-1),(screen_height-1)-index,0,vbuffer,lpitch);
						HLine16(0,(screen_width-1),index,0,vbuffer,lpitch);
						HLine16(0,(screen_width-1),(screen_height-1)-(index+1),0,vbuffer,lpitch);
						HLine16(0,(screen_width-1),index+1,0,vbuffer,lpitch);
					} // end if 16-bit mode
					
			} // end for index
			
			
		} break;
		
	case SCREEN_SCRUNCH:
		{
			// do a screen wipe from top to bottom, bottom to top
			for (index=0; index < (screen_width/2); index+=2)
			{
				// use this as a 1/70th of second time delay
				
				Start_Clock(); Wait_Clock(12);
				
				// test screen depth             
				if (screen_bpp==8)
				{ 
					// draw two horizontal lines at opposite ends of the screen
					HLine(0,(screen_width-1),(screen_height-1)-index%(screen_height/2),0,vbuffer,lpitch);
					HLine(0,(screen_width-1),index%(screen_height/2),0,vbuffer,lpitch);
					HLine(0,(screen_width-1),(screen_height-1)-(index%(screen_height/2)+1),0,vbuffer,lpitch);
					HLine(0,(screen_width-1),index%(screen_height/2)+1,0,vbuffer,lpitch);
					
					// draw two vertical lines at opposite ends of the screen
					VLine(0,(screen_height-1),(screen_width-1)-index,0,vbuffer,lpitch);
					VLine(0,(screen_height-1),index,0,vbuffer,lpitch);
					VLine(0,(screen_height-1),(screen_width-1)-(index+1),0,vbuffer,lpitch);
					VLine(0,(screen_height-1),index+1,0,vbuffer,lpitch);
				} // end if 8-bit mode
				else
					// test screen depth             
					if (screen_bpp==16)
					{ 
						// draw two horizontal lines at opposite ends of the screen
						HLine16(0,(screen_width-1),(screen_height-1)-index%(screen_height/2),0,vbuffer,lpitch);
						HLine16(0,(screen_width-1),index%(screen_height/2),0,vbuffer,lpitch);
						HLine16(0,(screen_width-1),(screen_height-1)-(index%(screen_height/2)+1),0,vbuffer,lpitch);
						HLine16(0,(screen_width-1),index%(screen_height/2)+1,0,vbuffer,lpitch);
						
						// draw two vertical lines at opposite ends of the screen
						VLine16(0,(screen_height-1),(screen_width-1)-index,0,vbuffer,lpitch);
						VLine16(0,(screen_height-1),index,0,vbuffer,lpitch);
						VLine16(0,(screen_height-1),(screen_width-1)-(index+1),0,vbuffer,lpitch);
						VLine16(0,(screen_height-1),index+1,0,vbuffer,lpitch);
					} // end if 8-bit mode
					
			} // end for index
			
		} break;
		
		
	case SCREEN_DISOLVE:
		{
			// disolve the screen by plotting zillions of little black dots
			HCDDrawLib drawLib;
			if (screen_bpp==8)
				for (index=0; index<=screen_width*screen_height*4; index++)
					drawLib.Draw_Pixel(rand()%screen_width,rand()%screen_height,0,vbuffer,lpitch);
				else
					if (screen_bpp==16)
						for (index=0; index<=screen_width*screen_height*4; index++)
							drawLib.Draw_Pixel16(rand()%screen_width,rand()%screen_height,0,vbuffer,lpitch);
						
		} break;
		
	default:break;
		
      } // end switch
	  
} // end Screen_Transitions


int HCDXBuilder::Blink_Colors(int command, BLINKER_PTR new_light, int id)
{
	// this function blinks a set of lights
	
	static BLINKER lights[256]; // supports up to 256 blinking lights
	static int initialized = 0; // tracks if function has initialized
	
	// test if this is the first time function has ran
	if (!initialized)
	{
		// set initialized
		initialized = 1;
		
		// clear out all structures
		memset((void *)lights,0, sizeof(lights));
		
	} // end if
	
	// now test what command user is sending
	switch (command)
	{
	case BLINKER_ADD: // add a light to the database
		{
            // run thru database and find an open light
            for (int index=0; index < 256; index++)
			{
                // is this light available?
                if (lights[index].state == 0)
				{
					// set light up
					lights[index] = *new_light;
					
					// set internal fields up
					lights[index].counter =  0;
					lights[index].state   = -1; // off
					
					// update palette entry
					lpddpal->SetEntries(0,lights[index].color_index,1,&lights[index].off_color);
					
					// return id to caller
					return(index);
					
				} // end if
				
			} // end for index
			
		} break;
		
	case BLINKER_DELETE: // delete the light indicated by id
		{
            // delete the light sent in id 
            if (lights[id].state != 0)
			{
				// kill the light
				memset((void *)&lights[id],0,sizeof(BLINKER));
				
				// return id
				return(id);
				
			} // end if
            else
                return(-1); // problem
			
			
		} break;
		
	case BLINKER_UPDATE: // update the light indicated by id
		{ 
            // make sure light is active
            if (lights[id].state != 0)
			{
				// update on/off parms only
				lights[id].on_color  = new_light->on_color; 
				lights[id].off_color = new_light->off_color;
				lights[id].on_time   = new_light->on_time;
				lights[id].off_time  = new_light->off_time; 
				
				// update palette entry
				if (lights[id].state == -1)
					lpddpal->SetEntries(0,lights[id].color_index,1,&lights[id].off_color);
				else
					lpddpal->SetEntries(0,lights[id].color_index,1,&lights[id].on_color);
				
				// return id
				return(id);
				
			} // end if
            else
                return(-1); // problem
			
		} break;
		
	case BLINKER_RUN: // run the algorithm
		{
            // run thru database and process each light
            for (int index=0; index < 256; index++)
			{
                // is this active?
                if (lights[index].state == -1)
				{
					// update counter
					if (++lights[index].counter >= lights[index].off_time)
					{
						// reset counter
						lights[index].counter = 0;
						
						// change states 
						lights[index].state = -lights[index].state;               
						
						// update color
						lpddpal->SetEntries(0,lights[index].color_index,1,&lights[index].on_color);
						
					} // end if
					
				} // end if
                else
					if (lights[index].state == 1)
					{
						// update counter
						if (++lights[index].counter >= lights[index].on_time)
						{
							// reset counter
							lights[index].counter = 0;
							
							// change states 
							lights[index].state = -lights[index].state;               
							
							// update color
							lpddpal->SetEntries(0,lights[index].color_index,1,&lights[index].off_color);
							
						} // end if
					} // end else if
					
			} // end for index
			
		} break;
		
	default: break;
		
       } // end switch
	   
	   // return success
	   return(1);
	   
} // end Blink_Colors



int HCDXBuilder::Rotate_Colors(int start_index, int end_index)
{
	// this function rotates the color between start and end
	
	int colors = end_index - start_index + 1;
	
	PALETTEENTRY work_pal[MAX_COLORS_PALETTE]; // working palette
	
	// get the color palette
	lpddpal->GetEntries(0,start_index,colors,work_pal);
	
	// shift the colors
	lpddpal->SetEntries(0,start_index+1,colors-1,work_pal);
	
	// fix up the last color
	lpddpal->SetEntries(0,start_index,1,&work_pal[colors - 1]);
	
	// update shadow palette
	lpddpal->GetEntries(0,0,MAX_COLORS_PALETTE,palette);
	
	// return success
	return(1);
	
} // end Rotate_Colors
int HCDXBuilder::Set_Palette(LPPALETTEENTRY set_palette)
{
	// this function writes the sent palette
	
	// first save the new palette in shadow
	memcpy(palette, set_palette,MAX_COLORS_PALETTE*sizeof(PALETTEENTRY));
	
	// now set the new palette
	lpddpal->SetEntries(0,0,MAX_COLORS_PALETTE,palette);
	
	// return success
	return(1);
} // end Set_Palette


int HCDXBuilder::Save_Palette(LPPALETTEENTRY sav_palette)
{
	// this function saves the current palette 
	
	memcpy(sav_palette, palette,MAX_COLORS_PALETTE*sizeof(PALETTEENTRY));
	
	// return success
	return(1);
} // end Save_Palette
int HCDXBuilder::Save_Palette_To_File(char *filename, LPPALETTEENTRY palette)
{
	// this function saves a palette to disk
	
	FILE *fp_file; // working file
	
	// try and open file
	if ((fp_file = fopen(filename,"w"))==NULL)
		return(0);
	
	// write in all 256 colors RGBF
	for (int index=0; index<MAX_COLORS_PALETTE; index++)
    {
		// read the next entry in
		fprintf(fp_file,"\n%d %d %d %d",palette[index].peRed,
			palette[index].peGreen,
			palette[index].peBlue,                                
			palette[index].peFlags);
    } // end for index
	
	// close the file
	fclose(fp_file);
	
	// return success
	return(1);
	
} // end Save_Palette_To_Disk


int HCDXBuilder::Get_Palette_Entry(int color_index,LPPALETTEENTRY color)
{
	// this function retrieves a palette entry from the color
	// palette
	
	// copy data out from shadow palette
	memcpy(color, &palette[color_index],sizeof(PALETTEENTRY));
	
	// return success
	return(1);
} // end Get_Palette_Entry


int HCDXBuilder::Set_Palette_Entry(int color_index, LPPALETTEENTRY color)
{
	// this function sets a palette color in the palette
	lpddpal->SetEntries(0,color_index,1,color);
	
	// set data in shadow palette
	memcpy(&palette[color_index],color,sizeof(PALETTEENTRY));
	
	// return success
	return(1);
} // end Set_Palette_Entry


int HCDXBuilder::Load_Palette_From_File(char *filename, LPPALETTEENTRY palette)
{
	// this function loads a palette from disk into a palette
	// structure, but does not set the pallette
	hclog.Open_Error_File();
	FILE *fp_file; // working file
	
	// try and open file
	if ((fp_file = fopen(filename,"r"))==NULL)
	{
		hclog.Write_Error("\nLoad_Palette_From_File: can't open file! filename: %s !",filename);
		return(0);
	}
	// read in all 256 colors RGBF
	for (int index=0; index<MAX_COLORS_PALETTE; index++)
    {
		// read the next entry in
		fscanf(fp_file,"%d %d %d %d",&palette[index].peRed,
			&palette[index].peGreen,
			&palette[index].peBlue,                                
			&palette[index].peFlags);
    } // end for index
	
	// close the file
	fclose(fp_file);
	hclog.Close_Error_File();
	// return success
	return(1);
} // end Load_Palette_From_Disk


int HCDXBuilder::DDraw_Unlock_Back_Surface(void)
{
	// this unlocks the secondary
	hclog.Open_Error_File();
	// is this surface valid
	if (!back_buffer)
	{
		hclog.Write_Error("\n HCDXBuilder::DDraw_Unlock_Back_Surface: back_buffer is invalid!");
		return(0);
	}
	// unlock the secondary surface
	lpddsback->Unlock(NULL);
	
	// reset the secondary surface
	back_buffer = NULL;
	back_lpitch = 0;
	hclog.Close_Error_File();
	// return success
	return(1);
} // end DDraw_Unlock_Back_Surface

UCHAR *HCDXBuilder::DDraw_Lock_Back_Surface(void)
{
	// this function locks the secondary back surface and returns a pointer to it
	// and updates the global variables secondary buffer, and back_lpitch
	
	// is this surface already locked
	if (back_buffer)
	{
		// return to current lock
		return(back_buffer);
	} // end if
	
	// lock the primary surface
	DDRAW_INIT_STRUCT(ddsd);
	lpddsback->Lock(NULL,&ddsd,DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,NULL); 
	
	// set globals
	back_buffer = (UCHAR *)ddsd.lpSurface;
	back_lpitch = ddsd.lPitch;
	
	// return pointer to surface
	return(back_buffer);
	
} // end DDraw_Lock_Back_Surface


int HCDXBuilder::DDraw_Unlock_Primary_Surface(void)
{
	// this unlocks the primary
	
	// is this surface valid
	if (!primary_buffer)
		return(0);
	
	// unlock the primary surface
	lpddsprimary->Unlock(NULL);
	
	// reset the primary surface
	primary_buffer = NULL;
	primary_lpitch = 0;
	
	// return success
	return(1);
} // end DDraw_Unlock_Primary_Surface



UCHAR *HCDXBuilder::DDraw_Lock_Primary_Surface(void)
{
	// this function locks the priamary surface and returns a pointer to it
	// and updates the global variables primary_buffer, and primary_lpitch
	
	// is this surface already locked
	if (primary_buffer)
	{
		// return to current lock
		return(primary_buffer);
	} // end if
	
	// lock the primary surface
	DDRAW_INIT_STRUCT(ddsd);
	lpddsprimary->Lock(NULL,&ddsd,DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,NULL); 
	
	// set globals
	primary_buffer = (UCHAR *)ddsd.lpSurface;
	primary_lpitch = ddsd.lPitch;
	
	// return pointer to surface
	return(primary_buffer);
	
} // end DDraw_Lock_Primary_Surface


int HCDXBuilder::DDraw_Unlock_Surface(LPDIRECTDRAWSURFACE7 lpdds)
{
	// this unlocks a general surface
	
	// is this surface valid
	if (!lpdds)
		return(0);
	
	// unlock the surface memory
	lpdds->Unlock(NULL);
	
	// return success
	return(1);
} // end DDraw_Unlock_Surface

UCHAR *HCDXBuilder::DDraw_Lock_Surface(LPDIRECTDRAWSURFACE7 lpdds, int *lpitch)
{
	// this function locks the sent surface and returns a pointer to it
	
	// is this surface valid
	if (!lpdds)
		return(NULL);
	
	// lock the surface
	DDRAW_INIT_STRUCT(ddsd);
	lpdds->Lock(NULL,&ddsd,DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,NULL); 
	
	// set the memory pitch
	if (lpitch)
		*lpitch = ddsd.lPitch;
	
	// return pointer to surface
	return((UCHAR *)ddsd.lpSurface);
	
} // end DDraw_Lock_Surface

int HCDXBuilder::DDraw_Fill_Surface(LPDIRECTDRAWSURFACE7 lpdds, USHORT color, RECT *client)
{
	DDBLTFX ddbltfx; // this contains the DDBLTFX structure
	
	// clear out the structure and set the size field 
	DDRAW_INIT_STRUCT(ddbltfx);
	
	// set the dwfillcolor field to the desired color
	ddbltfx.dwFillColor = color; 
	
	// ready to blt to surface
	lpdds->Blt(client,     // ptr to dest rectangle
		NULL,       // ptr to source surface, NA            
		NULL,       // ptr to source rectangle, NA
		DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
		&ddbltfx);  // ptr to DDBLTFX structure
	
	// return success
	return(1);
} // end DDraw_Fill_Surface

int HCDXBuilder::DDraw_Wait_For_Vsync()
{
	// this function waits for a vertical blank to begin
    
	lpdd7->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN,0);
	
	// return success
	return(1);
} // end DDraw_Wait_For_Vsync


int HCDXBuilder::DDraw_Flip(void)
{
	// this function flip the primary surface with the secondary surface
	
	// test if either of the buffers are locked
	if (primary_buffer || back_buffer)
		return(0);
	
	// flip pages
	if (!screen_windowed)
		while(FAILED(lpddsprimary->Flip(NULL, DDFLIP_WAIT)));
		else
		{
			RECT    dest_rect;    // used to compute destination rectangle
			
			// get the window's rectangle in screen coordinates
			GetWindowRect(this->main_hwnd, &dest_rect);   
			
			// compute the destination rectangle
			dest_rect.left   +=window_client_x0;
			dest_rect.top    +=window_client_y0;
			
			dest_rect.right  =dest_rect.left+screen_width;
			dest_rect.bottom =dest_rect.top +screen_height;
			
			// clip the screen coords 
			
			
			
			// blit the entire back surface to the primary
			if (FAILED(lpddsprimary->Blt(&dest_rect, lpddsback,NULL,DDBLT_WAIT,NULL)))
				return(0);    
			
		} // end if
		
		// return success
		return(1);
		
} // end DDraw_Flip


LPDIRECTDRAWSURFACE7 HCDXBuilder::DDraw_Create_Surface(int width, 
													   int height, 
													   int mem_flags, 
													   USHORT color_key_value)
{
	// this function creates an offscreen plain surface
	
	DDSURFACEDESC2 ddsd;         // working description
	LPDIRECTDRAWSURFACE7 lpdds;  // temporary surface
    
	// set to access caps, width, and height
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize  = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	
	// set dimensions of the new bitmap surface
	ddsd.dwWidth  =  width;
	ddsd.dwHeight =  height;
	
	// set surface to offscreen plain
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | mem_flags;
	
	// create the surface
	if (FAILED(lpdd7->CreateSurface(&ddsd,&lpdds,NULL)))
		return(NULL);
	
	// set color key to default color 000
	// note that if this is a 8bit bob then palette index 0 will be 
	// transparent by default
	// note that if this is a 16bit bob then RGB value 000 will be 
	// transparent
	DDCOLORKEY color_key; // used to set color key
	color_key.dwColorSpaceLowValue  = color_key_value;
	color_key.dwColorSpaceHighValue = color_key_value;
	
	// now set the color key for source blitting
	lpdds->SetColorKey(DDCKEY_SRCBLT, &color_key);
	
	// return surface
	return(lpdds);
} // end DDraw_Create_Surface

LPDIRECTDRAWCLIPPER HCDXBuilder::DDraw_Attach_Clipper(LPDIRECTDRAWSURFACE7 lpdds,
													  int num_rects,
													  LPRECT clip_list)
													  
{
	// this function creates a clipper from the sent clip list and attaches
	// it to the sent surface
	
	int index;                         // looping var
	LPDIRECTDRAWCLIPPER lpddclipper;   // pointer to the newly created dd clipper
	LPRGNDATA region_data;             // pointer to the region data that contains
	// the header and clip list
	
	// first create the direct draw clipper
	if (FAILED(lpdd7->CreateClipper(0,&lpddclipper,NULL)))
		return(NULL);
	
	// now create the clip list from the sent data
	
	// first allocate memory for region data
	region_data = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER)+num_rects*sizeof(RECT));
	
	// now copy the rects into region data
	memcpy(region_data->Buffer, clip_list, sizeof(RECT)*num_rects);
	
	// set up fields of header
	region_data->rdh.dwSize          = sizeof(RGNDATAHEADER);
	region_data->rdh.iType           = RDH_RECTANGLES;
	region_data->rdh.nCount          = num_rects;
	region_data->rdh.nRgnSize        = num_rects*sizeof(RECT);
	
	region_data->rdh.rcBound.left    =  64000;
	region_data->rdh.rcBound.top     =  64000;
	region_data->rdh.rcBound.right   = -64000;
	region_data->rdh.rcBound.bottom  = -64000;
	
	// find bounds of all clipping regions
	for (index=0; index<num_rects; index++)
	{
		// test if the next rectangle unioned with the current bound is larger
		if (clip_list[index].left < region_data->rdh.rcBound.left)
			region_data->rdh.rcBound.left = clip_list[index].left;
		
		if (clip_list[index].right > region_data->rdh.rcBound.right)
			region_data->rdh.rcBound.right = clip_list[index].right;
		
		if (clip_list[index].top < region_data->rdh.rcBound.top)
			region_data->rdh.rcBound.top = clip_list[index].top;
		
		if (clip_list[index].bottom > region_data->rdh.rcBound.bottom)
			region_data->rdh.rcBound.bottom = clip_list[index].bottom;
		
	} // end for index
	
	// now we have computed the bounding rectangle region and set up the data
	// now let's set the clipping list
	
	if (FAILED(lpddclipper->SetClipList(region_data, 0)))
	   {

		// release memory and return error
		free(region_data);
		return(NULL);
	   } // end if
	
	// now attach the clipper to the surface
	if (FAILED(lpdds->SetClipper(lpddclipper)))
	   {
		// release memory and return error
		free(region_data);
		return(NULL);
	   } // end if
	
	// all is well, so release memory and send back the pointer to the new clipper
	free(region_data);
	return(lpddclipper);
	
} // end DDraw_Attach_Clipper


int HCDXBuilder::DDraw_Shutdown(void)
{
	
	// this function release all the resources directdraw
	// allocated, mainly to com objects
	
	// release the clippers first
	if (!ISNULL(lpddclipper))
		lpddclipper->Release();
	
	if (!ISNULL(lpddclipperwin))
		lpddclipperwin->Release();
	
	// release the palette if there is one
	if (!ISNULL(lpddpal))
		lpddpal->Release();
	
	// release the secondary surface
	if (!ISNULL(lpddsback))
		lpddsback->Release();
	
	// release the primary surface
	if (!ISNULL(lpddsprimary))
		lpddsprimary->Release();
	
	// finally, the main dd object
	if (!ISNULL(lpdd7))
		lpdd7->Release();
	return 1;
}

int HCDXBuilder::DDraw_Init(int width, int height, int bpp, int windowed)
{
	// this function initializes directdraw
	int index; // looping variable
	
	hclog.Open_Error_File();
	// create IDirectDraw interface 7.0 object and test for error
	if (FAILED(DirectDrawCreateEx(NULL, (void **)&lpdd7, IID_IDirectDraw7, NULL)))
	{
		hclog.Write_Error("\n HCDXBuilder::DDraw_Init: DirectDrawCreateEx Failed!");
		return(0);
	}
	// based on windowed or fullscreen set coorperation level
	if(this->main_hwnd == NULL)
	{
		hclog.Write_Error("\n HCDXBuilder::DDraw_Init:must  call CreateWndClassEx frist!");
		return(0);
	}
	if (windowed)
	   {
			// set cooperation level to windowed mode 
			if (FAILED(lpdd7->SetCooperativeLevel(main_hwnd,DDSCL_NORMAL)))
			{
				
				hclog.Write_Error("\n  HCDXBuilder::DDraw_Init:SetCooperativeLevel Failed in Normal mode!");
				
				return(0);
			}
	   } // end if
	else
	   {
		// set cooperation level to fullscreen mode 
		if (FAILED(lpdd7->SetCooperativeLevel(main_hwnd,
			DDSCL_ALLOWMODEX | DDSCL_FULLSCREEN | 
			DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT | DDSCL_MULTITHREADED )))
		{
			hclog.Write_Error("\n  HCDXBuilder::DDraw_Init:SetCooperativeLevel Failed in FullScreen mode!");
			return(0);
		}
		// set the display mode
		if (FAILED(lpdd7->SetDisplayMode(width,height,bpp,0,0)))
		{
			hclog.Write_Error("\n  HCDXBuilder::DDraw_Init:SetDisplayMode Failed with width: %d,height: %d,bpp: %d !",width,height,bpp);
			return(0);
		}
	   } // end else
	
	// set globals
	screen_height   = height;
	screen_width    = width;
	screen_bpp      = bpp;
	screen_windowed = windowed;
	
	// Create the primary surface
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	
	// we need to let dd know that we want a complex 
	// flippable surface structure, set flags for that
	if (!screen_windowed)
	   {
		// fullscreen mode
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
		
		// set the backbuffer count to 0 for windowed mode
		// 1 for fullscreen mode, 2 for triple buffering
		ddsd.dwBackBufferCount = 1;
	   } // end if
	else
	   {
		// windowed mode
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		
		// set the backbuffer count to 0 for windowed mode
		// 1 for fullscreen mode, 2 for triple buffering
		ddsd.dwBackBufferCount = 0;
	   } // end else
	
	// create the primary surface
	if(FAILED(lpdd7->CreateSurface(&ddsd,&lpddsprimary,NULL)))
	{
		hclog.Write_Error("\n HCDXBuilder::DDraw_Init:CreateSurface Failed with backbuffercount: %d!",ddsd.dwBackBufferCount);
		return 0;
	}
	// get the pixel format of the primary surface
	DDPIXELFORMAT ddpf; // used to get pixel format
	
	// initialize structure
	DDRAW_INIT_STRUCT(ddpf);
	
	// query the format from primary surface
	lpddsprimary->GetPixelFormat(&ddpf);
	
	// based on masks determine if system is 5.6.5 or 5.5.5
	//RGB Masks for 5.6.5 mode
	//DDPF_RGB  16 R: 0x0000F800  
	//             G: 0x000007E0  
	//             B: 0x0000001F  
	
	//RGB Masks for 5.5.5 mode
	//DDPF_RGB  16 R: 0x00007C00  
	//             G: 0x000003E0  
	//             B: 0x0000001F  
	// test for 6 bit green mask)
	//if (ddpf.dwGBitMask == 0x000007E0)
	//   dd_pixel_format = DD_PIXEL_FORMAT565;
	
	// use number of bits, better method
	this->dd_pixel_format = ddpf.dwRGBBitCount;
	
	hclog.Write_Error("\n HCDXBuilder::DDraw_Init:pixel format of dwRGBBitCount = %d",dd_pixel_format);
	
	// set up conversion macros, so you don't have to test which one to use
	if (dd_pixel_format == DD_PIXEL_FORMAT555)
	   {
		RGB16Bit = RGB16Bit555;
		hclog.Write_Error("\n HCDXBuilder::DDraw_Init:pixel format = 5.5.5");
	   } // end if
	else
	   {
		RGB16Bit = RGB16Bit565;
		hclog.Write_Error("\n HCDXBuilder::DDraw_Init:pixel format = 5.6.5");
	   } // end else
	
	// only need a backbuffer for fullscreen modes
	if (!screen_windowed)
	   {
		// query for the backbuffer i.e the secondary surface
		ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
		
		if (FAILED(lpddsprimary->GetAttachedSurface(&ddscaps,&lpddsback)))
		{
			hclog.Write_Error("\n HCDXBuilder::DDraw_Init:GetAttachedSurface Failed");
			return(0);
		}
	   } // end if
	else
	   {
		// must be windowed, so create a double buffer that will be blitted
		// rather than flipped as in full screen mode
		lpddsback = DDraw_Create_Surface(width, height); // int mem_flags, USHORT color_key_flag);
		
	   } // end else
	
	// create a palette only if 8bit mode
	if (screen_bpp==DD_PIXEL_FORMAT8)
	{
		// create and attach palette
		// clear all entries, defensive programming
		memset(palette,0,MAX_COLORS_PALETTE*sizeof(PALETTEENTRY));
		
		// load a pre-made "good" palette off disk
		if(Load_Palette_From_File(DEFAULT_PALETTE_FILE, palette)!=1)
		{
			hclog.Write_Error("\n HCDXBuilder::DDraw_Init:Load_Palette_From_File Failed");
			return 0;
		}
		
		// load and attach the palette, test for windowed mode
		if (screen_windowed)
		{
			// in windowed mode, so the first 10 and last 10 entries have
			// to be slightly modified as does the call to createpalette
			// reset the peFlags bit to PC_EXPLICIT for the "windows" colors
			for (index=0; index < 10; index++)
				palette[index].peFlags = palette[index+246].peFlags = PC_EXPLICIT;         
			
			// now create the palette object, but disable access to all 256 entries
			if (FAILED(lpdd7->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE,
				palette,&lpddpal,NULL)))
			{
				hclog.Write_Error("\n HCDXBuilder::DDraw_Init:CreatePalette Failed");	
				return(0);
			}
		} // end 
		else
		{
			// in fullscreen mode, so simple create the palette with the default palette
			// and fill in all 256 entries
			if (FAILED(lpdd7->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE | DDPCAPS_ALLOW256,
				palette,&lpddpal,NULL)))
			{
				hclog.Write_Error("\n HCDXBuilder::DDraw_Init:CreatePalette Failed!");	
				return(0);
			}
		} // end if
		
		// now attach the palette to the primary surface
		if (FAILED(lpddsprimary->SetPalette(lpddpal)))
		{
			hclog.Write_Error("\n HCDXBuilder::DDraw_Init:SetPalette Failed!");
			return(0);
		}
	} // end if attach palette for 8bit mode
	
	// clear out both primary and secondary surfaces
	if (screen_windowed)
	   {
			// only clear backbuffer
			DDraw_Fill_Surface(lpddsback,0);
	   } // end if
	else
	   {
			// fullscreen, simply clear everything
			DDraw_Fill_Surface(lpddsprimary,0);
			DDraw_Fill_Surface(lpddsback,0);
	   } // end else
	
	// set software algorithmic clipping region
	min_clip_x = 0;
	max_clip_x = screen_width - 1;
	min_clip_y = 0;
	max_clip_y = screen_height - 1;
	
	// setup backbuffer clipper always
	RECT screen_rect = {0,0,screen_width,screen_height};
	lpddclipper = DDraw_Attach_Clipper(lpddsback,1,&screen_rect);
	
	// set up windowed mode clipper
	if (screen_windowed)
	   {
		// set windowed clipper
		if (FAILED(lpdd7->CreateClipper(0,&lpddclipperwin,NULL)))
		{
			hclog.Write_Error("\n HCDXBuilder::DDraw_Init:CreateClipper Failed!");
			return(0);
		}
		if (FAILED(lpddclipperwin->SetHWnd(0, main_hwnd)))
		{
			hclog.Write_Error("\n HCDXBuilder::DDraw_Init:SetHWnd Failed!");
			return(0);
		}
		if (FAILED(lpddsprimary->SetClipper(lpddclipperwin)))
		{	

			hclog.Write_Error("\n HCDXBuilder::DDraw_Init:SetClipper Failed!");	
			return(0);
		}
	 } // end if screen windowed
	hclog.Close_Error_File();
	// return success
	return(1);
	
} // end DDraw_Init















int HCDXBuilder::Get_Back_lPitch()
{
	return this->back_lpitch;
}

int HCDXBuilder::Get_Primary_lPitch()
{
	return this->primary_lpitch;
}

UCHAR *HCDXBuilder::Get_Back_Buffer()
{
	if(back_lpitch == -1) return NULL;
	return this->back_buffer;
}

UCHAR *HCDXBuilder::Get_Primary_Buffer()
{
	if(primary_lpitch == -1) return NULL;
	
	return this->primary_buffer;
}