#pragma once

#include "Vector.h"

//2D,3D参数化直线
template<int dimension=3>
class CParmline
{
public:
	CParmline()
	{
		m_p0.Zero();
		m_p1.Zero();
		m_v.Zero();
	}
	CParmline(CVector<dimension>& p0,CVector<dimension>& p1,CVector<dimension>& v)
	{
		m_p0 = p0;
		m_p1 = p1;
		m_v = v;
	}
	CParmline(CParmline<dimension>& parmline)
	{
		m_p0 = parmline.GetP0();
		m_p1 = parmline.GetP1();
		m_v = parmline.GetV();
	}


	/*
		线段起点
	*/
	CVector<dimension>& GetP0()
	{
		return m_p0;
	}
	/*
		线段终点
	*/
	CVector<dimension>& GetP1()
	{
		return m_p1;
	}
	/*
		线段的方向向量
		v = p1 - p0 
	*/
	CVector<dimension>& GetV()
	{
		return m_v;
	}
	int SetP0(CVector<dimension>& p0)
	{
		m_p0 = p0;
		return 1;
	}
	int SetP1(CVector<dimension>& p1)
	{
		m_p1 = p1;
		return 1;
	}
	int SetV(CVector<dimension>& v)
	{
		m_v = v;
		return 1;
	}
	//根据p0，p1计算v方向向量
	int CalculateV()
	{
		m_v[0] = m_p1[0] - m_p0[0];
		m_v[1] = m_p1[1] - m_p0[1];
		if(dimension == 3)
			m_v[2] = m_p1[2] - m_p0[2];
		return 1;
	}
protected:
	CVector<dimension> m_p0;
	CVector<dimension> m_p1;
	CVector<dimension> m_v;
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
class CCylinderical3D
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

