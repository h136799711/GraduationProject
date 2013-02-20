#ifndef H_MATERIAL_INCLUDED
#define H_MATERIAL_INCLUDED

#include "HCLib\HCResource.h"

//�������ԣ����������Ծ�������һ��
#define MATV1_ATTR_2SIDED				0X0001
#define MATV1_ATTR_TRANSPARENT			0X0002
#define MATV1_ATTR_8BITCOLOR			0X0004
#define MATV1_ATTR_RGB16				0X0008
#define MATV1_ATTR_RGB24				0X0010

#define MATV1_ATTR_SHADE_MODE_CONSTANT	0X0020
#define MATV1_ATTR_SHADE_MODE_EMMISIVE	0X0020 //����
#define MATV1_ATTR_SHADE_MODE_FLAT		0X0040
#define MATV1_ATTR_SHADE_MODE_GOURAUD	0X0080
#define MATV1_ATTR_SHADE_MODE_FASTPHONG	0X0100
#define MATV1_ATTR_SHADE_MODE_TEXTURE	0X0200

//����״̬
#define MATV1_STATE_ACTIVE				0X0001

//����ϵͳ������

#define MAX_MATERIALS					256

#define _RGBA32BIT(r,g,b,a) ((a)+((b)<<8)+((g)<<16)+((r) << 24))
typedef struct RGBAV1_TYPE
{
	union
	{
		int rgba;
		UCHAR rgba_M[4];
		struct 
		{
				UCHAR a,b,g,r;
			
		};
	};

}RGBAV1,*RGBAV1_PTR;

typedef struct MATV1_TYP
{
	int state;
	int id;
	char name[64];
	int attr;

	RGBAV1 color;
	float ka,kd,ks,power;//�Ի����⡢ɢ���;��淴���ķ���ϵ��
	
	RGBAV1 ra,rd,rs;
	char texture_file[80];
	BITMAP_IMAGE texture;

}MATV1 ,*MATV1_PTR;


class MaterialsLib
{
public:
	int ResetMaterialsMATV1(void)
	{
		static int first_time = 1;

		//����ǵ�һ�ε��ô˺��������������в���
		if(first_time)
		{
			memset(m_materials,0,MAX_MATERIALS*sizeof(MATV1));
			first_time = 0;
		}
		HCBitmap bitmap;
		
		for(int curr_matt = 0 ; curr_matt < MAX_MATERIALS ; curr_matt++)
		{
			bitmap.Destroy_Bitmap(&(m_materials[curr_matt].texture));
			memset(&m_materials[curr_matt],0,sizeof(MATV1));
		}

		return (1);


	}
	MATV1 m_materials[MAX_MATERIALS];
	int m_num_materials;//��ǰ�Ĳ�����
};
#endif