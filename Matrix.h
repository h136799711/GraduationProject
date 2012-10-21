#pragma once
/*****************************************************************
**矩阵定义
	
**
*******************************************************************/
template<int row=3,int col=3>
class CMatrix
{

public:
	float M[row][col];
};
typedef CMatrix<1><2> CMatrix12;
typedef CMatrix<1><3> CMatrix13;
typedef CMatrix<1><4> CMatrix14;

typedef CMatrix<2><1> CMatrix21;
typedef CMatrix<2><2> CMatrix22;
typedef CMatrix<2><3> CMatrix23;
typedef CMatrix<2><4> CMatrix24;

typedef CMatrix<3, 1>	CMatrix31;
typedef CMatrix<3, 2>	CMatrix32;
typedef CMatrix<3, 3>	CMatrix33;
typedef CMatrix<3, 4>	CMatrix34;

typedef CMatrix<4, 1>	CMatrix41;
typedef CMatrix<4, 2>	CMatrix42;
typedef CMatrix<4, 3>	CMatrix43;
typedef CMatrix<4, 4>	CMatrix44;

//单位矩阵定义

const float M[4][4] = {1,0,0,0, 
0,1,0,0, 
0,0,1,0, 
0,0,0,1};

// 4X3 单位矩阵在数学上是不正确定义

const float M[4][3] = {1,0,0, 
0,1,0, 
0,0,1, 
0,0,0,};


// 3x3 identity matrix
const float M[3][3] = {1,0,0, 
0,1,0, 
0,0,1};

// 2x2 identity matrix
const float M[2][2] = {1,0, 
0,1};

