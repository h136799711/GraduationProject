
#pragma once

#include "ObjectDef.h"
#include "Matrix.h"
#include "Math3D.h"


#define CULL_OBJECT_X_PLANE		0X0001//根据左右裁剪面进行剔除
#define CULL_OBJECT_Y_PLANE		0X0002//根据上下裁剪面进行剔除
#define CULL_OBJECT_Z_PLANE		0X0004//根据远近裁剪面进行剔除
#define CULL_OBJECT_XYZ_PLANE   (CULL_OBJECT_X_PLANE | CULL_OBJECT_Y_PLANE |CULL_OBJECT_Z_PLANE)
class CRenderPipeline3DV1
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
	int World_To_Camera_Object(CObject4DV1& obj,CCamera4DV1 cam);
	/*
	世界坐标到相机坐标的变换 --- 渲染列表
	*/
	int World_To_Camera_RenderList(CRenderList4DV1& rendlist,CCamera4DV1 cam);
	/*
	相机坐标到透视坐标的变换 --- 物体
	*/
	int Camera_To_Perspective_Object(CObject4DV1& obj,CCamera4DV1 cam)
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
	int Camera_To_Perspective_Renderlist(CRenderList4DV1& rend_list,CCamera4DV1 cam)
	{
		//这个函数不是基于矩阵的
		for(int poly=0;poly < rend_list.m_polys;poly++)
		{
			CPolyF4DV1* curr_poly = rend_list.m_poly_ptrs[poly];
			if((curr_poly == NULL) || !(curr_poly->m_state & POLY4DV1_STATE_ACTIVE) ||
				(curr_poly->m_state & POLY4DV1_STATE_CLIPPED) || (curr_poly->m_state & POLY4DV1_STATE_BACKFACE))
				continue;
			for(int vertex= 0;vertex<3;vertex++)
			{
				float z = curr_poly->m_tvlist[vertex].GetZ();
				
				curr_poly->m_tvlist[vertex].SetX(cam.m_view_dist * curr_poly->m_tvlist[vertex].GetX() / z );
				curr_poly->m_tvlist[vertex].SetY(cam.m_view_dist * curr_poly->m_tvlist[vertex].GetY() / z );
				
			}
		}
		return 1;
	}
	/*
	透视坐标到屏幕坐标的变换 --- 物体
	这个方法基于实验目的而编写
	可能不会将物体保留到此阶段，
	*/
	int Perspective_To_Screen_Object(CObject4DV1& obj,CCamera4DV1 cam)
	{
		float alpha = 0.5*cam.m_viewport_width - 0.5;
		float beta  = 0.5*cam.m_viewport_height - 0.5;
		
		for(int vertex=0;vertex<obj.m_vertices;vertex++)
		{
			obj.m_vlist_trans[vertex].SetX(alpha + alpha * obj.m_vlist_trans[vertex].GetX());
			obj.m_vlist_trans[vertex].SetY(beta - beta * obj.m_vlist_trans[vertex].GetY());
		}
		
		return 1;
	}
	
	/*
	透视坐标到屏幕坐标的变换 --- 渲染列表
	*/
	int Perspective_To_Screen_Rendlist(CRenderList4DV1& rend_list,CCamera4DV1 cam)
	{
		for(int poly=0;poly<rend_list.m_polys;poly++)
		{
			CPolyF4DV1* curr_poly = rend_list.m_poly_ptrs[poly];
			
			if((curr_poly == NULL) || !(curr_poly->m_state & POLY4DV1_STATE_ACTIVE)
				|| (curr_poly->m_state & POLY4DV1_STATE_CLIPPED)
				|| (curr_poly->m_state & POLY4DV1_STATE_BACKFACE))
				continue;
			
			float alpha = 0.5*cam.m_viewport_width - 0.5;
			float beta  = 0.5*cam.m_viewport_height - 0.5;
			for(int vertex=0;vertex<3;vertex++)
			{
				curr_poly->m_tvlist[vertex].SetX(alpha + alpha*curr_poly->m_tvlist[vertex].GetX());
				curr_poly->m_tvlist[vertex].SetY(alpha + alpha*curr_poly->m_tvlist[vertex].GetY());
				
			}
		}
		
		
		return 1;
	}
	/*
	相机坐标直接到屏幕坐标的变换 -- 物体级别
	*/
	int Camera_To_Perspective_Screen_Object(CObject4DV1& obj,CCamera4DV1 cam)
	{
		float alpha = (0.5*cam.m_viewport_width - 0.5);
		float beta  = (0.5*cam.m_viewport_height - 0.5);
		
		for(int vertex=0;vertex<obj.m_vertices;vertex++)
		{
			float z = obj.m_vlist_trans[vertex].GetZ();
			obj.m_vlist_trans[vertex].SetX(cam.m_view_dist * obj.m_vlist_trans[vertex].GetX() / z);
			obj.m_vlist_trans[vertex].SetY(cam.m_view_dist * obj.m_vlist_trans[vertex].GetY() * cam.m_aspect_ratio / z);
			
			obj.m_vlist_trans[vertex].SetX(alpha + obj.m_vlist_trans[vertex].GetX());
			obj.m_vlist_trans[vertex].SetY(beta - obj.m_vlist_trans[vertex].GetY());
		}
		
		return 1;
	}
	/*
	相机坐标直接到屏幕坐标的变换 -- 渲染列表级别
	*/
	int Camera_To_Perspective_Screen_Renderlist(CRenderList4DV1& rend_list,CCamera4DV1 cam)
	{
		for(int poly=0;poly<rend_list.m_polys;poly++)
		{
			CPolyF4DV1* curr_poly = rend_list.m_poly_ptrs[poly];
			
			if((curr_poly == NULL) || !(curr_poly->m_state == POLY4DV1_STATE_ACTIVE)
				||(curr_poly->m_state & POLY4DV1_STATE_CLIPPED)
				||(curr_poly->m_state & POLY4DV1_STATE_BACKFACE))
				continue;
			
			float alpha = 0.5*cam.m_viewport_width -0.5;
			float beta  = 0.5*cam.m_viewport_height -0.5;
			
			for(int vertex=0;vertex<3;vertex++)
			{
				float z = curr_poly->m_tvlist[vertex].GetZ();
				
				curr_poly->m_tvlist[vertex].SetX(curr_poly->m_tvlist[vertex].GetX()*cam.m_view_dist / z);
				curr_poly->m_tvlist[vertex].SetY(curr_poly->m_tvlist[vertex].GetY()*cam.m_view_dist / z);
				
				curr_poly->m_tvlist[vertex].SetX(curr_poly->m_tvlist[vertex].GetX() + alpha);
				curr_poly->m_tvlist[vertex].SetY(curr_poly->m_tvlist[vertex].GetX() + beta);
			}
		}
		return 1;
	}
	/*
	执行剔除物体操作，物体被一球包围着用作物体的体积
	*/
	int Cull_Object(CObject4DV1& obj,CCamera4DV1 cam,int cull_flags);
	/*
	物体的背面剔除,在物体剔除之后和世界坐标到相机坐标变换之前进行调用
	*/
	void Remove_Backfaces_Object(CObject4DV1& obj,CCamera4DV1 cam)
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
	/*
	渲染列表级别的背面剔除
	*/
	void Remove_Backfaces_Renderlist(CRenderList4DV1& rend_list,CCamera4DV1 cam)
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
	建立透视坐标到屏幕坐标的变换矩阵 
	这里假定要对齐次坐标进行变换，并在光栅化期间执行4D齐次坐标到3D坐标转换
	假定透视坐标是归一化的，即视平面大小为2X2
	x坐标的取值范围到-1到1，y坐标的取值范围到-1/ar 到 1/ar
	*/
	void Build_Perspective_To_Screen_4D_Matrix44(CCamera4DV1 cam,CMatrix44& m44)
	{
		float alpha = 0.5*cam.m_viewport_width - 0.5;
		float beta  = 0.5*cam.m_viewport_height - 0.5;
		
		m44.M[0][0] = alpha;m44.M[0][1] = 0;m44.M[0][2] = 0;m44.M[0][3] = 0;
		
		m44.M[1][0] = 0;m44.M[1][1] = -beta;m44.M[1][2] = 0;m44.M[1][3] = 0;
		
		m44.M[2][0] = alpha;m44.M[2][1] = beta;m44.M[2][2] = 1;m44.M[2][3] = 1;
		
		m44.M[3][0] = 0;m44.M[3][1] = 0;m44.M[3][2] = 0;m44.M[3][3] = 0;
		
	}
	/*
	建立透视坐标到屏幕坐标的变换矩阵
	这个方法假定透视坐标已经是3D坐标
	*/
	void Build_Perspective_To_Screen_Matrix44(CCamera4DV1 cam,CMatrix44& m44)
	{
		float alpha = 0.5*cam.m_viewport_width - 0.5;
		float beta  = 0.5*cam.m_viewport_height - 0.5;
		
		m44.M[0][0] = alpha;m44.M[0][1] = 0;m44.M[0][2] = 0;m44.M[0][3] = 0;
		
		m44.M[1][0] = 0;m44.M[1][1] = -beta;m44.M[1][2] = 0;m44.M[1][3] = 0;
		
		m44.M[2][0] = alpha;m44.M[2][1] = beta;m44.M[2][2] = 1;m44.M[2][3] = 1;
		
		m44.M[3][0] = 0;m44.M[3][1] = 0;m44.M[3][2] = 0;m44.M[3][3] = 0;
		
	}
	
	/*
		欧拉角度
	*/
	void Build_XYZ_Rotation_Matrix44(float theta_x, 
		float theta_y, 
		float theta_z,
		CMatrix44& mrot)  
	{
		// this helper function takes a set if euler angles and computes
		// a rotation matrix from them, usefull for object and camera
		// work, also  we will do a little testing in the function to determine
		// the rotations that need to be performed, since there's no
		// reason to perform extra matrix multiplies if the angles are
		// zero!
		
		CMatrix44 mx, my, mz, mtmp;       // working matrices
		float sin_theta=0, cos_theta=0;   // used to initialize matrices
		int rot_seq = 0;                  // 1 for x, 2 for y, 4 for z
		
		// step 0: fill in with identity matrix
		(mrot).Identify();
		
		// step 1: based on zero and non-zero rotation angles, determine
		// rotation sequence
		if (fabs(theta_x) > EPSILON_E5) // x
			rot_seq = rot_seq | 1;
		
		if (fabs(theta_y) > EPSILON_E5) // y
			rot_seq = rot_seq | 2;
		
		if (fabs(theta_z) > EPSILON_E5) // z
			rot_seq = rot_seq | 4;
		
		// now case on sequence
		switch(rot_seq)
		{
		case 0: // no rotation
			{
				// what a waste!
				return;
			} break;
			
		case 1: // x rotation
			{
				// compute the sine and cosine of the angle
				cos_theta = math3d.Fast_Cos(theta_x);
				sin_theta = math3d.Fast_Sin(theta_x);
				
				mx.M[0][0] = 1;mx.M[0][1] = 0;mx.M[0][2] = 0;mx.M[0][3] = 0;
				mx.M[1][0] = 0;mx.M[1][1] = cos_theta;mx.M[1][2] = sin_theta;mx.M[1][3] = 0;
				mx.M[2][0] = 0;mx.M[2][1] = -sin_theta;mx.M[2][2] = cos_theta;mx.M[2][3] = 0;
				mx.M[3][0] = 0;mx.M[3][1] = 0;mx.M[3][2] = 0;mx.M[3][3] = 1;



				// that's it, copy to output matrix
				mx =  mrot;
				return;
				
			} break;
			
		case 2: // y rotation
			{
				// compute the sine and cosine of the angle
				cos_theta = math3d.Fast_Cos(theta_y);
				sin_theta = math3d.Fast_Sin(theta_y);
				
				my.M[0][0] = cos_theta;my.M[0][1] = 0;my.M[0][2] = -sin_theta;my.M[0][3] = 0;
				my.M[1][0] = 0;my.M[1][1] = 1;my.M[1][2] = 0;my.M[1][3] = 0;
				my.M[2][0] = sin_theta;my.M[2][1] = 0;my.M[2][2] = cos_theta;my.M[2][3] = 0;
				my.M[3][0] = 0;my.M[3][1] = 0;my.M[3][2] = 0;my.M[3][3] = 1;


				
				// that's it, copy to output matrix
				my = mrot;
				return;
				
			} break;
			
		case 3: // xy rotation
			{
				// compute the sine and cosine of the angle for x
				cos_theta = math3d.Fast_Cos(theta_x);
				sin_theta = math3d.Fast_Sin(theta_x);
				
				mx.M[0][0] = 1;mx.M[0][1] = 0;mx.M[0][2] = 0;mx.M[0][3] = 0;
				mx.M[1][0] = 0;mx.M[1][1] = cos_theta;mx.M[1][2] = sin_theta;mx.M[1][3] = 0;
				mx.M[2][0] = 0;mx.M[2][1] = -sin_theta;mx.M[2][2] = cos_theta;mx.M[2][3] = 0;
				mx.M[3][0] = 0;mx.M[3][1] = 0;mx.M[3][2] = 0;mx.M[3][3] = 1;
			

				
				// compute the sine and cosine of the angle for y
				cos_theta = math3d.Fast_Cos(theta_y);
				sin_theta = math3d.Fast_Sin(theta_y);
				
				my.M[0][0] = cos_theta;my.M[0][1] = 0;my.M[0][2] = -sin_theta;my.M[0][3] = 0;
				my.M[1][0] = 0;my.M[1][1] = 1;my.M[1][2] = 0;my.M[1][3] = 0;
				my.M[2][0] = sin_theta;my.M[2][1] = 0;my.M[2][2] = cos_theta;my.M[2][3] = 0;
				my.M[3][0] = 0;my.M[3][1] = 0;my.M[3][2] = 0;my.M[3][3] = 1;

				
				// concatenate matrices 
				mrot = mx * my;
				
				return;
				
			} break;
			
		case 4: // z rotation
			{
				// compute the sine and cosine of the angle
				cos_theta = math3d.Fast_Cos(theta_z);
				sin_theta = math3d.Fast_Sin(theta_z);
				
				mz.M[0][0] = cos_theta;mz.M[0][1] = sin_theta;mz.M[0][2] = 0;mz.M[0][3] = 0;
				mz.M[1][0] = -sin_theta;mz.M[1][1] = cos_theta;mz.M[1][2] = 0;mz.M[1][3] = 0;
				mz.M[2][0] = 0;mz.M[2][1] = 0;mz.M[2][2] = 1;mz.M[2][3] = 0;
				mz.M[3][0] = 0;mz.M[3][1] = 0;mz.M[3][2] = 0;mz.M[3][3] = 1;

				
				
				// that's it, copy to output matrix
				
				mrot = mz;
				return;
				
			} break;
			
		case 5: // xz rotation
			{
				// compute the sine and cosine of the angle x
				cos_theta = math3d.Fast_Cos(theta_x);
				sin_theta = math3d.Fast_Sin(theta_x);
				
				mx.M[0][0] = 1;mx.M[0][1] = 0;mx.M[0][2] = 0;mx.M[0][3] = 0;
				mx.M[1][0] = 0;mx.M[1][1] = cos_theta;mx.M[1][2] = sin_theta;mx.M[1][3] = 0;
				mx.M[2][0] = 0;mx.M[2][1] = -sin_theta;mx.M[2][2] = cos_theta;mx.M[2][3] = 0;
				mx.M[3][0] = 0;mx.M[3][1] = 0;mx.M[3][2] = 0;mx.M[3][3] = 1;

				
				// compute the sine and cosine of the angle z
				cos_theta = math3d.Fast_Cos(theta_z);
				sin_theta = math3d.Fast_Sin(theta_z);
				
				mz.M[0][0] = cos_theta;mz.M[0][1] = sin_theta;mz.M[0][2] = 0;mz.M[0][3] = 0;
				mz.M[1][0] = -sin_theta;mz.M[1][1] = cos_theta;mz.M[1][2] = 0;mz.M[1][3] = 0;
				mz.M[2][0] = 0;mz.M[2][1] = 0;mx.M[2][2] = 1;mz.M[2][3] = 0;
				mz.M[3][0] = 0;mz.M[3][1] = 0;mx.M[3][2] = 0;mz.M[3][3] = 1;


				
				// concatenate matrices 

				mrot = mx * mz;
				return;
				
			} break;
			
		case 6: // yz rotation
			{
				// compute the sine and cosine of the angle y
				cos_theta = math3d.Fast_Cos(theta_y);
				sin_theta = math3d.Fast_Sin(theta_y);
				my.M[0][0] = cos_theta;my.M[0][1] = 0;my.M[0][2] = -sin_theta;my.M[0][3] = 0;
				my.M[1][0] = 0;my.M[1][1] = 1;my.M[1][2] = 0;my.M[1][3] = 0;
				my.M[2][0] = sin_theta;my.M[2][1] = 0;my.M[2][2] = cos_theta;my.M[2][3] = 0;
				my.M[3][0] = 0;my.M[3][1] = 0;my.M[3][2] = 0;my.M[3][3] = 1;

				
				// compute the sine and cosine of the angle z
				cos_theta = math3d.Fast_Cos(theta_z);
				sin_theta = math3d.Fast_Sin(theta_z);
				
				mz.M[0][0] = cos_theta;mz.M[0][1] = sin_theta;mz.M[0][2] = 0;mz.M[0][3] = 0;
				mz.M[1][0] = -sin_theta;mz.M[1][1] = cos_theta;mz.M[1][2] = 0;mz.M[1][3] = 0;
				mz.M[2][0] = 0;mz.M[2][1] = 0;mz.M[2][2] = 1;mz.M[2][3] = 0;
				mz.M[3][0] = 0;mz.M[3][1] = 0;mz.M[3][2] = 0;mz.M[3][3] = 1;
				

				

				mrot = my * mz;
				return;
				
			} break;
			
		case 7: // xyz rotation
			{
				// compute the sine and cosine of the angle x
				cos_theta = math3d.Fast_Cos(theta_x);
				sin_theta = math3d.Fast_Sin(theta_x);
				
				mx.M[0][0] = 1;mx.M[0][1] = 0;mx.M[0][2] = 0;mx.M[0][3] = 0;
				mx.M[1][0] = 0;mx.M[1][1] = cos_theta;mx.M[1][2] = sin_theta;mx.M[1][3] = 0;
				mx.M[2][0] = 0;mx.M[2][1] = -sin_theta;mx.M[2][2] = cos_theta;mx.M[2][3] = 0;
				mx.M[3][0] = 0;mx.M[3][1] = 0;mx.M[3][2] = 0;mx.M[3][3] = 1;
				
				
				
				// compute the sine and cosine of the angle y
				cos_theta = math3d.Fast_Cos(theta_y);
				sin_theta = math3d.Fast_Sin(theta_y);
				
				my.M[0][0] = cos_theta;my.M[0][1] = 0;my.M[0][2] = -sin_theta;my.M[0][3] = 0;
				my.M[1][0] = 0;my.M[1][1] = 1;my.M[1][2] = 0;my.M[1][3] = 0;
				my.M[2][0] = sin_theta;my.M[2][1] = 0;my.M[2][2] = cos_theta;my.M[2][3] = 0;
				my.M[3][0] = 0;my.M[3][1] = 0;my.M[3][2] = 0;my.M[3][3] = 1;
				

				
				// compute the sine and cosine of the angle z
				cos_theta = math3d.Fast_Cos(theta_z);
				sin_theta = math3d.Fast_Sin(theta_z);
				
				mz.M[0][0] = cos_theta;mz.M[0][1] = sin_theta;mz.M[0][2] = 0;mz.M[0][3] = 0;
				mz.M[1][0] = -sin_theta;mz.M[1][1] = cos_theta;my.M[1][2] = 0;my.M[1][3] = 0;
				mz.M[2][0] = 0;mz.M[2][1] = 0;mz.M[2][2] = 1;mz.M[2][3] = 0;
				mz.M[3][0] = 0;mz.M[3][1] = 0;mz.M[3][2] = 0;mz.M[3][3] = 1;
				

				// concatenate matrices, watch order!

				mtmp = mx * my;
				mrot = mtmp * mz;
				
			} break;
			
		default: break;
			
      } // end switch
	  
} // end Build_XYZ_Rotation_MATRIX4X4                                    








  /*
		将物体的坐标从齐次坐标转换为真正的3D坐标 -- 物体级
		*/
		void Convert_From_Homogeneous4D_Object(CObject4DV1& obj)
		{
			for(int vertex=0;vertex<obj.m_polys;vertex++)
			{
				assert(obj.m_vlist_trans[vertex].DivByW() == 1);
			}
		}
		/*
		将多边形的坐标从齐次坐标转换为非齐次的坐标 -- 列表级别
		*/
		void Convert_From_Homogeneous4D_Renderlist(CRenderList4DV1 & rend_list)
		{
			for(int poly=0;poly<rend_list.m_polys;poly++)
			{
				CPolyF4DV1* curr_poly = rend_list.m_poly_ptrs[poly];
				
				if((curr_poly == NULL) || !(curr_poly->m_state & POLY4DV1_STATE_ACTIVE) ||
					(curr_poly->m_state & POLY4DV1_STATE_CLIPPED) || (curr_poly->m_state & POLY4DV1_STATE_BACKFACE))
					continue;
				
				for(int vertex=0;vertex<3;vertex++)
				{
					if(curr_poly->m_tvlist[vertex].DivByW() == 1)
					{
						printf("Convert_From_Homogeneous4D_Rendlist Faild!");
					}
				}
				
				
			}
		}
		
		
		
		CMath3D math3d;
};












