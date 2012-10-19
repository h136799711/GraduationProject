#include "HCLog.h"

int HCLog::Write_Error(char *string, ...)
{
	// this function prints out the error string to the error file
	
	char buffer[80]; // working buffer
	
	va_list arglist; // variable argument list
	
	// make sure both the error file and string are valid
	if (!string || !fp_log)
		return(0);
	
	// print out the string using the variable number of arguments on stack
	va_start(arglist,string);
	vsprintf(buffer,string,arglist);
	va_end(arglist);
	
	// write string to file
	fprintf(fp_log,buffer);
	
	// flush buffer incase the system bails
	fflush(fp_log);
	
	// return success
	return(1);
} // end Write_Error


int HCLog::Close_Error_File(void)
{
	// this function closes the error file
	
	if (fp_log)
    {
		// write close file string
		
		Write_Error("\nClosing Error Output File.");
		if (fp_log!=stdout || fp_log!=stderr)
		{
			// close the file handle
			fclose(fp_log);
		} 
		
		fp_log = NULL;
		
		// return success
		return(1);
    } // end if
	else
		return(0);
	
} // end Close_Error_File

int HCLog::Open_Error_File(char *filename, FILE *fp_override)
{
	// this function creates the output error file
	
	// is user requesting special file handle? stdout, stderr, etc.?
	if (fp_override)
	{
		fp_log = fp_override;
	}
	else
	{
		// test if this file is valid
		if ((fp_log = fopen(filename,"w"))==NULL)
			return(0);
	}
	
	// get the current time
	struct _timeb timebuffer;
	char *timeline;
	char timestring[280];
	
	_ftime(&timebuffer);
	timeline = ctime(&(timebuffer.time));
	
	sprintf(timestring, "%.19s.%hu, %s", timeline, timebuffer.millitm, &timeline[20]);
	
	// write out error header with time
	Write_Error("\nOpening Error Output File (%s) on %s\n",filename,timestring);
	
	// now the file is created, re-open with append mode
	
	if (!fp_override)
	{
		fclose(fp_log);
		if ((fp_log = fopen(filename,"a+"))==NULL)
			return(0);
	}
	
	// return success
	return(1);
	
} // end Open_Error_File
