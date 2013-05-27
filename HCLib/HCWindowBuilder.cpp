#include "HCWindowBuilder.h"

BOOLEAN HCWindowBuilder::RegisterWndClassEx(WNDPROC lpfnWndProc,HINSTANCE &hinstance,
							LPCTSTR lpszClassName="HC_CLASS_NAME",
							UINT style,
							char* iconCursor,
							char* icon,
							char* iconSm,
							LPCTSTR lpszmenuname,
							int  brushID)
{
		
	
	hclog.Open_Error_File();
	this->winclassex.cbSize = sizeof(WNDCLASSEX);
	this->winclassex.lpfnWndProc = lpfnWndProc;
	this->winclassex.hInstance = hinstance;
	this->winclassex.lpszClassName = lpszClassName;
	this->winclassex.style = style;
	this->winclassex.hCursor = LoadCursor(hinstance,iconCursor);
	this->winclassex.hIcon = LoadIcon(hinstance,(icon));
	this->winclassex.hIconSm = LoadIcon(hinstance,(iconSm));
	this->winclassex.lpszMenuName = lpszmenuname;
	this->winclassex.hbrBackground = (HBRUSH)GetStockObject(brushID);
	this->winclassex.cbWndExtra = 0;
	this->winclassex.cbClsExtra = 0;

	if(!RegisterClassEx(&this->winclassex))
	{
		hclog.Write_Error("\n HCWindowBuilder::RegisterWndClassEx:RegisterClassEx Failed!");
		return FALSE;
	}
	this->main_instance = hinstance;
	hclog.Close_Error_File();
	return TRUE;
}
BOOLEAN HCWindowBuilder::CreateWndClassEx(LPCTSTR lpWindowName,int windowed,int x,int y,int nWidth,int nHeight,HMENU hMenu,DWORD dwExStyle,LPVOID lpParam)
{
	
	hclog.Open_Error_File();
	if(!this->main_instance || this->main_instance == NULL)
	{
		hclog.Write_Error("\n HCWindowBuilder::RegisterWndClassEx:main_instance is NULL!");
		return FALSE;
	}
	unsigned long dwStyle = (windowed ? (WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION) : (WS_POPUP | WS_VISIBLE));
	if(!(this->main_hwnd = CreateWindowEx(dwExStyle,this->winclassex.lpszClassName,lpWindowName,dwStyle,x,y,nWidth,nHeight,NULL,hMenu,this->main_instance,lpParam)))
	{
		hclog.Write_Error("\n HCWindowBuilder::RegisterWndClassEx:CreateWindowEx Failed!");
		return FALSE;
	}

	this->screen_width = nWidth;
	this->screen_height = nHeight;
	
	if (windowed)
	{
		// now resize the window, so the client area is the actual size requested
		// since there may be borders and controls if this is going to be a windowed app
		// if the app is not windowed then it won't matter
		//现在重新测量窗口的大小，因此客户区域被请求的是实际大小
		//尽管它可能有边框和控件如果是窗口化程序的话
		//如果不是窗口化就无关紧要
		RECT window_rect = {0,0,nWidth-1,nHeight-1};
		// make the call to adjust window_rect
		AdjustWindowRectEx(&window_rect,
			GetWindowStyle(this->main_hwnd),
			GetMenu(this->main_hwnd) != NULL,
			GetWindowExStyle(this->main_hwnd));
		
		// save the global client offsets, they are needed in DDraw_Flip()
		this->window_client_x0 = -window_rect.left;
		this->window_client_y0 = -window_rect.top;
		
		// now resize the window with a call to MoveWindow()
		MoveWindow(this->main_hwnd,
			0, // x position
			0, // y position
			window_rect.right - window_rect.left, // width
			window_rect.bottom - window_rect.top, // height
			TRUE);
		
		// show the window, so there's no garbage on first render
		//显示窗口，这样第一次渲染时就不会有垃圾，指渲染的区域已经调整为正常，
		//不正常，渲染区域可能大了或小了
		ShowWindow(this->main_hwnd, SW_SHOW);
	} // end if windowed

	return TRUE;
}
