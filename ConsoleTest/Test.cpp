#include "..\Vector.h"

#ifdef TO_FILE_HBD
#define NDEBUG
#endif
#include <assert.h>
#include "Tools.h"
using namespace ToolsHBD;
void TestGetSet()
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
void TestOperator()
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
	
	
}
void Test2D3D4DDef()
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
void TestFunctions()
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
	TestGetSet();
	Print_NewLine("TestGetSet Successed!!");
	TestOperator();
	Print_NewLine("TestOperator Successed!!");
	Test2D3D4DDef();
	Print_NewLine("Test2D3D4DDef Successed!!");
	TestFunctions();
	Print_NewLine("TestFunctions Successed!!");
	Print_NewLine("");
	Print_NewLine("END=======TestVector=============END");

}
void TestPoint()
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
void TestQuad()
{

}
void TestGeom()
{
	
}

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
	TestPoint();
	TestGeom();
	Print_NewLine("");
	Print_NewLine("END=========================END");
	printf("All Test Successed!!Wooh\n");
	return 0;
}