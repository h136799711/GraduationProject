#include"HCResource.h"

int HCBitmap::Scan_Image_Bitmap(BITMAP_FILE_PTR bitmap,     // bitmap file to scan image data from
								LPDIRECTDRAWSURFACE7 lpdds, // surface to hold data
								int cx,int cy)              // cell to scan image from
{
	// this function extracts a bitmap out of a bitmap file
	
	UCHAR *source_ptr,   // working pointers
		*dest_ptr;
	
	DDSURFACEDESC2 ddsd;  //  direct draw surface description 
	
	// get the addr to destination surface memory
	
	// set size of the structure
	ddsd.dwSize = sizeof(ddsd);
	
	// lock the display surface
	lpdds->Lock(NULL,
		&ddsd,
		DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,
		NULL);
	
	// compute position to start scanning bits from
	cx = cx*(ddsd.dwWidth+1) + 1;
	cy = cy*(ddsd.dwHeight+1) + 1;
	
	// extract bitmap data
	source_ptr = bitmap->buffer + cy*bitmap->bitmapinfoheader.biWidth+cx;
	
	// assign a pointer to the memory surface for manipulation
	dest_ptr = (UCHAR *)ddsd.lpSurface;
	
	// iterate thru each scanline and copy bitmap
	for (unsigned int index_y=0; index_y < ddsd.dwHeight; index_y++)
    {
		// copy next line of data to destination
		memcpy(dest_ptr, source_ptr, ddsd.dwWidth);
		
		// advance pointers
		dest_ptr   += (ddsd.lPitch);
		source_ptr += bitmap->bitmapinfoheader.biWidth;
    } // end for index_y
	
	// unlock the surface 
	lpdds->Unlock(NULL);
	
	// return success
	return(1);
	
} // end Scan_Image_Bitmap

int HCBitmap::Flip_Bitmap(UCHAR *image, int bytes_per_line, int height)
{
	// this function is used to flip bottom-up .BMP images
	
	UCHAR *buffer; // used to perform the image processing
	int index;     // looping index
	
	// allocate the temporary buffer
	if (!(buffer = (UCHAR *)malloc(bytes_per_line*height)))
		return(0);
	
	// copy image to work area
	memcpy(buffer,image,bytes_per_line*height);
	
	// flip vertically
	for (index=0; index < height; index++)
		memcpy(&image[((height-1) - index)*bytes_per_line],
		&buffer[index*bytes_per_line], bytes_per_line);
	
	// release the memory
	free(buffer);
	
	// return success
	return(1);
	
} // end Flip_Bitmap

int HCBitmap::Unload_Bitmap_File(BITMAP_FILE_PTR bitmap)
{
	// this function releases all memory associated with "bitmap"
	if (!ISNULL(bitmap->buffer))
	{
		// release memory
		free(bitmap->buffer);
		
		// reset pointer
		bitmap->buffer = NULL;
		
	} // end if
	
	// return success
	return(1);
	
} // end Unload_Bitmap_File

