#pragma once

#include "Vector.h"
#include "Geom.h"
#include "Matrix.h"
#include "HCLib\HCMacros.h"
#include "Math3D.h"
//���������
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

//���ڶ����б�Ķ���� ʵ��������ָ������
class CPoly4DV1
{
public:
	int m_state;// ״̬��Ϣ
	int m_attr;// ����ε���������
	int m_color;//����ε���ɫ
	
	CPoint4D* m_vlist;//�����б����������б�
	int m_vert[3];//�����б���Ԫ�ص�����
};

//������Ⱦ�б�����ݽṹ
class CPolyF4DV1
{
public:
	int m_state;	//״̬��Ϣ
	int m_attr;	//����ε���������
	int m_color;//����ε���ɫ
	
	CPoint4D m_vlist[3];//�������εĶ���
	CPoint4D m_tvlist[3];//�洢�任��Ķ���
	
	CPolyF4DV1* m_next;//ָ����Ⱦ�б��е���һ�������ָ��
	CPolyF4DV1* m_prev;//ָ����Ⱦ�б��е�ǰһ�������ָ��
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
	int m_id;//���������ID
	char m_name[64];//������ַ�����
	
	int m_state;//�����״̬
	int m_attr;//���������
	float m_avg_radius;//�����ƽ���뾶��������ײ���
	float m_max_radius;//��������뾶
	
	CPoint4D m_world_pos;//��������������ϵ�е�λ��
	
	CVector4D m_dir;//�����ھֲ�����ϵ�е���תλ��
	CVector4D m_ux,m_uy,m_uz;//�ֲ������ᣬ���ڴ洢����ĳ���
	//��ת�ڼ佫������
	int m_vertices;//����Ķ�����
	CPoint4D m_vlist_local[OBJECT4DV1_MAX_VERTICES];
	CPoint4D m_vlist_trans[OBJECT4DV1_MAX_VERTICES];
	
	int m_polys;//��������Ķ������
	CPoly4DV1 m_plist[OBJECT4DV1_MAX_POLYS];//���������
	
	
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
		���������״̬���
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

//������ʱ���У�����ֵ�й�ϵ
#define RENDERLIST4DV1_MAX_POLYS	8192  //32768

#define TRANSFORM_LOCAL_ONLY		0X0001
#define TRANSFORM_TRANS_ONLY		0X0002
#define TRANSFORM_LOCAL_TO_TRANS	0X0004

//��Ⱦ�б���
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
	int m_state;//��Ⱦ�б��״̬
	int m_attr;//��Ⱦ�б������
	
	//��Ⱦ�б���һ��ָ������
	//����ÿһ��ָ��ָ��һ���԰����ģ�����Ⱦ�Ķ������(CPolyF4DV1)	
	CPolyF4DV1_PTR m_poly_ptrs[RENDERLIST4DV1_MAX_POLYS];
	
	//Ϊ����ÿ֡��Ϊ����η�����ͷſռ�
	//����ʹ洢������������
	CPolyF4DV1 m_poly_data[RENDERLIST4DV1_MAX_POLYS];
	
	int m_polys;
	
	
};

//�������
#define CAM_MODEL_EULER		0X0001
#define CAM_MODEL_UVN		0X0002

//���ѡ��˳��
#define CAM_ROT_SEQ_XYZ		0X0010
#define CAM_ROT_SEQ_XZY		0X0012
#define CAM_ROT_SEQ_YXZ		0X0014
#define CAM_ROT_SEQ_YZX		0X0016
#define CAM_ROT_SEQ_ZXY		0X0017
#define CAM_ROT_SEQ_ZYX		0X0020

#define UVN_MODE_SPHERICAL	0X0001
#define UVN_MODE_SIMPLE		0X0002

//���ģ�͵Ķ���
class CCamera4DV1
{
public:
	int m_state;
	int m_attr;
	
	CPoint4D m_pos;//��������������е�λ��
	CVector4D m_dir;//ŷ���ǶȻ�UVN���ģ�͵�ע�ӷ���
	
	CVector4D m_u;
	CVector4D m_v;
	CVector4D m_n;//UVNģ�͵ĳ�������
	
	CPoint4D m_target;//UVNģ�͵�Ŀ��λ��
	
