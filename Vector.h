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
Ĭ�Ϲ���
	*/
	CVector();
	/*
	ֵ���ݣ�ͨ��=���������й���
	*/
	CVector(CVector& vec);
	/*
	ͨ������float����,ȷ��float���鳤����dimension�ĳ���
	*/	
	CVector(float* pVec);
	/*
	�õ�X����ֵ
	*/
	float GetX();
	/*
	�õ�Y����ֵ
	*/
	float GetY();
	/*
	�õ�Z����ֵ
	*/
	float GetZ();
	/*
	�õ�W����ֵ
	*/
	float GetW();
	/*
	����X����ֵ
	0 ʧ��
	1 �ɹ�
	*/
	int SetX(float x);
	/*
	����Y����ֵ
	0 ʧ��
	1 �ɹ�
	*/
	int SetY(float y);
	/*
	����Z����ֵ
	0 ʧ��
	1 �ɹ�
	*/
	int SetZ(float z);
	/*
	����W����ֵ
	0 ʧ��
	1 �ɹ�
	*/
	int SetW(float w);
	/*
	�����±�������Vector��ֵ
	0 ʧ��
	1 �ɹ�
	*/
	int SetByIndex(int index=0,float value=0);
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