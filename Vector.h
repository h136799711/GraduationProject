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
默认构造
	*/
	CVector()
	{
		Zero();
	}
	/*
	值传递，通过=操作符进行构造
	*/
	CVector(CVector& vec)
	{
		*this = vec;
	}
	
	/*
	通过传入float数组,确保float数组长度有dimension的长度
	*/	
	CVector(float* pVec)
	{
		memcpy(m_vector,pVec,dimension*sizeof(float));
	}
	/*
	得到X分量值
	*/
	inline float GetX()
	{
		if(dimension > X) 
			return m_vector[X];
		return 0;
	}
	/*
	得到Y分量值
	*/
	inline float GetY()
	{		
		if(dimension > Y) 
			return m_vector[Y];
		return 0;
	}
	/*
	得到Z分量值
	*/
	inline float GetZ()
	{
		if(dimension > Z) 
			return m_vector[Z];
		return 0;
	}
	/*
	得到W分量值
	*/
	inline float GetW()
	{
		if(dimension > W) 
			return m_vector[W];
		return 0;
	}
	/*
	设置X分量值
	0 失败
	1 成功
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
	设置Y分量值
	0 失败
	1 成功
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
	设置Z分量值
	0 失败
	1 成功
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
	设置W分量值
	0 失败
	1 成功
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
	根据下标来设置Vector的值
	0 失败
	1 成功
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
	返回字符串描述
	入参，最好已分配64个char大小的内存以上。
	返回0，字符串描述获取失败
	返回1，字符串描述获取成功
	*/
	int GetString(char* desc);
	/*
	置为0
	*/
	inline void Zero()
	{
		memset(m_vector,0,sizeof(m_vector));
	}
	/*
	[]运算
	以数组方式访问
	*/
	inline float operator[] (int index)
	{
		return m_vector[index];
	}
	
	/*
	赋值运算 = ,数组入参
	
	*/
	inline CVector& operator = (float* vec)
	{
		
		memcpy(m_vector,vec,dimension * sizeof(float));
		
		return *this;
		
	}
	/*
	赋值运算 = ，CVector入参
	
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
	矩阵与浮点数 *= 
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
	+=运算
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
	-=运算
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
	== 运算 
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
	向量的长度,注意溢出问题，注意向量的坐标大小
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
	快速计算向量长度，误差较大
	根据公式
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
	向量归一化
	0:向量已经为0向量，接近0向量
	1:归一化成功
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
	向量点乘，返回点乘结果float数值
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
	计算此向量与入参向量的夹角theta的cos(theta)
	公式cos(theta) = (u * v )/( |u|*|v|)
	returns the cosine of the angle between two vectors.
	*/
	float CosTh(CVector& vec)
	{
		return (Dot(vec) / (Length()*vec.Length()));
	}
	/*
	4D齐次化坐标转换为3D坐标
	在4D坐标下调用有效，可成功
	返回值
	0:失败
	1:成功
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
	缩放，依赖于 *= 操作符
	*/
	int Scale(float k)
	{
		(*this) *= k;
		return 1;
	}
	
	/*
	缩放，依赖于 *= 操作符
	第二个参数存放结果
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


//2D,3D,4D点，向量定义
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