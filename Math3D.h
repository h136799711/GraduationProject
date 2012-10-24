#pragma once

#include "HCLib\HCMath.h"
#include "Geom.h"
#include "Vector.h"
#include "Matrix.h"

class CMath3D :public HCMath
{

public:

	//====================坐标系变换方法
	/*
	2D极坐标转换为2D点
	*/
	int Polar2D_To_Point2D(CPolar2D polar,CPoint2D& rect);
	/*
	
	*/
	int Polar2D_To_RectXY(CPolar2D polar,float &x,float&y);
	/*
	2D点转换为2D极坐标
	*/
	int Point2D_To_Polar2D(CPoint2D rect,CPolar2D& polar);
	/*
	*/
	int Point2D_To_PolarRTh(CPoint2D rect,float &r,float &theta);
	/*
	3D柱面坐标转换为3D点
	*/
	int Cylinderical3D_To_Point3D(CCylinderical3D cyl,CPoint3D& rect);
	/*
	*/
	int Cylinderical3D_To_RectXYZ(CCylinderical3D cyl,float& x,float &y,float &z);
	/*
	
	*/
	int Point3D_To_Cylinderical3D(CPoint3D rect,CCylinderical3D& cyl);
	/*
	*/
	int Point3D_To_CylindericalRThz(CPoint3D rect,float &r,float &theta,float &z);
	/*
	*/
	int Spherical3D_To_Point3D(CSpherical3D sph,CPoint3D& rect);
	/*
	*/
	int Spherical3D_To_RectXYZ(CSpherical3D sph,float &x,float &y,float &z);
	/*
	*/
	int Point3D_To_Spherical3D(CPoint3D rect,CSpherical3D& sph);
	/*
	*/
	int Point3D_To_SphericalThPh(CPoint3D rect,float & p,float & theta,float & phi);
	
	//============================矩阵，向量相乘方法==========================================================
	
	/*
	x,c,y
	ma数组 符合ma[x][c];
	mb数组 符合mb[c][y];
	在矩阵模板类中，不好处理内维数相同 ，外维有可能不同的情况
	
	*/
	int Mat_Mul_XC_CY(int x,int c,int y,float *ma,float *mb,float *result);
	//3D向量与3X3矩阵
	int Mat_Mul_3D_33(CVector3D v3d,CMatrix33 m33,CVector3D& rst);
	//3d向量与4X4矩阵
	int Mat_Mul_3D_44(CVector3D v3d,CMatrix44 m44,CVector3D& rst);
	//3d向量与4X3矩阵
	int Mat_Mul_3D_43(CVector3D v3d,CMatrix43 m43,CVector3D& rst);
	//4d向量与4X4矩阵
	int Mat_Mul_4D_44(CVector4D v4d,CMatrix44 m44,CVector4D& rst);
	//4d向量与4X3矩阵
	int Mat_Mul_4D_43(CVector4D v4d,CMatrix43 m43,CVector4D& rst);
	//14矩阵与4X4矩阵
	int Mat_Mul_14_44(CMatrix14 v14,CMatrix44 m44,CMatrix14& rst);
	/*
	A 系数矩阵
	X 变量矩阵
	B 常量矩阵
	*/
	int Solve_2X2_System(CMatrix22 mA,CMatrix12& mX,CMatrix12 mB);
	int Solve_3X3_System(CMatrix33 mA, CMatrix13& mX, CMatrix13 mB);
	
	

	
	
};

int CMath3D::Mat_Mul_XC_CY(int x,int c,int y,float *ma,float *mb,float *result)
{
	for(int i=0;i<x;i++)
	{
		for(int j=0;j<y;j++)
		{
			float sum =0 ;
			
			for(int k=0;k<c;k++)
			{
				float tmp = (ma[i*c+k] * mb[k*y+j]);
				//printf("[%d][%d] %f * [%d][%d] %f = %f,",i+1,k+1,ma[i*x+k] ,k+1,j+1 ,mb[k*c+j],tmp);
				sum += tmp;
			}	
			//printf(" result[%d][%d]= %f \n",i,j,sum);
			result[i*x+j] = sum;
		}
		
	}
	return 1;
}




