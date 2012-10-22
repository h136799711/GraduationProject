#include "HCMath.h"






void HCMath::Build_Sin_Cos_Tables(void)
{
	
	// create sin/cos lookup table
	// note the creation of one extra element; 360
	// this helps with logic in using the tables
	
	// generate the tables 0 - 360 inclusive
	for (int ang = 0; ang <= 360; ang++)
    {
		// convert ang to radians
		float theta = (float)ang*PI/(float)180;
		
		// insert next entry into table
		cos_look[ang] = cos(theta);
		sin_look[ang] = sin(theta);
		
    } // end for ang
	
} // end Build_Sin_Cos_Tables

int HCMath::Mat_Mul_3X3(MATRIX3X3_PTR ma, 
               MATRIX3X3_PTR mb,
               MATRIX3X3_PTR mprod)
{
// this function multiplies two matrices together and 
// and stores the result

for (int row=0; row<3; row++)
    {
    for (int col=0; col<3; col++)
        {
        // compute dot product from row of ma 
        // and column of mb

        float sum = 0; // used to hold result

        for (int index=0; index<3; index++)
             {
             // add in next product pair
             sum+=(ma->M[row][index]*mb->M[index][col]);
             } // end for index

        // insert resulting row,col element
        mprod->M[row][col] = sum;

        } // end for col

    } // end for row

return(1);

} // end Mat_Mul_3X3


int HCMath::Mat_Mul_1X3_3X3(MATRIX1X3_PTR ma, 
                   MATRIX3X3_PTR mb,
                   MATRIX1X3_PTR mprod)
{
// this function multiplies a 1x3 matrix against a 
// 3x3 matrix - ma*mb and stores the result

    for (int col=0; col<3; col++)
        {
        // compute dot product from row of ma 
        // and column of mb

        float sum = 0; // used to hold result

        for (int index=0; index<3; index++)
             {
             // add in next product pair
             sum+=(ma->M[index]*mb->M[index][col]);
             } // end for index

        // insert resulting col element
        mprod->M[col] = sum;

        } // end for col

return(1);

} // end Mat_Mul_1X3_3X3


int HCMath::Mat_Mul_1X2_3X2(MATRIX1X2_PTR ma, 
                   MATRIX3X2_PTR mb,
                   MATRIX1X2_PTR mprod)
{
// this function multiplies a 1x2 matrix against a 
// 3x2 matrix - ma*mb and stores the result
// using a dummy element for the 3rd element of the 1x2 
// to make the matrix multiply valid i.e. 1x3 X 3x2

    for (int col=0; col<2; col++)
        {
        // compute dot product from row of ma 
        // and column of mb

        float sum = 0; // used to hold result

        for (int index=0; index<2; index++)
             {
             // add in next product pair
             sum+=(ma->M[index]*mb->M[index][col]);
             } // end for index

        // add in last element * 1 
        sum+= mb->M[index][col];

        // insert resulting col element
        mprod->M[col] = sum;

        } // end for col

return(1);

} // end Mat_Mul_1X2_3X2


int HCMath::Mat_Init_3X2(MATRIX3X2_PTR ma, 
                        float m00, float m01,
                        float m10, float m11,
                        float m20, float m21)
{
// this function fills a 3x2 matrix with the sent data in row major form
ma->M[0][0] = m00; ma->M[0][1] = m01; 
ma->M[1][0] = m10; ma->M[1][1] = m11; 
ma->M[2][0] = m20; ma->M[2][1] = m21; 

// return success
return(1);

} // end Mat_Init_3X2
