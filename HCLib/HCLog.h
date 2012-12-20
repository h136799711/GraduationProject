#ifndef H_HCLOG_INCLUDED
#define H_HCLOG_INCLUDED

#include <stdio.h>
#include <sys/timeb.h>
#include <stdarg.h>
#include <time.h>
//默认日志文件名
#define DEFAULT_LOG_FILE_NAME	"log.txt"
class HCLog
{

protected :
	FILE *fp_log;                           // log日志

public:
	
	/*
	打开一个错误记录文件

	filename:文件名
	fp_override: 文件指针，默认为NULL，如果指定有效值，filename无效
	*/
	int Open_Error_File(char *filename=DEFAULT_LOG_FILE_NAME, FILE *fp_override=NULL);
	/*
		关闭文件
		确保在打开之后必定要关闭
	*/
	int Close_Error_File(void);
	/*
		向错误文件中写入信息
		例如
		Write_Error("Error code %d",2);
	*/
	int Write_Error(char *string, ...);
};


#endif