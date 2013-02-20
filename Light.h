#ifndef H_LIGHT_INCLUDED
#define H_LIGHT_INCLUDED

#include "Vector.h"

#define LIGHTV1_ATTR_AMBIENT		0X0001	// 环境光源
#define LIGHTV1_ATTR_INFINITE		0X0002  // 无穷远光源
#define LIGHTV1_ATTR_POINT			0X0004	// 点光源
#define LIGHTV1_ATTR_SPOTLIGHT1		0X0008	// 1类（简单）聚光灯
#define LIGHTV1_ATTR_SPOTLIGHT2		0X0010	// 2类（复杂）聚光灯

#define LIGHTV1_STATE_ON			1		// 光源打开
#define LIGHTV1_STATE_OFF			0		// 光源关闭

#define MAX_LIGHTS					8		//最多支持的光源数目

typedef struct LIGHTV1_TYP
{
	int state;
	int id;
	int attr;

	RGBAV1 c_ambient;// 环境光强度
	RGBAV1 c_diffuse;// 散射光强度
	RGBAV1 c_specular;// 镜面发射光强度

	CPoint4D pos;//光源位置
	CVector4D dir;//光源方向
	float kc,kl,kq;//衰减因子

	float spot_inner;//聚光灯内锥角
	float spot_outer;//聚光灯外锥角

	float pf;//聚光灯指数因子

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
		int index,//要创建的光源索引(0到MAX_LIGHTS-1)
		int _state,//光源状态
		int _attr,//光源类型及其他属性
		RGBAV1 _c_ambient,//环境光强度
		RGBAV1 _c_diffuse,//散射光强度
		RGBAV1 _c_specular,//镜面反射光强度
		CPoint4D * _pos,//光源位置
		CVector4D * _dir,//光源方向
		float kc,float kl,float kq,//衰减因子
		float _spot_inner,//聚光灯内锥角
		float _spot_outer,//聚光灯外锥角
		float _pf)//聚光灯指数因子
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