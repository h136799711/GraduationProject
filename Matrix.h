#pragma once

/*****************************************************************
**矩阵定义
不支持矩阵之间的乘法
**
*******************************************************************/
template<int row=3,int col=3>
class CMatrix
{
	
public:
	CMatrix()
	{
		Zero();
	}
	CMatrix(float* matrix)
	{
		memcpy(M,matrix,sizeof(M));
	}
	inline void Zero()
	{
		memset(M,0,sizeof(M));
	}

	/*
		设为单位矩阵
	*/
	inline int Identify();

	/*
		取得此矩阵的逆矩阵，
	*/
	int Inverse(CMatrix<row,col>& inv);
	/*
		矩阵的行列式
	*/
	inline int Det(float& det);
	/*
		矩阵乘法
		2个相乘矩阵的内维数必须相同
	*/
//	int Mul(CMatrix<row,col> right,CMatrix<row,col>& result);
	/*
		使用高斯消元法,
		获得上三角矩阵根据以下公式求行列式
		|A| = det(A) = (-1)p次方 a11*a22*a33..ann
	*/
	int GaussDet(float& det)
	{
		if(row != col) return 0;
		
		
		int i,j,k;//用于循环
		int p=0;//用于系数

		for(i=0;i<row;i++)
		{
			int max = M[i][i],max_r = i;
			for(j=i+1;j<col;j++)
			{
				if(M[j][i] > max)
				{
					max = M[j][i];
					max_r = j;
				}
			}
			float tmp;
			if(max_r != i)
			{
				p = p+1;
				for(j=0;j<col;j++)
				{
					tmp = M[max_r][j];
					M[max_r][j] = M[i][j];
					M[i][j] = tmp;
				}
			}
			for(j=i+1;j<row;j++)
			{
				tmp = M[j][i] / M[i][i];
				for(k=i;k<col;k++)
				{
					M[j][k] = M[j][k] - tmp * M[i][k];
				}
			}
		}

		if(p%2 == 0)
			p  = 1;
		else p = -1;
		det = 1.0f;
		for(i=0;i<row;i++)
		{
			det = det*M[i][i];
		}
		return 1;
	}

	inline bool operator == (CMatrix<row,col>& right)
	{
		for(int i=0;i<row;i++)
		{
			for(int j=0;j<col;j++)
			{
				if(M[i][j] != right.M[i][j])
					return false;
			}
		}
		return true;
	}
	inline CMatrix<row,col> operator = (float *matrix)
	{
		memcpy(M,matrix,sizeof(M));
		return *this;
	}
	inline CMatrix<row,col> operator = (CMatrix<row,col>& right)
	{
		for(int i=0;i<row;i++)
		{
			for(int j=0;j<col;j++)
			{
				M[i][j] = right.M[i][j];
			}
		}
		return *this;
	}
	inline 	CMatrix<row,col>& operator += (CMatrix<row,col>& right)
	{
		for(int i=0;i<row;i++)
		{
			for(int j=0;j<col;j++)
			{
				M[i][j] += right.M[i][j];
			}
		}
		return *this;
	}
	inline CMatrix<row,col>& operator -= (CMatrix<row,col>& right)
	{
		for(int i=0;i<row;i++)
		{
			for(int j=0;j<col;j++)
			{
				M[i][j] -= right.M[i][j];
			}
		}
		return *this;
	}
	/*
	标量乘以系数	*=	
	*/
	inline CMatrix<row,col> operator *= (float k)
	{
		for(int i=0;i<row;i++)
		{
			for(int j=0;j<col;j++)
			{
				M[i][j] *= k;
			}
		}
		return *this;
	}
	inline CMatrix<row,col> operator + (CMatrix<row,col>& right)
	{
		CMatrix<row,col> rst;
		for(int i=0;i<row;i++)
		{
			for(int j=0;j<col;j++)
			{	
				rst.M[i][j] = M[i][j] + right.M[i][j];
			}
		}
		return rst;
	}
	inline CMatrix<row,col> operator - (CMatrix<row,col>& right)
	{
		CMatrix<row,col> result;
		for(int i=0;i<row;i++)
		{
			for(int j=0;j<col;j++)
			{	
				result.M[i][j] = M[i][j] - right.M[i][j];
			}
		}
		return result;
	}
	inline CMatrix<row,col> operator * (CMatrix<row,col>& right)
	{	
		CMatrix<row,col> result;
		for(int i=0;i<x;i++)
		{
			for(int j=0;j<y;j++)
			{
				float sum =0 ;

				for(int k=0;k<c;k++)
				{
					float tmp = (M[i*c+k] * right.M[k*y+j]);
					//printf("[%d][%d] %f * [%d][%d] %f = %f,",i+1,k+1,ma[i*x+k] ,k+1,j+1 ,mb[k*c+j],tmp);
					sum += tmp;
				}	
				//printf(" result[%d][%d]= %f \n",i,j,sum);
				result.M[i*x+j] = sum;
			}

		}
		return result;
	}

