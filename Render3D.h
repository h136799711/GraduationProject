
#pragma once

#include "ObjectDef.h"
#include "Matrix.h"
#include "Math3D.h"


#define CULL_OBJECT_X_PLANE		0X0001//根据左右裁剪面进行剔除
#define CULL_OBJECT_Y_PLANE		0X0002//根据上下裁剪面进行剔除
#define CULL_OBJECT_Z_PLANE		0X0004//根据远近裁剪面进行剔除
#define CULL_OBJECT_XYZ_PLANE   (CULL_OBJECT_X_PLANE | CULL_OBJECT_Y_PLANE |CULL_OBJECT_Z_PLANE)
class CRender3DV1
{
public:
	
	/*
		渲染列表的通用变换
		mt: 变换矩阵
		coord_select:如何变换坐标
	*/
	int Transform_Renderlist(CRenderList4DV1& rend_list,CMatrix44 mt,int coord_select);
	/*
		物体的通用变换
		mt: 变换矩阵
		coord_select:如何变换坐标
	*/
	int Transform_Object(CObject4DV1& obj,CMatrix44 mt,int coord_select,int tranform_basis);
	/*
		局部坐标到世界坐标的变换 --- 物体
	*/
	int Model_To_World_Object(CObject4DV1& obj,int coord_select=TRANSFORM_LOCAL_TO_TRANS);
	/*
		局部坐标到世界坐标的变换 --- 渲染列表
	*/
	int Model_To_World_Renderlist(CRenderList4DV1& rend_list,CPoint4D world_pos,int coord_select=TRANSFORM_LOCAL_TO_TRANS);
	/*
		世界坐标到相机坐标的变换 --- 物体
	*/
	int World_To_Camera_Object(CCamera4DV1 cam,CObject4DV1& obj);
	/*
		世界坐标到相机坐标的变换 --- 渲染列表
	*/
	int World_To_Camera_RenderList(CRenderList4DV1& rendlist,CCamera4DV1 cam);
	/*
		相机坐标到透视坐标的变换 --- 物体
	*/
	int Camera_To_Perspective_Object4DV1(CObject4DV1& obj,CCamera4DV1 cam)
	{
		for(int vertex=0;vertex<obj.m_polys;vertex++)
		{
			float z = obj.m_vlist_trans[vertex].GetZ();

			obj.m_vlist_trans[vertex].SetX(cam.m_view_dist * obj.m_vlist_trans[vertex].GetX() / z);
			obj.m_vlist_trans[vertex].SetY(cam.m_view_dist * obj.m_vlist_trans[vertex].GetY() * cam.m_aspect_ratio / z);
		}

		return 1;

	}
	/*
		相机坐标到透视坐标的变换 --- 渲染列表
	*/
	/*
		透视坐标到屏幕坐标的变换 --- 物体
	*/
	/*
		透视坐标到屏幕坐标的变换 --- 渲染列表
	*/

