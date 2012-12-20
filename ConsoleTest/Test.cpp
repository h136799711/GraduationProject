/*****************************************************************************************
**	Version: 2.0v																**********
**	author:hebidu.																**********
**	Description:数学库的测试项目.												**********
**	date :2012-10-23	13:00													**********	
*****************************************************************************************/
#include "..\HCLib\HCMath.h"
#include "..\Vector.h"
#include "..\Geom.h"
#include "..\Matrix.h"
#include "..\Math3D.h"
#include "..\ObjectDef.h"
//#define NDEBUG

#include <assert.h>
#include "Tools.h"
#include "test_ctest.h"
//GetSet测试
void Test_Vector_GetSet();
//operator操作符测试
void Test_Vector_Operator();
//2D,3D,4D向量，点测试
void Test_Vector_2D3D4DDef();
//测试vector的方法
void Test_Vector_Functions();
//测试vector点
void Test_Vector_Point();
//测试四元数
void Test_Vector_Quad();
//测试向量
void TestVector();
//测试几何，坐标系的定义
void TestGeom();
//测试Matrix矩阵
void TestMatrix();
//测试Math3D
void TestMath3D();	

int main()
{
#ifndef TO_FILE_HBD
	freopen("G:\\OUTPUT.txt", "w", stdout);
	freopen("G:\\ERROR.txt", "w", stderr);
#endif
	CTest_test();
	printf("Test Begin\n");
	Print_NewLine("BEGIN========================BEGIN");
	Print_NewLine("");
	Print_NewLine("");
	TestVector();
	Print_NewLine("TestVector Successed!!");
	Test_Vector_Point();
	Print_NewLine("Test_Vector_Point Successed!!");
	TestGeom();
	Print_NewLine("TestGeom Successed!!");
	TestMatrix();
	Print_NewLine("TestMatrix Successed!!");
	TestMath3D();
	Print_NewLine("TestMath3D Successed!!");
	
	
	Print_NewLine("BEGIN========================BEGIN");
	Print_NewLine("");
	Print_NewLine("END=========================END");
	printf("All Test Successed!! Wooh!!!Great! :) :) \n");
	return 0;
}
void TestMath3D()
{
	CMath3D math3d;
	float arg1[1][2] = {{2.0f,4.0f}};
	float arg2[2][1] = {{4.0f},{2.0f}};
	CMatrix<1,2> m12(&arg1[0][0]);
	CMatrix<2,1> m21(&arg2[0][0]);
	CMatrix<1,1> m11;

//	m11 = m12 * m21;
	math3d.Mat_Mul_XC_CY(1,2,1,&m12.M[0][0],&m21.M[0][0],&m11.M[0][0]);
	assert(m11.M[0][0] == 16);

	float arg22[2][2] = {{1.0f,2.0f},
						{3.0f,4.0f}};
	float arg23[2][3] = {{1.0f,3.0f,5.0f},
						{6.0f,0.0f,4.0f}};
	float * ar = &arg23[0][0];
	float tmp = ar[2+1];
	CMatrix<2,2> m22(&arg22[0][0]);
	CMatrix<2,3> m23(&arg23[0][0]);
	CMatrix<2,3> mb23;

	math3d.Mat_Mul_XC_CY(2,2,3,&m22.M[0][0],&m23.M[0][0],&mb23.M[0][0]);
	
	
	//继续
	arg22[0][0] = 3;
	arg22[0][1] = 5;
	arg22[1][0] = -2;
	arg22[1][1] = 2;
	m22 = &arg22[0][0];
	m22.Inverse(m22);
	float f00 = 0.125f;
	float fm00 = m22.M[0][0];
	assert(m22.M[0][0] - f00 < EPSILON_E5); 
	assert(m22.M[0][1] - (-2.5*f00) < EPSILON_E5); 
	assert(m22.M[1][0] - f00 < EPSILON_E5); 
	assert(m22.M[1][1] - 1.5*f00 < EPSILON_E5); 

	float arg[4][4] = { {1.0f,1.0f,4.0f,1.0f,},
						{4.0f,1.0f,1.0f,4.0f,},
						{1.0f,4.0f,1.0f,1.0f,},
						{1.0f,1.0f,4.0f,1.0f,}};
	float argv[4] = {1.0f,2.0f,4.0f};
	CMatrix14 m14a(&arg[0][0]),m14b(&arg[0][0]);
	CMatrix44 m44(&arg[0][0]);
	CVector3D v3d(argv),rst,rst2;
	CMatrix33 m33(&arg[0][0]);
	CMatrix43 m43(&arg[0][0]);
	CVector4D v4d(argv);
	CVector4D rst4d;
	math3d.Mat_Mul_14_44(m14a,m44,m14b);
	CVector4D rst4dTmp;
	math3d.Mat_Mul_XC_CY(1,4,4,&m14a.M[0][0],&m44.M[0][0],rst4dTmp.m_vector);	
	assert(fabs(m14b.M[0][HC_X] - rst4dTmp.GetX()) <= EPSILON_E5 );
	assert(fabs(m14b.M[0][HC_Y] - rst4dTmp.GetY()) <= EPSILON_E5 );
	assert(fabs(m14b.M[0][HC_Z] - rst4dTmp.GetZ()) <= EPSILON_E5 );

	math3d.Mat_Mul_3D_33(v3d,m33,rst);
	math3d.Mat_Mul_XC_CY(1,3,3,v3d.m_vector,&m33.M[0][0],rst2.m_vector);	
	assert(fabs(rst.GetX() - rst2.GetX()) <= EPSILON_E5 );
	assert(fabs(rst.GetY() - rst2.GetY()) <= EPSILON_E5 );
	assert(fabs(rst.GetZ() - rst2.GetZ()) <= EPSILON_E5 );

	v4d.SetW(1);
	v3d = v4d.m_vector;
	math3d.Mat_Mul_3D_43(v3d,m43,rst);
	math3d.Mat_Mul_XC_CY(1,4,3,v4d.m_vector,&m43.M[0][0],rst2.m_vector);	
	assert(fabs(rst.GetX() - rst2.GetX()) <= EPSILON_E5 );
	assert(fabs(rst.GetY() - rst2.GetY()) <= EPSILON_E5 );
	assert(fabs(rst.GetZ() - rst2.GetZ()) <= EPSILON_E5 );

	math3d.Mat_Mul_3D_44(v3d,m44,rst);
	math3d.Mat_Mul_XC_CY(1,4,4,v4d.m_vector,&m44.M[0][0],rst2.m_vector);	
	assert(fabs(rst.GetX() - rst2.GetX()) <= EPSILON_E5 );
	assert(fabs(rst.GetY() - rst2.GetY()) <= EPSILON_E5 );
	assert(fabs(rst.GetZ() - rst2.GetZ()) <= EPSILON_E5 );

	math3d.Mat_Mul_4D_43(v4d,m43,rst4d);
	math3d.Mat_Mul_XC_CY(1,4,3,v4d.m_vector,&m43.M[0][0],rst4dTmp.m_vector);
	assert(fabs(rst4d.GetX() - rst4dTmp.GetX()) <= EPSILON_E5 );
	assert(fabs(rst4d.GetY() - rst4dTmp.GetY()) <= EPSILON_E5 );
	assert(fabs(rst4d.GetZ() - rst4dTmp.GetZ()) <= EPSILON_E5 );

	math3d.Mat_Mul_4D_44(v4d,m44,rst4d);
	math3d.Mat_Mul_XC_CY(1,4,4,v4d.m_vector,&m44.M[0][0],rst4dTmp.m_vector);
	assert(fabs(rst4d.GetX() - rst4dTmp.GetX()) <= EPSILON_E5 );
	assert(fabs(rst4d.GetY() - rst4dTmp.GetY()) <= EPSILON_E5 );
	assert(fabs(rst4d.GetZ() - rst4dTmp.GetZ()) <= EPSILON_E5 );	


}


