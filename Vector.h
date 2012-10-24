#ifndef H_VECTOR_INCLUDED
#define H_VECTOR_INCLUDED

#include<iostream>
#include<assert.h>
#include "MathDef.h"
#include "HCLib\HCMath.h"


//int Fast_Distance_2D(int x, int y);
//float Fast_Distance_3D(float x, float y, float z);
#define X 0
#define Y 1
#define Z 2
#define W 3
//4ά�µ�Zero����W����Ϊ1����0
template<int dimension=2>class CVector
{
public:
	
	/*	
		Ĭ�Ϲ���
	*/
	CVector()
	{
		Zero();
	}	
	/*
	ͨ������float����,ȷ��float���鳤����dimension�ĳ���
	*/	
	CVector(float* pVec)
	{
		memcpy(m_vector,pVec,sizeof(m_vector));
	}
	
	inline int SetXYZW(float x,float y,float z=0,float w=1);
	/*
	�õ�X����ֵ
	*/
	inline float GetX()
	{
		if(dimension > X) 
			return m_vector[X];
		return 0;
	}
	/*
	�õ�Y����ֵ
	*/
	inline float GetY()
	{		
		if(dimension > Y) 
			return m_vector[Y];
		return 0;
	}
	/*
	�õ�Z����ֵ
	*/
	inline float GetZ()
	{
		if(dimension > Z) 
			return m_vector[Z];
		return 0;
	}
	/*
	�õ�W����ֵ
	*/
	inline float GetW()
	{
		if(dimension > W) 
			return m_vector[W];
		return 0;
	}
	/*
	����X����ֵ
	0 ʧ��
	1 �ɹ�
	*/
	inline int SetX(float x)
	{
		
		if(X < dimension)
		{
			m_vector[X] = x;
			return 1;
		}
		return 0;
	}
	/*
	����Y����ֵ
	0 ʧ��
	1 �ɹ�
	*/
	inline int SetY(float y)
	{
		
		if(Y < dimension)
		{
			m_vector[Y] = y;
			return 1;
		}
		return 0;
	}
	/*
	����Z����ֵ
	0 ʧ��
	1 �ɹ�
	*/
	inline int SetZ(float z)
	{
		
		if(Z < dimension)
		{
			m_vector[Z] = z;
			return 1;
		}
		return 0;
	}
	/*
	����W����ֵ
	0 ʧ��
	1 �ɹ�
	*/
	inline int SetW(float w)
	{
		
		if(W < dimension)
		{
			m_vector[W] = w;
			return 1;
		}
		return 0;
	}
	/*
	�����±�������Vector��ֵ
	0 ʧ��
	1 �ɹ�
	*/
	inline int SetByIndex(int index=0,float value=0)
	{
		if(index < dimension)
		{
			m_vector[index] = value;
			return 1;
		}
		return 0;
	}
	/*
	�����ַ�������
	��Σ�����ѷ���64��char��С���ڴ����ϡ�
	����0���ַ���������ȡʧ��
	����1���ַ���������ȡ�ɹ�
	*/
	int GetString(char* desc);
	/*
	��Ϊ0
	*/
	inline void Zero();
	/*
	[]����
	�����鷽ʽ����
	*/
	inline float& operator[] (int index)
	{
		return m_vector[index];
	}
	
	/*
	��ֵ���� = ,�������
	
	*/
	
	inline CVector<dimension> operator = (float* vec)
	{		
		memcpy(m_vector,vec,dimension * sizeof(float));		
		return *this;
	}
	/*
	��ֵ���� = ��CVector���
	
	*/
	inline CVector<dimension> operator = (CVector<dimension> vec)
	{
		
		for(int i=0;i<dimension;i++)
		{
			m_vector[i] = vec.m_vector[i];
		}
		
		return *this;
		
	}
	
	
	/*
	�����븡���� *= 
	*/
	
	inline CVector<dimension>& operator *= (const float k)
		
	{		
		for(int i=0;i<dimension;i++)
		{
			m_vector[i] = k*m_vector[i];
		}
		return *this;
	}
	
	/*
	+=����
	*/
	inline CVector<dimension>& operator += (const CVector<dimension>& vec)
		
	{
		for(int i=0;i<dimension;i++)
		{
			m_vector[i] += vec.m_vector[i];
		}
		return *this;
	}
	/*
	-=����
	*/
	
	inline CVector<dimension>& operator -= (const CVector<dimension>& vec)
	{
		for(int i=0;i<dimension;i++)
		{
			m_vector[i] -= vec.m_vector[i];
		}
		return *this;
	}
	/*
	== ���� 
	*/
	
	
	inline bool operator == (CVector<dimension>& vec)
	{
		if(this == &vec )
			return true;
		for(int i=0;i<dimension;i++)
		{
			if(vec.m_vector[i] != m_vector[i])
				return false;
		}
		return true;
	}
	/*
	�����ĳ���,ע��������⣬ע�������������С
	*/
	inline float Length()
	{
		float result=0;
		for(int i=0;i<dimension;i++)
		{
			result += m_vector[i] * m_vector[i];
		}
		return sqrtf(result);
	}
	/*
	���ټ����������ȣ����ϴ�
	���ݹ�ʽ
	*/
	
	float Fast_Length()
	{
		HCMath hcmath;
		switch(dimension)
		{
		case 2:
			return (float)hcmath.Fast_Distance_2D(m_vector[0],m_vector[1]);
		case 3:
			return (float)hcmath.Fast_Distance_3D(m_vector[0],m_vector[1],m_vector[2]);
		default:
			break;
		}
		return 0.0f;
	}
	/*
	������һ��
	0:�����Ѿ�Ϊ0�������ӽ�0����
	1:��һ���ɹ�
	*/
	int Normalize()
	{
		float length = Length();
		
		if(length < EPSILON_E5)
			return 0;
		float length_inv = 1/length;
		for(int i=0;i<dimension;i++)
			m_vector[i] = m_vector[i] * length_inv;
		
		return 1;
	}
	/*
	������ˣ����ص�˽��float��ֵ
	*/
	float Dot(CVector<dimension>& vec)	
	{
		float result=0;
		for(int i=0;i<dimension;i++)
		{
			result = result + m_vector[i] * vec.m_vector[i];
		}
		return result;
	}
	/*
	�������������������ļн�theta��cos(theta)
	��ʽcos(theta) = (u * v )/( |u|*|v|)
	returns the cosine of the angle between two vectors.
	*/
	float CosTh(CVector<dimension>& vec)
		
	{
		return (Dot(vec) / (Length()*vec.Length()));
	}
	/*
	4D��λ�����ת��Ϊ3D����
	��4D�����µ�����Ч���ɳɹ�
	����ֵ
	0:ʧ��
	1:�ɹ�
	*/
	int DivByW()
	{
		if(dimension != 4) return 0;
		float w_inv = 1/ m_vector[W];
		
		m_vector[X] *= w_inv;
		m_vector[Y] *= w_inv;
		m_vector[Z] *= w_inv;
		
		
		return 1;
	}
	/*
	���ţ������� *= ������
	*/
	int Scale(float k)
	{
		(*this) *= k;
		return 1;
	}
	
	/*
	�ڶ���������Ž��
	*/
	int Scale(float k,CVector<dimension>& result)
	{
		for(int i=0;i<dimension;i++)
		{
			result.m_vector[i] = m_vector[i] * k;
		}		
		return 1;
	}
	/*
		�����Ĳ��,
		���Ϊ��ֱ�ڲ����2������
	*/
	int Cross(CVector<dimension> right,CVector<dimension>& result)
	{
		result.SetX((GetY()*right.GetZ() - GetZ()*right.GetY()));
		result.SetY((GetX()*right.GetZ() - GetZ()*right.GetX()));
		result.SetZ((GetX()*right.GetY() - GetY()*right.GetX()));

		return 1;	
	}
	CVector<dimension> operator +(CVector<dimension> &  right)
	{	
		CVector<dimension> result;
		
		for (int i=0; i<dimension; i++)
			result.SetByIndex(i,m_vector[i] + right.m_vector[i]);
		
		return result;
	}
	
	CVector<dimension> operator -(CVector<dimension> &  right)
	{
		
		CVector<dimension> result;
		
		for (int i=0; i<dimension; i++)
			result.SetByIndex(i,m_vector[i] - right.m_vector[i]);
		
		return result;
	}
	float operator *(CVector<dimension> right)
	{
		float result = 0;
		for(int i=0;i<dimension;i++)
		{
			result += (m_vector[i] * right.m_vector[i]);
		}
		return result;
	}
	CVector<dimension> operator * (float k)
	{
		CVector<dimension> tmp;
		for(int i=0;i<dimension;i++)
		{
			tmp.m_vector[i] = m_vector[i] * k;
		}
		return tmp;
	}


	float m_vector[dimension];
	
				
};//end of class CVector

