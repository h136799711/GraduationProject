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
	plgLoader.Load_Object4DV1_PLG(obj,filename,scale,pos,rot);
	

	
	return 0;
}