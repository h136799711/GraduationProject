#ifndef H_HCLOG_INCLUDED
#define H_HCLOG_INCLUDED

#include <stdio.h>
#include <sys/timeb.h>
#include <stdarg.h>
#include <time.h>
//Ĭ����־�ļ���
#define DEFAULT_LOG_FILE_NAME	"log.txt"
class HCLog
{

protected :
	FILE *fp_log;                           // log��־

public:
	
	/*
	��һ�������¼�ļ�

	filename:�ļ���
	fp_override: �ļ�ָ�룬Ĭ��ΪNULL�����ָ����Чֵ��filename��Ч
	*/
	int Open_Error_File(char *filename=DEFAULT_LOG_FILE_NAME, FILE *fp_override=NULL);
	/*
		�ر��ļ�
		ȷ���ڴ�֮��ض�Ҫ�ر�
	*/
	int Close_Error_File(void);
	/*
		������ļ���д����Ϣ
		����
		Write_Error("Error code %d",2);
	*/
	int Write_Error(char *string, ...);
};


#endif