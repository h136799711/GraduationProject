#pragma once

#include "Vector.h"
#include "Geom.h"
#include "Matrix.h"
#include "HCLib\HCMacros.h"
#include "Math3D.h"
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
	
	CPoint4D* m_vlist;//顶点列表，基于物体列表
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

#define OBJECT4DV1_MAX_VERTICES           64  // 64
#define OBJECT4DV1_MAX_POLYS              128 // 128

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
	/*
		重置物体的状态标记
	*/
	void Reset_Object(CObject4DV1& obj)
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
	
	
	
};

//本地有时不行，跟数值有关系
#define RENDERLIST4DV1_MAX_POLYS	8192  //32768

#define TRANSFORM_LOCAL_ONLY		0X0001
#define TRANSFORM_TRANS_ONLY		0X0002
#define TRANSFORM_LOCAL_TO_TRANS	0X0004

//渲染列表定义
class CRenderList4DV1
{
public :
	
	void Reset_RenderListV1()
	{
		m_polys = 0;
	}
	int Insert_Poly4DV1(CPoly4DV1 poly)
	{
		if(m_polys >= RENDERLIST4DV1_MAX_POLYS)
			return 0;
		
		m_poly_ptrs[m_polys] = &m_poly_data[m_polys];
		
		m_poly_data[m_polys].m_state = poly.m_state;
		m_poly_data[m_polys].m_attr  = poly.m_attr;
		m_poly_data[m_polys].m_color = poly.m_color;
		
		m_poly_data[m_polys].m_tvlist[0] = poly.m_vlist[poly.m_vert[0]];
		
		m_poly_data[m_polys].m_tvlist[1] = poly.m_vlist[poly.m_vert[1]];
		
		m_poly_data[m_polys].m_tvlist[2] = poly.m_vlist[poly.m_vert[2]];
		
		m_poly_data[m_polys].m_vlist[0] = poly.m_vlist[poly.m_vert[0]];
		
		m_poly_data[m_polys].m_vlist[1] = poly.m_vlist[poly.m_vert[1]];
		
		m_poly_data[m_polys].m_vlist[2] = poly.m_vlist[poly.m_vert[2]];
		
		if(m_polys == 0)
		{
			m_poly_data[0].m_next = NULL;
			m_poly_data[1].m_prev = NULL;
		}
		else
		{
			m_poly_data[m_polys].m_next  = NULL;
			m_poly_data[m_polys].m_prev  = &m_poly_data[m_polys-1];
			m_poly_data[m_polys-1].m_next = &m_poly_data[m_polys];
		}
		
		m_polys ++;
		
		return 1;
	}
	
	int Insert_PolyF4DV1(CPolyF4DV1 poly)
	{
		if(m_polys >= RENDERLIST4DV1_MAX_POLYS)
			return 0;
		m_poly_ptrs[m_polys] = &m_poly_data[m_polys];
		
		memcpy((void*)&m_poly_data[m_polys],(void*)&poly,sizeof(CPolyF4DV1));
		
		if(m_polys == 0)
		{
			m_poly_data[m_polys].m_next = NULL;
			m_poly_data[m_polys].m_prev = NULL;
		}
		else
		{
			m_poly_data[m_polys].m_next = NULL;
			m_poly_data[m_polys].m_prev = &m_poly_data[m_polys-1];
			
			m_poly_data[m_polys-1].m_next = &m_poly_data[m_polys];
		}
		
		m_polys++;
		
		
		return 1;
	}
	