int HCBitmap::Load_Bitmap_File(BITMAP_FILE_PTR bitmap, char *filename,int dd_pixel_format)
{
	// this function opens a bitmap file and loads the data into bitmap
	
	int file_handle,  // the file handle
		index;        // looping index
	
	UCHAR   *temp_buffer = NULL; // used to convert 24 bit images to 16 bit
	OFSTRUCT file_data;          // the file data information
	
	// open the file if it exists
	if ((file_handle = OpenFile(filename,&file_data,OF_READ))==-1)
		return(0);
	
	// now load the bitmap file header
	_lread(file_handle, &bitmap->bitmapfileheader,sizeof(BITMAPFILEHEADER));
	
	// test if this is a bitmap file
	if (bitmap->bitmapfileheader.bfType!=BITMAP_ID)
	{
		// close the file
		_lclose(file_handle);
		
		// return error
		return(0);
	} // end if
	
	// now we know this is a bitmap, so read in all the sections
	
	// first the bitmap infoheader
	
	// now load the bitmap file header
	_lread(file_handle, &bitmap->bitmapinfoheader,sizeof(BITMAPINFOHEADER));
	
	// now load the color palette if there is one
	if (bitmap->bitmapinfoheader.biBitCount == 8)
	{
		_lread(file_handle, &bitmap->palette,MAX_COLORS_PALETTE*sizeof(PALETTEENTRY));
		
		// now set all the flags in the palette correctly and fix the reversed 
		// BGR RGBQUAD data format
		for (index=0; index < MAX_COLORS_PALETTE; index++)
		{
			// reverse the red and green fields
			int temp_color                = bitmap->palette[index].peRed;
			bitmap->palette[index].peRed  = bitmap->palette[index].peBlue;
			bitmap->palette[index].peBlue = temp_color;
			
			// always set the flags word to this
			bitmap->palette[index].peFlags = PC_NOCOLLAPSE;
		} // end for index
		
    } // end if
	
	// finally the image data itself
	_lseek(file_handle,-(int)(bitmap->bitmapinfoheader.biSizeImage),SEEK_END);
	
	// now read in the image
	if (bitmap->bitmapinfoheader.biBitCount==8 || bitmap->bitmapinfoheader.biBitCount==16) 
	{
		// delete the last image if there was one
		if (bitmap->buffer)
			free(bitmap->buffer);
		
		// allocate the memory for the image
		if (!(bitmap->buffer = (UCHAR *)malloc(bitmap->bitmapinfoheader.biSizeImage)))
		{
			// close the file
			_lclose(file_handle);
			
			// return error
			return(0);
		} // end if
		
		// now read it in
		_lread(file_handle,bitmap->buffer,bitmap->bitmapinfoheader.biSizeImage);
		
	} // end if
	else
		if (bitmap->bitmapinfoheader.biBitCount==24)
		{
			// allocate temporary buffer to load 24 bit image
			if (!(temp_buffer = (UCHAR *)malloc(bitmap->bitmapinfoheader.biSizeImage)))
			{
				// close the file
				_lclose(file_handle);
				
				// return error
				return(0);
			} // end if
			
			// allocate final 16 bit storage buffer
			if (!(bitmap->buffer=(UCHAR *)malloc(2*bitmap->bitmapinfoheader.biWidth*bitmap->bitmapinfoheader.biHeight)))
			{
				// close the file
				_lclose(file_handle);
				
				// release working buffer
				free(temp_buffer);
				
				// return error
				return(0);
			} // end if
			
			// now read the file in
			_lread(file_handle,temp_buffer,bitmap->bitmapinfoheader.biSizeImage);
			
			// now convert each 24 bit RGB value into a 16 bit value
			for (index=0; index < bitmap->bitmapinfoheader.biWidth*bitmap->bitmapinfoheader.biHeight; index++)
			{
				// build up 16 bit color word
				USHORT color;
				
				// build pixel based on format of directdraw surface
				if (dd_pixel_format==DD_PIXEL_FORMAT555)
				{
					// extract RGB components (in BGR order), note the scaling
					UCHAR blue  = (temp_buffer[index*3 + 0] >> 3),
						green = (temp_buffer[index*3 + 1] >> 3),
						red   = (temp_buffer[index*3 + 2] >> 3); 
					// use the 555 macro
					color = _RGB16BIT555(red,green,blue);
				} // end if 555
				else
					if (dd_pixel_format==DD_PIXEL_FORMAT565) 
					{
						// extract RGB components (in BGR order), note the scaling
						UCHAR blue  = (temp_buffer[index*3 + 0] >> 3),
							green = (temp_buffer[index*3 + 1] >> 2),
							red   = (temp_buffer[index*3 + 2] >> 3);
						
						// use the 565 macro
						color = _RGB16BIT565(red,green,blue);
						
					} // end if 565
					
					// write color to buffer
					((USHORT *)bitmap->buffer)[index] = color;
					
			} // end for index
			
			// finally write out the correct number of bits
			bitmap->bitmapinfoheader.biBitCount=16;
			
			// release working buffer
			free(temp_buffer);
			
		} // end if 24 bit
		else
		{
			// serious problem
			return(0);
			
		} // end else
		
#if 0
		// write the file info out 
		printf("\nfilename:%s \nsize=%d \nwidth=%d \nheight=%d \nbitsperpixel=%d \ncolors=%d \nimpcolors=%d",
			filename,
			bitmap->bitmapinfoheader.biSizeImage,
			bitmap->bitmapinfoheader.biWidth,
			bitmap->bitmapinfoheader.biHeight,
			bitmap->bitmapinfoheader.biBitCount,
			bitmap->bitmapinfoheader.biClrUsed,
			bitmap->bitmapinfoheader.biClrImportant);
#endif
		
		// close the file
		_lclose(file_handle);
		
		// flip the bitmap
		Flip_Bitmap(bitmap->buffer, 
            bitmap->bitmapinfoheader.biWidth*(bitmap->bitmapinfoheader.biBitCount/8), 
            bitmap->bitmapinfoheader.biHeight);
		
		// return success
		return(1);
		
} // end Load_Bitmap_File


int HCBitmap::Copy_Bitmap(BITMAP_IMAGE_PTR dest_bitmap, int dest_x, int dest_y, 
						  BITMAP_IMAGE_PTR source_bitmap, int source_x, int source_y, 
						  int width, int height)
{
	// this function copies a bitmap from one source to another
	
	// make sure the pointers are at least valid
	if (!dest_bitmap || !source_bitmap)
		return(0);
	
	// do some computations
	int bytes_per_pixel = (source_bitmap->bpp >> 3);
	
	// create some pointers
	UCHAR *source_ptr = source_bitmap->buffer + (source_x + source_y*source_bitmap->width)*bytes_per_pixel;
	UCHAR *dest_ptr   = dest_bitmap->buffer   + (dest_x   + dest_y  *dest_bitmap->width)  *bytes_per_pixel;
	
	// now copying is easy :)
	for (int y = 0; y < height; y++)
    {
		// copy this line
		memcpy(dest_ptr, source_ptr, bytes_per_pixel*width);
		
		// advance the pointers
		source_ptr+=(source_bitmap->width*bytes_per_pixel);
		dest_ptr  +=(dest_bitmap->width*bytes_per_pixel);
    } // end for
	
	// return success
	return(1);
	
} // end Copy_Bitmap