	/*
		执行剔除物体操作，物体被一球包围着用作物体的体积
	*/
	int Cull_Object4DV1(CObject4DV1& obj,CCamera4DV1 cam,int cull_flags);
	/*
		重置物体的状态标记
	*/
	void Reset_Object4DV1(CObject4DV1& obj)
	{
		RESET_BIT(obj.m_state,OBJECT4DV1_STATE_CULLED);

		for(int poly=0;poly<obj.m_polys;poly++)
		{
			CPoly4DV1* curr_poly = &obj.m_plist[poly];

			if(!(curr_poly->m_state & POLY4DV1_STATE_ACTIVE))
				continue;

			RESET_BIT(curr_poly->m_state,POLY4DV1_STATE_CLIPPED);
			RESET_BIT(curr_poly->m_state,POLY4DV1_STATE_BACKFACE);
		}
	}
	/*
		物体的背面剔除,在物体剔除之后和世界坐标到相机坐标变换之前进行调用
	*/
	void Remove_Backfaces_Object4DV1(CObject4DV1& obj,CCamera4DV1 cam)
	{
		//物体是否已经被剔除
		if(obj.m_state & OBJECT4DV1_STATE_CULLED)
			return ;
		for(int poly=0;poly< obj.m_polys;poly++)
		{
			CPoly4DV1* curr_poly = &obj.m_plist[poly];

			//判断多边形是否没有被裁剪掉，没有被剔除，出于活动状态，出于活动状态的，可见的双面的
			if(!(curr_poly->m_state & POLY4DV1_STATE_ACTIVE) || 
				(curr_poly->m_state & POLY4DV1_STATE_CLIPPED) ||
				(curr_poly->m_attr & POLY4DV1_ATTR_2SIDED) ||
				(curr_poly->m_state & POLY4DV1_STATE_BACKFACE))
				continue;

			//获取顶点列表中的顶点索引
			int vindex_0 = curr_poly->m_vert[0];
			int vindex_1 = curr_poly->m_vert[1];
			int vindex_2 = curr_poly->m_vert[2];

			//使用变换后的多边形列表
			CVector4D u,v,n;

			//计算u，v
			u = obj.m_vlist_trans[vindex_1] - obj.m_vlist_trans[vindex_0];
			v = obj.m_vlist_trans[vindex_2] - obj.m_vlist_trans[vindex_0];

			u.Cross(v,n);

			CVector4D view;
			view = cam.m_pos - obj.m_vlist_trans[vindex_0];

			float dp = n.Dot(view);
			if(dp <= 0.0)
				SET_BIT(curr_poly->m_state,POLY4DV1_STATE_BACKFACE);

		}
	}
	void Remove_Backfaces_Renderlist4DV1(CRenderList4DV1& rend_list,CCamera4DV1 cam)
	{
		for(int poly=0;poly<rend_list.m_polys;poly++)
		{
			CPolyF4DV1* curr_poly = rend_list.m_poly_ptrs[poly];
			if((curr_poly == NULL) || !(curr_poly->m_state & POLY4DV1_STATE_ACTIVE)
				|| (curr_poly->m_state & POLY4DV1_STATE_CLIPPED)
				|| (curr_poly->m_attr & POLY4DV1_ATTR_2SIDED)
				|| (curr_poly->m_state & POLY4DV1_STATE_BACKFACE))
				continue;
			CVector4D u,v,n;

			u = curr_poly->m_tvlist[1] - curr_poly->m_tvlist[0];
			v = curr_poly->m_tvlist[2] - curr_poly->m_tvlist[0];
			
			u.Cross(v,n);

			CVector4D view;
			view = cam.m_pos - curr_poly->m_tvlist[0];
			float dp = u.Dot(view);
			if(dp <= 0.0)
			{
				SET_BIT(curr_poly->m_state,POLY4DV1_STATE_BACKFACE);
			}
		}
	}
	/*
		建立局部坐标到世界坐标的变换矩阵
	*/
	void Build_Model_To_Word_Matrix44(CVector4D vpos,CMatrix44& m)
	{
		m.M[0][0] = 1;m.M[0][1] = 0;m.M[0][2] = 0;m.M[0][3] = 0;
		m.M[1][0] = 0;m.M[1][1] = 1;m.M[1][2] = 0;m.M[1][3] = 0;
		m.M[2][0] = 0;m.M[2][1] = 0;m.M[2][2] = 1;m.M[2][3] = 0;
		m.M[3][0] = vpos.GetX();m.M[3][1] = vpos.GetY();m.M[3][2] = vpos.GetZ();m.M[3][3] = 1;
	}

	/*
		建立相机变换到透视变换的矩阵
	*/
	void Build_Camera_To_Perspective_Matrix44(CCamera4DV1 cam,CMatrix44& m44)
	{
		m44.M[0][0] = cam.m_view_dist;m44.M[0][1] = 0;m44.M[0][2] = 0;m44.M[0][3] = 0;
		
		m44.M[1][0] = 0;m44.M[1][1] = cam.m_view_dist * cam.m_aspect_ratio;m44.M[1][2] = 0;m44.M[1][3] = 0;
		
		m44.M[2][0] = 0;m44.M[2][1] = 0;m44.M[2][2] = 1;m44.M[2][3] = 1;
		
		m44.M[3][0] = 0;m44.M[3][1] = 0;m44.M[3][2] = 0;m44.M[3][3] = 0;
		
	}
	/*
		将物体的坐标从齐次坐标转换为真正的3D坐标
	*/
	void Convert_From_Homogeneous4D_Object4DV1(CObject4DV1& obj)
	{
		for(int vertex=0;vertex<obj.m_polys;vertex++)
		{
			obj.m_vlist_trans[vertex].DivByW();
		}
	}
	
