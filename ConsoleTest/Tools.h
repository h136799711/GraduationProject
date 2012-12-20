#pragma once

#include <stdio.h>
#include <stdarg.h>

/*
在一行文字输出后换行
*/
int Print_NewLine(char *string="", ...)
{
	
	char buffer[80]; // working buffer
	
	va_list arglist; // variable argument list
	

	if (!string)
		return(0);
	

	va_start(arglist,string);
	vsprintf(buffer,string,arglist);
	va_end(arglist);
	
	printf("%s \n",buffer);
	return 1;
	
}

