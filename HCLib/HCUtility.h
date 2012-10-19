#ifndef H_HCUTILITY_INCLUDED
#define H_HCUTILITY_INCLUDED

#include <stdio.h>
#include <windows.h>
#include "HCMacros.h"


int Color_Scan16(int x1,int y1,int x2,int y2,USHORT scan_start,USHORT scan_end,
				 UCHAR *scan_buffer,int scan_lpitch,int screen_width,int screen_height);
int Color_Scan(int x1,int y1,int x2,int y2,UCHAR scan_start,UCHAR scan_end
			   ,UCHAR *scan_buffer,int scan_lpitch,int screen_width,int screen_height);


// memory manipulation functions
void Mem_Set_WORD(void *dest, USHORT data, int count);
void Mem_Set_QUAD(void *dest, UINT   data, int count);



// root functions
USHORT RGB16Bit565(int r, int g, int b);
USHORT RGB16Bit555(int r, int g, int b);
// function ptr to RGB16 builder

static unsigned long start_clock_count;
DWORD Get_Clock(void);
DWORD Start_Clock(void);
DWORD Wait_Clock(DWORD count);


#endif