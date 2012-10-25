#pragma once

#include "HCLib\HCDDrawLib.h"


class CDraw3DV1:public HCDDrawLib
{
public:
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
		}
	}
	/*
		ªÊ÷∆CObject4DV1ŒÔÃÂ
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
};




