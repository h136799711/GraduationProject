#ifndef H_HCDXBUILDER_INCLUDED
#define H_HCDXBUILDER_INCLUDED


#pragma warning (disable:4244)
#include <ddraw.h>
#include "HCMacros.h"
#include "HCWindowBuilder.h"
#include "HCUtility.h"
#include "HCDDrawLib.h"


#define DEFAULT_PALETTE_FILE "DEFAULT_PALDATA.PAL"


// defines for Blink_Colors
#define BLINKER_ADD           0    // add a light to database  
#define BLINKER_DELETE        1    // delete a light from database
#define BLINKER_UPDATE        2    // update a light
#define BLINKER_RUN           3    // run normal
typedef struct BLINKER_TYP
{
	int color_index;
	PALETTEENTRY on_color;
	PALETTEENTRY off_color;
	int on_time;
	int off_time;	
	int counter;
	int state;

}BLINKER,*BLINKER_PTR;


class HCDXBuilder:public HCWindowBuilder
{
protected:
	UCHAR *primary_buffer;//primary video buffer	
	int   primary_lpitch;//memory line pitch	
	UCHAR *back_buffer;
	int   back_lpitch;//memory line pitch
	
public :
	//construct
	HCDXBuilder(){
		primary_lpitch = -1;
		back_lpitch = -1;
		min_clip_x = 0;
		max_clip_x = 0;
		min_clip_y = 0;
		max_clip_y = 0;
		this->dd_pixel_format = -1;
	}
	//descontruct
	~HCDXBuilder(){

	}
	//getter setter
	UCHAR *Get_Primary_Buffer();
	UCHAR *Get_Back_Buffer();
	int Get_Back_lPitch();
	int Get_Primary_lPitch();
	int Set_Clip_Rect(int min_x,int min_y,int max_x,int max_y);
	//functions
	/*
		初始化DirectDraw
	*/
	int DDraw_Init(int widht,int height,int bpp,int windowed=0);
	
	/*
		销毁DirectDraw
	*/
	int DDraw_Shutdown(void);
	
	/*
		给页面附加裁剪器
	*/
	LPDIRECTDRAWCLIPPER DDraw_Attach_Clipper(
		LPDIRECTDRAWSURFACE7 lpdds,int num_rects,LPRECT clip_list);
	
	/*
		创建页面
		DDraw_Create_Surface() is used to create a generic offscreen DirectDraw surface in system memory,
		VRAM, or AGP memory. The default is DDSCAPS_OFFSCREENPLAIN. 
		Any additional control flags are logically ORed with the default. They're the standard DirectDraw DDSCAP* flags, 
		such as DDSCAPS_SYSTEMMEMORY and DDSCAPS_VIDEOMEMORY for system memory and VRAM,
		respectively. Also, you may select a color key value, it currently defaults to 0. 
		If the function is successful, it returns a pointer to the new surface. Otherwise, it returns NULL.
	*/
	LPDIRECTDRAWSURFACE7 DDraw_Create_Surface(int width,int height,
		int mem_flags=0,USHORT color_key_value=0);
	/*
		交换页面
		 DDraw_Flip() simply flips the primary surface with the secondary surface for full screen modes, 
		 or in windowed mode copies the offscreen back buffer to the client area of the windowed display. 
		 The call waits until the flip can take place,so it may not return immediately. Returns TRUE if successful.
	*/

	int DDraw_Flip(void);
	
	/*
		
		DDraw_Wait_For_Vsync() waits until the next vertical blank 
		period begins (when the raster hits the bottom of the screen).
		Returns TRUE if successful and FALSE if something really bad happened.
	*/
	int DDraw_Wait_For_Vsync();
	
	/*
		DDraw_Fill_Surface() is used to fill a surface or 
		rectangle within a surface with a color. The color
		must be in the color depth format of the surface, 
		such as a single byte in 256-color mode or a RGB 
		descriptor in high-color modes, if you want to
		fill the entire surface send set client to NULL 
		which is its default, otherwise, you may send a
		RECT pointer as client and fill a region only of 
		the surface. Returns TRUE if successful.

	*/
	int DDraw_Fill_Surface(LPDIRECTDRAWSURFACE7 lpdds,USHORT color,RECT *client=NULL);
	