int HCBitmap::Scroll_Bitmap(BITMAP_IMAGE_PTR image, int dx, int dy)
{
	// this function scrolls a bitmap
	
	BITMAP_IMAGE temp_image; // temp image buffer
	
	// are the parms valid 
	if (!image || (dx==0 && dy==0))
		return(0);
	
	
	// scroll on x-axis first
	if (dx!=0)
	{
		// step 1: normalize scrolling amount
		dx %= image->width;
		
		// step 2: which way?
		if (dx > 0)
		{
			// scroll right
			// create bitmap to hold region that is scrolled around
			Create_Bitmap(&temp_image, 0, 0, dx, image->height, image->bpp);
			
			// copy region we are going to scroll and wrap around
			Copy_Bitmap(&temp_image,0,0, 
                image, image->width-dx,0, 
                dx, image->height);
			
			// set some pointers up
			UCHAR *source_ptr = image->buffer;  // start of each line
			int shift         = (image->bpp >> 3)*dx;
			
			// now scroll image to right "scroll" pixels
			for (int y=0; y < image->height; y++)
			{
				// scroll the line over
				memmove(source_ptr+shift, source_ptr, (image->width-dx)*(image->bpp >> 3));
				
				// advance to the next line
				source_ptr+=((image->bpp >> 3)*image->width);
			} // end for
			
			// and now copy it back
			Copy_Bitmap(image, 0,0, &temp_image,0,0, 
				dx, image->height);           
			
		} // end if
		else
		{
			// scroll left
			dx = -dx; // invert sign
			
			// create bitmap to hold region that is scrolled around
			Create_Bitmap(&temp_image, 0, 0, dx, image->height, image->bpp);
			
			// copy region we are going to scroll and wrap around
			Copy_Bitmap(&temp_image,0,0, 
                image, 0,0, 
                dx, image->height);
			
			// set some pointers up
			UCHAR *source_ptr = image->buffer;  // start of each line
			int shift         = (image->bpp >> 3)*dx;
			
			// now scroll image to left "scroll" pixels
			for (int y=0; y < image->height; y++)
			{
				// scroll the line over
				memmove(source_ptr, source_ptr+shift, (image->width-dx)*(image->bpp >> 3));
				
				// advance to the next line
				source_ptr+=((image->bpp >> 3)*image->width);
			} // end for
			
			// and now copy it back
			Copy_Bitmap(image, image->width-dx,0, &temp_image,0,0, 
				dx, image->height);           
			
		} // end else
	} // end scroll on x-axis
	
	
	// return success
	return(1);
	
} // end Scroll_Bitmap


int HCBitmap::Load_Image_Bitmap16(BITMAP_IMAGE_PTR image,  // bitmap image to load with data
								  BITMAP_FILE_PTR bitmap,  // bitmap to scan image data from
								  int cx,int cy,   // cell or absolute pos. to scan image from
								  int mode)        // if 0 then cx,cy is cell position, else 
								  // cx,cy are absolute coords
{
	// this function extracts a 16-bit bitmap out of a 16-bit bitmap file
	
	// is this a valid bitmap
	if (!image)
		return(0);
	
	// must be a 16bit bitmap
	USHORT *source_ptr,   // working pointers
		*dest_ptr;
	
	// test the mode of extraction, cell based or absolute
	if (mode==BITMAP_EXTRACT_MODE_CELL)
	{
		// re-compute x,y
		cx = cx*(image->width+1) + 1;
		cy = cy*(image->height+1) + 1;
	} // end if
	
	// extract bitmap data
	source_ptr = (USHORT *)bitmap->buffer + 
		cy*bitmap->bitmapinfoheader.biWidth+cx;
	
	// assign a pointer to the bimap image
	dest_ptr = (USHORT *)image->buffer;
	
	int bytes_per_line = image->width*2;
	
	// iterate thru each scanline and copy bitmap
	for (int index_y=0; index_y < image->height; index_y++)
    {
		// copy next line of data to destination
		memcpy(dest_ptr, source_ptr,bytes_per_line);
		
		// advance pointers
		dest_ptr   += image->width;
		source_ptr += bitmap->bitmapinfoheader.biWidth;
    } // end for index_y
	
	// set state to loaded
	image->attr |= BITMAP_ATTR_LOADED;
	
	// return success
	return(1);
	
} // end Load_Image_Bitmap16

int HCBitmap::Load_Image_Bitmap(BITMAP_IMAGE_PTR image,  // bitmap image to load with data
								BITMAP_FILE_PTR bitmap,  // bitmap to scan image data from
								int cx,int cy,   // cell or absolute pos. to scan image from
								int mode)        // if 0 then cx,cy is cell position, else 
								// cx,cy are absolute coords
{
	// this function extracts a bitmap out of a bitmap file
	
	// is this a valid bitmap
	if (!image)
		return(0);
	
	UCHAR *source_ptr,   // working pointers
		*dest_ptr;
	
	// test the mode of extraction, cell based or absolute
	if (mode==BITMAP_EXTRACT_MODE_CELL)
	{
		// re-compute x,y
		cx = cx*(image->width+1) + 1;
		cy = cy*(image->height+1) + 1;
	} // end if
	
	// extract bitmap data
	source_ptr = bitmap->buffer +
		cy*bitmap->bitmapinfoheader.biWidth+cx;
	
	// assign a pointer to the bimap image
	dest_ptr = (UCHAR *)image->buffer;
	
	// iterate thru each scanline and copy bitmap
	for (int index_y=0; index_y<image->height; index_y++)
    {
		// copy next line of data to destination
		memcpy(dest_ptr, source_ptr,image->width);
		
		// advance pointers
		dest_ptr   += image->width;
		source_ptr += bitmap->bitmapinfoheader.biWidth;
    } // end for index_y
	
	// set state to loaded
	image->attr |= BITMAP_ATTR_LOADED;
	
	// return success
	return(1);
	
} // end Load_Image_Bitmap


