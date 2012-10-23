#pragma once

#include "Vector.h"
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

	CPoint4D m_vlist;//�����б�
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

#define OBJECT4DV1_MAX_VERTICES           1024  // 64
#define OBJECT4DV1_MAX_POLYS              1024 // 128

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



};


#define RENDERLIST4DV1_MAX_POLYS	32768
//��Ⱦ�б���
class CRenderList4DV1
{
public :
	
	void Reset_RenderList4DV1(CRenderList4DV1& rend_list)
	{
		rend_list.m_polys = 0;
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






