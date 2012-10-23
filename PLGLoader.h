#pragma once

#include "stdio.h"
#include "ObjectDef.h"
//��������
//CSSD | RRRR | GGGG | BBBB
//CΪRGB/������ɫ���
//SS��������ɫģʽ
//DΪ��\˫����
//RRRR,GGGG,BBBBΪRGBģʽ�º�ɫ����ɫ����ɫ�ķ���
//GGGGBBBBΪ8λ��ɫ����ģʽ�µ���ɫ����

//���ڼ򻯲��Ե�λ����
#define PLX_RGB_MASK		0X8000	//��ȡRGB/����ģʽ������
#define PLX_SHADE_MODE_MASK	0X6000 //��ȡ��ɫģʽ������
#define PLX_2SIDED_MASK		0X1000 //��ȡ˫��״̬���õ�����
#define PLX_COLOR_MASK		0X0FFF //XXXXRRRRGGGGBBBB��ÿ��RGB����4λ
								//XXXXXXXXIIIIIIII ��ɫ����ģʽ�µ�8Ϊ����

//ʹ������λ������ȡ���������������ж���ϸ��Ϣ

//�����ж���ɫģʽ�ı��
#define	PLX_COLOR_MODE_RGB_FLAG		0X8000	//�����ʹ��RGB��ɫ
#define PLX_COLOR_MODE_INDEXED_FLAG	0X0000	//�����ʹ��8λ����


//��ɫģʽ���
#define PLX_SHADE_MODE_PURE_FLAG		0X0000 //�����ʹ�ù̶���ɫ
#define PLX_SHADE_MODE_FLAT_FLAG		0X2000 //�����ʹ�ú㶨��ɫ
#define PLX_SHADE_MODE_GOURAUD_FLAG		0X4000 //�����ʹ��Gouraud��ɫ
#define PLX_SHADE_MODE_PHONG_FLAG		0X6000 //phong ��ɫ

//PLG �ļ��ĸ�ʽ
/*
# ����ע��

# ����������Ϣ
object_name_string num_verts_int num_polys_int

 #�����б�
 x0_float y0_float z0_float
 x1_float y1_float z1_float
 x2_float y2_float z2_float
.
.
xn_float yn_float zn_float

  #������б�
  surface_description_ushort num_verts_int v0_index_int v1_index_int v2_index_int . vn_index_int
  .
  .
  surface_description_ushort num_verst_int v0_index_int v1_index_int v2_index_int .. vn_index_int 

 ÿ��Ԫ��ռһ��



*************************/	


class CPlgLoader
{
public:

	int Load_Object4DV1_PLG(CObject4DV1 & obj,//һ�����������
		char *filename,//plg�ļ�������
		CVector4D& scale,//��������
		CVector4D& pos,//��ʼλ��
		CVector4D& rot)//��ʼ��ת�Ƕ�
	{
		//��������Ӵ����м���plg�ļ�
		//���������������ӣ���ת�Ƕȣ�λ�����������������
		FILE* fp;
		char buffer[256];//���ݻ���
		
		char * token_string;
		//��һ������գ���ʼ��
		memset((&obj),0,sizeof(CObject4DV1));
		
		obj.m_state = OBJECT4DV1_STATE_ACTIVE | OBJECT4DV1_STATE_VISIBLE;
		
		obj.m_world_pos.SetX(pos.GetX());
		obj.m_world_pos.SetY(pos.GetY());
		obj.m_world_pos.SetZ(pos.GetZ());
		
		//�ڶ��������ļ�
		if(!(fp = fopen(filename,"r")))
		{
			printf("Couldn't open PLG file %s.\n",filename);
			return 0;
		}
		
		//������: ��ȡ����������
		if(!(token_string = Get_Line_PLG(buffer,256,fp)))
		{
			printf("PLG file error with file %s (objet descriptor invalid). ",filename);
			return 0;
		}
		printf("Object Descriptor: %s \n",token_string);

		sscanf(token_string,"%s %d %d",obj.m_name,obj.m_vertices,obj.m_polys);

		//��
		for(int vertex = 0; vertex<obj.m_vertices;vertex++)
		{
			if(!(token_string = Get_Line_PLG(buffer,255,fp)))
			{
				printf("PLG file error with file %s (vertex list invalid)\n",filename);
				return 0;
			}
			sscanf(token_string,"%f %f %f",obj.m_vlist_local[vertex].m_vector[X],obj.m_vlist_local[vertex].m_vector[Y],obj.m_vlist_local[vertex].m_vector[Z]);
			obj.m_vlist_local[vertex].SetW(1.0f);
			//���Ŷ�������
			obj.m_vlist_local[vertex].m_vector[X] *= scale.GetX();
			obj.m_vlist_local[vertex].m_vector[Y] *= scale.GetY();
			obj.m_vlist_local[vertex].m_vector[Z] *= scale.GetZ();

			printf("\nVertex %d = %f %f %f %f",vertex,obj.m_vlist_local[vertex].m_vector[X]
				,obj.m_vlist_local[vertex].m_vector[Y]
				,obj.m_vlist_local[vertex].m_vector[Z]);
		}

		//����ƽ���뾶�����뾶

		return 1;
	}
	char *Get_Line_PLG(char* buffer,int maxlength,FILE* fp)
	{
		//����PLG�ļ��е�ע�ͺͿ���
		//����һ��������
		//����ļ�Ϊ�գ��򷵻�NULL
		
		int index= 0;//����
		int length=0;//����
		while(true)
		{
			if(!fgets(buffer,maxlength,fp))
				return (NULL);

			for(length = strlen(buffer),index=0;isspace(buffer[index]);index++);

			//����Ƿ�Ϊ���л�ע��
			if(index >= length || buffer[index] == '#')
				continue;

			return &buffer[index];
		}
	}
};