	/*
		DDraw_Lock_Surface() locks the sent surface (if possible) and returns a UCHAR pointer to the surface, along with updating the sent lpitch variable with the linear memory pitch of the surface. While the surface is locked, you can manipulate it and write pixels to it, but the blitter will be blocked, so remember to unlock the surface ASAP. In addition, after unlocking the surface, the memory pointer and pitch most likely become invalid and should not be used. DDraw_Lock_Surface() returns the non-NULL address of the surface memory if successful and NULL otherwise. Also, remember if you are in 16-bit mode then there are 2 bytes per pixel, but the pointer returned is still a UCHAR * and 
		value written in lpitch is in bytes not pixels, 
		so beware!

	*/
	UCHAR *DDraw_Lock_Surface(LPDIRECTDRAWSURFACE7 lpdds,int *lpitch);
	
	/*
		DDraw_Unlock_Surface() unlocks a surface previously locked with DDraw_Lock_Surface(). 
		You need only send the pointer to the surface. Returns TRUE if successful.

	*/
	int DDraw_Unlock_Surface(LPDIRECTDRAWSURFACE7 lpdds);
	
	int Set_Palette_Entry(int color_index, LPPALETTEENTRY color);
	int Get_Palette_Entry(int color_index, LPPALETTEENTRY color);
	int Load_Palette_From_File(char *filename, LPPALETTEENTRY palette);
	int Save_Palette_To_File(char *filename, LPPALETTEENTRY palette);
	int Save_Palette(LPPALETTEENTRY sav_palette);
	int Set_Palette(LPPALETTEENTRY set_palette);
	// palette functions
	int Blink_Colors(int command, BLINKER_PTR new_light, int id);
	int Rotate_Colors(int start_index, int end_index);

	void HLine(int x1,int x2,int y,int color, UCHAR *vbuffer, int lpitch);
	void VLine(int y1,int y2,int x,int color, UCHAR *vbuffer, int lpitch);
	void HLine16(int x1,int x2,int y,int color, UCHAR *vbuffer, int lpitch);
	void VLine16(int y1,int y2,int x,int color, UCHAR *vbuffer, int lpitch);
	
	
	void Screen_Transitions(int effect, UCHAR *vbuffer, int lpitch);
	
	/*

	*/
	UCHAR *DDraw_Lock_Primary_Surface(void);
	int DDraw_Unlock_Primary_Surface(void);
	UCHAR *DDraw_Lock_Back_Surface(void);
	int DDraw_Unlock_Back_Surface(void);

	LPDIRECTDRAW7        lpdd7;                 // dd object
	LPDIRECTDRAWSURFACE7 lpddsprimary;         // dd primary surface
	LPDIRECTDRAWSURFACE7 lpddsback;            // dd back surface
	LPDIRECTDRAWPALETTE  lpddpal;              // a pointer to the created dd palette
	LPDIRECTDRAWCLIPPER  lpddclipper;          // dd clipper for back surface
	LPDIRECTDRAWCLIPPER  lpddclipperwin;       // dd clipper for window
	PALETTEENTRY         palette[MAX_COLORS_PALETTE];         // color palette
	PALETTEENTRY         save_palette[MAX_COLORS_PALETTE];    // used to save palettes
	DDSURFACEDESC2       ddsd;                 // a direct draw surface description struct
	DDBLTFX              ddbltfx;              // used to fill
	DDSCAPS2             ddscaps;              // a direct draw surface capabilities struct
	HRESULT              ddrval;               // result back from dd calls
	USHORT (*RGB16Bit)(int , int , int );
	int dd_pixel_format;
	int min_clip_x,max_clip_x,min_clip_y,max_clip_y;

};

#endif