int CMath3D::Polar2D_To_Point2D(CPolar2D polar,CPoint2D& rect)
{
	rect.SetX(polar.m_r * cosf(polar.m_theta));
	rect.SetY(polar.m_r * sinf(polar.m_theta));
	return 1;
}
int  CMath3D::Polar2D_To_RectXY(CPolar2D polar,float& x,float& y)
{
	x = polar.m_r * cosf(polar.m_theta);
	y = polar.m_r * sinf(polar.m_theta);
	return 1;
}


int  CMath3D::Point2D_To_Polar2D(CPoint2D rect,CPolar2D& polar)
{
	polar.m_r = sqrtf((rect.GetX() * rect.GetX()) + (rect.GetY() * rect.GetY()));
	polar.m_theta = atanf(rect.GetY() / rect.GetX());
	return 1;
	
}


int  CMath3D::Point2D_To_PolarRTh(CPoint2D rect,float& r,float& theta)
{
	r = sqrtf((rect.GetX() * rect.GetX()) + (rect.GetY() * rect.GetY()));
	theta = atanf(rect.GetY() / rect.GetX());
	return 1;
}


int  CMath3D::Cylinderical3D_To_Point3D(CCylinderical3D cyl,CPoint3D& rect)
{
	rect.SetX(cyl.m_r * cosf(cyl.m_theta));
	rect.SetY(cyl.m_r * sinf(cyl.m_theta));
	rect.SetZ(cyl.m_z);
	return 1;
}


int  CMath3D::Cylinderical3D_To_RectXYZ(CCylinderical3D cyl,float& x,float& y,float& z)
{
	x = (cyl.m_r * cosf(cyl.m_theta));
	y = (cyl.m_r * sinf(cyl.m_theta));
	z = (cyl.m_z);
	return 1;
}


int  CMath3D::Point3D_To_Cylinderical3D(CPoint3D rect,CCylinderical3D& cyl)
{
	cyl.m_r = sqrtf((rect.GetX()*rect.GetX()) + (rect.GetY()*rect.GetY()));
	cyl.m_theta = atanf(rect.GetY() / rect.GetX());
	cyl.m_z = rect.GetZ();
	return 1;
	
}


int  CMath3D::Point3D_To_CylindericalRThz(CPoint3D rect,float& r,float& theta,float& z)
{
	r = sqrtf((rect.GetX()*rect.GetX()) + (rect.GetY()*rect.GetY()));
	theta = atanf(rect.GetY() / rect.GetX());
	z = rect.GetZ();
	return 1;
	
}


int  CMath3D::Spherical3D_To_Point3D(CSpherical3D sph,CPoint3D& rect)
{
	float r;
	//计算 r,and z
	r = sph.m_p * sinf(sph.m_phi);
	rect.SetZ(sph.m_p * cosf(sph.m_phi));
	rect.SetX(r*cosf(sph.m_theta));
	rect.SetY(r*sinf(sph.m_theta));
	
	return 1;
}

int  CMath3D::Spherical3D_To_RectXYZ(CSpherical3D sph,float& x,float& y,float& z)
{
	
	float r;
	//计算 r和z
	r = sph.m_p * sinf(sph.m_phi);
	z = sph.m_p * cosf(sph.m_phi);
	
	x = r * cosf(sph.m_theta);
	y = r * sinf(sph.m_theta);
	return 1;
}