	inline int ColumnSwap(int c,CMatrix<1,row> vec);

	float M[row][col];
	
	
};
typedef CMatrix<1,1> CMatrix11;
typedef CMatrix<1,2> CMatrix12;
typedef CMatrix<1,3> CMatrix13;
typedef CMatrix<1,4> CMatrix14;

typedef CMatrix<2,1> CMatrix21;
typedef CMatrix<2,2> CMatrix22;
typedef CMatrix<2,3> CMatrix23;
typedef CMatrix<2,4> CMatrix24;

typedef CMatrix<3,1>	CMatrix31;
typedef CMatrix<3,2>	CMatrix32;
typedef CMatrix<3,3>	CMatrix33;
typedef CMatrix<3,4>	CMatrix34;

typedef CMatrix<4,1>	CMatrix41;
typedef CMatrix<4,2>	CMatrix42;
typedef CMatrix<4,3>	CMatrix43;
typedef CMatrix<4,4>	CMatrix44;

//单位矩阵定义

const float IDM44[4][4] = {1,0,0,0, 
0,1,0,0, 
0,0,1,0, 
0,0,0,1};

// 4X3 单位矩阵在数学上是不正确定义

const float IDM43[4][3] = {1,0,0, 
0,1,0, 
0,0,1, 
0,0,0,};


// 3x3 identity matrix
const float IDM33[3][3] = {1,0,0, 
0,1,0, 
0,0,1};

// 2x2 identity matrix
const float IDM22[2][2] = {1,0, 
0,1};

/*
	4X4 设为单位矩阵
*/
template<>
inline int CMatrix<4,4>::Identify()
{
	memcpy(&M[0][0],&IDM44,sizeof(M));
	return 1;
}
/*
	4X4 设为单位矩阵
*/
template<>
inline int CMatrix<4,3>::Identify()
{
	memcpy(&M[0][0],&IDM43,sizeof(M));
	return 1;
}
/*
	3X3 设为单位矩阵
*/
template<>
inline int CMatrix<3,3>::Identify()
{
	memcpy(&M[0][0],&IDM33,sizeof(M));
	return 1;
}

