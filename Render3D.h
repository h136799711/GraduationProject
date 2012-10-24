
#pragma once

#include "ObjectDef.h"
#include "Matrix.h"
#include "Math3D.h"


#define CULL_OBJECT_X_PLANE		0X0001//�������Ҳü�������޳�
#define CULL_OBJECT_Y_PLANE		0X0002//�������²ü�������޳�
#define CULL_OBJECT_Z_PLANE		0X0004//����Զ���ü�������޳�
#define CULL_OBJECT_XYZ_PLANE   (CULL_OBJECT_X_PLANE | CULL_OBJECT_Y_PLANE |CULL_OBJECT_Z_PLANE)
class CRender3DV1
{
public:
	
	/*
		��Ⱦ�б��ͨ�ñ任
		mt: �任����
		coord_select:��α任����
	*/
	int Transform_Renderlist(CRenderList4DV1& rend_list,CMatrix44 mt,int coord_select);
	/*
		�����ͨ�ñ任
		mt: �任����
		coord_select:��α任����
	*/
	int Transform_Object(CObject4DV1& obj,CMatrix44 mt,int coord_select,int tranform_basis);
	/*
		�ֲ����굽��������ı任 --- ����
	*/
	int Model_To_World_Object(CObject4DV1& obj,int coord_select=TRANSFORM_LOCAL_TO_TRANS);
	/*
		�ֲ����굽��������ı任 --- ��Ⱦ�б�
	*/
	int Model_To_World_Renderlist(CRenderList4DV1& rend_list,CPoint4D world_pos,int coord_select=TRANSFORM_LOCAL_TO_TRANS);
	/*
		�������굽�������ı任 --- ����
	*/
	int World_To_Camera_Object(CCamera4DV1 cam,CObject4DV1& obj);
	/*
		�������굽�������ı任 --- ��Ⱦ�б�
	*/
	int World_To_Camera_RenderList(CRenderList4DV1& rendlist,CCamera4DV1 cam);
	/*
		������굽͸������ı任 --- ����
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
		������굽͸������ı任 --- ��Ⱦ�б�
	*/
	/*
		͸�����굽��Ļ����ı任 --- ����
	*/
	/*
		͸�����굽��Ļ����ı任 --- ��Ⱦ�б�
	*/

	/*
		ִ���޳�������������屻һ���Χ��������������
	*/
	int Cull_Object4DV1(CObject4DV1& obj,CCamera4DV1 cam,int cull_flags);
	/*
		���������״̬���
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
		����ı����޳�,�������޳�֮����������굽�������任֮ǰ���е���
	*/
	void Remove_Backfaces_Object4DV1(CObject4DV1& obj,CCamera4DV1 cam)
	{
		//�����Ƿ��Ѿ����޳�
		if(obj.m_state & OBJECT4DV1_STATE_CULLED)
			return ;
		for(int poly=0;poly< obj.m_polys;poly++)
		{
			CPoly4DV1* curr_poly = &obj.m_plist[poly];

			//�ж϶�����Ƿ�û�б��ü�����û�б��޳������ڻ״̬�����ڻ״̬�ģ��ɼ���˫���
			if(!(curr_poly->m_state & POLY4DV1_STATE_ACTIVE) || 
				(curr_poly->m_state & POLY4DV1_STATE_CLIPPED) ||
				(curr_poly->m_attr & POLY4DV1_ATTR_2SIDED) ||
				(curr_poly->m_state & POLY4DV1_STATE_BACKFACE))
				continue;

			//��ȡ�����б��еĶ�������
			int vindex_0 = curr_poly->m_vert[0];
			int vindex_1 = curr_poly->m_vert[1];
			int vindex_2 = curr_poly->m_vert[2];

			//ʹ�ñ任��Ķ�����б�
			CVector4D u,v,n;

			//����u��v
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
		�����ֲ����굽��������ı任����
	*/
	void Build_Model_To_Word_Matrix44(CVector4D vpos,CMatrix44& m)
	{
		m.M[0][0] = 1;m.M[0][1] = 0;m.M[0][2] = 0;m.M[0][3] = 0;
		m.M[1][0] = 0;m.M[1][1] = 1;m.M[1][2] = 0;m.M[1][3] = 0;
		m.M[2][0] = 0;m.M[2][1] = 0;m.M[2][2] = 1;m.M[2][3] = 0;
		m.M[3][0] = vpos.GetX();m.M[3][1] = vpos.GetY();m.M[3][2] = vpos.GetZ();m.M[3][3] = 1;
	}

	/*
		��������任��͸�ӱ任�ľ���
	*/
	void Build_Camera_To_Perspective_Matrix44(CCamera4DV1 cam,CMatrix44& m44)
	{
		m44.M[0][0] = cam.m_view_dist;m44.M[0][1] = 0;m44.M[0][2] = 0;m44.M[0][3] = 0;
		
		m44.M[1][0] = 0;m44.M[1][1] = cam.m_view_dist * cam.m_aspect_ratio;m44.M[1][2] = 0;m44.M[1][3] = 0;
		
		m44.M[2][0] = 0;m44.M[2][1] = 0;m44.M[2][2] = 1;m44.M[2][3] = 1;
		
		m44.M[3][0] = 0;m44.M[3][1] = 0;m44.M[3][2] = 0;m44.M[3][3] = 0;
		
	}
	/*
		�������������������ת��Ϊ������3D����
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
	//��������ǻ��ھ����
	//�����ݴ���������Ϣ�ж������Ƿ����Ӿ�����
	//����cull_flagsָ������Щ����ִ���޳�
	
	//step1: 	ת�������Χ�������Ϊ�������
	CPoint4D sphere_pos;

	math3d.Mat_Mul_4D_44(obj.m_world_pos,cam.mcam,sphere_pos);
	
	//step2:�����޳���Ƕ�����ִ���޳�����
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
		//ʹ������ü������Χ��������ߺ����ұ�
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
	//����δʹ�þ���任
	//�����ݽ�������Ⱦ�б�ľֲ�/ģ������任Ϊ��������
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
	//�Ƿ�Ҫ�Գ����������б任
	if(transform_basis)
	{
		//��ת����ĳ�������
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