int HCBitmap::Draw_Bitmap16(BITMAP_IMAGE_PTR source_bitmap,UCHAR *dest_buffer, int lpitch, int transparent)
{
	// this function draws the bitmap onto the destination memory surface
	// if transparent is 1 then color 0 (8bit) or 0.0.0 (16bit) will be transparent
	// note this function does NOT clip, so be carefull!!!
	
	// test if this bitmap is loaded
	if (!(source_bitmap->attr & BITMAP_ATTR_LOADED))
		return(0);
	
	USHORT *dest_addr,   // starting address of bitmap in destination
		*source_addr; // starting adddress of bitmap data in source
	
	USHORT pixel;        // used to hold pixel value
	
	int index,           // looping vars
		pixel_x,
		lpitch_2 = lpitch >> 1; // lpitch in USHORT terms
	
	// compute starting destination address
	dest_addr = ((USHORT *)dest_buffer) + source_bitmap->y*lpitch_2 + source_bitmap->x;
	
	// compute the starting source address
	source_addr = (USHORT *)source_bitmap->buffer;
	
	// is this bitmap transparent
	if (transparent)
	{
		// copy each line of bitmap into destination with transparency
		for (index=0; index<source_bitmap->height; index++)
		{
			// copy the memory
			for (pixel_x=0; pixel_x<source_bitmap->width; pixel_x++)
			{
				if ((pixel = source_addr[pixel_x])!=0)
					dest_addr[pixel_x] = pixel;
				
			} // end if
			
			// advance all the pointers
			dest_addr   += lpitch_2;
			source_addr += source_bitmap->width;
			
		} // end for index
	} // end if
	else
	{
		// non-transparent version
		// copy each line of bitmap into destination
		
		int source_bytes_per_line = source_bitmap->width*2;
		
		for (index=0; index < source_bitmap->height; index++)
		{
			// copy the memory
			memcpy(dest_addr, source_addr, source_bytes_per_line);
			
			// advance all the pointers
			dest_addr   += lpitch_2;
			source_addr += source_bitmap->width;
			
		} // end for index
		
	} // end else
	
	// return success
	return(1);
	
} // end Draw_Bitmap16


int HCBitmap::Draw_Bitmap(BITMAP_IMAGE_PTR source_bitmap,UCHAR *dest_buffer, int lpitch, int transparent)
{
	// this function draws the bitmap onto the destination memory surface
	// if transparent is 1 then color 0 (8bit) or 0.0.0 (16bit) will be transparent
	// note this function does NOT clip, so be carefull!!!
	
	// test if this bitmap is loaded
	if (!(source_bitmap->attr & BITMAP_ATTR_LOADED))
		return(0);
	
    UCHAR *dest_addr,   // starting address of bitmap in destination
		*source_addr; // starting adddress of bitmap data in source
	
    UCHAR pixel;        // used to hold pixel value
	
    int index,          // looping vars
        pixel_x;
	
	// compute starting destination address
	dest_addr = dest_buffer + source_bitmap->y*lpitch + source_bitmap->x;
	
	// compute the starting source address
	source_addr = source_bitmap->buffer;
	
	// is this bitmap transparent
	if (transparent)
	{
		// copy each line of bitmap into destination with transparency
		for (index=0; index<source_bitmap->height; index++)
		{
			// copy the memory
			for (pixel_x=0; pixel_x<source_bitmap->width; pixel_x++)
			{
				if ((pixel = source_addr[pixel_x])!=0)
					dest_addr[pixel_x] = pixel;
				
			} // end if
			
			// advance all the pointers
			dest_addr   += lpitch;
			source_addr += source_bitmap->width;
			
		} // end for index
	} // end if
	else
	{
		// non-transparent version
		// copy each line of bitmap into destination
		
		for (index=0; index < source_bitmap->height; index++)
		{
			// copy the memory
			memcpy(dest_addr, source_addr, source_bitmap->width);
			
			// advance all the pointers
			dest_addr   += lpitch;
			source_addr += source_bitmap->width;
			
		} // end for index
		
	} // end else
	
	// return success
	return(1);
	
} // end Draw_Bitmap

int HCBitmap::Destroy_Bitmap(BITMAP_IMAGE_PTR image)
{
	// this function releases the memory associated with a bitmap
	
	if (image && image->buffer)
	{
		// free memory and reset vars
		free(image->buffer);
		
		// set all vars in structure to 0
		memset(image,0,sizeof(BITMAP_IMAGE));
		
		// return success
		return(1);
		
	} // end if
	else  // invalid entry pointer of the object wasn't initialized
		return(0);
	
} // end Destroy_Bitmap


int HCBitmap::Create_Bitmap(BITMAP_IMAGE_PTR image, int x, int y, int width, int height, int bpp)
{
	// this function is used to intialize a bitmap, 8 or 16 bit
	
	// allocate the memory
	if (!(image->buffer = (UCHAR *)malloc(width*height*(bpp>>3))))
		return(0);
	
	// initialize variables
	image->state     = BITMAP_STATE_ALIVE;
	image->attr      = 0;
	image->width     = width;
	image->height    = height;
	image->bpp       = bpp;
	image->x         = x;
	image->y         = y;
	image->num_bytes = width*height*(bpp>>3);
	
	// clear memory out
	memset(image->buffer,0,width*height*(bpp>>3));
	
	// return success
	return(1);
	
} // end Create_Bitmap