	CMath3D math3d;
};
int CRender3DV1::Cull_Object4DV1(CObject4DV1& obj,CCamera4DV1 cam,int cull_flags)
{
	//这个方法是基于矩阵的
	//它根据传入的相机信息判断物体是否在视景体内
	//参数cull_flags指定在哪些轴上执行剔除
	
	//step1: 	转换物体包围球的中心为相机坐标
	CPoint4D sphere_pos;

	math3d.Mat_Mul_4D_44(obj.m_world_pos,cam.mcam,sphere_pos);
	
	//step2:根据剔除标记对物体执行剔除操作
	if(cull_flags & CULL_OBJECT_Z_PLANE)
	{
		if(((sphere_pos.GetZ() - obj.m_max_radius > cam.m_far_clip_z) 
			|| (sphere_pos.GetZ() + obj.m_max_radius < cam.m_near_clip_z)))
		{
			SET_BIT(obj.m_state,OBJECT4DV1_STATE_CULLED);
			return 1;
		}
	}	
	float z_test;
	if(cull_flags & CULL_OBJECT_X_PLANE)
	{
		//使用右左裁剪面检测包围球上最左边和最右边
		z_test = (0.5)*cam.m_viewplane_width * (sphere_pos.GetZ() / cam.m_view_dist);
		if(((sphere_pos.GetX() - obj.m_max_radius) > z_test) ||
			((sphere_pos.GetX() + obj.m_max_radius) < -z_test))
		{
			SET_BIT(obj.m_state,OBJECT4DV1_STATE_CULLED);
			return 1;
		}
	}

	if(cull_flags & CULL_OBJECT_Y_PLANE)
	{
		z_test = (0.5)*cam.m_viewplane_height * (sphere_pos.GetZ() / cam.m_view_dist);

		if(((sphere_pos.GetY() - obj.m_max_radius) > z_test)||
			((sphere_pos.GetY() + obj.m_max_radius) < -z_test))
		{
			SET_BIT(obj.m_state,OBJECT4DV1_STATE_CULLED);
			return 1;
		}
	}
	
	
	return 0;
}
int CRender3DV1::World_To_Camera_RenderList(CRenderList4DV1& rendlist,CCamera4DV1 cam)
{
	for(int poly=0;poly<rendlist.m_polys;poly++)
	{
		CPolyF4DV1_PTR curr_poly = rendlist.m_poly_ptrs[poly];
		if((curr_poly == NULL ) || 
			(curr_poly->m_attr & POLY4DV1_STATE_ACTIVE)||
			(curr_poly->m_attr & POLY4DV1_STATE_CLIPPED)||
			(curr_poly->m_attr & POLY4DV1_STATE_BACKFACE))
				continue;

		for(int vertex=0;vertex<3;vertex++)
		{
			CPoint4D presult;
			math3d.Mat_Mul_4D_44(curr_poly->m_tvlist[vertex],cam.mcam,presult);
			curr_poly->m_tvlist[vertex] = presult;
		}
	}
	return 1;
}
int CRender3DV1::World_To_Camera_Object(CCamera4DV1 cam,CObject4DV1 & obj)
{
	for(int vertex=0;vertex<obj.m_vertices;vertex++)
	{
		CPoint4D presult;

		math3d.Mat_Mul_4D_44(obj.m_vlist_trans[vertex],cam.mcam,presult);

		obj.m_vlist_trans[vertex] = presult;
	}
	return 1;
}
int CRender3DV1::Model_To_World_Renderlist(CRenderList4DV1& rend_list,CPoint4D world_pos,int coord_select)
{
	//这里未使用矩阵变换
	//将传递进来的渲染列表的局部/模型坐标变换为世界坐标
	int poly,vertex;
	if(coord_select == TRANSFORM_LOCAL_TO_TRANS)
	{
		for(poly = 0 ;poly<rend_list.m_polys;poly++)
		{
			CPolyF4DV1_PTR curr_poly = rend_list.m_poly_ptrs[poly];
			if((curr_poly == NULL ) || 
				(curr_poly->m_attr & POLY4DV1_STATE_ACTIVE)||
				(curr_poly->m_attr & POLY4DV1_STATE_CLIPPED)||
				(curr_poly->m_attr & POLY4DV1_STATE_BACKFACE))
				continue;
			for(vertex = 0;vertex < 3;vertex++)
			{
				curr_poly->m_tvlist[vertex] = curr_poly->m_vlist[vertex] + world_pos;
			}
		}
	}else
	{
		for(poly = 0 ;poly<rend_list.m_polys;poly++)
		{
			
			CPolyF4DV1_PTR curr_poly = rend_list.m_poly_ptrs[poly];
			if((curr_poly == NULL ) || 
				(curr_poly->m_attr & POLY4DV1_STATE_ACTIVE)||
				(curr_poly->m_attr & POLY4DV1_STATE_CLIPPED)||
				(curr_poly->m_attr & POLY4DV1_STATE_BACKFACE))
				continue;
			for(vertex = 0;vertex < 3;vertex++)
			{
				curr_poly->m_tvlist[vertex] = curr_poly->m_tvlist[vertex] + world_pos;
			}
		}
	}
	return 1;
}
int CRender3DV1::Model_To_World_Object(CObject4DV1& obj,int coord_select)
{
	int vertex;

	if(coord_select == TRANSFORM_LOCAL_TO_TRANS)
	{
		for(vertex = 0 ; vertex < obj.m_vertices; vertex ++)
		{
			obj.m_vlist_trans[vertex] = obj.m_vlist_local[vertex] + obj.m_world_pos;
		}
	}
	else
	{
		for(vertex = 0 ; vertex < obj.m_vertices; vertex ++)
		{
			obj.m_vlist_trans[vertex] = obj.m_vlist_trans[vertex] + obj.m_world_pos;
		}
	}
	return 1;
}
int CRender3DV1::Transform_Object(CObject4DV1& obj,CMatrix44 mt,int coord_select ,int transform_basis)
{
	int vertex;
	CPoint4D presult;
	switch(coord_select)
	{
	case TRANSFORM_LOCAL_ONLY:
		{
			for(vertex=0;vertex<obj.m_vertices;vertex++)
			{
				math3d.Mat_Mul_4D_44(obj.m_vlist_local[vertex],mt,presult);
				obj.m_vlist_local[vertex] = presult;
			}
		}break;
	case TRANSFORM_TRANS_ONLY:
		{
			
			for(vertex=0;vertex<obj.m_vertices;vertex++)
			{				
				math3d.Mat_Mul_4D_44(obj.m_vlist_trans[vertex],mt,presult);
				obj.m_vlist_trans[vertex] = presult;
			}
		}break;
	case TRANSFORM_LOCAL_TO_TRANS:
		{
			for(vertex=0;vertex<obj.m_vertices;vertex++)
			{
				math3d.Mat_Mul_4D_44(obj.m_vlist_local[vertex],mt,obj.m_vlist_trans[vertex]);
			}
		}break;
	default:break;

	}
	//是否要对朝向向量进行变换
	if(transform_basis)
	{
		//旋转物体的朝向向量
		CVector4D vresult;

		math3d.Mat_Mul_4D_44(obj.m_ux,mt,vresult);
		obj.m_ux = vresult;
		math3d.Mat_Mul_4D_44(obj.m_uy,mt,vresult);
		obj.m_ux = vresult;
		math3d.Mat_Mul_4D_44(obj.m_uz,mt,vresult);
		obj.m_ux = vresult;
	}
	return 1;
}
int CRender3DV1::Transform_Renderlist(CRenderList4DV1& rend_list,CMatrix44 mt,int coord_select)	
{
	
	int poly,vertex;
	switch(coord_select)
	{
	case TRANSFORM_LOCAL_ONLY:
		{
			
			for(poly = 0; poly<rend_list.m_polys;poly++)
			{
				CPolyF4DV1_PTR curr_poly = rend_list.m_poly_ptrs[poly];
				
				if((curr_poly == NULL) || !(curr_poly->m_state & POLY4DV1_STATE_ACTIVE)
					||(curr_poly->m_state & POLY4DV1_STATE_CLIPPED)||(curr_poly->m_state & POLY4DV1_STATE_BACKFACE))
					continue;
				
				for(vertex=0;vertex<3;vertex++)
				{
					CPoint4D presult;
					math3d.Mat_Mul_4D_44(curr_poly->m_vlist[vertex],mt,presult);
					
					curr_poly->m_vlist[vertex] = presult;
					assert(curr_poly->m_vlist[vertex] == presult);
				}
				
			}
			
		}break;
	case TRANSFORM_TRANS_ONLY:
		{
			for(poly = 0; poly<rend_list.m_polys;poly++)
			{
				CPolyF4DV1_PTR curr_poly = rend_list.m_poly_ptrs[poly];
				
				if((curr_poly == NULL) || !(curr_poly->m_state & POLY4DV1_STATE_ACTIVE)
					||(curr_poly->m_state & POLY4DV1_STATE_CLIPPED)||(curr_poly->m_state & POLY4DV1_STATE_BACKFACE))
					continue;
				
				for(vertex=0;vertex<3;vertex++)
				{
					CPoint4D presult;
					math3d.Mat_Mul_4D_44(curr_poly->m_tvlist[vertex],mt,presult);
					
					curr_poly->m_tvlist[vertex] = presult;
					assert(curr_poly->m_vlist[vertex] == presult);
				}	
			}
		}break;
	case TRANSFORM_LOCAL_TO_TRANS:
		{
			for(poly = 0; poly<rend_list.m_polys;poly++)
			{
				CPolyF4DV1_PTR curr_poly = rend_list.m_poly_ptrs[poly];
				
				if((curr_poly == NULL) || !(curr_poly->m_state & POLY4DV1_STATE_ACTIVE)
					||(curr_poly->m_state & POLY4DV1_STATE_CLIPPED)||(curr_poly->m_state & POLY4DV1_STATE_BACKFACE))
					continue;
				
				for(vertex=0;vertex<3;vertex++)
				{
					math3d.Mat_Mul_4D_44(curr_poly->m_vlist[vertex],mt,curr_poly->m_tvlist[vertex]);
					assert(curr_poly->m_vlist[vertex] == curr_poly->m_tvlist[vertex]);
					
				}
			}
		}break;
	default:break;
	}//end switch
	return 1;
}
