#pragma once

#include "Vector.h"

//2D,3D参数化直线
template<int dimension=2>
class CParmline
{
public:
	CParmline(){};
	~CParmline(){};
	CVector<dimension> p0;
	CVector<dimension> p1;
	CVector<dimension> v;
};
typedef CParmline<2> CParmline2D;
typedef CParmline<3> CParmline3D;

//2D,3D平面
template<int dimension=2>
class CPlane
{
	public:
		CPlane(){};
		~CPlane(){};
		CVector<dimension> p0;
		CVector<dimension> n;
};
typedef CPlane<2> CPlane2D;
typedef CPlane<3> CPlane3D;

//2D极坐标系
class CPolar2D
{
public:
	float r;//半径
	float theta;//角度
};
//3D柱面坐标系
class CCylindrical3D
{
public:
	float r;
	float theta;
	float z;	
};
//3D球面坐标系
class CSpherical3D
{
public:
	float p;//到原点的距离
	float theta;//线段 原点o->p 和 正Z轴之间的夹角
	float phi;//线段 原点o->p 在x-y 平面上的投影 和 正x轴之间的夹角
};