template<>
inline int CVector<3>::SetXYZW(float x,float y,float z,float w)
{
	m_vector[X] = x;
	m_vector[Y] = y;
	m_vector[Z] = z;
}
template<>
inline int CVector<4>::SetXYZW(float x,float y,float z,float w)
{
	m_vector[X] = x;
	m_vector[Y] = y;
	m_vector[Z] = z;
	m_vector[W] = w;
}


template<int dimension>
void CVector<dimension>::Zero()
{
	memset(m_vector,0,sizeof(m_vector));
}
template<>
void CVector<4>::Zero()
{
	m_vector[X] = 0.0f;
	m_vector[Y] = 0.0f;
	m_vector[Z] = 0.0f;
	m_vector[W] = 1.0f;
}
template<int dimension>
int CVector<dimension>::GetString(char* desc)
{
	if(!desc)
	{
		return 0;
	}
	switch(dimension)
	{
	case 2:
		{
			sprintf(desc," (%f,%f) ",GetX(),GetY());
			
		}break;
	case 3:
		{
			sprintf(desc," (%f,%f,%f) ",GetX(),GetY(),GetZ());
			
		}break;
	case 4:
		{	
			sprintf(desc," (%f,%f,%f,%f) ",GetX(),GetY(),GetZ(),GetW());
		}
		break;
	default:
		{		
			sprintf(desc," (ERR,ERR,ERR,ERR) ");
			return 0;
		}break;
	}
	
	return 1;
	
}


