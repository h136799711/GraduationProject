#ifndef H_HCRESOURCE_INCLUDED
#define H_HCRESOURCE_INCLUDED
#pragma warning (disable:4244)
 
#include <stdio.h>
#include <io.h>
#include <ddraw.h>
#include "HCMacros.h"

#define BITMAP_ID	0X4D42
#define BITMAP_STATE_DEAD 0
#define BITMAP_STATE_ALIVE 1
#define BITMAP_STATE_DYING	2
#define BITMAP_ATTR_LOADED	128

#define BITMAP_EXTRACT_MODE_CELL	0
#define BITMAP_EXTRACT_MODE_ABS		1

//defines for BOBS

#define BOB_STATE_DEAD		0
#define BOB_STATE_ALIVE		1
#define BOB_STATE_DYING		2
#define BOB_STATE_ANIM_DONE	1
#define MAX_BOB_FRAMES		64
#define MAX_BOB_ANIMATIONS	16


#define BOB_ATTR_SINGLE_FRAME	1//bob has single frame
#define BOB_ATTR_MULTI_FRAME	2//bob has multiple frames
#define BOB_ATTR_MULTI_ANIM		4//bob has multiple animations
#define BOB_ATTR_ANIM_ONE_SHOT	8// bob will perform the animation once
#define BOB_ATTR_VISIBLE        16  // bob is visible
#define BOB_ATTR_BOUNCE         32  // bob bounces off edges
#define BOB_ATTR_WRAPAROUND     64  // bob wraps around edges
#define BOB_ATTR_LOADED         128 // the bob has been loaded
#define BOB_ATTR_CLONE          256 // the bob is a clone


typedef struct BITMAP_FILE_TAG
{
	BITMAPFILEHEADER bitmapfileheader;
	BITMAPINFOHEADER bitmapinfoheader;
	PALETTEENTRY	palette[256];
	UCHAR		*buffer;
}BITMAP_FILE,*BITMAP_FILE_PTR;

typedef struct BOT_TYP
{
	int state;
	int anim_state;
	int attr;
	float x,y;
	float xv,yv;
	int width,height;
	int width_fill;
	int bpp;
	int counter_1;
	int counter_2;
	int max_count_1;
	int max_count_2;
	int varsI[16];
	float varsF[16];
	int curr_frame;
	int num_frames;
	int curr_animation;
	int anim_counter;
	int anim_index;
	int anim_count_max;
	/*
	 所有图片存储
	*/
	int *animations[MAX_BOB_ANIMATIONS];

	LPDIRECTDRAWSURFACE7 images[MAX_BOB_FRAMES];

}BOB,*BOB_PTR;

//简略的位图图像
typedef struct BITMAP_IMAGE_TYP
{
	int state;
	int attr;
	int x,y;
	int width,height;
	int num_bytes;
	int bpp;
	UCHAR *buffer;
}BITMAP_IMAGE,*BITMAP_IMAGE_PTR;

class HCBitmap
{
public:

	// simple bitmap image functionsExample:
	/*
	// let's create an 8-bit 64x64 bitmap image at (0,0)
	BITMAP_IMAGE ship;

	Create_Bitmap(&ship, 0,0, 64,64,8);

	// and here's the same example in 16-bit mode
	BITMAP_IMAGE ship;
	Create_Bitmap(&ship, 0,0, 64,64,16);*/
	int Create_Bitmap(BITMAP_IMAGE_PTR image, int x, int y, int width, int height, int bpp=8);
	int Destroy_Bitmap(BITMAP_IMAGE_PTR image);
	int Draw_Bitmap(BITMAP_IMAGE_PTR source_bitmap,UCHAR *dest_buffer, int lpitch, int transparent);
	int Draw_Bitmap16(BITMAP_IMAGE_PTR source_bitmap,UCHAR *dest_buffer, int lpitch, int transparent);
	/*
	1.cell mode, BITMAP_EXTRACT_MODE_CELL, the image is scanned making the assumption that all the images are in the .BMP file in a template that is some given size, mxn, with a 1-pixel border between each cell. The cells usually range from 8x8, 16x16, 32x32, 64x64, and so on. Take a look at TEMPLATE*.BMP on the CD; it contains a number of templates. 
		Cell numbers range from left to right, top to bottom, and they start with (0,0).

	2.absolute coordinate mode, BITMAP_EXTRACT_MODE_ABS. In this mode, the image is scanned at the exact coordinates sent in cx, cy. This method is good 
	if you want to load your artwork with various-sized images on the same .BMP; 
	hence, you can't template them.
	*/
	int Load_Image_Bitmap(BITMAP_IMAGE_PTR image,BITMAP_FILE_PTR bitmap,int cx,int cy,int mode);  
	/*
		参考Load_Image_Bitmap
	*/
	int Load_Image_Bitmap16(BITMAP_IMAGE_PTR image,BITMAP_FILE_PTR bitmap,int cx,int cy,int mode);               
	int Scroll_Bitmap(BITMAP_IMAGE_PTR image, int dx, int dy=0);

	int Copy_Bitmap(BITMAP_IMAGE_PTR dest_bitmap, int dest_x, int dest_y, 
					BITMAP_IMAGE_PTR source_bitmap, int source_x, int source_y, 
					int width, int height);
	
	int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height);

	int Scan_Image_Bitmap(BITMAP_FILE_PTR bitmap,     // bitmap file to scan image data from
                      LPDIRECTDRAWSURFACE7 lpdds, // surface to hold data
                      int cx,int cy)       ;       // cell to scan image from

	// bitmap file functions
	int Load_Bitmap_File(BITMAP_FILE_PTR bitmap, char *filename,int dd_pixel_format);
	int Unload_Bitmap_File(BITMAP_FILE_PTR bitmap);
};

