#ifndef H_VECTOR_INCLUDED
#define H_VECTOR_INCLUDED
#include<iostream>
namespace HBD
{
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
	CVector();
	/*
	值传递，通过=操作符进行构造
	*/
	CVector(CVector& vec);
	/*
	通过传入float数组,确保float数组长度有dimension的长度
	*/	
	CVector(float* pVec);
	/*
	得到X分量值
	*/
	float GetX();
	/*
	得到Y分量值
	*/
	float GetY();
	/*
	得到Z分量值
	*/
	float GetZ();
	/*
	得到W分量值
	*/
	float GetW();
	/*
	设置X分量值
	0 失败
	1 成功
	*/
	int SetX(float x);
	/*
	设置Y分量值
	0 失败
	1 成功
	*/
	int SetY(float y);
	/*
	设置Z分量值
	0 失败
	1 成功
	*/
	int SetZ(float z);
	/*
	设置W分量值
	0 失败
	1 成功
	*/
	int SetW(float w);
	/*
	根据下标来设置Vector的值
	0 失败
	1 成功
	*/
	int SetByIndex(int index=0,float value=0);
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
	void Zero();
	
	
	
protected:
	float m_vector[dimension];
	
	};
		
	template<int dimension>
		CVector<dimension>::CVector()
	{
		Zero();
	}
	
	template<int dimension>
		CVector<dimension>::CVector(CVector& vec)
	{
		*this = vec;
	}

	template<int dimension>
		CVector<dimension>::CVector(float* pVec)
	{
		memcpy(m_vector,pVec,dimension*sizeof(float));
	}

	template<int dimension>
		float CVector<dimension>::GetX()
	{
		if(dimension > X) 
			return m_vector[X];
		return 0;
	}

	template<int dimension>
		float CVector<dimension>::GetY()
	{
		if(dimension > Y)
			return m_vector[Y];
		return 0;
	}

	template<int dimension>
		float CVector<dimension>::GetZ()
	{
		if(dimension > Z)
			return m_vector[Z];
		return 0;
	}
	
	template<int dimension>
		float CVector< dimension>::GetW()
	{
		if(dimension > W)
			return m_vector[W];
		return 0;
	}
	template<int  dimension>
		int CVector< dimension>::SetX(float x)
	{
		if(X < dimension)
		{
			m_vector[X] = x;
			return 1;
		}
		return 0;
	}
	template<int dimension>
		int CVector< dimension>::SetY(float y)
	{
		if(Y < dimension)
		{
			m_vector[Y] = y;
			return 1;
		}
		return 0;
	}

	template<int dimension>
		int CVector< dimension>::SetZ(float z)
	{
		if(Z < dimension)
		{
			m_vector[Z] = z;
			return 1;
		}
		return 0;
	}
	template<int dimension>
		int CVector< dimension>::SetW(float w)
	{
		if(W < dimension)
		{
			m_vector[W] = w;
			return 1;
		}

		return 0;
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
	
	template<int dimension>
	void CVector<dimension>::Zero()
	{
		for(int i=0;i<dimension;i++)
		{
			SetByIndex(i,0);
		}
	}

	template<int dimension>
	int CVector<dimension>::SetByIndex(int index,float value)
	{
		if(index >= dimension) return 0;

		m_vector[index] = value;
		return 1;
	}
}// end of namespace HBD

#endif