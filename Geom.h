#pragma once

#include "Vector.h"

//2D,3D������ֱ��
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

//2D,3Dƽ��
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

//2D������ϵ
class CPolar2D
{
public:
	float r;//�뾶
	float theta;//�Ƕ�
};
//3D��������ϵ
class CCylindrical3D
{
public:
	float r;
	float theta;
	float z;	
};
//3D��������ϵ
class CSpherical3D
{
public:
	float p;//��ԭ��ľ���
	float theta;//�߶� ԭ��o->p �� ��Z��֮��ļн�
	float phi;//�߶� ԭ��o->p ��x-y ƽ���ϵ�ͶӰ �� ��x��֮��ļн�
};

