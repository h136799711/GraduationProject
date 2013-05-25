#pragma once

#include "HCLib\HCDDrawLib.h"
#include "HCLib\HCLog.h"


class CDraw3DV1:public HCDDrawLib
{
public:
	
	/*
		绘制线框- 渲染列表级别
		之前要先设置裁剪矩形，不然裁剪矩形为默认
	*/
	void Draw_RenderList4DV1_Wire16(CRenderList4DV1& rend_list,UCHAR* video_buffer,int lpitch)
	{
		for(int poly=0;poly<rend_list.m_polys;poly++)
		{
					
			
			if(!(rend_list.m_poly_ptrs[poly]->m_state & POLY4DV1_STATE_ACTIVE)
				|| (rend_list.m_poly_ptrs[poly]->m_state & POLY4DV1_STATE_CLIPPED)
				|| (rend_list.m_poly_ptrs[poly]->m_state & POLY4DV1_STATE_BACKFACE))
				continue;
			
			
			Draw_Clip_Line16(rend_list.m_poly_ptrs[poly]->m_tvlist[0].GetX(),
				rend_list.m_poly_ptrs[poly]->m_tvlist[0].GetY(),
				rend_list.m_poly_ptrs[poly]->m_tvlist[1].GetX(),
				rend_list.m_poly_ptrs[poly]->m_tvlist[1].GetY(),
				rend_list.m_poly_ptrs[poly]->m_color,
				video_buffer,lpitch);
			Draw_Clip_Line16(rend_list.m_poly_ptrs[poly]->m_tvlist[1].GetX(),
				rend_list.m_poly_ptrs[poly]->m_tvlist[1].GetY(),
				rend_list.m_poly_ptrs[poly]->m_tvlist[2].GetX(),
				rend_list.m_poly_ptrs[poly]->m_tvlist[2].GetY(),
				rend_list.m_poly_ptrs[poly]->m_color,
				video_buffer,lpitch);
			Draw_Clip_Line16(rend_list.m_poly_ptrs[poly]->m_tvlist[2].GetX(),
				rend_list.m_poly_ptrs[poly]->m_tvlist[2].GetY(),
				rend_list.m_poly_ptrs[poly]->m_tvlist[0].GetX(),
				rend_list.m_poly_ptrs[poly]->m_tvlist[0].GetY(),
				rend_list.m_poly_ptrs[poly]->m_color,
				video_buffer,lpitch);
			
				/*
				
				  Draw_Line16(rend_list.m_poly_ptrs[poly]->m_tvlist[0].GetX(),
				  rend_list.m_poly_ptrs[poly]->m_tvlist[0].GetY(),
				  rend_list.m_poly_ptrs[poly]->m_tvlist[1].GetX(),
				  rend_list.m_poly_ptrs[poly]->m_tvlist[1].GetY(),
				  rend_list.m_poly_ptrs[poly]->m_color,
				  video_buffer,lpitch);
				  Draw_Line16(rend_list.m_poly_ptrs[poly]->m_tvlist[1].GetX(),
				  rend_list.m_poly_ptrs[poly]->m_tvlist[1].GetY(),
				  rend_list.m_poly_ptrs[poly]->m_tvlist[2].GetX(),
				  rend_list.m_poly_ptrs[poly]->m_tvlist[2].GetY(),
				  rend_list.m_poly_ptrs[poly]->m_color,
				  video_buffer,lpitch);
				  Draw_Line16(rend_list.m_poly_ptrs[poly]->m_tvlist[2].GetX(),
				  rend_list.m_poly_ptrs[poly]->m_tvlist[2].GetY(),
				  rend_list.m_poly_ptrs[poly]->m_tvlist[0].GetX(),
				  rend_list.m_poly_ptrs[poly]->m_tvlist[0].GetY(),
				  rend_list.m_poly_ptrs[poly]->m_color,
				  video_buffer,lpitch);
			*/
		}
	}
	/*
	绘制CObject4DV1物体
	*/
	void Draw_Object_Wire16(CObject4DV1& obj,UCHAR* video_buffer,int lpitch)
	{
		for(int poly=0;poly<obj.m_polys;poly++)
		{
			if(!(obj.m_plist[poly].m_state & POLY4DV1_STATE_ACTIVE)
				|| (obj.m_plist[poly].m_state & POLY4DV1_STATE_CLIPPED)
				|| (obj.m_plist[poly].m_state & POLY4DV1_STATE_BACKFACE))
				continue;
			
			int vindex_0 = obj.m_plist[poly].m_vert[0];
			int vindex_1 = obj.m_plist[poly].m_vert[1];
			int vindex_2 = obj.m_plist[poly].m_vert[2];
			
			this->Draw_Clip_Line16(obj.m_vlist_trans[vindex_0].GetX(),
				obj.m_vlist_trans[vindex_0].GetY(),
				obj.m_vlist_trans[vindex_1].GetX(),
				obj.m_vlist_trans[vindex_1].GetY(),
				obj.m_plist[poly].m_color,
				video_buffer,lpitch);
			this->Draw_Clip_Line16(obj.m_vlist_trans[vindex_1].GetX(),
				obj.m_vlist_trans[vindex_1].GetY(),
				obj.m_vlist_trans[vindex_2].GetX(),
				obj.m_vlist_trans[vindex_2].GetY(),
				obj.m_plist[poly].m_color,
				video_buffer,lpitch);
			this->Draw_Clip_Line16(obj.m_vlist_trans[vindex_2].GetX(),
				obj.m_vlist_trans[vindex_2].GetY(),
				obj.m_vlist_trans[vindex_0].GetX(),
				obj.m_vlist_trans[vindex_0].GetY(),
				obj.m_plist[poly].m_color,
				video_buffer,lpitch);
		}
	}
	
