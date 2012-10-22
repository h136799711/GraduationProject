#ifndef H_HCMATH_INCLUDED
#define H_HCMATH_INCLUDED

/***************************************************************************************
**数学引擎支持2D,3D,4D点和向量。其中4D表示格式为(x,y,z,w)的齐次坐标。
**				
**
**
**
**
*****************************************************************************************/
#pragma warning (disable:4244)
#include <math.h>
#include "HCMacros.h"
//数学 与 碰撞
//PI 定义
#define PI		((float)3.141592654f)
#define PI2		((float)6.283185307f)
#define PI_DIV_2 ((float)1.570796327f)
#define PI_DIV_4 ((float)0.785398163f)
#define PI_INV	((float)0.318309886f)	// 1/PI
//
typedef struct MATRIX3X3_TYP
{
	union
	{
		float M[3][3];
		struct
		{
			float M00,M01,M02;
			float M10,M11,M12;
			float M20,M21,M22;
		};
	};
}MATRIX3X3,*MATRIX3X3_PTR;

typedef struct MATRIX1X3_TYP
{
	union
	{
		float M[3];
		struct
		{
			float M00,M01,M02;
		};
	};
}MATRIX1X3,*MATRIX1X3_PTR;

typedef struct MATRIX3X2_TYP
{
	union
	{
		float M[3][2];
		struct
		{
			float M00,M01;
			float M10,M11;
			float M20,M21;
		};
	};
}MATRIX3X2,*MATRIX3X2_PTR;

typedef struct MATRIX1X2_TYP
{
	union 
	{
		float M[2];
		struct 
		{
			float M00,M01;
		};
	};
}MATRIX1X2,*MATRIX1X2_PTR;


class HCMath
{
public:
	float cos_look[361];
	float sin_look[361];
	void Build_Sin_Cos_Tables();
	int Mat_Mul_1X2_3X2(MATRIX1X2_PTR ma, 
					   MATRIX3X2_PTR mb,
					   MATRIX1X2_PTR mprod);

	int Mat_Mul_1X3_3X3(MATRIX1X3_PTR ma, 
					   MATRIX3X3_PTR mb,
					   MATRIX1X3_PTR mprod);

	int Mat_Mul_3X3(MATRIX3X3_PTR ma, 
				   MATRIX3X3_PTR mb,
				   MATRIX3X3_PTR mprod);

	int Mat_Init_3X2(MATRIX3X2_PTR ma, 
							float m00, float m01,
							float m10, float m11,
							float m20, float m21);
	// math functions
int Fast_Distance_2D(int x, int y);
float Fast_Distance_3D(float x, float y, float z);
};

float HCMath::Fast_Distance_3D(float fx, float fy, float fz)
{
	// this function computes the distance from the origin to x,y,z
	
	int temp;  // used for swaping
	int x,y,z; // used for algorithm
	
	// make sure values are all positive
	x = fabs(fx) * 1024;
	y = fabs(fy) * 1024;
	z = fabs(fz) * 1024;
	
	// sort values
	if (y < x) SWAP(x,y,temp)
		
		if (z < y) SWAP(y,z,temp)
			
			if (y < x) SWAP(x,y,temp)
				
				int dist = (z + 11*(y >> 5) + (x >> 2) );
			
			// compute distance with 8% error
			return((float)(dist >> 10));
			
} // end Fast_Distance_3D
int HCMath::Fast_Distance_2D(int x, int y)
{
	// this function computes the distance from 0,0 to x,y with 3.5% error
	
	// first compute the absolute value of x,y
	x = abs(x);
	y = abs(y);
	
	// compute the minimum of x,y
	int mn = MIN(x,y);
	
	// return the distance
	return(x+y-(mn>>1)-(mn>>2)+(mn>>4));
	
} // end Fast_Distance_2D



#endif