int HCBob::Collision_BOBS(BOB_PTR bob1, BOB_PTR bob2)
{
	// are these a valid bobs
	if (!bob1 || !bob2)
		return(0);
	
	// get the radi of each rect
	int width1  = (bob1->width>>1) - (bob1->width>>3);
	int height1 = (bob1->height>>1) - (bob1->height>>3);
	
	int width2  = (bob2->width>>1) - (bob2->width>>3);
	int height2 = (bob2->height>>1) - (bob2->height>>3);
	
	// compute center of each rect
	int cx1 = bob1->x + width1;
	int cy1 = bob1->y + height1;
	
	int cx2 = bob2->x + width2;
	int cy2 = bob2->y + height2;
	
	// compute deltas
	int dx = abs(cx2 - cx1);
	int dy = abs(cy2 - cy1);
	
	// test if rects overlap
	if (dx < (width1+width2) && dy < (height1+height2))
		return(1);
	else
		// else no collision
		return(0);
	
} // end Collision_BOBS

int HCBob::Show_BOB(BOB_PTR bob)
{
	// this function shows a bob
	
	// is this a valid bob
	if (!bob)
		return(0);
	
	// set the visibility bit
	SET_BIT(bob->attr, BOB_ATTR_VISIBLE);
	
	// return success
	return(1);
} // end Show_BOB


int HCBob::Hide_BOB(BOB_PTR bob)
{
	// this functions hides bob 
	
	// is this a valid bob
	if (!bob)
		return(0);
	
	// reset the visibility bit
	RESET_BIT(bob->attr, BOB_ATTR_VISIBLE);
	
	// return success
	return(1);
} // end Hide_BOB


int HCBob::Set_Vel_BOB(BOB_PTR bob,int xv, int yv)
{
	// this function sets the velocity of a bob
	
	// is this a valid bob
	if (!bob)
		return(0);
	
	// set velocity
	bob->xv = xv;
	bob->yv = yv;
	
	// return success
	return(1);
} // end Set_Vel_BOB


int HCBob::Set_Animation_BOB(BOB_PTR bob, int anim_index)
{
	// this function sets the animation to play
	
	// is this a valid bob
	if (!bob)
		return(0);
	
	// set the animation index
	bob->curr_animation = anim_index;
	
	// reset animation 
	bob->anim_index = 0;
	
	// return success
	return(1);
	
} // end Set_Animation_BOB

int HCBob::Set_Anim_Speed_BOB(BOB_PTR bob,int speed)
{
	// this function simply sets the animation speed of a bob
    
	// is this a valid bob
	if (!bob)
		return(0);
	
	// set speed
	bob->anim_count_max = speed;
	
	// return success
	return(1);
	
} // end Set_Anim_Speed

int HCBob::Set_Pos_BOB(BOB_PTR bob, int x, int y)
{
	// this functions sets the postion of a bob
	
	// is this a valid bob
	if (!bob)
		return(0);
	
	// set positin
	bob->x = x;
	bob->y = y;
	
	// return success
	return(1);
} // end Set_Pos_BOB


int HCBob::Load_Animation_BOB(BOB_PTR bob, 
							  int anim_index, 
							  int num_frames, 
							  int *sequence)
{
	// this function load an animation sequence for a bob
	// the sequence consists of frame indices, the function
	// will append a -1 to the end of the list so the display
	// software knows when to restart the animation sequence
	
	// is this bob valid
	if (!bob)
		return(0);
	
	// allocate memory for bob animation
	if (!(bob->animations[anim_index] = (int *)malloc((num_frames+1)*sizeof(int))))
		return(0);
	
	// load data into 
	for (int index=0; index<num_frames; index++)
		bob->animations[anim_index][index] = sequence[index];
	
	// set the end of the list to a -1
	bob->animations[anim_index][index] = -1;
	
	// return success
	return(1);
	
} // end Load_Animation_BOB

int HCBob::Move_BOB(BOB_PTR bob,int min_clip_x,int min_clip_y,int max_clip_x,int max_clip_y)
{
	// this function moves the bob based on its current velocity
	// also, the function test for various motion attributes of the'
	// bob and takes the appropriate actions
	
	
	// is this a valid bob
	if (!bob)
		return(0);
	
	// translate the bob
	bob->x+=bob->xv;
	bob->y+=bob->yv;
	
	// test for wrap around
	if (bob->attr & BOB_ATTR_WRAPAROUND)
	   {
		// test x extents first
		if (bob->x > max_clip_x)
			bob->x = min_clip_x - bob->width;
		else
			if (bob->x < min_clip_x-bob->width)
				bob->x = max_clip_x;
			
			// now y extents
			if (bob->x > max_clip_x)
				bob->x = min_clip_x - bob->width;
			else
				if (bob->x < min_clip_x-bob->width)
					bob->x = max_clip_x;
				
	   } // end if
	else
		// test for bounce
		if (bob->attr & BOB_ATTR_BOUNCE)
		{
			// test x extents first
			if ((bob->x > max_clip_x - bob->width) || (bob->x < min_clip_x) )
				bob->xv = -bob->xv;
			
			// now y extents 
			if ((bob->y > max_clip_y - bob->height) || (bob->y < min_clip_y) )
				bob->yv = -bob->yv;
			
		} // end if
		
		// return success
		return(1);
} // end Move_BOB




