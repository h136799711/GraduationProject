#pragma once

#include <stdio.h>
#include "ObjectDef.h"

//面描述符
//CSSD | RRRR | GGGG | BBBB
//C为RGB/索引颜色标记
//SS描述了着色模式
//D为单\双面标记
//RRRR,GGGG,BBBB为RGB模式下红色，绿色，蓝色的分量
//GGGGBBBB为8位颜色索引模式下的颜色索引

//用于简化测试的位掩码
#define PLX_RGB_MASK		0X8000	//抽取RGB/索引模式的掩码
#define PLX_SHADE_MODE_MASK	0X6000 //抽取着色模式的掩码
#define PLX_2SIDED_MASK		0X1000 //抽取双面状态设置的掩码
#define PLX_COLOR_MASK		0X0FFF //XXXXRRRRGGGGBBBB，每个RGB分量4位
//XXXXXXXXIIIIIIII 颜色索引模式下的8为索引

//使用上述位掩码提取后再用下述掩码判断详细信息

//用于判断颜色模式的标记
#define	PLX_COLOR_MODE_RGB_FLAG		0X8000	//多边形使用RGB颜色
#define PLX_COLOR_MODE_INDEXED_FLAG	0X0000	//多边形使用8位索引

//用于双，单面标记
#define PLX_2SIDED_FLAG              0x1000  
#define PLX_1SIDED_FLAG              0x0000   

//着色模式标记
#define PLX_SHADE_MODE_PURE_FLAG		0X0000 //多边形使用固定着色
#define PLX_SHADE_MODE_CONSTANCT_FLAG	0X0000//别名，含义同上
#define PLX_SHADE_MODE_FLAT_FLAG		0X2000 //多边形使用恒定着色
#define PLX_SHADE_MODE_GOURAUD_FLAG		0X4000 //多边形使用Gouraud着色
#define PLX_SHADE_MODE_PHONG_FLAG		0X6000 //phong 着色
#define PLX_SHADE_MODE_FASTPHON_FLAG	0X6000 //别名同上


					
inline int RGB16BIT555(int r,int g,int b) 
{
	return ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10));
}
inline int RGB16BIT565(int r,int g,int b)
{
	return ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11));
}

//PLG 文件的格式
/*
# 这是注释

  # 物体描述信息
  object_name_string num_verts_int num_polys_int
  
	#顶点列表
	x0_float y0_float z0_float
	x1_float y1_float z1_float
	x2_float y2_float z2_float
	.
	.
	xn_float yn_float zn_float
	
	  #多边形列表
	  surface_description_ushort num_verts_int v0_index_int v1_index_int v2_index_int . vn_index_int
	  .
	  .
	  surface_description_ushort num_verst_int v0_index_int v1_index_int v2_index_int .. vn_index_int 
	  
		每个元素占一行
		
		  
			
*************************/	

class CPlgLoader
{
public:
	
	int Load_Object4DV1_PLG(CObject4DV1 & obj,//一个物体的引用
		char *filename,//plg文件的名称
		CVector4D& scale,//缩放因子
		CVector4D& pos,//初始位置
		CVector4D& rot,//初始旋转角度
		int dd_pixel_format)//颜色转换函数，从HCDXBuilder中获得即可
	{
		//这个方法从磁盘中加载plg文件
		//并允许传入缩放因子，旋转角度，位置来设置物体的属性
		FILE* fp;
		char buffer[256];//数据缓冲
		
		char *token_string;
		//第一步：清空，初始化
		memset((&obj),0,sizeof(CObject4DV1));
		
		obj.m_state = OBJECT4DV1_STATE_ACTIVE | OBJECT4DV1_STATE_VISIBLE;
		
		obj.m_world_pos.SetX(pos.GetX());
		obj.m_world_pos.SetY(pos.GetY());
		obj.m_world_pos.SetZ(pos.GetZ());
		obj.m_world_pos.SetW(pos.GetW());
		
		//第二步，打开文件
		if(!(fp = fopen(filename,"r")))
		{
			printf("Couldn't open PLG file %s.\n",filename);
			return 0;
		}
		
		//第三步: 读取物体描述符
		if(!(token_string = Get_Line_PLG(buffer,256,fp)))
		{
			printf("PLG file error with file %s (objet descriptor invalid). ",filename);
			return 0;
		}
		printf("Object Descriptor: %s \n",token_string);
		
		sscanf(token_string,"%s %d %d",obj.m_name,&obj.m_vertices,&obj.m_polys);
		
		//四
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
			//缩放顶点坐标
			obj.m_vlist_local[vertex].m_vector[HC_X] *= scale.GetX();
			obj.m_vlist_local[vertex].m_vector[HC_Y] *= scale.GetY();
			obj.m_vlist_local[vertex].m_vector[HC_Z] *= scale.GetZ();
			
			printf("\nVertex %d = %f %f %f %f",vertex
				,obj.m_vlist_local[vertex].m_vector[HC_X]
				,obj.m_vlist_local[vertex].m_vector[HC_Y]
				,obj.m_vlist_local[vertex].m_vector[HC_Z]
				,obj.m_vlist_local[vertex].m_vector[HC_W]);
		}
		
		//计算平均半径和最大半径
		obj.Compute_Radius();
		printf("\navg,max radius = %f,%f",obj.m_avg_radius,obj.m_max_radius);
		
		int poly_surface_desc =0;//PLG/PLX 多边形描述符
		int poly_num_verts = 0;//当前多边形的顶点数(总是3)
		char tmp_string[8];//存储多边形的描述符的字符串
		
		//第5步
		for(int poly=0;poly<obj.m_polys;poly++)
		{
			if(!(token_string = Get_Line_PLG(buffer,255,fp)))
			{
				printf("\n PLG file error with file %s (polygon descriptor invalid).");
				return 0;
			}
			printf("\n Polygon %d ",poly);
			//每个多边形有3个顶点
			sscanf(token_string,"%s %d %d %d %d",tmp_string,&poly_num_verts,&obj.m_plist[poly].m_vert[0]
				,&obj.m_plist[poly].m_vert[1],&obj.m_plist[poly].m_vert[2]);
			//面描述可以是十六进制(0x开头)
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
			
			//设置颜色模式
			if((poly_surface_desc & PLX_COLOR_MODE_RGB_FLAG))
			{
				SET_BIT(obj.m_plist[poly].m_attr,POLY4DV1_ATTR_RGB16);
				
				int red = ((poly_surface_desc & 0x0f00) >> 8);
				int green = ((poly_surface_desc & 0x00f0) >> 4);
				int blue = (poly_surface_desc & 0x000f);
				//在文件中,RGB颜色为4,4,4模式
				//而图形卡中RGB颜色模式为5,5,5,或5,6,5
				//但虚拟颜色系统将把8，8，8格式转换为5，5，5或5，6，5
				//因此要将4，4，4的RGB颜色转换为8，8，8
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
				//使用的是8位颜色索引
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
		//跳过PLG文件中的注释和空行
		//返回一整行数据
		//如果文件为空，则返回NULL
		
		int index= 0;//索引
		int length=0;//长度
		while(true)
		{
			if(!fgets(buffer,maxlength,fp))
				return (NULL);
			
			for(length = strlen(buffer),index=0;isspace(buffer[index]);index++);
			
			//检测是否为空行或注释
			if(index >= length || buffer[index] == '#')
				continue;
			
			return &buffer[index];
		}
	}
};