/*
	2X2 设为单位矩阵
*/
template<>
inline int CMatrix<2,2>::Identify()
{
	memcpy(&M[0][0],&IDM22,sizeof(M));
	return 1;
}
/*
	取得此矩阵的逆矩阵，未实现
*/
template<>
int CMatrix<2,2>::Inverse(CMatrix<2,2>& inv)
{
	float det ;
	Det(det);
	if(fabs(det) < EPSILON_E5)//没有逆矩阵
		return 0;
	float det_inv = 1.0 / det;

	inv.M[0][0] =  M[1][1] * det_inv;
	inv.M[0][1] = -M[0][1] * det_inv;
	inv.M[1][0] = -M[1][0] * det_inv;
	inv.M[1][1] =  M[0][0] * det_inv;
	return 1;
}
template<>
int CMatrix<3,3>::Inverse(CMatrix<3,3>& inv)
{
	float det;
	Det(det);
	if (fabs(det) < EPSILON_E5)
		return -1;

	// compute inverse to save divides
	float det_inv = 1.0/det;

	// compute inverse using m-1 = adjoint(m)/det(m)
	inv.M[0][0] =  det_inv * (M[1][1] * M[2][2] - M[2][1] * M[1][2]);
	inv.M[1][0] = -det_inv * (M[1][0] * M[2][2] - M[2][0] * M[1][2]);
	inv.M[2][0] =  det_inv * (M[1][0] * M[2][1] - M[2][0] * M[1][1]);

	inv.M[0][1] = -det_inv * (M[0][1] * M[2][2] - M[2][1] * M[0][2]);
	inv.M[1][1] =  det_inv * (M[0][0] * M[2][2] - M[2][0] * M[0][2]);
	inv.M[2][1] = -det_inv * (M[0][0] * M[2][1] - M[2][0] * M[0][1]);
	
	inv.M[0][2] =  det_inv * (M[0][1] * M[1][2] - M[1][1] * M[0][2]);
	inv.M[1][2] = -det_inv * (M[0][0] * M[1][2] - M[1][0] * M[0][2]);
	inv.M[2][2] =  det_inv * (M[0][0] * M[1][1] - M[1][0] * M[0][1]);

	return 1;
}
template<>
int CMatrix<4,4>::Inverse(CMatrix<4,4>& inv)
{
	float det;
	Det(det);
	if (fabs(det) < EPSILON_E5)
		return -1;

	// compute inverse to save divides
	float det_inv = 1.0/det;

	// compute inverse using m-1 = adjoint(m)/det(m)
	inv.M[0][0] =  det_inv * (M[1][1] * M[2][2] - M[2][1] * M[1][2]);
	inv.M[1][0] = -det_inv * (M[1][0] * M[2][2] - M[2][0] * M[1][2]);
	inv.M[2][0] =  det_inv * (M[1][0] * M[2][1] - M[2][0] * M[1][1]);
	
	inv.M[0][1] = -det_inv * (M[0][1] * M[2][2] - M[2][1] * M[0][2]);
	inv.M[1][1] =  det_inv * (M[0][0] * M[2][2] - M[2][0] * M[0][2]);
	inv.M[2][1] = -det_inv * (M[0][0] * M[2][1] - M[2][0] * M[0][1]);
	
	inv.M[0][2] =  det_inv * (M[0][1] * M[1][2] - M[1][1] * M[0][2]);
	inv.M[1][2] = -det_inv * (M[0][0] * M[1][2] - M[1][0] * M[0][2]);
	inv.M[2][2] =  det_inv * (M[0][0] * M[1][1] - M[1][0] * M[0][1]);
	
	inv.M[0][3] = 0.0f;//always 0
	inv.M[1][3] = 0.0f;//always 0
	inv.M[2][3] = 0.0f;//always 0
	inv.M[3][3] = 1.0f;//always 1
	return 1;
}
/*
	矩阵的行列式
*/
template<int row,int col>
inline int CMatrix<row,col>::Det(float& det)
{
	return GaussDet(det);
}
template<>
inline int CMatrix<2,2>::Det(float & det)
{	
	det = M[0][0] * M[1][1] - M[0][1] * M[1][0];

	return 1;
}
template<>
inline int CMatrix<3,3>::Det(float& det)
{
	det = M[0][0] * (M[1][1] * M[2][2] - M[2][1] * M[1][2]) - 
		M[0][1] * (M[1][0] * M[2][2] - M[2][0] * M[1][2]) + 
		M[0][2] * (M[1][0] * M[2][1] - M[2][0] * M[1][1]);
	return 1;
}
template<>
inline int CMatrix<4,4>::Det(float& det)
{
	 
	det = M[0][0] * (M[1][1] * M[2][2] - M[2][1] * M[1][2]) - 
		M[0][1] * (M[1][0] * M[2][2] - M[2][0] * M[1][2]) + 
		M[0][2] * (M[1][0] * M[2][1] - M[2][0] * M[1][1]);
	return 1;
}
template<>
inline int CMatrix<2,2>::ColumnSwap(int c,CMatrix<1,2> vec)
{
	M[0][c] = vec.M[0][0];
	M[1][c] = vec.M[0][1];
	return 1;
}
template<>
inline int CMatrix<3,3>::ColumnSwap(int c,CMatrix<1,3> vec)
{
	M[0][c] = vec.M[0][0];
	M[1][c] = vec.M[0][1];
	M[2][c] = vec.M[0][2];
	return 1;
}
template<>
inline int CMatrix<4,4>::ColumnSwap(int c,CMatrix<1,4> vec)
{
	M[0][c] = vec.M[0][0];
	M[1][c] = vec.M[0][1];
	M[2][c] = vec.M[0][2];
	M[3][c] = vec.M[0][3];
	return 1;
}
template<>
inline int CMatrix<4,3>::ColumnSwap(int c,CMatrix<1,4> vec)
{
	M[0][c] = vec.M[0][0];
	M[1][c] = vec.M[0][1];
	M[2][c] = vec.M[0][2];
	M[3][c] = vec.M[0][3];
	return 1;
}