//global operator 
/*
float Fast_Distance_3D(float fx, float fy, float fz)
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
int Fast_Distance_2D(int x, int y)
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

  */
//2D,3D,4D�㣬��������
typedef CVector<2>  CVector2D;
typedef CVector<3>	CVector3D;
typedef CVector<4>  CVector4D;	
typedef CVector<2> CPoint2D;
typedef CVector<3> CPoint3D;
typedef CVector<4> CPoint4D;

/*******
	
	  ��Ԫ����������
	  ��3D�������Ƕ�֮��Ĺ�ϵ
	  �����¹�ʽ
	  ��Ԫ�� : Quat,q0Ϊquat��w����,qvΪquat��x��y��z3��������ϵ�3D����
	  theta : �Ƕ�(����)
	  v : 3D����
	  Quat = cos(theta/2) + sin(theta/2) * v;
	  cos(theta/2) = q0;
	  sin(theta/2) * v = qv;

*************/
class CQuat :public CVector4D
{
public:
	CQuat()
	{
		Zero();
	}
	CQuat(float q0,CVector3D qv)
	{	
		SetP0(q0);
	}
	float GetP0()
	{
		return GetW();
	}
	/*******
	*******
	*******
	*******
	*************/
	int SetP0(float q0)
	{
		SetW(q0);
		return 1;
	}
	int GetQv(CVector3D& qv)
	{
		qv.SetX(GetX());
		qv.SetY(GetY());
		qv.SetZ(GetZ());
		return 1;
	}
	int SetQv(CVector3D qv)
	{
		SetX(qv.GetX());
		SetY(qv.GetY());
		SetZ(qv.GetZ());
		return 1;
	}
	/*
		���� ��Ч�������ĳ���
	*/
	float Norm()
	{
		return Length();
	}
	/*
		������ƽ��
	*/
	float Norm2()
	{
		return (GetX()*GetX() + GetY()*GetY() + GetZ()*GetZ() + GetW()*GetW());
	}
	/*
		������Ϊ 1 �ĸ���
		��ΪUnit
		Unit�ĵ��������乲���
	*/
	void Unit_Inverse()
	{
		SetX(-GetX());
		SetY(-GetY());
		SetZ(-GetZ());
	}
	/*
		����
	*/
	void Inverse()
	{
		// 1/q = *q / |q|2
		float norm2_inv = 1.0f / Norm2();

		SetW(GetW() * norm2_inv);
		SetX(-GetX() * norm2_inv);
		SetY(-GetY() * norm2_inv);
		SetZ(-GetZ() * norm2_inv);
	}
	/*
		�����ĳ˷�
	*/
	void Mul(CQuat right,CQuat& result)
	{
		
		float prd_0 = (GetZ() - GetY()) * (right.GetY() - right.GetZ());
		float prd_1 = (GetW() + GetX()) * (right.GetW() + right.GetX());
		float prd_2 = (GetW() - GetX()) * (right.GetY() + right.GetZ());
		float prd_3 = (GetY() + GetZ()) * (right.GetW() - right.GetX());
		float prd_4 = (GetZ() - GetX()) * (right.GetX() - right.GetY());
		float prd_5 = (GetZ() + GetX()) * (right.GetX() + right.GetY());
		float prd_6 = (GetW() + GetY()) * (right.GetW() - right.GetZ());
		float prd_7 = (GetW() - GetY()) * (right.GetW() + right.GetZ());

		float prd_8 = prd_5 + prd_6 + prd_7;
		float prd_9 = 0.5 * (prd_4 + prd_8);

		// and finally build up the result with the temporary products

		result.SetW(prd_0 + prd_9 - prd_5);
		result.SetX(prd_1 + prd_9 - prd_8);
		result.SetY(prd_2 + prd_9 - prd_7);
		result.SetZ(prd_3 + prd_9 - prd_6);
		
	}
	void Triple_Product(CQuat middle,CQuat right,CQuat& result)
	{
		Mul(middle,result);
		*this = result;
		Mul(right,result);
	}
	/*
		ʹ��һ��3D��������(��һ��)
		��һ����ת�Ƕ�(����)����ʼ����Ԫ��
	*/
	void Init_With_Vector3D_Theta(CVector3D v,float theta)
	{
		float theta_div_2 = (0.5)*theta;
		float sinf_theta = sinf(theta_div_2);

		SetX(sinf_theta * v.GetX());
		SetY(sinf_theta * v.GetY());
		SetZ(sinf_theta * v.GetZ());
		SetW(cosf(theta_div_2));

	}
	/*
		ʹ��һ��3D��������(��һ��)
		��һ����ת�Ƕ�(����)����ʼ����Ԫ��
	*/
	void Init_With_Vector3D_Theta(CVector4D v,float theta)
	{
		float theta_div_2 = (0.5)*theta;
		float sinf_theta = sinf(theta_div_2);

		SetX(sinf_theta * v.GetX());
		SetY(sinf_theta * v.GetY());
		SetZ(sinf_theta * v.GetZ());
		SetW(cosf(theta_div_2));

	}
	/*
		������x��y��z��ת�ĽǶȣ�����һ��zyx˳�������ת��Ӧ����Ԫ��
		��Ԫ��qfinal
		qxtheta: xtheta��x����ת�ĽǶ�(���)
		qytheta: ytheta��y����ת�ĽǶ�(ƫ����)
		qztheta: ztheta��z����ת�ĽǶ�(����)
		i,j,k��x��y��z���ϵķ������
		qxtheta = cos(xtheta / 2) + sin(xtheta/2)*i + 0 *j+0*k;
				
		qfinal= qxtheta * qytheta*qztheta
				= qztheta * qytheta*qytheta

	**/
	void Init_With_EulerZYX(float theta_z, float theta_y, float theta_x)
	{
				
		// precompute values
		float cos_z_2 = 0.5*cosf(theta_z);
		float cos_y_2 = 0.5*cosf(theta_y);
		float cos_x_2 = 0.5*cosf(theta_x);

		float sin_z_2 = 0.5*sinf(theta_z);
		float sin_y_2 = 0.5*sinf(theta_y);
		float sin_x_2 = 0.5*sinf(theta_x);

		// and now compute quaternion
		SetW(cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2);
		SetX(cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2);
		SetY(cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2);
		SetZ(sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2);
	}
	//��һ����λ��Ԫ��ת��Ϊһ����λ����������һ���Ƹ�������ת�ĽǶ�
	int Get_Vecotr3D_Theta(CVector3D& v,float& theta)
	{
		theta = acosf(GetW());
		float sin_theta_inv = 1.0 / sinf(theta);

		SetX(v.GetX() * sin_theta_inv);
		SetY(v.GetY() * sin_theta_inv);
		SetZ(v.GetZ() * sin_theta_inv);
		
		theta *= 2;
		return 1;
	}

};


#endif