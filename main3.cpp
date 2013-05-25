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
#define WINDOW_WIDTH        800
#define WINDOW_HEIGHT       600
#define SCREEN_BPP16		16

// object defines
#define NUM_OBJECTS     16
#define OBJECT_SPACING  250

#define APP_CLASS_NAME	"hebiduClass"

#define WINDOW_TITLE	"3D线框引擎测试2" //窗口的标题

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
CPoint4D cam_target;
CVector4D cam_dir;
CVector4D vscale,vpos,vrot;//



//用于立方体
CPlgLoader plgloader;
CObject4DV1 tank;

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
	if(hcdxBuilder.RegisterWndClassEx(WindowProc,hinstance,APP_CLASS_NAME,CS_DBLCLKS | CS_OWNDC | 
                          CS_HREDRAW | CS_VREDRAW) == FALSE)
	{
		hcLog.Write_Error("\n WinMain::hcdxBuilder.RegisterWndClassEx Failed");
		return 0;
	}
	if(hcdxBuilder.CreateWndClassEx(WINDOW_TITLE,SCREEN_FULLSCREEN,0,0,WINDOW_WIDTH,WINDOW_HEIGHT) == FALSE)
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
char buffer[2048];
int App_Init(void *params ,int num_params)
{
	
	// seed random number generator
	srand(Start_Clock());
	math3d.Build_Sin_Cos_Tables(cos_look,sin_look);
	if(FAILED(hcdxBuilder.DDraw_Init(WINDOW_WIDTH,WINDOW_HEIGHT,SCREEN_BPP16,SCREEN_FULLSCREEN)))
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
	draw3d.SetClipRect(0,0,hcdxBuilder.screen_width- 1 ,hcdxBuilder.screen_height-1);

	
	cam_pos.SetXYZW(0,0,0,1);
	cam_target.SetXYZW(0,0,0,1);
	cam_dir.SetXYZW(0,0,0,1);


	vscale.SetXYZW(1,1,1,1);
	vpos.SetXYZW(0,0,0,1);
	vrot.SetXYZW(0,0,0,1);
	
	

	
	
	cam.Init(CAM_MODEL_UVN,
		cam_pos,cam_dir,NULL,
		50.0,8000.0,90.0,WINDOW_WIDTH,WINDOW_HEIGHT);
	
	
	plgloader.Load_Object4DV1_PLG(tank,"asset\\tank1.plg",vscale,vpos,vrot,hcdxBuilder.dd_pixel_format);

//	plgloader.Load_Object4DV1_PLG(tank,"asset\\cube1.plg",vscale,vpos,vrot,hcdxBuilder.dd_pixel_format);
	
	tank.m_world_pos.SetX(0);
	tank.m_world_pos.SetY(0);
	tank.m_world_pos.SetZ(0);
	return 1;
}
int App_Shutdown(void *params,int num_params)
{
	hcdxBuilder.DDraw_Shutdown();
	hcInput.DInput_Release_Keyboard();
	hcInput.DInput_Shutdown();
	return 1;
}

float view_angle = 0; 

int App_Main(void *params ,int num_params)
{

	//	memset(buffer,0,sizeof(buffer));
	

	
	static CMatrix44 mrot;//旋转

	static float ang_x = 0,ang_y = 5,ang_z = 0;

	// 用于旋转相机
	static float camera_distance = 1750;

	char work_string[256]; // temp string
	Start_Clock();
	

	hcdxBuilder.DDraw_Fill_Surface(hcdxBuilder.lpddsback,0);

	if(hcInput.DInput_Read_Keyboard() != 1)
	{
		hcLog.Write_Error("\n App_Main::hcInput.DInput_Read_Keyboard FAILED!");
		return 0;
	}

	
	rendList.Reset_RenderListV1();
	ang_x = 0;
	ang_y = 1;
	ang_z = 0;

		
	//tank.Reset_Object();

	rpl3d.Build_XYZ_Rotation_Matrix44(ang_x,ang_y,ang_z,mrot);

	rpl3d.Transform_Object(tank,mrot,TRANSFORM_LOCAL_ONLY,1);
	
	
	cam.m_pos.SetX(camera_distance*math3d.Fast_Cos(view_angle));
	cam.m_pos.SetY(camera_distance*math3d.Fast_Sin(view_angle));
	cam.m_pos.SetZ(2*camera_distance*math3d.Fast_Sin(view_angle));

	
//
	// advance angle to circle around
	if ((view_angle+=1)>=360)
		view_angle = 0;


	cam.Buid_Cam4DV1_Matrix_UVN(UVN_MODE_SIMPLE);

	strcpy(buffer,"Objects Culled: ");



	for (int x=-NUM_OBJECTS/2; x < NUM_OBJECTS/2; x++)
	
	 for (int z=-NUM_OBJECTS/2; z < NUM_OBJECTS/2; z++)
	 {
   
		tank.Reset_Object();
		// set position of object
		tank.m_world_pos.SetX( x*OBJECT_SPACING+OBJECT_SPACING/2);
		tank.m_world_pos.SetY(0);
		tank.m_world_pos.SetZ(z*OBJECT_SPACING+OBJECT_SPACING/2);



		if (!rpl3d.Cull_Object(tank, cam, CULL_OBJECT_XYZ_PLANE))
       {
		
			// 未被裁剪就可以渲染，
			
			rpl3d.Model_To_World_Object(tank,2);
			// insert the object into render list
			rendList.Insert_Object(tank,0);
       } // end if
		else
       {
			
       sprintf(work_string, "[%d, %d] ", x,z);
       strcat(buffer, work_string);
       }
	 
    } // end for


	draw3d.Draw_Text_GDI(buffer, 0, WINDOW_HEIGHT-20, RGB(0,255,0), hcdxBuilder.lpddsback);
	
	rpl3d.Remove_Backfaces_Renderlist(rendList,cam);

	rpl3d.World_To_Camera_RenderList(rendList,cam);
	
	
	rpl3d.Camera_To_Perspective_Renderlist(rendList,cam);

	rpl3d.Perspective_To_Screen_Rendlist(rendList,cam);

	draw3d.Draw_Text_GDI("Press ESC to exit.", 0, 0, RGB(0,255,0), hcdxBuilder.lpddsback);

	

	//绘制
	hcdxBuilder.DDraw_Lock_Back_Surface();
	
	draw3d.Draw_RenderList4DV1_Wire16(rendList,hcdxBuilder.Get_Back_Buffer(),hcdxBuilder.Get_Back_lPitch());
	
	hcdxBuilder.DDraw_Unlock_Back_Surface();
	
	hcdxBuilder.DDraw_Flip();
	
	Wait_Clock(30);
	
	


	if (KEY_DOWN(VK_ESCAPE) || hcInput.keyboard_state[DIK_ESCAPE])
    {
		PostMessage(hcdxBuilder.main_hwnd, WM_DESTROY,0,0);

    } 

	
	
	return 1;
}
