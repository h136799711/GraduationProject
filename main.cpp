#define WIN32_LEAN_AND_MEAN	//不包括 MFC相关的

#pragma comment (lib,"dxguid.lib")//GUID的定义包括

#include "HCLib\HCDXBuilder.h"
#include "HCLib\HCInput.h"
#include "HCLib\HCMacros.h"
#include "HCLib\HCUtility.h"
#include "ObjectDef.h"
#include "PLGLoader.h"
#include "RenderPipeline3DV1.h"
#include "Draw3DV1.h"
#include "Math3D.h"
/*******************************************************

【单个3D三角形的渲染】
	1.加载的物体: 手工创建一个多边形
	2.相机类型: 欧拉相机，位置固定，视野为90度，视平面是归一化的
	3.投影类型: 先进行透视变换，然后进行视口变换
	4.3D流水线使用的计算方法: 所以方法都使用手工计算方法，不基于矩阵
	5.背面消除:没有执行
	6.物体剔除: 没有执行
	7.渲染方式: 包含单个多边形的渲染列表


*********************************************************/

#define SCREEN_BPP16		16
#define APP_CLASS_NAME	"hebiduClass"

#define WINDOW_TITLE	"2D模板整理V1" //窗口的标题

//functions declaration
int App_Init(void *params = NULL,int num_params=0);
int App_Main(void *params = NULL,int num_params=0);
int App_Shutdown(void *params = NULL,int num_params=0);

//variable 
HCDXBuilder hcdxBuilder;
HCInput		hcInput;
HCLog		hcLog;
CMath3D		math3d;
float sin_look[361];
float cos_look[361];

/*********************************************************************
//测试数据
//
*///////////////////////////////////////////////////////////////////

//3D必须的类
CCamera4DV1 cam;
CRenderList4DV1 rendList;
CDraw3DV1		draw3d;
CRenderPipeline3DV1		rpl3d;
//初始化量
CPoint4D cam_pos;
CVector4D cam_dir;
CVector4D vscale,vpos,vrot;//
CPolyF4DV1  poly;
CPoint4D poly_pos;


///////////////////////////////////////////////////////////////////////////
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
	
	if(App_Init() != 1)
	{
		
		hcLog.Write_Error("\n WinMain::hcdxBuilder Failed");
		return 0;
	}
	math3d.Build_Sin_Cos_Tables(cos_look,sin_look);
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

int App_Init(void *params ,int num_params)
{
	if(FAILED(hcdxBuilder.DDraw_Init(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP16,SCREEN_WINDOWED)))
	{
		hcLog.Write_Error("\n App_Init FAILED!");
		return 0;
	}
	if(FAILED(hcInput.DInput_Init(hcdxBuilder.main_instance)))
	{
		hcLog.Write_Error("\n App_Init hcInput.DInput_Init FAILED!");
		return 0;
	}
	if(FAILED(hcInput.DInput_Init_Keyboard(hcdxBuilder.main_hwnd)))
	{
		hcLog.Write_Error("\n App_Init hcInput.DInput_Init_Keyboard Failed!");
		return 0;
	}
	RECT screen_rect =
	{0,0,hcdxBuilder.screen_width-1,hcdxBuilder.screen_height-1};
	hcdxBuilder.lpddclipper = hcdxBuilder.DDraw_Attach_Clipper(hcdxBuilder.lpddsback,1,&screen_rect);


		
	cam_pos.SetXYZW(0,0,-100,1);
	cam_dir.SetXYZW(0,0,0,1);
	vscale.SetXYZW(.5,.5,.5,1);
	vpos.SetXYZW(0,0,0,1);
	vrot.SetXYZW(0,0,0,1);
	
	poly_pos.SetXYZW(0,0,100,1);

	math3d.Build_Sin_Cos_Tables(cos_look,sin_look);

	poly.m_state= POLY4DV1_STATE_ACTIVE;
	poly.m_color= hcdxBuilder.RGB16Bit(0,255,0);
	poly.m_vlist[0].SetX(0);
	poly.m_vlist[0].SetY(50);
	poly.m_vlist[0].SetZ(0);
	poly.m_vlist[0].SetW(1);
	
	
	poly.m_vlist[1].SetX(50);
	poly.m_vlist[1].SetY(-50);
	poly.m_vlist[1].SetZ(0);
	poly.m_vlist[1].SetW(1);
	
	poly.m_vlist[2].SetX(-50);
	poly.m_vlist[2].SetY(-50);
	poly.m_vlist[2].SetZ(0);
	poly.m_vlist[2].SetW(1);
	
	
	CPoint4D origin;
	origin.SetXYZW(0,0,0,1);
	cam.Init(CAM_MODEL_EULER,cam_pos,cam_dir,&origin,50.0,500.0,90,SCREEN_WIDTH,SCREEN_HEIGHT);
	
	return 1;
}
int App_Main(void *params ,int num_params)
{
	static bool iClosed = false;
	if(iClosed)
	{
		return 1;
	}
	
	static CMatrix44 mrot;//旋转
	static float ang_y=0;//旋转角度

	int index;//loop variable

	Start_Clock();

	hcdxBuilder.DDraw_Fill_Surface(hcdxBuilder.lpddsback,0);
	if(hcInput.DInput_Read_Keyboard() != 1)
	{
		hcLog.Write_Error("\n App_Main::hcInput.DInput_Read_Keyboard FAILED!");
		return 0;
	}

     rendList.Reset_RenderListV1();
	 rendList.Insert_PolyF4DV1(poly);
	 
	 rpl3d.Build_XYZ_Rotation_Matrix44(0,ang_y,0,mrot);

	 if(++ang_y >= 360.0) ang_y = 0;

	 rpl3d.Transform_Renderlist(rendList,mrot,TRANSFORM_LOCAL_ONLY);

	 rpl3d.Model_To_World_Renderlist(rendList,poly_pos);

	 cam.Build_Cam4DV1_Matrix_Euler(CAM_ROT_SEQ_ZYX);

	 rpl3d.World_To_Camera_RenderList(rendList,cam);

	 rpl3d.Camera_To_Perspective_Renderlist(rendList,cam);

	 rpl3d.Perspective_To_Screen_Rendlist(rendList,cam);

	 hcdxBuilder.DDraw_Lock_Back_Surface();
	
	 draw3d.Draw_RenderList4DV1_Wire16(rendList,hcdxBuilder.Get_Back_Buffer(),hcdxBuilder.Get_Back_lPitch());

	 hcdxBuilder.DDraw_Unlock_Back_Surface();

	 hcdxBuilder.DDraw_Flip();

	 Wait_Clock(30);

	

	
	if(KEY_DOWN(VK_ESCAPE) || hcInput.keyboard_state[DIK_ESCAPE])
	{
		PostMessage(hcdxBuilder.main_hwnd,WM_DESTROY,0,0);
		hcLog.Write_Error("\n App_Main::PostMessage WM_DESTROY!");
		iClosed = true;
	}



	return 1;
}
int App_Shutdown(void *params,int num_params)
{
	hcdxBuilder.DDraw_Shutdown();
	hcInput.DInput_Release_Keyboard();
	hcInput.DInput_Shutdown();
	return 1;
}