void TestMatrix()
{
	static CMatrix44 mrot;

	assert(mrot.M[0][0] == 0.0f);

	CMatrix33 matrix1,matrix2;
	matrix1.M[1][1] = 2;
	matrix2 = matrix1;
	assert(matrix1.M[1][1] == matrix2.M[1][1]);
	matrix1 *= 2;
	assert(matrix1.M[1][1] == 2*matrix2.M[1][1]);
	matrix1 -= matrix2;
	assert(matrix1.M[1][1] == matrix2.M[1][1]);

	assert(matrix1 == matrix2);
	
	matrix1.Zero();
	matrix1.M[1][1] = 5;
	matrix2.Zero();
	matrix2.M[1][1] = 5;
	CMatrix33 matrix3 ;
	matrix3 = matrix1 + matrix2;
	assert(matrix3.M[1][1] == 5+5);
	matrix3 = matrix3 - matrix1;
	assert(matrix3.M[1][1] == 5);
	//行列式计算Gauss消元法

	float m22[2][2] = {	{3,5},
						{-2,2}};
	CMatrix22 matrix22;
	memcpy(&matrix22.M[0][0],&m22[0][0],sizeof(m22));
	float det3=0;
	matrix22.GaussDet(det3);

	float det4=0;
	matrix22.Det(det4);
	Print_NewLine("2X2DET det3= %f,det4 = %f",det3,det4);
	assert(fabs(det4 - det3) < EPSILON_E6);

	float m33[3][3] = {	{2,4,3},
						{5,2,4},
						{0,0,4}};
	memcpy(&matrix3.M[0][0],&m33[0][0],sizeof(m33));
	float det1=0;
	matrix3.GaussDet(det1);
	float det2=0;
	matrix3.Det(det2);
	Print_NewLine("3X3DET det1= %f,det2 = %f",det1,det2);
	assert(fabs(det2 - det1) < EPSILON_E6);

	matrix3.Identify();
	
	assert(matrix3.M[0][0] == matrix3.M[1][1]);
	assert(matrix3.M[2][2] == matrix3.M[1][1]);
	assert(matrix3.M[0][0] == 1.0f);

	CMatrix<1,3> vec;
	vec.M[0][1] = 15.0f;
	matrix3.ColumnSwap(1,vec);
	assert(matrix3.M[1][1] == 15.0f);
	vec.M[0][2] = 15.0f;
	matrix3.ColumnSwap(2,vec);
	assert(matrix3.M[2][2] == 15.0f);
	matrix3.ColumnSwap(2,vec);
	assert(matrix3.M[0][2] == 0.0f);
}
void Test_Vector_Quad()
{
	float v3d[] = {1.0f,2.0f,3.0f};
	CQuat quat;
	CVector3D qv(v3d);
	quat.SetQv(qv);
	assert(qv.GetX() == quat.GetX());
	assert(qv.GetY() == quat.GetY());
	assert(qv.GetZ() == quat.GetZ());
	qv.Zero();
	quat.GetQv(qv);
	assert(qv.GetX() == quat.GetX());
	assert(qv.GetY() == quat.GetY());
	assert(qv.GetZ() == quat.GetZ());
	assert(quat.Norm() - 14.0f < EPSILON_E5);

}
void TestGeom()
{
	float arg[] = {1.0f,2.0f,3.0f};
	CVector2D		vector2d(arg);
	CVector3D		vector3d(arg);

	CParmline2D		parmline2d(vector2d,vector2d*2);
	CParmline3D		parmline3d(vector3d,vector3d*2);
	
	assert(vector2d == parmline2d.GetP0());
	assert(parmline2d.GetP1() == vector2d*2);
	
	CVector2D tmp = (vector2d*2 - vector2d);
	
	assert(parmline2d.GetV() == tmp);

	assert(parmline3d.GetV() == (vector3d*2 - vector3d));
	
	int ist_rst = parmline2d.Intersect_Parm_Lines(parmline2d,vector2d);
	assert(ist_rst == PARM_LINE_NO_INTERSECT);
	CParmline2D pline2dTmp;
	pline2dTmp  = parmline2d;
	CPoint2D p12d =  parmline2d.GetP1();
	p12d.SetY(2.0f);parmline2d.SetP1(p12d);
	p12d =  parmline2d.GetP0();
	p12d.SetX(0.0f);
	p12d.SetY(1.0f);
	parmline2d.SetP0(p12d);
	
	ist_rst = pline2dTmp.Intersect_Parm_Lines(parmline2d,vector2d);
	assert(ist_rst != PARM_LINE_NO_INTERSECT);
	assert(ist_rst != PARM_LINE_INTERSECT_IN_SEGMENT);
	assert(ist_rst == PARM_LINE_INTERSECT_OUT_SEGMENT);
	//2D 坐标系 极坐标转换
	CMath3D	math3d;
	CPolar2D		polar2d;
	CPoint2D rect;
	polar2d.m_r = 6;
	polar2d.m_theta     = DEG_TO_RAD(60);
	math3d.Polar2D_To_Point2D(polar2d,rect);
	assert(fabs(rect.GetX() - 3.0f) < EPSILON_E5);
	polar2d.Zero();
	math3d.Point2D_To_Polar2D(rect,polar2d);
	assert(fabs(polar2d.m_r - 6.0f) < EPSILON_E5);
	assert(fabs(polar2d.m_theta - DEG_TO_RAD(60)) < EPSILON_E5);

	float x,y;
	polar2d.m_r = 6;
	polar2d.m_theta     = DEG_TO_RAD(60);
	math3d.Polar2D_To_RectXY(polar2d,x,y);
	assert(fabs(x - 3.0f) < EPSILON_E5);
	polar2d.Zero();
	math3d.Point2D_To_Polar2D(rect,polar2d);
	assert(fabs(polar2d.m_r - 6.0f) < EPSILON_E5);
	assert(fabs(polar2d.m_theta - DEG_TO_RAD(60)) < EPSILON_E5);
	

	CCylinderical3D cylinderical3d;
	cylinderical3d.m_r = 6;
	cylinderical3d.m_z = 6;
	cylinderical3d.m_theta = DEG_TO_RAD(60);
	CVector3D	rect3d;
	math3d.Cylinderical3D_To_Point3D(cylinderical3d,rect3d);
	assert(fabs(rect3d.GetX() - 3.0f) < EPSILON_E5);	

	CSpherical3D	spherical3d;
	spherical3d.m_p = 24;
	spherical3d.m_phi = DEG_TO_RAD(60);
	spherical3d.m_theta = DEG_TO_RAD(30);
	rect3d.Zero();
	math3d.Spherical3D_To_Point3D(spherical3d,rect3d);
	assert(fabs(rect3d.GetZ() - 12.0f)<EPSILON_E5);
	
	//测试2元一次方程组
	float Adata[2][2] = {{3,5},{-2,2}};
	float Bdata[1][2] = {{6,4}};
	CMatrix22 mA(&Adata[0][0]);
	CMatrix12 mB(&Bdata[0][0]),mX;

	math3d.Solve_2X2_System(mA,mX,mB);
	assert(fabs(mX.M[0][0] - (-0.5f)) < EPSILON_E5);
	assert(fabs(mX.M[0][1] - (1.5f)) < EPSILON_E5);


	//测试3元一次方程组
	float Adata3[3][3] = {{3,5,0},{-2,2,0},{1,1,1}};
	float Bdata3[1][3] = {{6,4,1}};
	CMatrix33 mA3(&Adata3[0][0]);
	CMatrix13 mB3(&Bdata3[0][0]),mX3;

	math3d.Solve_3X3_System(mA3,mX3,mB3);
	assert(fabs(mX3.M[0][0] - (-0.5f)) < EPSILON_E5);
	assert(fabs(mX3.M[0][1] - (1.5f)) < EPSILON_E5);
	assert(fabs(mX3.M[0][2] - (0.0f)) < EPSILON_E5);

	//测试相交	
	float pt_arr [] = {0.0f,0.0f,0.0f};
//	float plane_arr[] = {}
	CPlane3D		plane3d;
	CPoint3D		pt;
	pt = pt_arr;
	float normal_arr[] = {0.0f,-1.0f,0.0f};
	CVector3D		normal(normal_arr);
	plane3d.Init(pt,normal);
	
	float hs = plane3d.Compute_Point_In(pt);	
	printf("hs = %f",hs);
	pt_arr[0] = 2.0f;
	pt_arr[2] = 2.0f;
	pt = pt_arr;
	hs = plane3d.Compute_Point_In(pt);	
	printf("hs = %f",hs);
	pt_arr[0] = 5.0f;
	pt_arr[1] = -20.0f;
	pt = pt_arr;
	hs = plane3d.Compute_Point_In(pt);	
	printf("hs = %f",hs);
	
	CParmline3D pline3d;
	CPoint3D p0,p1;
	p0.Zero();p1.Zero();
	p1.SetY(2.0f);
	
	pline3d.SetP0(p0);pline3d.SetP1(p1);
	
	assert(pline3d.GetV().GetX() == pline3d.GetP1().GetX() - pline3d.GetP0().GetX());
	assert(pline3d.GetV().GetY() == pline3d.GetP1().GetY() - pline3d.GetP0().GetY());
	assert(pline3d.GetV().GetZ() == pline3d.GetP1().GetZ() - pline3d.GetP0().GetZ());


	
//	CPlane2D		plane2d;
	

}

