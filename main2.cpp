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
#include "Material.h"
#include "Light.h"
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
#define WINDOW_WIDTH        400
#define WINDOW_HEIGHT       400
#define SCREEN_BPP16		16
#define APP_CLASS_NAME	"hebiduClass"

#define WINDOW_TITLE	"3D线框引擎测试" //窗口的标题

//functions declaration
int App_Init(void *params = NULL,int num_params=0);
int App_Main(void *params = NULL,int num_params=0);
int App_Shutdown(void *params = NULL,int num_params=0);
int Input_Process();

//variable 
HCDXBuilder hcdxBuilder;
HCInput		hcInput;
HCLog		hcLog;
CMath3D		math3d;
float sin_look[361];
float cos_look[361];
bool bClosed = false;

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


CPoint4D poly_pos;

//用于立方体
CPlgLoader plgloader;
CObject4DV1 sphere;
float ang_x = 0,ang_y = 0,ang_z = 0;
int vx=0,vy=0;
void DrawRect(int x0,int y0,int x1,int y1,int color,UCHAR* video_buffer,int lpitch)
{
	
	draw3d.Draw_Line16(x0,y0,x1,y0,color,video_buffer,lpitch);
	draw3d.Draw_Line16(x1,y0,x1,y1,color,video_buffer,lpitch);
	draw3d.Draw_Line16(x1,y1,x0,y1,color,video_buffer,lpitch);
	draw3d.Draw_Line16(x0,y1,x0,y0,color,video_buffer,lpitch);

}
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
	if(hcdxBuilder.CreateWndClassEx(WINDOW_TITLE,SCREEN_WINDOWED,0,0,WINDOW_WIDTH,WINDOW_HEIGHT) == FALSE)
	{
		hcLog.Write_Error("\n WinMain::hcdxBuilder.CreateWndClassEx Failed");
		return 0;
	}
	
	if(App_Init() != 1)
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
char buffer[120];
int App_Init(void *params ,int num_params)
{
	math3d.Build_Sin_Cos_Tables(cos_look,sin_look);
	if(FAILED(hcdxBuilder.DDraw_Init(WINDOW_WIDTH,WINDOW_HEIGHT,SCREEN_BPP16,SCREEN_WINDOWED)))
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
	{0,0,hcdxBuilder.screen_width - 1,hcdxBuilder.screen_height-1};
	hcdxBuilder.lpddclipper = hcdxBuilder.DDraw_Attach_Clipper(hcdxBuilder.lpddsback,1,&screen_rect);
	draw3d.SetClipRect(60,60,350,350);
	
	
	cam_pos.SetXYZW(0,0,0,1);
	cam_dir.SetXYZW(0,0,0,1);
	vscale.SetXYZW(5,5,5,1);
	vpos.SetXYZW(0,0,0,1);
	vrot.SetXYZW(0,0,0,1);
	
	

	
	
	CPoint4D origin;
	origin.SetXYZW(0,0,0,1);
	cam.Init(CAM_MODEL_EULER,
		cam_pos,cam_dir,
		NULL,50.0,500.0,90.0,WINDOW_WIDTH,WINDOW_HEIGHT);
	
	
	plgloader.Load_Object4DV1_PLG(sphere,"asset\\cube2.plg",vscale,vpos,vrot,hcdxBuilder.dd_pixel_format);
	sphere.m_world_pos.SetZ(100);
	return 1;
}
int App_Main(void *params ,int num_params)
{

	//	memset(buffer,0,sizeof(buffer));
	
	if(bClosed)
	{
		return 1;
	}
	
	static CMatrix44 mrot;//旋转


	Start_Clock();
	
	hcdxBuilder.DDraw_Fill_Surface(hcdxBuilder.lpddsback,0);
	if(hcInput.DInput_Read_Keyboard() != 1)
	{
		hcLog.Write_Error("\n App_Main::hcInput.DInput_Read_Keyboard FAILED!");
		return 0;
	}
	vx = vy = 0;
	Input_Process();
		
	sphere.Reset_Object();
	rpl3d.Build_XYZ_Rotation_Matrix44(ang_x,ang_y,ang_z,mrot);
	mrot.M[3][0] = vx;
	mrot.M[3][1] = vy;
	rpl3d.Transform_Object(sphere,mrot,TRANSFORM_LOCAL_ONLY,1);
	
	rpl3d.Model_To_World_Object(sphere);

	rpl3d.Remove_Backfaces_Object(sphere,cam);

	cam.Build_Cam4DV1_Matrix_Euler(CAM_ROT_SEQ_ZYX);
	rpl3d.World_To_Camera_Object(sphere,cam);
	
	rpl3d.Camera_To_Perspective_Object(sphere,cam);
	rpl3d.Perspective_To_Screen_Object(sphere,cam);


	//绘制
	hcdxBuilder.DDraw_Lock_Back_Surface();
	
	draw3d.Draw_Text_GDI(buffer,10,30,hcdxBuilder.RGB16Bit(0,255,255),hcdxBuilder.lpddsback);
	

	
	draw3d.Draw_Object_Wire16(sphere,hcdxBuilder.Get_Back_Buffer(),hcdxBuilder.Get_Back_lPitch());

//draw3d.Draw_Object4DV1_Solid16(sphere,hcdxBuilder.Get_Back_Buffer(),hcdxBuilder.Get_Back_lPitch());
	
	draw3d.Draw_Text_GDI("3D立方体测试 方向键控制旋转角度",10,10,hcdxBuilder.RGB16Bit(0,255,255),hcdxBuilder.lpddsback);

	DrawRect(60,60,350,350,hcdxBuilder.RGB16Bit(0,255,255),hcdxBuilder.Get_Back_Buffer(),hcdxBuilder.Get_Back_lPitch());
	
	
	hcdxBuilder.DDraw_Unlock_Back_Surface();
	
	hcdxBuilder.DDraw_Flip();
	
	Wait_Clock(30);
	
	
	
	
	return 1;
}
int App_Shutdown(void *params,int num_params)
{
	hcdxBuilder.DDraw_Shutdown();
	hcInput.DInput_Release_Keyboard();
	hcInput.DInput_Shutdown();
	return 1;
}