int CRenderPipeline3DV1::Cull_Object(CObject4DV1& obj,CCamera4DV1 cam,int cull_flags)
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
int CRenderPipeline3DV1::World_To_Camera_RenderList(CRenderList4DV1& rendlist,CCamera4DV1 cam)
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
int CRenderPipeline3DV1::World_To_Camera_Object(CObject4DV1 & obj,CCamera4DV1 cam)
{
	for(int vertex=0;vertex<obj.m_vertices;vertex++)
	{
		CPoint4D presult;
		
		math3d.Mat_Mul_4D_44(obj.m_vlist_trans[vertex],cam.mcam,presult);
		
		obj.m_vlist_trans[vertex] = presult;
	}
	return 1;
}
int CRenderPipeline3DV1::Model_To_World_Renderlist(CRenderList4DV1& rend_list,CPoint4D world_pos,int coord_select)
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
int CRenderPipeline3DV1::Model_To_World_Object(CObject4DV1& obj,int coord_select)
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
int CRenderPipeline3DV1::Transform_Object(CObject4DV1& obj,CMatrix44 mt,int coord_select ,int transform_basis)
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
int CRenderPipeline3DV1::Transform_Renderlist(CRenderList4DV1& rend_list,CMatrix44 mt,int coord_select)	
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
