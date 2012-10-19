#define WIN32_LEAN_AND_MEAN	//不包括 MFC相关的

#pragma comment (lib,"dxguid.lib")//GUID的定义包括

#include "HCLib\HCDXBuilder.h"
#include "HCLib\HCInput.h"
#include "HCReturnType.h"
#ifdef SCREEN_BPP
#undef SCREEN_BPP
#endif
#define SCREEN_BPP		16
#define APP_CLASS_NAME	"hebiduClass"

#define WINDOW_TITLE	"2D模板整理V1" //窗口的标题

//functions declaration
HBDRESULT App_Init(void *params = NULL,int num_params=0);
HBDRESULT App_Main(void *params = NULL,int num_params=0);
HBDRESULT App_Shutdown(void *params = NULL,int num_params=0);

//variable 
HCDXBuilder hcdxBuilder;
HCInput		hcInput;
HCLog		hcLog;

LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wparam,LPARAM lparam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}break;
	default:break;
	}
	return DefWindowProc(hwnd,uMsg,wparam,lparam);
}

int WINAPI WinMain(HINSTANCE hinstance , HINSTANCE hpreinstance,
					LPSTR lpcmdline,int ncmdshow)
{
	hcLog.Open_Error_File();
	if(hcdxBuilder.RegisterWndClassEx(WindowProc,hinstance,APP_CLASS_NAME) == FALSE)
	{
		hcLog.Write_Error("\n WinMain::hcdxBuilder.RegisterWndClassEx Failed");
		return 0;
	}
	if(hcdxBuilder.CreateWndClassEx(WINDOW_TITLE,SCREEN_WINDOWED,0,0,SCREEN_WIDTH,SCREEN_HEIGHT) == FALSE)
	{
		hcLog.Write_Error("\n WinMain::hcdxBuilder.CreateWndClassEx Failed");
		return 0;
	}
	
	if(App_Init() != HBD_OK)
	{
		
		hcLog.Write_Error("\n WinMain::hcdxBuilder Failed");
		return 0;
	}
	MSG msg;
	
	while(TRUE)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}
		App_Main();

	}
	App_Shutdown();
	hcLog.Close_Error_File();
	return msg.wParam;
}

HBDRESULT App_Init(void *params ,int num_params)
{
	if(FAILED(hcdxBuilder.DDraw_Init(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SCREEN_WINDOWED)))
	{
		hcLog.Write_Error("\n App_Init FAILED!");
		return HBD_FAILED;
	}
	if(FAILED(hcInput.DInput_Init(hcdxBuilder.main_instance)))
	{
		hcLog.Write_Error("\n App_Init hcInput.DInput_Init FAILED!");
		return HBD_FAILED;
	}
	if(FAILED(hcInput.DInput_Init_Keyboard(hcdxBuilder.main_hwnd)))
	{
		hcLog.Write_Error("\n App_Init hcInput.DInput_Init_Keyboard Failed!");
		return HBD_FAILED;
	}
	RECT screen_rect =
	{0,0,hcdxBuilder.screen_width-1,hcdxBuilder.screen_height-1};
	hcdxBuilder.lpddclipper = hcdxBuilder.DDraw_Attach_Clipper(hcdxBuilder.lpddsback,1,&screen_rect);
	return HBD_OK;
}
HBDRESULT App_Main(void *params ,int num_params)
{
	static bool iClosed = false;
	if(iClosed)
	{
		return HBD_OK;
	}
	if(hcInput.DInput_Read_Keyboard() != 1)
	{
		hcLog.Write_Error("\n App_Main::hcInput.DInput_Read_Keyboard FAILED!");
		return HBD_FAILED;
	}
	if(KEY_DOWN(VK_ESCAPE) || hcInput.keyboard_state[DIK_ESCAPE])
	{
		PostMessage(hcdxBuilder.main_hwnd,WM_DESTROY,0,0);
		hcLog.Write_Error("\n App_Main::PostMessage WM_DESTROY!");
		iClosed = true;
	}
	return HBD_OK;
}
HBDRESULT App_Shutdown(void *params,int num_params)
{
	hcdxBuilder.DDraw_Shutdown();
	hcInput.DInput_Release_Keyboard();
	hcInput.DInput_Shutdown();
	return HBD_OK;
}