/* Create the BOB object, note that all BOBs 
// are created as offscreen surfaces in VRAM as the
// default, if you want to use system memory then
// set flags equal to:
// DDSCAPS_SYSTEMMEMORY 
// for video memory you can create either local VRAM surfaces or AGP 
// surfaces via the second set of constants shown below in the regular expression
// DDSCAPS_VIDEOMEMORY | (DDSCAPS_NONLOCALVIDMEM | DDSCAPS_LOCALVIDMEM ) 
*/
class HCBob
{
public:
	~HCBob()
	{

	
	
	};
	/*
	创建一个BOB对象
	lpdd7 DirectDraw7对象
	bob 一个BOB指针对象
	attr bob的属性
	mem_flags 页面内存标志 默认为0 是VRAM
	color_key_value index for 8-bit模式或RGB值for16BIT模式
	bpp 颜色深度 默认为8
	attr的取值

	BOB_ATTR_SINGLE_FRAME  Creates BOB with a single frame.  
	BOB_ATTR_MULTI_FRAME  Creates BOB with multiple frames, but the animation of the BOB will be a linear sequence through the frames 0..n.  
	BOB_ATTR_MULTI_ANIM  Creates a multiple-frame BOB that supports animation sequences.  
	BOB_ATTR_ANIM_ONE_SHOT  动画效果只执行一次就停止。 At that point the internal variable anim_state will be set. To play the animation again, reset this variable.  
	BOB_ATTR_BOUNCE  This flag tells the BOB to bounce off the screen boundaries like a ball. This only works if you use Move_BOB().  
	BOB_ATTR_WRAPAROUND  This flag tells the BOB to wrap around to the other side of the screen as it moves. This only works if you use Move_BOB().  

	*/
	int Create_BOB(LPDIRECTDRAW7 lpdd7,BOB_PTR bob,int x,int y,int width,int height,
		int num_frames,int attr,int mem_flags=0,USHORT color_key_value=0,int bpp=8);
	
	int Clone_BOB(BOB_PTR source,BOB_PTR dest);
	int Destroy_BOB(BOB_PTR bob);
	int Draw_BOB(BOB_PTR bob,LPDIRECTDRAWSURFACE7 dest);
	int Draw_Scaled_BOB(BOB_PTR bob,int swidth,int sheight,LPDIRECTDRAWSURFACE7 dest);
	int Draw_BOB16(BOB_PTR bob,LPDIRECTDRAWSURFACE7 dest);
	int Draw_Scaled_BOB16(BOB_PTR bob,int swidth,int sheight,LPDIRECTDRAWSURFACE7 dest);
	/*
		bob:ptr of bob to load frame into
		bitmap:ptr of bitmap file to scan data
		frame :frame number to place image into 0,1,2...
		cx,cy: cell pos or abs pos to scan from
		mode: scan mode same as Load_Frame_Bitmap()
	
	*/
	int Load_Frame_BOB(BOB_PTR bob,BITMAP_FILE_PTR bitmap,int frame,
		int cx,int cy,int mode);
	int Load_Frame_BOB16(BOB_PTR bob,BITMAP_FILE_PTR bitmap,int frame,int cx,int cy,int mode);
	/*
	// make animation sequence number 2 active
	Set_Animation_BOB(&alien, 2);

	NOTE

	This also resets the BOB animation to the first frame in the sequence.



	*/
	int Animate_BOB(BOB_PTR bob);
	int Move_BOB(BOB_PTR bob,int min_clip_x,int min_clip_y,int max_clip_x,int max_clip_y);
	/*
	You might have a BOB that has eight frames, 0,1,…7, but you might have four animations defined as follows:

	int anim_walk[]  = {0,1,2,1,0};
	int anim_fire[]  = {5,6,0};
	int anim_die[]   = {3,4};
	int anim_sleep[] = {0,0,7,0,0};

	Then, to load the animations into a 16-bit BOB, you would do the following:

	// create a mutli animation bob
	// create the bob
	if (!Create_BOB(&alien, 0,0, 32,32,8,BOB_ATTR_MULTI_ANIM,0,0,16))
	   {  error  }

	// load the bob frames in... use 16 bit load function!
	// load walk into animation 0
	Load_Animation_BOB(&alien, 0,5,anim_walk);

	// load fire into animation 1
	Load_Animation_BOB(&alien, 1,3,anim_fire);

	// load die into animation 2
	Load_Animation_BOB(&alien, 2,2,anim_die);

	// load sleep into animation 3
	Load_Animation_BOB(&alien, 3,5,anim_sleep);

	After loading the animations, you can set the active animation and play them with functions you'll see in a minute.
	*/
	int Load_Animation_BOB(BOB_PTR bob, int anm_index,int num_frames,int *sequence);
	int Set_Pos_BOB(BOB_PTR bob,int x,int y);
	int Set_Vel_BOB(BOB_PTR bob,int xv,int yv);
	int Set_Anim_Speed_BOB(BOB_PTR bob,int speed);
	int Set_Animation_BOB(BOB_PTR bob,int anim_index);
	int Hide_BOB(BOB_PTR bob);
	int Show_BOB(BOB_PTR bob);
	int Collision_BOBS(BOB_PTR bob1,BOB_PTR bob2);

};
class HCResource
{
public:
	HCResource(){};
};

#endif