#pragma once

#include "Vector.h"

//2D,3D������ֱ��
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
		�߶����
	*/
	CVector<dimension>& GetP0()
	{
		return m_p0;
	}
	/*
		�߶��յ�
	*/
	CVector<dimension>& GetP1()
	{
		return m_p1;
	}
	/*
		�߶εķ�������
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
	//����p0��p1����v��������
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
class CCylinderical3D
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

