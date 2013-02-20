#ifndef H_LIGHT_INCLUDED
#define H_LIGHT_INCLUDED

#include "Vector.h"

#define LIGHTV1_ATTR_AMBIENT		0X0001	// ������Դ
#define LIGHTV1_ATTR_INFINITE		0X0002  // ����Զ��Դ
#define LIGHTV1_ATTR_POINT			0X0004	// ���Դ
#define LIGHTV1_ATTR_SPOTLIGHT1		0X0008	// 1�ࣨ�򵥣��۹��
#define LIGHTV1_ATTR_SPOTLIGHT2		0X0010	// 2�ࣨ���ӣ��۹��

#define LIGHTV1_STATE_ON			1		// ��Դ��
#define LIGHTV1_STATE_OFF			0		// ��Դ�ر�

#define MAX_LIGHTS					8		//���֧�ֵĹ�Դ��Ŀ

typedef struct LIGHTV1_TYP
{
	int state;
	int id;
	int attr;

	RGBAV1 c_ambient;// ������ǿ��
	RGBAV1 c_diffuse;// ɢ���ǿ��
	RGBAV1 c_specular;// ���淢���ǿ��

	CPoint4D pos;//��Դλ��
	CVector4D dir;//��Դ����
	float kc,kl,kq;//˥������

	float spot_inner;//�۹����׶��
	float spot_outer;//�۹����׶��

	float pf;//�۹��ָ������

}LIGHTV1,*LIGHTV1_PTR;


class LightManagerV1
{
private:
	LIGHTV1 lights[MAX_LIGHTS];
	int num_lights; //
public:
	LIGHTV1* getLight(int index)
	{
		if(index >=0 && index < num_lights)
		{
			return &lights[index];
		}
		return NULL;
	}
	int ResetLightsLIGHTV1()
	{
		static int first_time = 1;

		memset(lights,0,MAX_LIGHTS * sizeof(LIGHTV1));

		num_lights = 0;
		first_time = 0;

		return 1;
	}
	int InitLightLIGHTV1(
		int index,//Ҫ�����Ĺ�Դ����(0��MAX_LIGHTS-1)
		int _state,//��Դ״̬
		int _attr,//��Դ���ͼ���������
		RGBAV1 _c_ambient,//������ǿ��
		RGBAV1 _c_diffuse,//ɢ���ǿ��
		RGBAV1 _c_specular,//���淴���ǿ��
		CPoint4D * _pos,//��Դλ��
		CVector4D * _dir,//��Դ����
		float kc,float kl,float kq,//˥������
		float _spot_inner,//�۹����׶��
		float _spot_outer,//�۹����׶��
		float _pf)//�۹��ָ������
	{
		if(index < 0 || index >= MAX_LIGHTS)
			return 0;
		lights[index].state = _state;
		lights[index].id = index;
		lights[index].c_ambient = _c_ambient;
		lights[index].c_diffuse = _c_diffuse;
		lights[index].c_specular = _c_specular;
		lights[index].kc = kc;
		lights[index].kl = kl;
		lights[index].kq = kq;

		if(_pos)
			lights[index].pos = *_pos;
		if(_dir)
		{
			lights[index].dir = *_dir;
			lights[index].dir.Normalize();
		}
		lights[index].spot_inner = _spot_inner;
		lights[index].spot_outer = _spot_outer;
		lights[index].pf = _pf;

		return (index);
	}
};
#endif