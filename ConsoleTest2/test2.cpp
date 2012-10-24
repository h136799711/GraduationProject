#include "..\PLGLoader.h"
#include "..\Render3D.h"

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>


void TestRender3D();
void TestPlgLoader();


int main()
{
#ifndef TO_FILE_HBD
	freopen("G:\\test2_out.txt", "w", stdout);
	freopen("G:\\test2_err.txt", "w", stderr);
#endif
	
	TestPlgLoader();
	TestRender3D();
	return 0;
}
float sin_look[361];
float cos_look[361];

void TestRender3D()
{

	CRender3DV1 render;
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
		

}

void TestPlgLoader()
{
	
	CPlgLoader plgLoader;
	CObject4DV1 obj;
	CVector4D scale,pos,rot;
	char filename[64] ;
	strcpy(filename,"cube1.plg");
	obj.m_attr = 14;
	strcpy(obj.m_name,"cube");
	scale.SetX(1.0f);scale.SetY(1.0f);scale.SetZ(1.0f);scale.SetW(1.0f);
	pos.SetX(0.0f);pos.SetY(0.0f);pos.SetZ(0.0f);pos.SetW(1.0f);
	rot.SetX(0.0f);rot.SetY(0.0f);rot.SetZ(0.0f);rot.SetW(1.0f);
	
	plgLoader.Load_Object4DV1_PLG(obj,filename,scale,pos,rot,DD_PIXEL_FORMAT565);
	
	
}