#pragma once

#include <stdio.h>
#include <stdarg.h>

namespace ToolsHBD
{
	/*
	在一行文字输出后换行
	*/
	int Print_NewLine(char *string="", ...)
	{
		
		char buffer[80]; // working buffer
	
		va_list arglist; // variable argument list
	
		// make sure both the error file and string are valid
		if (!string)
			return(0);
		
		// print out the string using the variable number of arguments on stack
		va_start(arglist,string);
		vsprintf(buffer,string,arglist);
		va_end(arglist);
		
		printf("%s \n",buffer);
		return 1;
		
	}
}