void Test_Vector_GetSet()
{

	CVector<> vec;
	assert(vec.GetX() == 0.0f);
	assert(vec.GetY() == 0.0f);
	if(vec.SetZ(3.0f))
		assert(vec.GetZ() == 3.0f);
	else
		assert(vec.GetZ() == 0.0f);
	
	float arg1[] = {1.0f,2.0f};
	CVector<> vec2(arg1);
	assert(vec2.GetX() == 1.0f);
	assert(vec2.GetY() == 2.0f);
	if(vec2.SetZ(3.0f))
		assert(vec2.GetZ() == 3.0f);
	else
		assert(vec2.GetZ() == 0.0f);
	
}
void Test_Vector_Operator()
{
	float arg[] = {1.0f,2.0f};
	CVector<> vec(arg);
	CVector<> vec2(arg);
	assert(vec.GetX() == 1.0f);
	assert(vec.GetY() == 2.0f);
	
	assert(vec[0] == 1.0f);
	assert(vec[1] == 2.0f);
	assert(vec == vec2);
	float k = 2.0f;
	vec *=k;
	assert(vec.GetX() == (1.0f*k));
	assert(vec.GetY() == (2.0f*k));
		
	vec = vec - vec2;
	assert(vec.GetX() == (1.0f*k - 1.0f));
	assert(vec.GetY() == (2.0f*k - 2.0f));
	vec = vec + vec2;
	assert(vec.GetX() == (1.0f*k));
	assert(vec.GetY() == (2.0f*k));

	vec.Zero();
	assert(vec.GetX() == 0.0f);
	assert(vec.GetY() == 0.0f);

	vec = arg;
	assert(vec.GetX() == arg[0]);
	assert(vec.GetY() == arg[1]);

	vec += vec2;
	assert(vec.GetX() == (arg[0] + vec2[0]));
	assert(vec.GetY() == (arg[1] + vec2[1]));
	CVector2D vec2d1(arg),vec2d2(arg);
	CVector3D vec3d1(arg),vec3d2(arg);
	
	assert(vec2d1 == vec2d2);
	assert(vec3d1 == vec3d2);
}

