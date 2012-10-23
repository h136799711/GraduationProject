#include "..\PLGLoader.h"


int main()
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
	
	
	
	return 0;
}