int HCBob::Animate_BOB(BOB_PTR bob)
{
	// this function animates a bob, basically it takes a look at
	// the attributes of the bob and determines if the bob is 
	// a single frame, multiframe, or multi animation, updates
	// the counters and frames appropriately
	
	// is this a valid bob
	if (!bob)
		return(0);
	
	// test the level of animation
	if (bob->attr & BOB_ATTR_SINGLE_FRAME)
	{
		// current frame always = 0
		bob->curr_frame = 0;
		return(1);
	} // end if
	else
		if (bob->attr & BOB_ATTR_MULTI_FRAME)
		{
			// update the counter and test if its time to increment frame
			if (++bob->anim_counter >= bob->anim_count_max)
			{
				// reset counter
				bob->anim_counter = 0;
				
				// move to next frame
				if (++bob->curr_frame >= bob->num_frames)
					bob->curr_frame = 0;
				
			} // end if
			
		} // end elseif
		else
			if (bob->attr & BOB_ATTR_MULTI_ANIM)
			{
				// this is the most complex of the animations it must look up the
				// next frame in the animation sequence
				
				// first test if its time to animate
				if (++bob->anim_counter >= bob->anim_count_max)
				{
					// reset counter
					bob->anim_counter = 0;
					
					// increment the animation frame index
					bob->anim_index++;
					
					// extract the next frame from animation list 
					bob->curr_frame = bob->animations[bob->curr_animation][bob->anim_index];
					
					// is this and end sequence flag -1
					if (bob->curr_frame == -1)
					{
						// test if this is a single shot animation
						if (bob->attr & BOB_ATTR_ANIM_ONE_SHOT)
						{
							// set animation state message to done
							bob->anim_state = BOB_STATE_ANIM_DONE;
							
							// reset frame back one
							bob->anim_index--;
							
							// extract animation frame
							bob->curr_frame = bob->animations[bob->curr_animation][bob->anim_index];    
							
						} // end if
						else
						{
							// reset animation index
							bob->anim_index = 0;
							
							// extract first animation frame
							bob->curr_frame = bob->animations[bob->curr_animation][bob->anim_index];
						} // end else
						
					}  // end if
					
				} // end if
				
			} // end elseif
			
			// return success
			return(1);
			
} // end Amimate_BOB


int HCBob::Load_Frame_BOB16(BOB_PTR bob, // bob to load with data
							BITMAP_FILE_PTR bitmap, // bitmap to scan image data from
							int frame,       // frame to load
							int cx,int cy,   // cell or absolute pos. to scan image from
							int mode)        // if 0 then cx,cy is cell position, else 
							// cx,cy are absolute coords
{
	// this function extracts a 16-bit bitmap out of a 16-bit bitmap file
	
	DDSURFACEDESC2 ddsd;  //  direct draw surface description 
	
	// is this a valid bob
	if (!bob)
		return(0);
	
	USHORT *source_ptr,   // working pointers
		*dest_ptr;
	
	// test the mode of extraction, cell based or absolute
	if (mode==BITMAP_EXTRACT_MODE_CELL)
	   {
		// re-compute x,y
		cx = cx*(bob->width+1) + 1;
		cy = cy*(bob->height+1) + 1;
	   } // end if
	
	// extract bitmap data
	source_ptr = (USHORT *)bitmap->buffer + cy*bitmap->bitmapinfoheader.biWidth+cx;
	
	// get the addr to destination surface memory
	
	// set size of the structure
	ddsd.dwSize = sizeof(ddsd);
	
	// lock the display surface
	(bob->images[frame])->Lock(NULL,
		&ddsd,
		DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,
		NULL);
	
	// assign a pointer to the memory surface for manipulation
	dest_ptr = (USHORT *)ddsd.lpSurface;
	
	// iterate thru each scanline and copy bitmap
	for (int index_y=0; index_y<bob->height; index_y++)
	{
		// copy next line of data to destination
		memcpy(dest_ptr, source_ptr,(bob->width*2));
		
		// advance pointers
		dest_ptr   += (ddsd.lPitch >> 1); 
		source_ptr += bitmap->bitmapinfoheader.biWidth;
	} // end for index_y
	
	// unlock the surface 
	(bob->images[frame])->Unlock(NULL);
	
	// set state to loaded
	bob->attr |= BOB_ATTR_LOADED;
	
	// return success
	return(1);
	
} // end Load_Frame_BOB16

