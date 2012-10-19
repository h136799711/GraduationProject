#ifndef H_HCMACROS_INCLUDED
#define H_HCMACROS_INCLUDED

#define SCREEN_WIDTH	640//ÆÁÄ»´ó
#define SCREEN_HEIGHT	480
#define SCREEN_BPP		8

#define SCREEN_FULLSCREEN	0
#define SCREEN_WINDOWED		1	

#define MAX_COLORS_PALETTE	256

#define KEY_DOWN(vk_code)	((GetAsyncKeyState(vk_code) & 0x8000) ? 1:0)
#define KEY_UP(vk_code)	((GetAsyncKeyState(vk_code) & 0x8000) ? 0:1)

// this builds a 16 bit color value in 5.5.5 format (1-bit alpha mode)
#define _RGB16BIT555(r,g,b) ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10))

// this builds a 16 bit color value in 5.6.5 format (green dominate mode)
#define _RGB16BIT565(r,g,b) ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11))

// this builds a 24 bit color value in 8.8.8 format 
#define _RGB24BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) )

// this builds a 32 bit color value in A.8.8.8 format (8-bit alpha mode)
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

// bit manipulation macros
#define SET_BIT(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define RESET_BIT(word,bit_flag) ((word)=((word) & (~bit_flag)))

// initializes a direct draw struct, basically zeros it and sets the dwSize field
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

// used to compute the min and max of two expresions
#define MIN(a, b)  (((a) < (b)) ? (a) : (b)) 
#define MAX(a, b)  (((a) > (b)) ? (b) : (a)) 

// used for swapping algorithm
#define SWAP(a,b,t) {t=a; a=b; b=t;}

// some math macros
#define DEG_TO_RAD(ang) ((ang)*PI/180.0)
#define RAD_TO_DEG(rads) ((rads)*180.0/PI)

#define RAND_RANGE(x,y) ( (x) + (rand()%((y)-(x)+1)))
#define ISNULL(value)		(!value || value == NULL)

// basic unsigned types
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;
typedef unsigned int   QUAD;
typedef unsigned int   UINT;
typedef unsigned long DWORD;

#define DD_PIXEL_FORMAT8		8
#define DD_PIXEL_FORMAT555		15
#define DD_PIXEL_FORMAT565		16
#define DD_PIXEL_FORMAT888		24
#define DD_PIXEL_FORMATALPHA888	32

// fixed point mathematics constants
#define FIXP16_SHIFT     16
#define FIXP16_MAG       65536
#define FIXP16_DP_MASK   0x0000ffff
#define FIXP16_WP_MASK   0xffff0000
#define FIXP16_ROUND_UP  0x00008000

#endif