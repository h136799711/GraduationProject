#pragma once

#include <stdio.h>
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

//����˫��������
#define PLX_2SIDED_FLAG              0x1000  
#define PLX_1SIDED_FLAG              0x0000   

//��ɫģʽ���
#define PLX_SHADE_MODE_PURE_FLAG		0X0000 //�����ʹ�ù̶���ɫ
#define PLX_SHADE_MODE_CONSTANCT_FLAG	0X0000//����������ͬ��
#define PLX_SHADE_MODE_FLAT_FLAG		0X2000 //�����ʹ�ú㶨��ɫ
#define PLX_SHADE_MODE_GOURAUD_FLAG		0X4000 //�����ʹ��Gouraud��ɫ
#define PLX_SHADE_MODE_PHONG_FLAG		0X6000 //phong ��ɫ
#define PLX_SHADE_MODE_FASTPHON_FLAG	0X6000 //����ͬ��


					
inline int RGB16BIT555(int r,int g,int b) 
{
	return ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10));
}
inline int RGB16BIT565(int r,int g,int b)
{
	return ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11));
}

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
		CVector4D& rot,//��ʼ��ת�Ƕ�
		int dd_pixel_format)//��ɫת����������HCDXBuilder�л�ü���
	{
		//��������Ӵ����м���plg�ļ�
		//���������������ӣ���ת�Ƕȣ�λ�����������������
		FILE* fp;
		char buffer[256];//���ݻ���
		
		char *token_string;
		//��һ������գ���ʼ��
		memset((&obj),0,sizeof(CObject4DV1));
		
		obj.m_state = OBJECT4DV1_STATE_ACTIVE | OBJECT4DV1_STATE_VISIBLE;
		
		obj.m_world_pos.SetX(pos.GetX());
		obj.m_world_pos.SetY(pos.GetY());
		obj.m_world_pos.SetZ(pos.GetZ());
		obj.m_world_pos.SetW(pos.GetW());
		
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
		
		sscanf(token_string,"%s %d %d",obj.m_name,&obj.m_vertices,&obj.m_polys);
		
		//��
		for(int vertex = 0; vertex<obj.m_vertices;vertex++)
		{
			if(!(token_string = Get_Line_PLG(buffer,255,fp)))
			{
				printf("PLG file error with file %s (vertex list invalid)\n",filename);
				return 0;
			}
			sscanf(token_string,"%f %f %f",&obj.m_vlist_local[vertex].m_vector[HC_X]
				,&obj.m_vlist_local[vertex].m_vector[HC_Y]
				,&obj.m_vlist_local[vertex].m_vector[HC_Z]);
			obj.m_vlist_local[vertex].SetW(1.0f);
			//���Ŷ�������
			obj.m_vlist_local[vertex].m_vector[HC_X] *= scale.GetX();
			obj.m_vlist_local[vertex].m_vector[HC_Y] *= scale.GetY();
			obj.m_vlist_local[vertex].m_vector[HC_Z] *= scale.GetZ();
			
			printf("\nVertex %d = %f %f %f %f",vertex
				,obj.m_vlist_local[vertex].m_vector[HC_X]
				,obj.m_vlist_local[vertex].m_vector[HC_Y]
				,obj.m_vlist_local[vertex].m_vector[HC_Z]
				,obj.m_vlist_local[vertex].m_vector[HC_W]);
		}
		
		//����ƽ���뾶�����뾶
		obj.Compute_Radius();
		printf("\navg,max radius = %f,%f",obj.m_avg_radius,obj.m_max_radius);
		
		int poly_surface_desc =0;//PLG/PLX �����������
		int poly_num_verts = 0;//��ǰ����εĶ�����(����3)
		char tmp_string[8];//�洢����ε����������ַ���
		
		//��5��
		for(int poly=0;poly<obj.m_polys;poly++)
		{
			if(!(token_string = Get_Line_PLG(buffer,255,fp)))
			{
				printf("\n PLG file error with file %s (polygon descriptor invalid).");
				return 0;
			}
			printf("\n Polygon %d ",poly);
			//ÿ���������3������
			sscanf(token_string,"%s %d %d %d %d",tmp_string,&poly_num_verts,&obj.m_plist[poly].m_vert[0]
				,&obj.m_plist[poly].m_vert[1],&obj.m_plist[poly].m_vert[2]);
			//������������ʮ������(0x��ͷ)
			if((tmp_string[0] == '0' && tmp_string[1] == 'X') || (tmp_string[0] == '0' && tmp_string[1] == 'x'))
				sscanf(tmp_string,"%x",&poly_surface_desc);
			else
				poly_surface_desc = atoi(tmp_string);
			
			obj.m_plist[poly].m_vlist = obj.m_vlist_local;
			printf("\n Surface Desc 0x%.4x,num_verts = %d,vert_indices[%d,%d,%d]",poly_surface_desc,poly_num_verts,obj.m_plist[poly].m_vert[0]
				,obj.m_plist[poly].m_vert[1],obj.m_plist[poly].m_vert[2]);
			
			if((poly_surface_desc & PLX_2SIDED_FLAG))
			{
				SET_BIT(obj.m_plist[poly].m_attr,POLY4DV1_ATTR_2SIDED);
				printf("\n2 sided. ");
			}else
			{
				printf("\n1 sided");
			}
			
			//������ɫģʽ
			if((poly_surface_desc & PLX_COLOR_MODE_RGB_FLAG))
			{
				SET_BIT(obj.m_plist[poly].m_attr,POLY4DV1_ATTR_RGB16);
				
				int red = ((poly_surface_desc & 0x0f00) >> 8);
				int green = ((poly_surface_desc & 0x00f0) >> 4);
				int blue = (poly_surface_desc & 0x000f);
				//���ļ���,RGB��ɫΪ4,4,4ģʽ
				//��ͼ�ο���RGB��ɫģʽΪ5,5,5,��5,6,5
				//��������ɫϵͳ����8��8��8��ʽת��Ϊ5��5��5��5��6��5
				//���Ҫ��4��4��4��RGB��ɫת��Ϊ8��8��8
				if (dd_pixel_format == DD_PIXEL_FORMAT555)
				{
					
					obj.m_plist[poly].m_color = RGB16BIT555(red*16,green*16,blue*16);
					
				} 
				else
				{
					
					obj.m_plist[poly].m_color = RGB16BIT565(red*16,green*16,blue*16);
					
				} 
				printf("\nRGB color = [%d,%d,%d] " , red,green,blue);
			}else
			{
				//ʹ�õ���8λ��ɫ����
				SET_BIT(obj.m_plist[poly].m_attr,POLY4DV1_ATTR_8BITCOLOR);
				obj.m_plist[poly].m_color = (poly_surface_desc & 0x00ff);
				printf("\n8 bit color index = %d ",obj.m_plist[poly].m_color);
			}
			
			int shade_mode = (poly_surface_desc & PLX_SHADE_MODE_MASK);
			
			switch(shade_mode)
			{
			case PLX_SHADE_MODE_PURE_FLAG:
				{
					SET_BIT(obj.m_plist[poly].m_attr,POLY4DV1_ATTR_SHADE_MODE_PURE);
					printf("\nShade mode = pure");
				}break;
			case PLX_SHADE_MODE_FLAT_FLAG:
				{
					SET_BIT(obj.m_plist[poly].m_attr,POLY4DV1_ATTR_SHADE_MODE_FLAT);
					printf("\nShade mode = FLAT");
				}break;
			case PLX_SHADE_MODE_GOURAUD_FLAG:
				{
					SET_BIT(obj.m_plist[poly].m_attr,POLY4DV1_ATTR_SHADE_MODE_GOURAUD);
					printf("\nShade mode = GOURAUD");
				}break;
			case PLX_SHADE_MODE_PHONG_FLAG:
				{
					SET_BIT(obj.m_plist[poly].m_attr,POLY4DV1_ATTR_SHADE_MODE_PHONG);
					printf("\nShade mode = PHONG");
				}break;
			default:break;
			}
			obj.m_plist[poly].m_state = POLY4DV1_STATE_ACTIVE;
		}//end for poly
	

		fclose(fp);
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