	float m_view_dist;//�Ӿ�
	
	float m_fov;//ˮƽ�ʹ�ֱ�������Ұ
	
	//3d�ü���
	//�����Ұ����90����3D�ü��淽�̽�Ϊһ���Է���
	float m_near_clip_z;//���ü���
	float m_far_clip_z;//Զ�ü���
	
	CPlane3D m_rt_clip_plane;//�Ҳü���
	CPlane3D m_lt_clip_plane;//��ü���
	CPlane3D m_tp_clip_plane;//�ϲü���
	CPlane3D m_bt_clip_plane;//�²ü���
	
	float m_viewplane_width;
	float m_viewplane_height;//��ƽ��Ŀ�Ⱥ͸߶ȣ����ڹ�һ����ͶӰ 2X2
	//�����С���ӿڻ���Ļ������ͬ
	float m_viewport_width;
	float m_viewport_height;//�ӿڴ�С
	float m_viewport_center_x;
	float m_viewport_center_y;//�ӿ�����
	
	float m_aspect_ratio;//��߱ȣ���Ļ�Ŀ�߱�
	
	CMatrix44 mcam;//�洢�������굽�������ı任����
	CMatrix44 mper;//�洢������굽͸������ı任����
	CMatrix44 mscr;//�洢͸�����굽��Ļ����ı任����
	
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
		CVector3D vn;//���ڳ�ʼ���ü���
		if(fov == 90.0)
		{
			pt_origin.Zero();
			//���������ĸ���
			//�Ҳü���			
			vn.SetXYZW(1.0f,0,-1.0f);//x = z ƽ��ķ���
			m_rt_clip_plane.Init(pt_origin,vn,1);
			//��ü���
			vn.SetXYZW(-1.0f,0,-1.0f);//-x = z ƽ��ķ���
			m_lt_clip_plane.Init(pt_origin,vn,1);
			//�ϲü���
			vn.SetXYZW(0.0f,1.0f,-1.0f);//y = z ƽ��ķ���
			m_tp_clip_plane.Init(pt_origin,vn,1);
			//�²ü���
			vn.SetXYZW(0.0f,-1.0f,-1.0f);//  -y = z ƽ��ķ���
			m_bt_clip_plane.Init(pt_origin,vn,1);
			
		}else
		{
			pt_origin.Zero();
			
			//���������ĸ���
			//�Ҳü���	
			vn.SetXYZW(m_view_dist,0,-m_viewplane_width/2.0f);
			m_rt_clip_plane.Init(pt_origin,vn,1);
			//��ü���
			vn.SetXYZW(-m_view_dist,0,-m_viewplane_width/2.0f);
			m_lt_clip_plane.Init(pt_origin,vn,1);
			//�ϲü���
			vn.SetXYZW(0.0f,m_view_dist,-m_viewplane_width/2.0f);
			m_tp_clip_plane.Init(pt_origin,vn,1);
			//�²ü���
			vn.SetXYZW(0.0f,-m_view_dist,-m_viewplane_width/2.0f);
			m_bt_clip_plane.Init(pt_origin,vn,1);
			
			
		}
		
	}
	void Buid_Cam4DV1_Matrix_UVN(int mode)
	{
		CMatrix44 mt_inv,//inverse camera translation
			mt_uvn,//the final uvn matrix
			mtmp;//temporary working matrix
		//step1:Ϊλ�ô�����ƽ�ƾ���
		
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
		//my(-1) �������
		
		CMatrix44 mt_inv,	//���ƽ�ƾ���������
			mx_inv,	//��x�����ת����������
			my_inv, //��y�����ת����������
			mz_inv,//��z�����ת����������
			mrot,//����ת����Ļ�
			mtmp;//���ڴ洢��ʱ����
		
		mt_inv.Identify();
		
		
		mt_inv.M[3][0] = -m_pos.GetX();mt_inv.M[3][1] = -m_pos.GetY();mt_inv.M[3][2] = -m_pos.GetZ();
		
		float theta_x = m_dir.GetX();
		float theta_y = m_dir.GetY();
		float theta_z = m_dir.GetZ();
		
		//����Ƕ�x�����Һ�����
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






