#define WIN32_LEAN_AND_MEAN	//不包括 MFC相关的

#include "..\PLGLoader.h"
#include "..\RenderPipeline3DV1.h"
#include "..\HCLib\HCDDrawLib.h"
#include "..\Draw3DV1.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>


void TestRender3D();
void TestPlgLoader();
void TestRenderPipeline();



int main()
{
#ifndef TO_FILE_HBD
	freopen("d:\\test2_out.txt", "w", stdout);
	freopen("d:\\test2_err.txt", "w", stderr);
#endif
	
	TestPlgLoader();
	TestRender3D();
	TestRenderPipeline();
	return 0;
}
float sin_look[361];
float cos_look[361];

void TestRenderPipeline()
{

	CRenderPipeline3DV1 render;
	CMath3D math3d;
	CPoint4D poly_pos;
	poly_pos.SetXYZW(0,0,100,1);
	CRenderList4DV1 rendlist;
	CPolyF4DV1 poly;
	CPoint4D cam_pos,cam_dir;
	CVector4D vscale,vpos,vrot;
	cam_pos.SetXYZW(0,0,-100,1);
	cam_dir.SetXYZW(0,0,0,1);

	vscale.SetXYZW(.5,.5,.5,1);
	vpos.SetXYZW(0,0,0,1);
	vrot.SetXYZW(0,0,0,1);
	
	poly_pos.SetXYZW(0,0,100,1);
	
	math3d.Build_Sin_Cos_Tables(cos_look,sin_look);
	
	poly.m_state= POLY4DV1_STATE_ACTIVE;
	poly.m_color= 2000;
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

	
	rendlist.Reset_RenderListV1();
	rendlist.Insert_PolyF4DV1(poly);
	assert(poly.m_vlist[0].GetX() == 0.0f);assert(poly.m_vlist[0].GetY() == 50.0f);assert(poly.m_vlist[0].GetZ() == 0.0f);
	assert(poly.m_vlist[1].GetX() == 50.0f);assert(poly.m_vlist[1].GetY() == -50.0f);assert(poly.m_vlist[1].GetZ() == 0.0f);
	assert(poly.m_vlist[2].GetX() == -50.0f);assert(poly.m_vlist[2].GetY() == -50.0f);assert(poly.m_vlist[2].GetZ() == 0.0f);
	
	CDraw3DV1 draw3d;CRenderPipeline3DV1 rpl3d;
	float ang_y = 30;
	CMatrix44 mrot;
	rpl3d.Build_XYZ_Rotation_Matrix44(0,ang_y,0,mrot);


	
	rpl3d.Transform_Renderlist(rendlist,mrot,TRANSFORM_LOCAL_ONLY);

	rpl3d.Model_To_World_Renderlist(rendlist,poly_pos);	 
	CCamera4DV1 cam;

	cam_pos.SetXYZW(0,0,-100,1);
	cam_dir.SetXYZW(0,0,0,1);
	cam.Init(CAM_MODEL_EULER,
		cam_pos,cam_dir,
		NULL,50.0,500.0,90.0,400,400);
	
	cam.Build_Cam4DV1_Matrix_Euler(CAM_ROT_SEQ_ZYX);
	

	rpl3d.World_To_Camera_RenderList(rendlist,cam);

	
		
	

	rpl3d.Camera_To_Perspective_Screen_Renderlist(rendlist,cam);

	UCHAR*dest_buffer = (UCHAR*)malloc(10*sizeof(UCHAR));
//	draw3d.Draw_RenderList4DV1_Wire16(rendlist,dest_buffer,1);

	render.Model_To_World_Renderlist(rendlist,poly_pos);
	
	

}
void TestRender3D()
{

	CRenderPipeline3DV1 render;
	CRenderList4DV1 rend_list;
	CCamera4DV1 camera;
	CMatrix44 mt;
	int coord_select = TRANSFORM_LOCAL_TO_TRANS;
	render.Transform_Renderlist(rend_list,mt,coord_select);
	CVector<4> cv;
	cv.Zero();
	assert(cv.GetW() == 1.0f);
	camera.m_pos.SetX(100);	camera.m_pos.SetY(200);	camera.m_pos.SetZ(300);
	camera.m_dir.SetX(-45);	camera.m_dir.SetY(0);	camera.m_dir.SetZ(0);
	
	HCMath::Build_Sin_Cos_Tables(sin_look,cos_look);
	//CMath3D::sin_look[0];
//	CMath3D::Fast_Cos(-45);
	camera.Build_Cam4DV1_Matrix_Euler(CAM_ROT_SEQ_ZYX);
	CCamera4DV1 cam;
	CPoint4D cam_pos;
	CVector4D cam_dir;
	cam.Init(CAM_MODEL_EULER,
		cam_pos,cam_dir,
		NULL,50.0,500.0,90.0,SCREEN_WIDTH,SCREEN_HEIGHT);
	


}

void TestPlgLoader()
{
	
	CPlgLoader plgLoader;
	CObject4DV1 obj;
	CVector4D scale,pos,rot;
	char filename[64] ;
	strcpy(filename,"../asset/cube1.plg");
	obj.m_attr = 14;
	strcpy(obj.m_name,"cube");
	scale.SetX(1.0f);scale.SetY(1.0f);scale.SetZ(1.0f);scale.SetW(1.0f);
	pos.SetX(0.0f);pos.SetY(0.0f);pos.SetZ(0.0f);pos.SetW(1.0f);
	rot.SetX(0.0f);rot.SetY(0.0f);rot.SetZ(0.0f);rot.SetW(1.0f);
	
	plgLoader.Load_Object4DV1_PLG(obj,filename,scale,pos,rot,DD_PIXEL_FORMAT565);
	assert(obj.m_attr == 0);
	
}