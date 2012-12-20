#include"../CTest.h"

void CTest_test()
{
	CTest test;
	test.openFile();
	test.writeMsg("message");
	test.closeFile();
}