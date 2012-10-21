#pragma once

// 定点数相关 常量
#define FIXP16_SHIFT     16
#define FIXP16_MAG       65536
#define FIXP16_DP_MASK   0x0000ffff
#define FIXP16_WP_MASK   0xffff0000
#define FIXP16_ROUND_UP  0x00008000

// 定义小数，浮点精度问题
#define EPSILON_E3 (float)(1E-3) 
#define EPSILON_E4 (float)(1E-4) 
#define EPSILON_E5 (float)(1E-5)
#define EPSILON_E6 (float)(1E-6)

typedef int FIXP16;
typedef int *FIXP16_PTR;


// 转换interger、float到fixed point 16.16
#define INT_TO_FIXP16(i) ((i) <<  FIXP16_SHIFT)
#define FLOAT_TO_FIXP16(f) (((float)(f) * (float)FIXP16_MAG + EPSILON_E5))
#define FIXP16_TO_FLOAT(fp) ( ((float)fp)/FIXP16_MAG)


// 从一个定点数16.16格式，取出整数部分和小数部分
#define FIXP16_WP(fp) ((fp) >> FIXP16_SHIFT)//取整
#define FIXP16_DP(fp) (fp & FIXP16_DP_MASK)//取小数