void Test_Vector_2D3D4DDef()
{
	float arg[] = {1.0f,2.0f,3.0f,4.0f};

	float test[3];

	memcpy(test,arg,3*sizeof(3));

	CPoint2D point2d(arg);
	CPoint3D point3d(arg);
	CPoint4D point4d(arg);

	CVector2D vector2d(arg);
	CVector3D vector3d(arg);
	CVector4D vector4d(arg);
	int dimension = 2,i;
	for(i=0;i<dimension;i++)
	{
		assert(point2d[i] == arg[i]);
		assert(vector2d[i] == arg[i]);
	}
	dimension = 3;
	for(i=0;i<dimension;i++)
	{
		assert(point3d[i] == arg[i]);
		assert(vector3d[i] == arg[i]);
	}
	dimension = 4;
	for(i=0;i<dimension;i++)
	{
		assert(point4d[i] == arg[i]);
		assert(vector4d[i] == arg[i]);
	}
	
	CPoint2D tmp2;
	tmp2 = point2d + point2d;
	assert(tmp2[0] == point2d[0] + point2d[0]);
	assert(tmp2[1] == point2d[1] + point2d[1]);
	
	CPoint3D tmp3;
	tmp3 = point3d + point3d;
	assert(tmp3[0] == point3d[0] + point3d[0]);
	assert(tmp3[1] == point3d[1] + point3d[1]);
	assert(tmp3[2] == point3d[2] + point3d[2]);

	CPoint4D tmp4;
	tmp4 = point4d + point4d;
	assert(tmp4[0] == point4d[0] + point4d[0]);
	assert(tmp4[1] == point4d[1] + point4d[1]);
	assert(tmp4[2] == point4d[2] + point4d[2]);
	assert(tmp4[3] == point4d[3] + point4d[3]);


}
void Test_Vector_Functions()
{
	FIXP16 f = FLOAT_TO_FIXP16(2.3f);
	
	int tmp = FIXP16_WP(f);
	float ft = FIXP16_DP(f);
	ft = FIXP16_TO_FLOAT(ft);

	float arg[] = {2.0f,3.0f,4.0f,5.0f};
	CVector2D vector2d(arg);
	float len = sqrtf(4.0+9.0);
	assert(fabs(vector2d.Length() - len) < EPSILON_E5);

	Print_NewLine("2d Length() = %f",vector2d.Length());
	Print_NewLine("2d len = %f",len);

	float fast_len = vector2d.Fast_Length();
	
	Print_NewLine("2d fast_len = %f",fast_len);
	assert(fabs(len - fast_len) < 1.0f);
	
	CVector3D vector3d(arg);
	len = sqrtf(4.0+9.0+16.0);
	assert(fabs(vector3d.Length() - len) < EPSILON_E5);

	Print_NewLine("3d Length() = %f",vector3d.Length());
	Print_NewLine("3d len = %f",len);

	fast_len = vector3d.Fast_Length();
	
	Print_NewLine("3d fast_len = %f",fast_len);
	assert(fabs(len - fast_len) < 1.0f);
	
	CVector3D result;
	vector3d.Scale(2.0f,result);
	assert(result[0] == 2.0f * vector3d[0]);
	assert(result[1] == 2.0f * vector3d[1]);
	assert(result[2] == 2.0f * vector3d[2]);

	assert(vector3d.DivByW() == 0);
	CVector4D vector4d(arg);
	
	assert(vector4d.DivByW() == 1);
	
	CVector3D vx,vy,vz;
	float argVy[] ={0.0f,1.0f,0.0f};
	vx = argVy;
	float argVx[] = {1.0f,0.0f,0.0f};
	vy = argVx;
	vz = vx;
	assert(&vz != &vx);
	vx.Cross(vy,vz);
	assert(vz[2] == -1.0f);
	assert(vx * vy == 0.0f);
	assert(vx.Dot(vy) == 0.0f);
	vy= vx;
	vx.Cross(vx,vx);
	assert(vx * vy == 0.0f);
	
	vx = arg;
	vx.Normalize();
	len = vx.Length();
	assert(len -1.0f < EPSILON_E5);

	



}
void TestVector()
{
	Print_NewLine("");
	Print_NewLine("");
	Test_Vector_GetSet();
	Print_NewLine("TestGetSet Successed!!");
	Test_Vector_Operator();
	Print_NewLine("TestOperator Successed!!");
	Test_Vector_2D3D4DDef();
	Print_NewLine("Test2D3D4DDef Successed!!");
	Test_Vector_Functions();
	Print_NewLine("TestFunctions Successed!!");
	Test_Vector_Quad();
	Print_NewLine("Test_Vector_Quad Successed!!");

	Print_NewLine("");

}
void Test_Vector_Point()
{
	float arg[] = {1.0f,2.0f,3.0f};
	CVector2D vector2d(arg);
	CPoint2D point2d;
	point2d = vector2d;
	assert(point2d.GetX() == vector2d.GetX());
	assert(point2d.GetY() == vector2d.GetY());

}