int  CMath3D::Point3D_To_Spherical3D(CPoint3D rect,CSpherical3D& sph)
{
	sph.m_p = sqrtf(rect.GetX()*rect.GetX() + rect.GetY() * rect.GetY() + rect.GetZ() * rect.GetZ());
	sph.m_theta = atanf(rect.GetY() / rect.GetX());
	
	
	float r = sqrtf(rect.GetX()*rect.GetX() + rect.GetY() * rect.GetY());
	sph.m_phi = asinf(r / sph.m_p);
	return 1;
}


int  CMath3D::Point3D_To_SphericalThPh(CPoint3D rect,float& p,float& theta,float& phi)
{	
	p = sqrtf(rect.GetX()*rect.GetX() + rect.GetY() * rect.GetY() + rect.GetZ() * rect.GetZ());
	theta = atanf(rect.GetY() / rect.GetX());
	
	
	float r = sqrtf(rect.GetX()*rect.GetX() + rect.GetY() * rect.GetY());
	phi = asinf(r / p);
	return 1;
	
}


int CMath3D::Solve_2X2_System(CMatrix22 mA,CMatrix12& mX,CMatrix12 mB)
{
	// solves the system AX=B and computes X=A(-1)*B
	// by using cramers rule and determinates
	
	// step 1: compute determinate of A
	float det_A;mA.Det(det_A);
	
	// test if det(a) is zero, if so then there is no solution
	if (fabs(det_A) < EPSILON_E5)
		return(0);
	
	// step 2: create x,y numerator matrices by taking A and
	// replacing each column of it with B(transpose) and solve
	CMatrix22 work_mat; // working matrix
	
	// solve for x /////////////////
	
	// copy A into working matrix
	work_mat = mA;
	
	// swap out column 0 (x column)
	work_mat.ColumnSwap(0, mB);
	
	// compute determinate of A with B swapped into x column
	float det_ABx;
	work_mat.Det(det_ABx);
	
	// now solve for X00 = x 
	mX.M[0][0] = det_ABx/det_A;
	
	// solve for y /////////////////
	
	// copy A into working matrix
	work_mat = mA;
	
	// swap out column 1 (y column)
	work_mat.ColumnSwap(1, mB);
	
	// compute determinate of A with B swapped into y column
	float det_ABy;
	work_mat.Det(det_ABy);
	
	// now solve for X01 = y
	mX.M[0][1] = det_ABy/det_A;
	
	return 1;
}


int CMath3D::Solve_3X3_System(CMatrix33 mA, CMatrix13& mX, CMatrix13 mB)
{
	// solves the system AX=B and computes X=A(-1)*B
	// by using cramers rule and determinates
	
	// step 1: compute determinate of A
	float det_A;mA.Det(det_A);
	
	// test if det(a) is zero, if so then there is no solution
	if (fabs(det_A) < EPSILON_E5)
		return(0);
	
	// step 2: create x,y,z numerator matrices by taking A and
	// replacing each column of it with B(transpose) and solve
	CMatrix33 work_mat; // working matrix
	
	// solve for x /////////////////
	
	// copy A into working matrix
	
	work_mat = mA;
	
	// swap out column 0 (x column)
	
	work_mat.ColumnSwap(0,mB);
	
	// compute determinate of A with B swapped into x column
	float det_ABx;
	work_mat.Det(det_ABx);
	
	// now solve for X00
	mX.M[0][0] = det_ABx/det_A;
	
	// solve for y /////////////////
	
	// copy A into working matrix
	
	work_mat = mA;
	
	// swap out column 1 (y column)
	work_mat.ColumnSwap(1,mB);
	// compute determinate of A with B swapped into y column
	float det_ABy;
	work_mat.Det(det_ABy);
	
	// now solve for X01
	mX.M[0][1] = det_ABy/det_A;
	
	// solve for z /////////////////
	
	// copy A into working matrix
	work_mat = mA;
	
	// swap out column 2 (z column)
	work_mat.ColumnSwap(2,mB);
	
	// compute determinate of A with B swapped into z column
	float det_ABz;
	work_mat.Det(det_ABz);
	
	// now solve for X02
	mX.M[0][2] = det_ABz/det_A;
	
	// return success
	return 1;
}

