#ifndef H_VECTOR_INCLUDED
#define H_VECTOR_INCLUDED
#include<iostream>
#include<assert.h>
#include "MathDef.h"
#include "HCLib\HCMath.h"

int Fast_Distance_2D(int x, int y);
float Fast_Distance_3D(float fx, float fy, float fz);

#define X 0
#define Y 1
#define Z 2
#define W 3
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
	ֵ���ݣ�ͨ��=���������й���
	*/
	CVector(CVector& vec)
	{
		*this = vec;
	}
	
	/*
	ͨ������float����,ȷ��float���鳤����dimension�ĳ���
	*/	
	CVector(float* pVec)
	{
		memcpy(m_vector,pVec,dimension*sizeof(float));
	}
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
	inline void Zero()
	{
		memset(m_vector,0,sizeof(m_vector));
	}
	/*
	[]����
	�����鷽ʽ����
	*/
	inline float operator[] (int index)
	{
		return m_vector[index];
	}
	
	/*
	��ֵ���� = ,�������
	
	*/
	inline CVector& operator = (float* vec)
	{
		
		memcpy(m_vector,vec,dimension * sizeof(float));
		
		return *this;
		
	}
	/*
	��ֵ���� = ��CVector���
	
	*/
	inline CVector& operator = (CVector& vec)
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
	inline CVector& operator *= (const float k)
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
	inline CVector& operator += (const CVector& vec)
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
	inline CVector& operator -= (const CVector& vec)
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
	inline bool operator == (CVector& vec)
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

		switch(dimension)
		{
		case 2:
			return (float)Fast_Distance_2D(m_vector[0],m_vector[1]);
		case 3:
			return (float)Fast_Distance_3D(m_vector[0],m_vector[1],m_vector[2]);
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
	float Dot(CVector& vec)
	{
		float result;
		for(int i=0;i<dimension;i++)
		{
			m_vector[i] *= vec.m_vector[i];
		}
		return result;
	}
	/*
	�������������������ļн�theta��cos(theta)
	��ʽcos(theta) = (u * v )/( |u|*|v|)
	returns the cosine of the angle between two vectors.
	*/
	float CosTh(CVector& vec)
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
	���ţ������� *= ������
	�ڶ���������Ž��
	*/
	int Scale(float k,CVector& result)
	{
		for(int i=0;i<dimension;i++)
		{
			result.m_vector[i] = m_vector[i] * k;
		}		
		return 1;
	}
	
	
	float m_vector[dimension];
	
				
};//end of class CVector


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
template<int dimension>
CVector<dimension> operator +(CVector<dimension> & left,CVector<dimension> &  right)
{	
	CVector<dimension> result;
	
	for (int i=0; i<dimension; i++)
		result.SetByIndex(i,left.m_vector[i] + right.m_vector[i]);
	
	return result;
}
template<int dimension>
CVector<dimension> operator -(CVector<dimension>  & left,CVector<dimension> &  right)
{
	
	CVector<dimension> result;
	
	for (int i=0; i<dimension; i++)
		result.SetByIndex(i,left.m_vector[i] - right.m_vector[i]);
	
	return result;
}
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


//2D,3D,4D�㣬��������
typedef CVector<2>  CVector2D;
typedef CVector<3>	CVector3D;
typedef CVector<4>  CVector4D;	
typedef CVector<2> CPoint2D;
typedef CVector<3> CPoint3D;
typedef CVector<4> CPoint4D;

class CQuat :public CVector<4>
{
public:
	float GetP0()
	{
		return m_vector[0];
	}
	int GetQV(CVector3D& qv)
	{
		qv = m_vector;
		return 1;	
	}
};


#endif