int HCBob::Load_Frame_BOB(BOB_PTR bob, // bob to load with data
						  BITMAP_FILE_PTR bitmap, // bitmap to scan image data from
						  int frame,       // frame to load
						  int cx,int cy,   // cell or absolute pos. to scan image from
						  int mode)        // if 0 then cx,cy is cell position, else 
						  // cx,cy are absolute coords
{
	// this function extracts a bitmap out of a bitmap file
	
	DDSURFACEDESC2 ddsd;  //  direct draw surface description 
	
	// is this a valid bob
	if (!bob)
		return(0);
	
	UCHAR *source_ptr,   // working pointers
		*dest_ptr;
	
	// test the mode of extraction, cell based or absolute
	if (mode==BITMAP_EXTRACT_MODE_CELL)
	   {
		// re-compute x,y
		cx = cx*(bob->width+1) + 1;
		cy = cy*(bob->height+1) + 1;
	   } // end if
	
	// extract bitmap data
	source_ptr = bitmap->buffer + cy*bitmap->bitmapinfoheader.biWidth+cx;
	
	// get the addr to destination surface memory
	
	// set size of the structure
	ddsd.dwSize = sizeof(ddsd);
	
	// lock the display surface
	(bob->images[frame])->Lock(NULL,
		&ddsd,
		DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,
		NULL);
	
	// assign a pointer to the memory surface for manipulation
	dest_ptr = (UCHAR *)ddsd.lpSurface;
	
	// iterate thru each scanline and copy bitmap
	for (int index_y=0; index_y<bob->height; index_y++)
	{
		// copy next line of data to destination
		memcpy(dest_ptr, source_ptr,bob->width);
		
		// advance pointers
		dest_ptr   += (ddsd.lPitch); // (bob->width+bob->width_fill);
		source_ptr += bitmap->bitmapinfoheader.biWidth;
	} // end for index_y
	
	// unlock the surface 
	(bob->images[frame])->Unlock(NULL);
	
	// set state to loaded
	bob->attr |= BOB_ATTR_LOADED;
	
	// return success
	return(1);
	
} // end Load_Frame_BOB

int HCBob::Draw_Scaled_BOB16(BOB_PTR bob, int swidth, int sheight,  // bob and new dimensions
							 LPDIRECTDRAWSURFACE7 dest) // surface to draw the bob on)
{
	// this function draws a scaled bob to the size swidth, sheight
	
	RECT dest_rect,   // the destination rectangle
		source_rect; // the source rectangle                             
	
	// is this a valid bob
	if (!bob)
		return(0);
	
	// is bob visible
	if (!(bob->attr & BOB_ATTR_VISIBLE))
		return(1);
	
	// fill in the destination rect
	dest_rect.left   = bob->x;
	dest_rect.top    = bob->y;
	dest_rect.right  = bob->x+swidth;
	dest_rect.bottom = bob->y+sheight;
	
	// fill in the source rect
	source_rect.left    = 0;
	source_rect.top     = 0;
	source_rect.right   = bob->width;
	source_rect.bottom  = bob->height;
	
	// blt to destination surface
	if (FAILED(dest->Blt(&dest_rect, bob->images[bob->curr_frame],
		&source_rect,(DDBLT_WAIT | DDBLT_KEYSRC),
		NULL)))
		return(0);
	
	// return success
	return(1);
} // end Draw_Scaled_BOB16


int HCBob::Draw_BOB16(BOB_PTR bob,             // bob to draw
					  LPDIRECTDRAWSURFACE7 dest) // surface to draw the bob on
{
	// draw a bob at the x,y defined in the BOB
	// on the destination surface defined in dest
	
	RECT dest_rect,   // the destination rectangle
		source_rect; // the source rectangle                             
	
	// is this a valid bob
	if (!bob)
		return(0);
	
	// is bob visible
	if (!(bob->attr & BOB_ATTR_VISIBLE))
		return(1);
	
	// fill in the destination rect
	dest_rect.left   = bob->x;
	dest_rect.top    = bob->y;
	dest_rect.right  = bob->x+bob->width;
	dest_rect.bottom = bob->y+bob->height;
	
	// fill in the source rect
	source_rect.left    = 0;
	source_rect.top     = 0;
	source_rect.right   = bob->width;
	source_rect.bottom  = bob->height;
	
	// blt to destination surface
	if (FAILED(dest->Blt(&dest_rect, bob->images[bob->curr_frame],
		&source_rect,(DDBLT_WAIT | DDBLT_KEYSRC),
		NULL)))
		return(0);
	
	// return success
	return(1);
}

int HCBob::Draw_Scaled_BOB(BOB_PTR bob,int swidth,int sheight,LPDIRECTDRAWSURFACE7 dest)
{
	// this function draws a scaled bob to the size swidth, sheight
	
	RECT dest_rect,   // the destination rectangle
		source_rect; // the source rectangle                             
	
	// is this a valid bob
	if (!bob)
		return(0);
	
	// is bob visible
	if (!(bob->attr & BOB_ATTR_VISIBLE))
		return(1);
	
	// fill in the destination rect
	dest_rect.left   = (long)bob->x;
	dest_rect.top    = (long)bob->y;
	dest_rect.right  = (long)bob->x+swidth;
	dest_rect.bottom = (long)bob->y+sheight;
	
	// fill in the source rect
	source_rect.left    = 0;
	source_rect.top     = 0;
	source_rect.right   = bob->width;
	source_rect.bottom  = bob->height;
	
	// blt to destination surface
	if (FAILED(dest->Blt(&dest_rect, bob->images[bob->curr_frame],
		&source_rect,(DDBLT_WAIT | DDBLT_KEYSRC),
		NULL)))
		return(0);
	
	// return success
	return(1);
	
}

