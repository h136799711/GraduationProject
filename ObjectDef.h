#pragma once

#include "Vector.h"
//多边形属性
#define POLY4DV1_ATTR_2SIDED		0X0001
#define POLY4DV1_ATTR_TRANSPARENT	0X0002
#define POLY4DV1_ATTR_8BITCOLOR		0X0004
#define POLY4DV1_ATTR_RGB16		    0X0008
#define POLY4DV1_ATTR_RGB24		    0X0010

#define POLY4DV1_ATTR_SHADE_MODE_PURE    0X0020
#define POLY4DV1_ATTR_SHADE_MODE_FLAT    0X0040
#define POLY4DV1_ATTR_SHADE_MODE_GOURAUD 0X0080
#define POLY4DV1_ATTR_SHADE_MODE_PHONG   0X0100

#define POLY4DV1_STATE_ACTIVE			0X0001
#define POLY4DV1_STATE_CLIPPED			0X0002
#define POLY4DV1_STATE_BACKFACE			0X0004

//基于顶点列表的多边形 实际上这里指三角形
class CPoly4DV1
{
public:
	int m_state;// 状态信息
	int m_attr;// 多边形的物理属性
	int m_color;//多边形的颜色

	CPoint4D m_vlist;//顶点列表
	int m_vert[3];//顶点列表中元素的索引
};

//用于渲染列表的数据结构
class CPolyF4DV1
{
public:
	int m_state;	//状态信息
	int m_attr;	//多边形的物理属性
	int m_color;//多边形的颜色

	CPoint4D m_vlist[3];//该三角形的顶点
	CPoint4D m_tvlist[3];//存储变换后的顶点

	CPolyF4DV1* m_next;//指向渲染列表中的下一个多边形指针
	CPolyF4DV1* m_prev;//指向渲染列表中的前一个多边形指针
};
typedef CPolyF4DV1* CPolyF4DV1_PTR;

#define OBJECT4DV1_MAX_VERTICES           1024  // 64
#define OBJECT4DV1_MAX_POLYS              1024 // 128

#define OBJECT4DV1_STATE_ACTIVE           0x0001
#define OBJECT4DV1_STATE_VISIBLE          0x0002 
#define OBJECT4DV1_STATE_CULLED           0x0004

class CObject4DV1
{
public:
	int m_id;//物体的数字ID
	char m_name[64];//物体的字符描述

	int m_state;//物体的状态
	int m_attr;//物体的属性
	float m_avg_radius;//物体的平均半径，用于碰撞检测
	float m_max_radius;//物体的最大半径

	CPoint4D m_world_pos;//物体在世界坐标系中的位置

	CVector4D m_dir;//物体在局部坐标系中的旋转位置
	CVector4D m_ux,m_uy,m_uz;//局部坐标轴，用于存储物体的朝向
					//旋转期间将被更新
	int m_vertices;//物体的顶点数
	CPoint4D m_vlist_local[OBJECT4DV1_MAX_VERTICES];
	CPoint4D m_vlist_trans[OBJECT4DV1_MAX_VERTICES];

	int m_polys;//物体网格的多边形数
	CPoly4DV1 m_plist[OBJECT4DV1_MAX_POLYS];//多边形数组


	int Compute_Radius()
	{
		m_avg_radius = 0;
		m_max_radius = 0;

		for(int vertex=0;vertex<m_vertices;vertex++)
		{
			float dist_to_vertex = 
				sqrt(m_vlist_local[vertex].GetX()*m_vlist_local[vertex].GetX()
					+m_vlist_local[vertex].GetY()*m_vlist_local[vertex].GetY()
					+m_vlist_local[vertex].GetZ()*m_vlist_local[vertex].GetZ());
			m_avg_radius += dist_to_vertex;

			if(dist_to_vertex > m_max_radius)
				m_max_radius = dist_to_vertex;
		}
		m_avg_radius /= m_vertices;
		return 1;
	}



};


#define RENDERLIST4DV1_MAX_POLYS	32768
//渲染列表定义
class CRenderList4DV1
{
public :
	
	void Reset_RenderList4DV1(CRenderList4DV1& rend_list)
	{
		rend_list.m_polys = 0;
	}
	
	
	
	
	int m_state;//渲染列表的状态
	int m_attr;//渲染列表的属性

	//渲染列表是一个指针数组
	//其中每一个指针指向一个自包含的，可渲染的多边形面(CPolyF4DV1)

	CPolyF4DV1_PTR m_poly_ptrs[RENDERLIST4DV1_MAX_POLYS];
	//为避免每帧都为多边形分配和释放空间
	//多边型存储在下述数组中
	CPolyF4DV1 m_poly_data[RENDERLIST4DV1_MAX_POLYS];
	
	int m_polys;


};