	int Insert_Object(CObject4DV1 obj,int insert_local)
	{
		if (!(obj.m_state & OBJECT4DV1_STATE_ACTIVE) ||
			(obj.m_state & OBJECT4DV1_STATE_CULLED) ||
			!(obj.m_state & OBJECT4DV1_STATE_VISIBLE))
			return(0); 
		for(int poly=0;poly<obj.m_polys;poly++)
		{
			CPoly4DV1* curr_poly = &obj.m_plist[poly];
			
			if (!(curr_poly->m_state & POLY4DV1_STATE_ACTIVE) ||
				(curr_poly->m_state & POLY4DV1_STATE_CLIPPED ) ||
				(curr_poly->m_state & POLY4DV1_STATE_BACKFACE) )
				continue; // move onto next poly

			CPoint4D* vlist_old = curr_poly->m_vlist;
			if(insert_local)
				curr_poly->m_vlist = obj.m_vlist_local;
			else
				curr_poly->m_vlist = obj.m_vlist_trans;

			if(!Insert_Poly4DV1(*curr_poly))
			{
				curr_poly->m_vlist = vlist_old;
				return 0;
			}
			curr_poly->m_vlist = vlist_old;
		}
		return 1;
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

//相机类型
#define CAM_MODEL_EULER		0X0001
#define CAM_MODEL_UVN		0X0002

//相机选择顺序
#define CAM_ROT_SEQ_XYZ		0X0010
#define CAM_ROT_SEQ_XZY		0X0012
#define CAM_ROT_SEQ_YXZ		0X0014
#define CAM_ROT_SEQ_YZX		0X0016
#define CAM_ROT_SEQ_ZXY		0X0017
#define CAM_ROT_SEQ_ZYX		0X0020

#define UVN_MODE_SPHERICAL	0X0001
#define UVN_MODE_SIMPLE		0X0002

//相机模型的定义
class CCamera4DV1
{
public:
	int m_state;
	int m_attr;
	
	CPoint4D m_pos;//相机在世界坐标中的位置
	CVector4D m_dir;//欧拉角度或UVN相机模型的注视方向
	
	CVector4D m_u;
	CVector4D m_v;
	CVector4D m_n;//UVN模型的朝向向量
	
	CPoint4D m_target;//UVN模型的目标位置
	
	float m_view_dist;//视距
	
	float m_fov;//水平和垂直方向的视野
	
	//3d裁剪面
	//如果视野不是90都，3D裁剪面方程将为一般性方程
	float m_near_clip_z;//近裁剪面
	float m_far_clip_z;//远裁剪面
	
	CPlane3D m_rt_clip_plane;//右裁剪面
	CPlane3D m_lt_clip_plane;//左裁剪面
	CPlane3D m_tp_clip_plane;//上裁剪面
	CPlane3D m_bt_clip_plane;//下裁剪面
	
	float m_viewplane_width;
	float m_viewplane_height;//视平面的宽度和高度，对于归一化的投影 2X2
	//否则大小与视口或屏幕窗口相同
	float m_viewport_width;
	float m_viewport_height;//视口大小
	float m_viewport_center_x;
	float m_viewport_center_y;//视口中心
	
	float m_aspect_ratio;//宽高比，屏幕的宽高比
	
	CMatrix44 mcam;//存储世界坐标到相机坐标的变换矩阵
	CMatrix44 mper;//存储相机坐标到透视坐标的变换矩阵
	CMatrix44 mscr;//存储透视坐标到屏幕坐标的变换矩阵
	
	void Init(int cam_attr,CPoint4D cam_pos,CVector4D cam_dir,CPoint4D* cam_target,
		float near_clip_z,float far_clip_z,float fov,float viewport_width,float viewport_height)
	{
		m_attr = cam_attr;
		m_pos = cam_pos;
		m_dir = cam_dir;
		
		
		m_u.Zero();
		m_v.Zero();
		m_n.Zero();
		if(cam_target == NULL)
			m_target = *cam_target;
		else
			m_target.Zero();
		m_near_clip_z = near_clip_z;
		m_far_clip_z  = far_clip_z;
		
		m_viewport_width = viewport_width;
		m_viewport_height = viewport_height;
		
		m_viewport_center_x = (viewport_width - 1)/2;
		m_viewport_center_y = (viewport_height - 1)/2;
		
		m_aspect_ratio = (float) viewport_width  / (float)viewport_height;
		
		mcam.Identify();
		mper.Identify();
		mscr.Identify();
		
		m_fov = fov;
		
		m_viewplane_width = 2.0f;
		m_viewplane_height= 2.0f/m_aspect_ratio;
		
		float tan_fov_div2 = tan(DEG_TO_RAD(fov/2));
		
		m_view_dist = (0.5) * m_viewplane_width *  tan_fov_div2;
		
		CPoint3D pt_origin;//
		CVector3D vn;//用于初始化裁剪面
		if(fov == 90.0)
		{
			pt_origin.Zero();
			//金字塔的四个面
			//右裁剪面			
			vn.SetXYZW(1.0f,0,-1.0f);//x = z 平面的法线
			m_rt_clip_plane.Init(pt_origin,vn,1);
			//左裁剪面
			vn.SetXYZW(-1.0f,0,-1.0f);//-x = z 平面的法线
			m_lt_clip_plane.Init(pt_origin,vn,1);
			//上裁剪面
			vn.SetXYZW(0.0f,1.0f,-1.0f);//y = z 平面的法线
			m_tp_clip_plane.Init(pt_origin,vn,1);
			//下裁剪面
			vn.SetXYZW(0.0f,-1.0f,-1.0f);//  -y = z 平面的法线
			m_bt_clip_plane.Init(pt_origin,vn,1);
			
		}else
		{
			pt_origin.Zero();
			
			//金字塔的四个面
			//右裁剪面	
			vn.SetXYZW(m_view_dist,0,-m_viewplane_width/2.0f);
			m_rt_clip_plane.Init(pt_origin,vn,1);
			//左裁剪面
			vn.SetXYZW(-m_view_dist,0,-m_viewplane_width/2.0f);
			m_lt_clip_plane.Init(pt_origin,vn,1);
			//上裁剪面
			vn.SetXYZW(0.0f,m_view_dist,-m_viewplane_width/2.0f);
			m_tp_clip_plane.Init(pt_origin,vn,1);
			//下裁剪面
			vn.SetXYZW(0.0f,-m_view_dist,-m_viewplane_width/2.0f);
			m_bt_clip_plane.Init(pt_origin,vn,1);
			
			
		}
		
	}
	void Buid_Cam4DV1_Matrix_UVN(int mode)
	{
		CMatrix44 mt_inv,//inverse camera translation
			mt_uvn,//the final uvn matrix
			mtmp;//temporary working matrix
		//step1:为位置创建逆平移矩阵
		
		mt_inv.Identify();
		mt_inv.M[2][0] = -m_pos.GetX();
		mt_inv.M[2][1] = -m_pos.GetY();
		mt_inv.M[2][2] = -m_pos.GetZ();
		
		if(mode == UVN_MODE_SPHERICAL)
		{
			float phi = m_dir.GetX();//elevation
			float theta = m_dir.GetY();//heading
			
			float sin_phi= CMath3D::Fast_Sin(phi);
			float cos_phi = CMath3D::Fast_Cos(phi);
			
			float sin_theta = CMath3D::Fast_Sin(theta);
			float cos_theta = CMath3D::Fast_Cos(theta);
			
			m_target.SetX(-1*sin_phi*sin_theta);
			m_target.SetY(1*cos_phi);
			m_target.SetZ(1*sin_phi*cos_theta);
			
		}
		
		m_n = m_pos - m_target;
		
		m_v.SetXYZW(0,1,0);
		
		m_v.Cross(m_n,m_v);
		
		m_n.Cross(m_u,m_v);
		
		m_u.Normalize();
		m_v.Normalize();
		m_n.Normalize();
		
		mt_uvn.M[0][0] = m_u.GetX();mt_uvn.M[0][1] = m_v.GetX();mt_uvn.M[0][2] = m_n.GetX();
		mt_uvn.M[1][0] = m_u.GetY();mt_uvn.M[1][1] = m_v.GetY();mt_uvn.M[1][2] = m_n.GetY();
		mt_uvn.M[2][0] = m_u.GetZ();mt_uvn.M[2][1] = m_v.GetZ();mt_uvn.M[2][2] = m_n.GetZ();
		
		mt_uvn.M[0][3] = 0;mt_uvn.M[1][3] = 0;mt_uvn.M[2][3] = 0;
		mt_uvn.M[3][0] = 0;mt_uvn.M[3][1] = 0;mt_uvn.M[3][2] = 0;
		mt_uvn.M[3][3] = 1;
		
		mcam = mt_inv * mt_uvn;
		
		
	}
	void Build_Cam4DV1_Matrix_Euler(int cam_rot_seq)
	{
		//mmcam = my(-1) *mx(-1) * mz(-1);
		//my(-1) 是逆矩阵
		
		CMatrix44 mt_inv,	//相机平移矩阵的逆矩阵
			mx_inv,	//绕x轴的旋转矩阵的逆矩阵
			my_inv, //绕y轴的旋转矩阵的逆矩阵
			mz_inv,//绕z轴的旋转矩阵的逆矩阵
			mrot,//逆旋转矩阵的积
			mtmp;//用于存储临时矩阵
		
		mt_inv.Identify();
		
		
		mt_inv.M[3][0] = -m_pos.GetX();mt_inv.M[3][1] = -m_pos.GetY();mt_inv.M[3][2] = -m_pos.GetZ();
		
		float theta_x = m_dir.GetX();
		float theta_y = m_dir.GetY();
		float theta_z = m_dir.GetZ();
		
		//计算角度x的正弦和余弦
		float cos_theta = CMath3D::Fast_Cos(theta_x);//cos(-x) = cos(x);
		float sin_theta = CMath3D::Fast_Sin(theta_x);//sin(-x) = -sin(x);
		
		mx_inv.Identify();
		mx_inv.M[1][1] = cos_theta;mx_inv.M[1][2]=sin_theta;
		mx_inv.M[2][2] = cos_theta;mx_inv.M[2][1]=-sin_theta;
		
		cos_theta = CMath3D::Fast_Cos(theta_y);
		sin_theta = -CMath3D::Fast_Sin(theta_y);
		
		my_inv.Identify();
		my_inv.M[0][0] = cos_theta;
		my_inv.M[2][0] = -sin_theta;
		my_inv.M[2][0] = sin_theta;
		my_inv.M[2][2] = cos_theta;
		
		cos_theta = CMath3D::Fast_Cos(theta_z);
		sin_theta = -CMath3D::Fast_Sin(theta_z);
		
		mz_inv.Identify();
		mz_inv.M[0][0] = cos_theta;
		mz_inv.M[0][1] = sin_theta;
		mz_inv.M[1][0] = -sin_theta;
		mz_inv.M[1][1] = cos_theta;
		
		switch(cam_rot_seq)
		{
		case CAM_ROT_SEQ_XYZ:
			{
				mtmp = mx_inv * my_inv;
				mrot = mtmp * mz_inv;
			}break;
		case CAM_ROT_SEQ_YXZ:
			{
				mtmp = my_inv * mx_inv;
				mrot = mtmp *mz_inv;
			}break;
		case CAM_ROT_SEQ_XZY:
			{
				mtmp = mx_inv * mz_inv ;
				mrot = mtmp * my_inv;
			}break;
		case CAM_ROT_SEQ_YZX:
			{
				mtmp = my_inv * mz_inv;
				mrot = mtmp * mx_inv;
			}break;
		case CAM_ROT_SEQ_ZYX:
			{
				mtmp = mz_inv * my_inv;
				mrot = mtmp * mx_inv;
			}break;
		case CAM_ROT_SEQ_ZXY:
			{
				mtmp = mz_inv * mx_inv;
				mrot = mtmp * my_inv;
			}break;
		default:break;
		}//end switch
		
		mcam = mt_inv * mrot;
	}
};






