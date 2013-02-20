#pragma once

#include "Vector.h"

#define PARM_LINE_NO_INTERSECT          0
#define PARM_LINE_INTERSECT_IN_SEGMENT  1
#define PARM_LINE_INTERSECT_OUT_SEGMENT 2
#define PARM_LINE_INTERSECT_EVERYWHERE  3

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
	CParmline(CVector<dimension> p0,CVector<dimension> p1)
	{
		m_p0 = p0;
		m_p1 = p1;
	}
	/*
	����t������ֱ���ϵĵ㣬�����pt
	*/
	void Compute_Parm_Line(float t,CVector<dimension>& pt);
	int Intersect_Parm_Lines(CParmline<dimension> right,CVector<dimension>& pt);
	/*
	�߶����
	*/
	CVector<dimension> GetP0()
	{
		return m_p0;
	}
	/*
	�߶��յ�
	*/
	CVector<dimension> GetP1()
	{
		return m_p1;
	}
	/*
	�߶εķ�������
	v = p1 - p0 
	*/
	CVector<dimension> GetV()
	{
		CalculateV();
		return m_v;
	}
	int SetP0(CVector<dimension> p0)
	{
		m_p0 = p0;
		return 1;
	}
	int SetP1(CVector<dimension> p1)
	{
		m_p1 = p1;
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

void CParmline<2>::Compute_Parm_Line(float t,CPoint2D & pt)
{
	pt.SetX(GetP0().GetX() + GetV().GetX() * t);
	pt.SetY(GetP0().GetY() + GetV().GetY() * t);
}
void CParmline<3>::Compute_Parm_Line(float t,CPoint3D& pt)
{
	pt.SetX(GetP0().GetX() + GetV().GetX() * t);
	pt.SetY(GetP0().GetY() + GetV().GetY() * t);
	pt.SetZ(GetP0().GetZ() + GetV().GetZ() * t);
}
int CParmline<2>::Intersect_Parm_Lines(CParmline<2> right,CVector<2>& pt)
{
	//���
	float det_p1p2 = GetV().GetX() * right.GetV().GetY()
		- GetV().GetY() * right.GetV().GetX();
	if(fabs(det_p1p2) <= EPSILON_E5)
	{
		return PARM_LINE_NO_INTERSECT;//���ཻ
	}
	
	// ����2��ֱ���ཻ����������ͬ
	
	//ֱ��1 p1   = p10   + v1*t1 
	
	// p1.x = p10.x + v1.x*t1
	// p1.y = p10.y + v1.y*t1
	
	//ֱ��2 p2 = p20 + v2*t2
	
	// p2.x = p20.x + v2.x*t2
	// p2.y = p20.y + v2.y*t2
	
	// p2.x == p1.x �� p2.y == p1.y 2����ʽ
	// �������
	float t1,t2;
	
	t1 = (right.GetV().GetX() * (GetP0().GetY() - right.GetP0().GetY())
		- right.GetV().GetY() *(GetP0().GetX() - right.GetP0().GetX()))/det_p1p2;
	t2 = (GetV().GetX() * (GetP0().GetY() - right.GetP0().GetY())
		- GetV().GetY() *(GetP0().GetX() - right.GetP0().GetX()))/det_p1p2;
	
	pt.SetX(GetP0().GetX() + t1*GetV().GetX());
	pt.SetY(GetP0().GetY() + t1*GetV().GetY());
	
	if ((t1>=0) && (t1<=1) && (t2>=0) && (t2<=1))
		return(PARM_LINE_INTERSECT_IN_SEGMENT);
	else
		return(PARM_LINE_INTERSECT_OUT_SEGMENT);
}

//2D,3Dƽ��
template<int dimension=2>
class CPlane
{
	public:
		CPlane(){};
		~CPlane(){};
		CVector<dimension> m_p0;
		CVector<dimension> m_n;
		/*
			��֧��3άƽ��
		*/
		int Intersect_Parm_Line(CParmline<dimension> pline,float& t,CVector<dimension> pt);
		/*
			��֧��3άƽ��
		*/
		float Compute_Point_In(CVector<dimension> pt);
		/*
			֧��3ά
		*/
		void Init(CVector<dimension> p0,CVector<dimension>& normal,int normalize=0);
};
typedef CPlane<2> CPlane2D;
typedef CPlane<3> CPlane3D;
int CPlane<3>::Intersect_Parm_Line(CParmline3D pline,float &t,CPoint3D pt)
{
	float plane_dot_line = pline.GetV().Dot(m_n);
	
	if(fabs(plane_dot_line) <= EPSILON_E5)
	{
		if(fabs(Compute_Point_In(pline.GetP0())) <= EPSILON_E5)
			return PARM_LINE_INTERSECT_EVERYWHERE;
		else
		return PARM_LINE_NO_INTERSECT;

	}

	t = -(m_n.GetX()*pline.GetP0().GetX() + 
       m_n.GetY()*pline.GetP0().GetY() + 
       m_n.GetZ()*pline.GetP0().GetZ() -
       m_n.GetX()*m_p0.GetX() - 
       m_n.GetY()*m_p0.GetY() - 
       m_n.GetZ()*m_p0.GetZ()) / (plane_dot_line);

	pt.SetX(pline.GetP0().GetX() + pline.GetV().GetX()*t);
	pt.SetY(pline.GetP0().GetY() + pline.GetV().GetY()*t);
	pt.SetZ(pline.GetP0().GetZ() + pline.GetV().GetZ()*t);
	
	if(t >= 0.0 && t <= 1.0)
		return PARM_LINE_INTERSECT_IN_SEGMENT;
	else
		return PARM_LINE_INTERSECT_OUT_SEGMENT;
}
template<>
void CPlane<3>::Init(CPoint3D p0,CVector3D& normal,int normalize)
{
	m_p0 = p0;
	m_n = normal;
	if(normalize)
	{
		m_n.Normalize();
	}
	
}
template<>
float CPlane<3>::Compute_Point_In(CPoint3D pt)

{
	float hs = m_n.GetX()*(pt.GetX() - m_p0.GetX())
		+m_n.GetY()*(pt.GetY() - m_p0.GetY())
		+m_n.GetZ()*(pt.GetZ() - m_p0.GetZ());
	return hs;
}
//2D������ϵ
class CPolar2D
{
public:
	float m_r;//�뾶
	float m_theta;//�Ƕ�
	inline void Zero()
	{
		m_r = 0;
		m_theta = 0;
	}
	
};
//3D��������ϵ
class CCylinderical3D
{
public:
	float m_r;
	float m_theta;
	float m_z;	
};
//3D��������ϵ
class CSpherical3D
{
public:
	float m_p;//��ԭ��ľ���
	float m_theta;//�߶� ԭ��o->p �� ��Z��֮��ļн�
	float m_phi;//�߶� ԭ��o->p ��x-y ƽ���ϵ�ͶӰ �� ��x��֮��ļн�
};

