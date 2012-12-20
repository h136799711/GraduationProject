#ifndef _CTEST_INCLUDED_H
#define _CTEST_INCLUDED_H

#include <time.h>

/**
** 
** 
** 
**/
class CTest
{
private:
	FILE* m_fp;
	char buffer[256]; 
public:
	int openFile(char* filename="result.test")
	{
		if(!(m_fp = fopen(filename,"w")))
		{
			return 0;
		}

		if(!(m_fp = fopen(filename,"a+")))
		{
			return 0;
		}
		writeMsg("Open File.");
		return 1;
	}
	int writeMsg(char* msg)
	{

		if (!msg || !m_fp)
			return(0);
		
		time_t t = time(0); 
		char tmp[64]; 
		strftime(tmp, sizeof(tmp), "%Y-%m-%d %X %A  %z : ",localtime(&t) ); 

		fprintf(m_fp,tmp);
		fprintf(m_fp,msg);
		fprintf(m_fp,"\n");		
		fflush(m_fp);
		
		return(1);
	}
	int closeFile()
	{
		if(m_fp)
		{
			writeMsg("Close File.");
			fclose(m_fp);
			return 1;
		}
		return 0;
	}
};


#endif