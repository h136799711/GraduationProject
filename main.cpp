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
#define WINDOW_WIDTH        400
#define WINDOW_HEIGHT       400
#define SCREEN_BPP16		16
#define APP_CLASS_NAME	"hebiduClass"

#define WINDOW_TITLE	"3D线框引擎测试" //窗口的标题

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
CVector4D cam_dir;
CVector4D vscale,vpos,vrot;//
CPolyF4DV1  poly;
CPoint4D poly_pos;

CPlgLoader plgloader;

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
char buffer[120];
int App_Init(void *params ,int num_params)
{
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
	draw3d.SetClipRect(0,0,250,250);
	
	
	cam_pos.SetXYZW(0,0,-100,1);
	cam_dir.SetXYZW(0,0,0,1);
	vscale.SetXYZW(0.5,0.5,0.5,1);
	vpos.SetXYZW(0,0,0,1);
	vrot.SetXYZW(0,0,0,1);
	
	poly_pos.SetXYZW(0,0,100,1);
	
	math3d.Build_Sin_Cos_Tables(cos_look,sin_look);
	
	poly.m_state= POLY4DV1_STATE_ACTIVE;
	poly.m_attr   =  0; 
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
	cam.Init(CAM_MODEL_EULER,
		cam_pos,cam_dir,
		NULL,50.0,500.0,90.0,WINDOW_WIDTH,WINDOW_HEIGHT);
	
	poly.m_next = poly.m_prev = NULL;
	
	return 1;
}
int App_Main(void *params ,int num_params)
{
	static bool bPause = false;
	//	memset(buffer,0,sizeof(buffer));
	
	if(bClosed)
	{
		return 1;
	}
	
	static CMatrix44 mrot;//旋转
	static float ang_y=0;//旋转角度
	
	int index;//loop variable
	
	//sprintf(buffer,"mrot : 2行 = %f %f %f %f \n 3行 = %f %f %f %f",mrot.M[2][0],mrot.M[2][1],mrot.M[2][2],mrot.M[2][3]
	//	,mrot.M[3][0],mrot.M[3][1],mrot.M[3][2],mrot.M[3][3]);
	Start_Clock();
	
	hcdxBuilder.DDraw_Fill_Surface(hcdxBuilder.lpddsback,0);
	if(hcInput.DInput_Read_Keyboard() != 1)
	{
		hcLog.Write_Error("\n App_Main::hcInput.DInput_Read_Keyboard FAILED!");
		return 0;
	}
	
	Input_Process();
	
	rendList.Reset_RenderListV1();
	rendList.Insert_PolyF4DV1(poly);
	
	//	 sprintf(buffer,"rendlist state: polys = %d,state = %d",rendList.m_polys,rendList.m_state);
	//	 sprintf(buffer,"rendlist poly1:color = %d,顶点0 = %f %f %f ",rendList.m_poly_data[0].m_color,rendList.m_poly_data[0].m_vlist[0].GetX(),rendList.m_poly_data[0].m_vlist[0].GetY(),rendList.m_poly_data[0].m_vlist[0].GetZ());
	// 	 sprintf(buffer,"rendlist poly1:color = %d,顶点1 = %f %f %f ",rendList.m_poly_data[0].m_color,rendList.m_poly_data[0].m_vlist[1].GetX(),rendList.m_poly_data[0].m_vlist[1].GetY(),rendList.m_poly_data[0].m_vlist[1].GetZ());
	//	 sprintf(buffer,"rendlist poly1:color = %d,顶点2 = %f %f %f ",rendList.m_poly_data[0].m_color,rendList.m_poly_data[0].m_vlist[2].GetX(),rendList.m_poly_data[0].m_vlist[2].GetY(),rendList.m_poly_data[0].m_vlist[2].GetZ());
	//	 sprintf(buffer,"rendlist poly1:color = %d,顶点0 = %f %f %f ",rendList.m_poly_ptrs[0]->m_color,rendList.m_poly_ptrs[0]->m_vlist[0].GetX(),rendList.m_poly_ptrs[0]->m_vlist[0].GetY(),rendList.m_poly_ptrs[0]->m_vlist[0].GetZ());
	
	
	rpl3d.Build_XYZ_Rotation_Matrix44(0,ang_y,0,mrot);
	//	sprintf(buffer,"mrot : 0行 = %f %f %f %f \n 1行 = %f %f %f %f",mrot.M[0][0],mrot.M[0][1],mrot.M[0][2],mrot.M[0][3]
	//	,mrot.M[1][0],mrot.M[1][1],mrot.M[1][2],mrot.M[1][3]);
	//	sprintf(buffer,"mrot : 2行 = %f %f %f %f \n 3行 = %f %f %f %f",mrot.M[2][0],mrot.M[2][1],mrot.M[2][2],mrot.M[2][3]
	//		,mrot.M[3][0],mrot.M[3][1],mrot.M[3][2],mrot.M[3][3]);
	

	
	//	sprintf(buffer,"第一个顶点 :  = %f %f %f  \n",rendList.m_poly_ptrs[0]->m_vlist[0].GetX(),rendList.m_poly_ptrs[0]->m_vlist[0].GetY(),rendList.m_poly_ptrs[0]->m_vlist[0].GetZ());
	
	//	CPoint4D rst ;
	//	math3d.Mat_Mul_4D_44(rendList.m_poly_data[0].m_vlist[0],mrot,rst);
	//	math3d.Mat_Mul_4D_44(rendList.m_poly_data[0].m_vlist[1],mrot,rst);
	//	math3d.Mat_Mul_4D_44(rendList.m_poly_data[0].m_vlist[2],mrot,rst);
	//	sprintf(buffer,"= %f %f %f ",rst.GetX(),rst.GetY(),rst.GetZ());
	
	
	rpl3d.Transform_Renderlist(rendList,mrot,TRANSFORM_LOCAL_ONLY);
	//	sprintf(buffer,"第一个顶点 :  = %f %f %f ",rendList.m_poly_ptrs[0]->m_vlist[0].GetX(),rendList.m_poly_ptrs[0]->m_vlist[0].GetY(),rendList.m_poly_ptrs[0]->m_vlist[0].GetZ());
	
//	CMatrix44 mt;
	
	//	rpl3d.Build_Model_To_Word_Matrix44(poly_pos,mt);		
	//	rpl3d.Transform_Renderlist(rendList,mt,TRANSFORM_LOCAL_TO_TRANS);
	
	rpl3d.Model_To_World_Renderlist(rendList,poly_pos);	 
	//	sprintf(buffer,"poly_pos:  = %f %f %f %f",poly_pos.GetX(),poly_pos.GetY(),poly_pos.GetZ(),poly_pos.GetW());	
	sprintf(buffer,"max x,y :  = %d  %d",hcdxBuilder.max_clip_x,hcdxBuilder.max_clip_y);
	
	sprintf(buffer,"width,height :  = %d  %d",hcdxBuilder.screen_width,hcdxBuilder.screen_height);
	

	
	//	sprintf(buffer,"第一个顶点 :  = %d  ",rendList.m_polys);
	//	sprintf(buffer,"属性 :  = %d  ",rendList.m_attr);
	//		CPolyF4DV1_PTR curr_poly = rendList.m_poly_ptrs[0];
	//		for(int vertex = 0;vertex < 3;vertex++)
	//		{
	//			curr_poly->m_tvlist[vertex] = curr_poly->m_vlist[vertex] + poly_pos;
	//		}
	//	CPolyF4DV1_PTR curr_poly = rendList.m_poly_ptrs[0];
	//	curr_poly->m_tvlist[1] = curr_poly->m_vlist[1] + poly_pos;
	
	//	sprintf(buffer,"第一个顶点 :  = %f %f %f ",rendList.m_poly_data[0].m_vlist[1].GetX(),rendList.m_poly_data[0].m_vlist[1].GetY(),rendList.m_poly_data[0].m_vlist[1].GetZ());
	
	//	sprintf(buffer,"第一个顶点 :  = %f %f %f ",rendList.m_poly_ptrs[0]->m_tvlist[1].GetX(),rendList.m_poly_ptrs[0]->m_tvlist[1].GetY(),rendList.m_poly_ptrs[0]->m_tvlist[1].GetZ());
	
	//根据欧拉模式下相机的3个旋转角度初始化相机的变换矩阵(旋转矩阵和平移矩阵合并)
	cam.Build_Cam4DV1_Matrix_Euler(CAM_ROT_SEQ_ZYX);
	
//	sprintf(buffer,"第1个顶点 :  = %f %f %f ",rendList.m_poly_ptrs[0]->m_tvlist[1].GetX(),rendList.m_poly_ptrs[0]->m_tvlist[1].GetY(),rendList.m_poly_ptrs[0]->m_tvlist[1].GetZ());
	//世界坐标到相机坐标变换
	rpl3d.World_To_Camera_RenderList(rendList,cam);
//	sprintf(&buffer[40],"第2个顶点 :  = %f %f %f ",rendList.m_poly_ptrs[0]->m_tvlist[1].GetX(),rendList.m_poly_ptrs[0]->m_tvlist[1].GetY(),rendList.m_poly_ptrs[0]->m_tvlist[1].GetZ());
	
	
	//	sprintf(buffer,"顶点 :  = %f %f %f ",rendList.m_poly_ptrs[0]->m_tvlist[1].GetX(),rendList.m_poly_ptrs[0]->m_tvlist[1].GetY(),rendList.m_poly_ptrs[0]->m_tvlist[1].GetZ());
	//	sprintf(buffer,"第2个顶点 :  = %f %f %f ",rendList.m_poly_ptrs[0]->m_tvlist[1].GetX(),rendList.m_poly_ptrs[0]->m_tvlist[1].GetY(),rendList.m_poly_ptrs[0]->m_tvlist[1].GetZ());
	
	//	rpl3d.Camera_To_Perspective_Renderlist(rendList,cam);
	//	rpl3d.Perspective_To_Screen_Rendlist(rendList,cam);
		
	
//	sprintf(buffer,":  = %f %f ",cam.m_viewport_width,cam.m_viewport_height);
	//相机直接到屏幕坐标的变换
	cam.m_viewplane_height =  cam.m_viewport_height;
	cam.m_viewplane_width =  cam.m_viewport_width;
	rpl3d.Camera_To_Perspective_Screen_Renderlist(rendList,cam);
	//	sprintf(buffer,"第2个顶点 :  = %f %f %f ",rendList.m_poly_ptrs[0]->m_tvlist[1].GetX(),rendList.m_poly_ptrs[0]->m_tvlist[1].GetY(),rendList.m_poly_ptrs[0]->m_tvlist[1].GetZ());
	
	//绘制
	hcdxBuilder.DDraw_Lock_Back_Surface();
	
	draw3d.Draw_Text_GDI(buffer,10,30,hcdxBuilder.RGB16Bit(0,255,255),hcdxBuilder.lpddsback);
	
	draw3d.Draw_Line16(2,2,250,2,hcdxBuilder.RGB16Bit(155,155,155),hcdxBuilder.Get_Back_Buffer(),hcdxBuilder.Get_Back_lPitch());
	draw3d.Draw_Line16(250,2,250,250,hcdxBuilder.RGB16Bit(155,155,155),hcdxBuilder.Get_Back_Buffer(),hcdxBuilder.Get_Back_lPitch());
	draw3d.Draw_Line16(250,250,2,250,hcdxBuilder.RGB16Bit(155,155,155),hcdxBuilder.Get_Back_Buffer(),hcdxBuilder.Get_Back_lPitch());
	draw3d.Draw_Line16(2,250,2,2,hcdxBuilder.RGB16Bit(155,155,155),hcdxBuilder.Get_Back_Buffer(),hcdxBuilder.Get_Back_lPitch());
	
	draw3d.Draw_RenderList4DV1_Wire16(rendList,hcdxBuilder.Get_Back_Buffer(),hcdxBuilder.Get_Back_lPitch());
	

	
	
	draw3d.Draw_Text_GDI("矩形用于2D裁剪 单个三角形",10,10,hcdxBuilder.RGB16Bit(0,255,255),hcdxBuilder.lpddsback);
	

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

	if(KEY_DOWN(VK_ESCAPE) || hcInput.keyboard_state[DIK_ESCAPE])
	{
		PostMessage(hcdxBuilder.main_hwnd,WM_DESTROY,0,0);
		hcLog.Write_Error("\n App_Main::PostMessage WM_DESTROY!");
		bClosed = true;
	}
	if(KEY_DOWN(VK_RIGHT) || hcInput.keyboard_state[DIK_RIGHT])
	{

	}
	return 1;
}