int HCBob::Draw_BOB(BOB_PTR bob,LPDIRECTDRAWSURFACE7 dest)
{
	// draw a bob at the x,y defined in the BOB
	// on the destination surface defined in dest
	
	RECT dest_rect,   // the destination rectangle
		source_rect; // the source rectangle                             
	
	// is this a valid bob
	if (!bob)
		return(0);
	
	// is bob visible
	if (!(bob->attr & BOB_ATTR_VISIBLE))
		return(1);
	
	// fill in the destination rect
	dest_rect.left   = (long)bob->x;
	dest_rect.top    = (long)bob->y;
	dest_rect.right  = (long)bob->x+bob->width;
	dest_rect.bottom = (long)bob->y+bob->height;
	
	// fill in the source rect
	source_rect.left    = 0;
	source_rect.top     = 0;
	source_rect.right   = bob->width;
	source_rect.bottom  = bob->height;
	
	// blt to destination surface
	if (FAILED(dest->Blt(&dest_rect, bob->images[bob->curr_frame],
		&source_rect,(DDBLT_WAIT | DDBLT_KEYSRC),
		NULL)))
		return(0);
	
	// return success
	return(1);
	
}

int HCBob::Destroy_BOB(BOB_PTR bob)
{
	
	// destroy the BOB, tests if this is a real bob or a clone
	// if real then release all the memory, otherwise, just resets
	// the pointers to null
	
	int index; // looping var
	
	// is this bob valid
	if (!bob)
		return(0);
	
	// test if this is a clone
	if (bob->attr && BOB_ATTR_CLONE)
    {
		// null link all surfaces
		for (index=0; index<MAX_BOB_FRAMES; index++)
			if (bob->images[index])
				bob->images[index]=NULL;
			
			// release memory for animation sequences 
			for (index=0; index<MAX_BOB_ANIMATIONS; index++)
				if (bob->animations[index])
					bob->animations[index]=NULL;
				
    } // end if
	else
    {
		// destroy each bitmap surface
		for (index=0; index<MAX_BOB_FRAMES; index++)
			if (bob->images[index])
				(bob->images[index])->Release();
			
			// release memory for animation sequences 
			for (index=0; index<MAX_BOB_ANIMATIONS; index++)
				if (bob->animations[index])
					free(bob->animations[index]);
				
    } // end else not clone
	
	return 1;
}

int HCBob::Clone_BOB(BOB_PTR source,BOB_PTR dest)
{	
	
	// this function clones a BOB and updates the attr var to reflect that
	// the BOB is a clone and not real, this is used later in the destroy
	// function so a clone doesn't destroy the memory of a real bob
	
	if ((source && dest) && (source!=dest))
	   {
		// copy the bob data
		memcpy(dest,source, sizeof(BOB));
		
		// set the clone attribute
		dest->attr |= BOB_ATTR_CLONE;
		
	   } // end if
	
	else
		return(0);
	
	return 1;
}

int HCBob::Create_BOB(LPDIRECTDRAW7 lpdd7,BOB_PTR bob,int x,int y,
					  int width,int height,int num_frames,
					  int attr,int mem_flags,
					  USHORT color_key_value,int bpp)
{
	
	
	DDSURFACEDESC2 ddsd; // used to create surface
	int index;           // looping var
	
	// set state and attributes of BOB
	bob->state          = BOB_STATE_ALIVE;
	bob->attr           = attr;
	bob->anim_state     = 0;
	bob->counter_1      = 0;     
	bob->counter_2      = 0;
	bob->max_count_1    = 0;
	bob->max_count_2    = 0;
	
	bob->curr_frame     = 0;
	bob->num_frames     = num_frames;
	bob->bpp            = bpp;
	bob->curr_animation = 0;
	bob->anim_counter   = 0;
	bob->anim_index     = 0;
	bob->anim_count_max = 0; 
	bob->x              = (float)x;
	bob->y              = (float)y;
	bob->xv             = 0;
	bob->yv             = 0;
	
	// set dimensions of the new bitmap surface
	bob->width  = width;
	bob->height = height;
	
	// set all images to null
	for (index=0; index<MAX_BOB_FRAMES; index++)
		bob->images[index] = NULL;
	
	// set all animations to null
	for (index=0; index<MAX_BOB_ANIMATIONS; index++)
		bob->animations[index] = NULL;
	
#if 0
	// make sure surface width is a multiple of 8, some old version of dd like that
	// now, it's unneeded...
	bob->width_fill = ((width%8!=0) ? (8-width%8) : 0);
	Write_Error("\nCreate BOB: width_fill=%d",bob->width_fill);
#endif
	
	// now create each surface
	for (index=0; index<bob->num_frames; index++)
    {
		// set to access caps, width, and height
		memset(&ddsd,0,sizeof(ddsd));
		ddsd.dwSize  = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		
		ddsd.dwWidth  = bob->width + bob->width_fill;
		ddsd.dwHeight = bob->height;
		
		// set surface to offscreen plain
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | mem_flags;
		
		// create the surfaces, return failure if problem
		if (FAILED(lpdd7->CreateSurface(&ddsd,&(bob->images[index]),NULL)))
			return(0);
		
		// set color key to default color 000
		// note that if this is a 8bit bob then palette index 0 will be 
		// transparent by default
		// note that if this is a 16bit bob then RGB value 000 will be 
		// transparent
		DDCOLORKEY color_key; // used to set color key
		color_key.dwColorSpaceLowValue  = color_key_value;
		color_key.dwColorSpaceHighValue = color_key_value;
		
		// now set the color key for source blitting
		(bob->images[index])->SetColorKey(DDCKEY_SRCBLT, &color_key);
		
    } // end for index
	
	return 1;
}