int Input_Process()
{
	static CMatrix44 mt;
	mt.Identify();
	if(KEY_DOWN(VK_ESCAPE) || hcInput.keyboard_state[DIK_ESCAPE])
	{
		PostMessage(hcdxBuilder.main_hwnd,WM_DESTROY,0,0);
		hcLog.Write_Error("\n App_Main::PostMessage WM_DESTROY!");
		bClosed = true;
	}
	if(KEY_DOWN(VK_RIGHT) || hcInput.keyboard_state[DIK_RIGHT])
	{
		ang_y = 1;
	}
	if(KEY_DOWN(VK_LEFT) || hcInput.keyboard_state[DIK_LEFT])
	{
		ang_y = -1;		
	}
	if(KEY_DOWN(VK_UP) || hcInput.keyboard_state[DIK_UP])
	{
		ang_x = 1;		
	}
	if(KEY_DOWN(VK_DOWN) || hcInput.keyboard_state[DIK_DOWN])
	{
		ang_x = -1;		
	}
	if(KEY_DOWN('W') || hcInput.keyboard_state[DIK_W])
	{
		vy = 1;		
	}
	if(KEY_DOWN('S') || hcInput.keyboard_state[DIK_S])
	{
		vy = -1;		
	}
	if(KEY_DOWN('A') || hcInput.keyboard_state[DIK_A])
	{
		vx = -1;	
	}
	if(KEY_DOWN('D') || hcInput.keyboard_state[DIK_D])
	{
		vx = 1;		
	}
	return 1;
}