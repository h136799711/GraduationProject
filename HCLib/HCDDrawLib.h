#ifndef H_HCDDRAWLIB_INCLUDED
#define H_HCDDRAWLIB_INCLUDED
#pragma once
#pragma comment (lib,"ddraw.lib")
#pragma warning (disable:4244)
#include <ddraw.h>
#include "HCMacros.h"
#include "HCUtility.h"
#include "HCMath.h"


typedef struct VERTEX2DF_TYP
{
	int x,y;

}VERTEX2DF,*VERTEX2DF_PTR;
typedef struct POLYGON2D_TYP
{
	int state;
	int num_verts;
	int x0,y0;
	int xv,yv;
	DWORD color;
	VERTEX2DF *vlist;
}POLYGON2D,*POLYGON2D_PTR;

class HCDDrawLib
{

public:
	HCDDrawLib()
	{
		
		min_clip_x = -1000;
		max_clip_x = 1000;
		min_clip_y = -1000;
		max_clip_y = 1000;
	}
	void SetClipRect(int minx,int miny,int maxx,int maxy)
	{		
		min_clip_x = minx;
		max_clip_x = maxx;
		min_clip_y = miny;
		max_clip_y = maxy;
	}
	// graphics functions
	int Draw_Pixel(int x, int y,int color,UCHAR *video_buffer, int lpitch);
	int Draw_Pixel16(int x, int y,int color,UCHAR *video_buffer, int lpitch);
	int Draw_Clip_Line(int x0,int y0, int x1, int y1, int color,UCHAR *dest_buffer, int lpitch);
	int Draw_Clip_Line16(int x0,int y0, int x1, int y1, int color,UCHAR *dest_buffer, int lpitch);
	int Clip_Line(int &x1,int &y1,int &x2, int &y2);
	int Draw_Line(int xo, int yo, int x1,int y1, int color,UCHAR *vb_start,int lpitch);
	int Draw_Line16(int xo, int yo, int x1,int y1, int color,UCHAR *vb_start,int lpitch);
	int Draw_Rectangle(int x1, int y1, int x2, int y2, int color,LPDIRECTDRAWSURFACE7 lpdds);

	


	// gdi functions
	int Draw_Text_GDI(char *text, int x,int y,COLORREF color, LPDIRECTDRAWSURFACE7 lpdds);
	int Draw_Text_GDI(char *text, int x,int y,int color, LPDIRECTDRAWSURFACE7 lpdds);

	// 2d 8-bit, 16-bit triangle rendering
	void Draw_Top_Tri(int x1,int y1,int x2,int y2, int x3,int y3,int color,UCHAR *dest_buffer, int mempitch);

	void Draw_Bottom_Tri(int x1,int y1, int x2,int y2, int x3,int y3,int color,UCHAR *dest_buffer, int mempitch);

	void Draw_Top_Tri16(int x1,int y1,int x2,int y2, int x3,int y3,int color,UCHAR *dest_buffer, int mempitch);

	void Draw_Bottom_Tri16(int x1,int y1, int x2,int y2, int x3,int y3,int color,UCHAR *dest_buffer, int mempitch);

	void Draw_Top_TriFP(int x1,int y1,int x2,int y2, int x3,int y3,int color,UCHAR *dest_buffer, int mempitch);

	void Draw_Bottom_TriFP(int x1,int y1, int x2,int y2, int x3,int y3,int color,UCHAR *dest_buffer, int mempitch);

	void Draw_Triangle_2D(int x1,int y1,int x2,int y2,int x3,int y3,
						  int color,UCHAR *dest_buffer, int mempitch);

	void Draw_Triangle_2D16(int x1,int y1,int x2,int y2,int x3,int y3,
							int color,UCHAR *dest_buffer, int mempitch);

	void Draw_TriangleFP_2D(int x1,int y1,int x2,int y2,int x3,int y3,
							int color,UCHAR *dest_buffer, int mempitch);

	inline void Draw_QuadFP_2D(int x0,int y0,int x1,int y1,
							   int x2,int y2,int x3, int y3,
							   int color,UCHAR *dest_buffer, int mempitch);


	// general 2D 8-bit, 16-bit polygon rendering and transforming functions
	void Draw_Filled_Polygon2D(POLYGON2D_PTR poly, UCHAR *vbuffer, int mempitch);
	void Draw_Filled_Polygon2D16(POLYGON2D_PTR poly, UCHAR *vbuffer, int mempitch);
	int Translate_Polygon2D(POLYGON2D_PTR poly, int dx, int dy);
	int Rotate_Polygon2D(POLYGON2D_PTR poly, int theta);
	int Scale_Polygon2D(POLYGON2D_PTR poly, float sx, float sy);
//	void Build_Sin_Cos_Tables(void);
	int Translate_Polygon2D_Mat(POLYGON2D_PTR poly, int dx, int dy);
	int Rotate_Polygon2D_Mat(POLYGON2D_PTR poly, int theta);
	int Scale_Polygon2D_Mat(POLYGON2D_PTR poly, float sx, float sy);
	int Draw_Polygon2D(POLYGON2D_PTR poly, UCHAR *vbuffer, int lpitch);
	int Draw_Polygon2D16(POLYGON2D_PTR poly, UCHAR *vbuffer, int lpitch);
	
	
	int min_clip_x,max_clip_x,min_clip_y,max_clip_y;
};

#endif