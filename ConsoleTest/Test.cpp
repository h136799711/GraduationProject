#include "..\Vector.h"
#include "..\Geom.h"


//#define NDEBUG

#include <assert.h>
#include "Tools.h"
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


int main()
{
#ifndef TO_FILE_HBD
	freopen("G:\\OUTPUT.txt", "w", stdout);
	freopen("G:\\ERROR.txt", "w", stderr);
#endif
	printf("Test Begin\n");
	Print_NewLine("BEGIN========================BEGIN");
	Print_NewLine("");
	Print_NewLine("");
	TestVector();
	Test_Vector_Point();
	Print_NewLine("Test_Vector_Point Successed!!");
	TestGeom();
	Print_NewLine("BEGIN========================BEGIN");
	Print_NewLine("");
	Print_NewLine("END=========================END");
	printf("All Test Successed!! Wooh!!! :) :) \n");
	return 0;
}

void Test_Vector_Quad()
{
	CQuat quat;

}
void TestGeom()
{
	float arg[] = {1.0f,2.0f,3.0f};
	CVector2D		vector2d(arg);
	CVector3D		vector3d(arg);

	CParmline2D		parmline2d(vector2d,vector2d,vector2d);
	CParmline3D		parmline3d(vector3d,vector3d,vector3d);
	
	assert(vector2d == parmline2d.GetP0());
	assert(parmline2d.GetP1() == vector2d);
	assert(parmline2d.GetV() == vector2d);
	
	parmline2d.CalculateV();
	CVector2D tmp = (vector2d - vector2d);
	char*desc = new char [66];
	tmp.GetString(desc);
	Print_NewLine(desc);
	assert(parmline2d.GetV() == tmp);
	parmline3d.CalculateV();
	assert(parmline3d.GetV() == (vector3d - vector3d));


	
	CCylinderical3D cylinderical3d;
	CPolar2D		polar2d;
	CSpherical3D	spherical3d;
	CPlane2D		plane2d;
	CPlane3D		plane3d;
	

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


}
void TestVector()
{
	Print_NewLine("BEGIN========TestVector===========BEGIN");
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
	Print_NewLine("END=======TestVector=============END");

}
void Test_Vector_Point()
{
	Print_NewLine("BEGIN========TestPoint===========BEGIN");
	Print_NewLine("");
	Print_NewLine("");
	float arg[] = {1.0f,2.0f,3.0f};
	CVector2D vector2d(arg);
	CPoint2D point2d;
	point2d = vector2d;
	assert(point2d.GetX() == vector2d.GetX());
	assert(point2d.GetY() == vector2d.GetY());
	Print_NewLine("END=======TestPoint=============END");


}

