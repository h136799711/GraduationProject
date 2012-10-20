#include "vector.h"
#include <assert.h>
#include "Tools.h"
using namespace HBD;
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


int main()
{

	printf("Test Begin\n");
	Print_NewLine("BEGIN========================BEGIN");
	Print_NewLine("");
	Print_NewLine("");
	TestGetSet();
	printf("TestGetSet Successed!!\n");

	Print_NewLine("");
	Print_NewLine("END=========================END");
	printf("All Test Successed!!Wooh\n");
	return 0;
}