	/**
	* 绘制物体（8位）
	*/
	void Draw_Object4DV1_Solid(CObject4DV1& obj,UCHAR * video_buffer,int lpitch)
	{
		for (int poly = 0; poly < obj.m_polys; poly++)
		{
			//可见,处于活动状态,没有被剔除掉
			if(!(obj.m_plist[poly].m_state & POLY4DV1_STATE_ACTIVE) || 
				(obj.m_plist[poly].m_state & POLY4DV1_STATE_CLIPPED) ||
				(obj.m_plist[poly].m_state & POLY4DV1_STATE_BACKFACE))
				continue;
			//三角形的三个顶点在物体顶点列表中的索引
			int vindex_0 = obj.m_plist[poly].m_vert[0];
			int vindex_1 = obj.m_plist[poly].m_vert[1];
			int vindex_2 = obj.m_plist[poly].m_vert[2];
			
			Draw_Triangle_2D(obj.m_vlist_trans[vindex_0] .GetX(),obj.m_vlist_trans[vindex_0] .GetY(),
				obj.m_vlist_trans[vindex_1] .GetX(),obj.m_vlist_trans[vindex_1] .GetY(),
				obj.m_vlist_trans[vindex_2] .GetX(),obj.m_vlist_trans[vindex_2] .GetY(),obj.m_plist[poly].m_color,video_buffer,lpitch);
			
		}
		
		
	}
	
	/**
	* 绘制渲染列表（8位）
	*/
	void Draw_RenderList4DV1_Solid(CRenderList4DV1& rend_list, UCHAR *video_buffer, int lpitch)
	{
		
		for (int poly=0; poly < rend_list.m_polys; poly++)
		{
			
			
			if (!(rend_list.m_poly_ptrs[poly]->m_state & POLY4DV1_STATE_ACTIVE) ||
				(rend_list.m_poly_ptrs[poly]->m_state & POLY4DV1_STATE_CLIPPED ) ||
				(rend_list.m_poly_ptrs[poly]->m_state & POLY4DV1_STATE_BACKFACE) )
				continue; 
			
			
			Draw_Triangle_2D(rend_list.m_poly_ptrs[poly]->m_tvlist[0].GetX(),rend_list.m_poly_ptrs[poly]->m_tvlist[0].GetY(),
				rend_list.m_poly_ptrs[poly]->m_tvlist[1].GetX(), rend_list.m_poly_ptrs[poly]->m_tvlist[1].GetY(),
				rend_list.m_poly_ptrs[poly]->m_tvlist[2].GetX(), rend_list.m_poly_ptrs[poly]->m_tvlist[2].GetY(),
				rend_list.m_poly_ptrs[poly]->m_color, video_buffer, lpitch);
			
		} 
		
	} /**
	* 绘制物体（16位）
	*/
	void Draw_Object4DV1_Solid16(CObject4DV1& obj,UCHAR * video_buffer,int lpitch)
	{
		for (int poly = 0; poly < obj.m_polys; poly++)
		{
			//可见,处于活动状态,没有被剔除掉
			if(!(obj.m_plist[poly].m_state & POLY4DV1_STATE_ACTIVE) || 
				(obj.m_plist[poly].m_state & POLY4DV1_STATE_CLIPPED) ||
				(obj.m_plist[poly].m_state & POLY4DV1_STATE_BACKFACE))
				continue;
			//三角形的三个顶点在物体顶点列表中的索引
			int vindex_0 = obj.m_plist[poly].m_vert[0];
			int vindex_1 = obj.m_plist[poly].m_vert[1];
			int vindex_2 = obj.m_plist[poly].m_vert[2];
			
			Draw_Triangle_2D16(obj.m_vlist_trans[vindex_0] .GetX(),obj.m_vlist_trans[vindex_0] .GetY(),
				obj.m_vlist_trans[vindex_1] .GetX(),obj.m_vlist_trans[vindex_1] .GetY(),
				obj.m_vlist_trans[vindex_2] .GetX(),obj.m_vlist_trans[vindex_2] .GetY(),obj.m_plist[poly].m_color,video_buffer,lpitch);
			
		}
		
		
	}
	
	/**
	* 绘制渲染列表（16位）
	*/
	void Draw_RenderList4DV1_Solid16(CRenderList4DV1& rend_list, UCHAR *video_buffer, int lpitch)
	{
		
		for (int poly=0; poly < rend_list.m_polys; poly++)
		{
			
			
			if (!(rend_list.m_poly_ptrs[poly]->m_state & POLY4DV1_STATE_ACTIVE) ||
				(rend_list.m_poly_ptrs[poly]->m_state & POLY4DV1_STATE_CLIPPED ) ||
				(rend_list.m_poly_ptrs[poly]->m_state & POLY4DV1_STATE_BACKFACE) )
				continue; 
			
			
			Draw_Triangle_2D16(rend_list.m_poly_ptrs[poly]->m_tvlist[0].GetX(), rend_list.m_poly_ptrs[poly]->m_tvlist[0].GetY(),
				rend_list.m_poly_ptrs[poly]->m_tvlist[1].GetX(), rend_list.m_poly_ptrs[poly]->m_tvlist[1].GetY(),
				rend_list.m_poly_ptrs[poly]->m_tvlist[2].GetX(), rend_list.m_poly_ptrs[poly]->m_tvlist[2].GetY(),
				rend_list.m_poly_ptrs[poly]->m_color, video_buffer, lpitch);
			
		} 
		
	} 
	
};