int CMath3D::Mat_Mul_3D_33(CVector3D v3d,CMatrix33 m33,CVector3D& rst)
{
	
	for (int col=0; col < 3; col++)
    {
        float sum = 0; 
		
        for (int row=0; row<3; row++)
		{
			// add in next product pair
			sum+=(v3d.m_vector[row] * m33.M[row][col]);
		} // end for index
		
        // insert resulting col element
        rst.m_vector[col] = sum;
		
	} // end for col
	return 1;
}


int CMath3D::Mat_Mul_3D_44(CVector3D v3d,CMatrix44 m44,CVector3D& rst)
{
	int col,row;
    for (col=0; col < 3; col++)
	{
        // compute dot product from row of ma 
        // and column of mb
        float sum = 0; // used to hold result
		
        for (row=0; row<3; row++)
		{
			// add in next product pair
			sum+=(v3d.m_vector[row]*m44.M[row][col]);
		} // end for index
		
        // add in last element in column or w*mb[3][col]
        sum+=m44.M[row][col];
		
        // insert resulting col element
        rst.m_vector[col] = sum;
		
	} // end for col
	return 1;
}
//3d向量与4X3矩阵
int CMath3D::Mat_Mul_3D_43(CVector3D v3d,CMatrix43 m43,CVector3D& rst)
{
	//这个3D向量在计算时被 假设为等于一个4D向量
	//这个4D向量的W分量为之外其他分量等于此3D向量
	int col,row;
    for (col=0; col < 3; col++)
	{
        // compute dot product from row of ma 
        // and column of mb
        float sum = 0; // used to hold result
		
        for (row=0; row<3; row++)
		{
			// add in next product pair
			sum+=(v3d.m_vector[row]*m43.M[row][col]);
		} // end for index
		
        // add in last element in column or w*m43[3][col]
        sum+=m43.M[row][col];    
		
        // insert resulting col element
        rst.m_vector[col] = sum;
		
	} // end for col
	return 1;
}
//4d向量与4X4矩阵
int CMath3D::Mat_Mul_4D_44(CVector4D v4d,CMatrix44 m44,CVector4D& rst)
{ 
	for (int col=0; col < 4; col++)
	{
        // compute dot product from row of ma 
        // and column of mb
        float sum = 0; // used to hold result
		
        for (int row=0; row<4; row++)
		{
			// add in next product pair
			sum+=(v4d.m_vector[row]*m44.M[row][col]);
		} // end for index
		
        // insert resulting col element
        rst.m_vector[col] = sum;
		
	} // end for col
	return 1;
}
//4d向量与4X3矩阵
int CMath3D::Mat_Mul_4D_43(CVector4D v4d,CMatrix43 m43,CVector4D& rst)
{
	for (int col=0; col < 3; col++)
	{
		// compute dot product from row of ma 
		// and column of mb
		float sum = 0; // used to hold result
		
		for (int row=0; row<4; row++)
		{
			// add in next product pair
			sum+=(v4d.m_vector[row]*m43.M[row][col]);
		} // end for index
		
		// insert resulting col element
		rst.m_vector[col] = sum;
		
	} // end for col
	
	// copy back w element
	rst.SetW(v4d.GetW());
	return 1;
}
//14矩阵与4X4矩阵
int CMath3D::Mat_Mul_14_44(CMatrix14 m14,CMatrix44 m44,CMatrix14& rst)
{
    for (int col=0; col<4; col++)
	{
        // compute dot product from row of ma 
        // and column of mb
        float sum = 0; // used to hold result
		
        for (int row=0; row<4; row++)
		{
			// add in next product pair
			sum+=(m14.M[0][row] * m44.M[row][col]);
		} // end for index
		
        // insert resulting col element
        rst.M[0][col] = sum;
		
	} // end for col
	return 1;
}


