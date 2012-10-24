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
//4维下的Zero方法W分量为1不是0
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
	通过传入float数组,确保float数组长度有dimension的长度
	*/	
	CVector(float* pVec)
	{
		memcpy(m_vector,pVec,sizeof(m_vector));
	}
	
	inline int SetXYZW(float x,float y,float z=0,float w=1);
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
	inline void Zero();
	/*
	[]运算
	以数组方式访问
	*/
	inline float& operator[] (int index)
	{
		return m_vector[index];
	}
	
	/*
	赋值运算 = ,数组入参
	
	*/
	
	inline CVector<dimension> operator = (float* vec)
	{		
		memcpy(m_vector,vec,dimension * sizeof(float));		
		return *this;
	}
	/*
	赋值运算 = ，CVector入参
	
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
	矩阵与浮点数 *= 
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
	+=运算
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
	-=运算
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
	== 运算 
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
	计算此向量与入参向量的夹角theta的cos(theta)
	公式cos(theta) = (u * v )/( |u|*|v|)
	returns the cosine of the angle between two vectors.
	*/
	float CosTh(CVector<dimension>& vec)
		
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
	第二个参数存放结果
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
		向量的叉积,
		结果为垂直于叉积的2个向量
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
//2D,3D,4D点，向量定义
typedef CVector<2>  CVector2D;
typedef CVector<3>	CVector3D;
typedef CVector<4>  CVector4D;	
typedef CVector<2> CPoint2D;
typedef CVector<3> CPoint3D;
typedef CVector<4> CPoint4D;

/*******
	
	  四元数：超复数
	  与3D向量，角度之间的关系
	  有以下公式
	  四元数 : Quat,q0为quat的w分量,qv为quat的x，y，z3个分量组合的3D向量
	  theta : 角度(度数)
	  v : 3D向量
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
		范数 等效于向量的长度
	*/
	float Norm()
	{
		return Length();
	}
	/*
		范数的平方
	*/
	float Norm2()
	{
		return (GetX()*GetX() + GetY()*GetY() + GetZ()*GetZ() + GetW()*GetW());
	}
	/*
		即范数为 1 的复数
		称为Unit
		Unit的倒数即是其共轭复数
	*/
	void Unit_Inverse()
	{
		SetX(-GetX());
		SetY(-GetY());
		SetZ(-GetZ());
	}
	/*
		倒数
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
		复数的乘法
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
		使用一个3D方向向量(归一化)
		和一个旋转角度(弧度)来初始化四元数
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
		使用一个3D方向向量(归一化)
		和一个旋转角度(弧度)来初始化四元数
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
		根据绕x、y、z旋转的角度，创建一个zyx顺序进行旋转对应的四元数
		四元数qfinal
		qxtheta: xtheta绕x轴旋转的角度(倾角)
		qytheta: ytheta绕y轴旋转的角度(偏航角)
		qztheta: ztheta绕z轴旋转的角度(倾侧角)
		i,j,k是x，y，z轴上的方向放量
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
	//将一个单位四元数转换为一个单位方向向量和一个绕该向量旋转的角度
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