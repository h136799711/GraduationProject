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







#endif