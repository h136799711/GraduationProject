#pragma once

// ��������� ����
#define FIXP16_SHIFT     16
#define FIXP16_MAG       65536
#define FIXP16_DP_MASK   0x0000ffff
#define FIXP16_WP_MASK   0xffff0000
#define FIXP16_ROUND_UP  0x00008000

// ����С�������㾫������
#define EPSILON_E3 (float)(1E-3) 
#define EPSILON_E4 (float)(1E-4) 
#define EPSILON_E5 (float)(1E-5)
#define EPSILON_E6 (float)(1E-6)

typedef int FIXP16;
typedef int *FIXP16_PTR;


// ת��interger��float��fixed point 16.16
#define INT_TO_FIXP16(i) ((i) <<  FIXP16_SHIFT)
#define FLOAT_TO_FIXP16(f) (((float)(f) * (float)FIXP16_MAG + EPSILON_E5))
#define FIXP16_TO_FLOAT(fp) ( ((float)fp)/FIXP16_MAG)


// ��һ��������16.16��ʽ��ȡ���������ֺ�С������
#define FIXP16_WP(fp) ((fp) >> FIXP16_SHIFT)//ȡ��
#define FIXP16_DP(fp) (fp & FIXP16_DP_MASK)//ȡС��
