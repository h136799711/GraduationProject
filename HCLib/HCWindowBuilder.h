#ifndef H_HCWINDOWBUILDER_INCLUDED
#define H_HCWINDOWBUILDER_INCLUDED


#define SCREEN_DARKNESS		0
#define SCREEN_WHITENESS	1
#define SCREEN_SWIPE_X		2
#define SCREEN_SWIPE_Y		3
#define SCREEN_DISOLVE		4
#define SCREEN_SCRUNCH		5//a square compression
#define SCREEN_BLUENESS		6
#define SCREEN_REDNESS		7
#define SCREEN_GREENNESS	8
#define BLINKER_ADD			0//ADD a light to database
#define BLINKER_DELETE		1	//delete a light from database
#define BLINKER_UPDATE		2//update a light
#define BLINKER_RUN			3//run normal

#include <windows.h>
#include <windowsx.h>
#include "HCLog.h"
#include "HCMacros.h"

class HCWindowBuilder
{
public :
	HCWindowBuilder()
	{
		screen_width = SCREEN_WIDTH;
		screen_height = SCREEN_HEIGHT;
		screen_bpp = SCREEN_BPP;
		screen_windowed = SCREEN_WINDOWED;
		window_client_x0 = 0;
		window_client_y0 = 0;
	};
public :
	HCLog hclog;

	/*
		Ӧ���ڵ���CreateWndClassEx֮�����ʹ�ô�ֵ

	*/
	int  screen_width,screen_height,screen_bpp,screen_windowed;
	int  window_client_x0,window_client_y0;
	HWND main_hwnd;
	HINSTANCE main_instance;
	WNDCLASSEX	winclassex;
/****************************************
* ����:ע��һ�������࣬��װwindows�еĳ�ʼ���������ע�����
* ����:
	lpfnWndProc: ���ڴ�����
	hinstance:����ʵ��
	lpszClassName:��������
	style:���ڷ��
*****************************************/
	BOOLEAN RegisterWndClassEx(WNDPROC lpfnWndProc,HINSTANCE &hinstance,
							LPCTSTR lpszClassName,
							UINT style=CS_HREDRAW|CS_VREDRAW,
							char* iconCursor=IDC_ARROW,
							char* icon=IDI_APPLICATION,
							char* iconSm=IDI_APPLICATION,
							LPCTSTR lpszmenuname=NULL,
							int  brushID = BLACK_BRUSH);	
/****************************************
* ����:����һ�������࣬
* ����:
*****************************************/
	BOOLEAN CreateWndClassEx(LPCTSTR lpWindowName="Application",int windowed=0,int x=0,int y=0,
							int nWidth=600,int nHeight=480,HMENU hMenu = NULL,DWORD dwExStyle=NULL,LPVOID lpParam=NULL);

};




#endif