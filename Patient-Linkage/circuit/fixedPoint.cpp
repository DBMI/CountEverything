//////////////////////////////////////////////////////////////////////////
// fixedPoint.cpp - Implemenation of fixed point operations				//
// version 1.0															//
//----------------------------------------------------------------------//
// Haoyi Shi (c) copyright 2015					                        //
// hshi05@syr.edu														//
// All rights granted provided this copyright notice is retained        //
//----------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////
#include "fixedPoint.h"
#include <vector>
using namespace std;
const int INTEGER_BITS = 17;
const int RADIX_BITS = 11;




BOOL FixPoint::Create(int nParties, const vector<int>& vParams)
{
	m_nNumParties = nParties;

	// parse params
	// params: vector length
	if (vParams.size() < (unsigned)1)
	{
		cout << "Error! This circuit needs " << 1
			<< "parameters for vector length"
			<< endl;
		return FALSE;
	}
	cout << "vParams[0] " << vParams[0] << endl;
	m_nRep = vParams[0];
	m_vNumVBits.resize(m_nNumParties, 1);
	m_vInputStart.resize(nParties);
	m_vInputEnd.resize(nParties);




// allocate gates for inputs
	m_nFrontier = 2;
	for (int i = 0; i < nParties; i++)
	{
		m_vInputStart[i] = m_nFrontier;
		m_nFrontier = m_nFrontier + m_nRep;
		m_vInputEnd[i] = m_nFrontier - 1;
	}

	//calculate the gates for circuit.
	m_othStartGate = m_nFrontier;
	m_nNumGates = m_othStartGate + 2000000000;
	InitGates();
/*
		//PutXORGate(m_vInputStart[0],m_vInputStart[1]);
	int hessianMatrix= fPMatrixAddition(m_vInputStart[0]+150*(INTEGER_BITS+RADIX_BITS), m_vInputStart[1]+150*(INTEGER_BITS+RADIX_BITS), 4, 4);
	for(int i=2;i<nParties;i++)
	hessianMatrix=fPMatrixAddition(hessianMatrix, m_vInputStart[i]+150*(INTEGER_BITS+RADIX_BITS), 4, 4);
	int hessianInver=fPMatrixInversion(hessianMatrix, 4, 4,15,2);
	//int o_start=hessianInver;

	int x=m_nFrontier;
	for(int j=0;j<nParties;j++)
	for(int i=0;i<30*4*(INTEGER_BITS+RADIX_BITS);i++)
		PutXORGate(m_vInputStart[j]+i,0);
	int y=m_nFrontier;
	for(int i=0;i<nParties;i++)
		for(int j=0;j<30*(INTEGER_BITS+RADIX_BITS);j++)
			PutXORGate(m_vInputStart[i]+30*4*(INTEGER_BITS+RADIX_BITS)+j,0);

	//------------------------------------
		
	int p=PutXORGate(0,0);
	int beta=m_nFrontier;
	for(int i=0;i<4*(INTEGER_BITS+RADIX_BITS);i++)
		PutXORGate(p,p);
	//int o_start=fPMatrixFirstderiv(x, y ,beta, 10, 4);
		

	int o_start= logRegNewton( hessianInver,x, 60, 4, y);
*/
	int o_start= fPMatrixAddition(m_vInputStart[0], m_vInputStart[1], 4, 4);
	//	int o_start= fPMatrixInnerProduct(m_vInputStart[0], m_vInputStart[1], 4, 4,4);

	int o_end=m_nFrontier-1;
	m_vOutputStart.resize(m_nNumParties,o_start);
	m_vOutputEnd.resize(m_nNumParties,o_end);
	
	
	m_nNumGates = m_nFrontier;

	//putOutputLayer();
cout<<"o_start     "<<o_start<<endl;

	PatchParentFields();
	return TRUE;
}



void FixPoint::putOutputLayer()
{
	
	//int o_start= fPAddition(m_vInputStart[0], m_vInputStart[1]);
	//int o_start=fPSubtraction(m_vInputStart[0], m_vInputStart[1]);

	//int o_start=fPMultiplication(m_vInputStart[0],m_vInputStart[1]);
	//int o_start=fPMatrixAddition(m_vInputStart[0],m_vInputStart[1],2,2);
	 //o_start=fPtranspose(m_vInputStart[0],2,2);
	//int o_start=fPvectorAddition(m_vInputStart[0],m_vInputStart[1],4);
	//int o_start=fPvectorMultiplication(m_vInputStart[0],m_vInputStart[1],4);
	//int o_start=fPMatrixScalaProduct(m_vInputStart[0],2,2,m_vInputStart[1]);
	//int o_start=fPMatrixInnerProduct(m_vInputStart[0],m_vInputStart[1],2,2,2);
	//int o_start=fPMatrixAddition(m_vInputStart[0],m_vInputStart[1],2,2);
	 //o_start=fPMatrixInversion(m_vInputStart[0],10,10,5);
	//o_start=fPMatrixInversion(m_vInputStart[0],1,1);
	//vector<int> array(INTEGER_BITS+RADIX_BITS,0);
	//array[RADIX_BITS]=1;
	//array[RADIX_BITS+2]=1;
	//int bound=m_nFrontier;
	//for(int i=0;i<INTEGER_BITS+RADIX_BITS;i++)
		//PutXORGate(array[i], 0);

	//o_start=fPFilter(m_vInputStart[0],bound);

	//o_start=fPExponenetial(m_vInputStart[0],15);

	//int o_start=fPMatrixInnerProductStrassen(m_vInputStart[0],m_vInputStart[1],2,2,2);
	//int o_start=fPMultiplication(m_vInputStart[0],m_vInputStart[1]);
	//int o_start=fPMatrixInnerProduct(m_vInputStart[0],m_vInputStart[1],2,2,2);
	//int o_start=fPMatrixAddition(m_vInputStart[0],m_vInputStart[1],1,1);
	//int o_start=fPMatrixInnerProduct(m_vInputStart[0],m_vInputStart[1],20,20,20);
	PutANDGate(m_vInputStart[0],m_vInputStart[1]);
	int o_start=fPMatrixInversion(m_vInputStart[0], 4, 4,16, 2);
	//int o_start=fPMatrixInnerProduct(m_vInputStart[0],m_vInputStart[1],1,1,1);
	cout<<"o_start   "<<o_start<<endl;
	int o_end = m_nFrontier-1;
	m_vOutputStart.resize(m_nNumParties,o_start);
	m_vOutputEnd.resize(m_nNumParties,o_end);
	
	
	m_nNumGates = m_nFrontier;
}




//-----------------<function fPAddition>------------------
// This function calculates the fixed point addition of two fixed point values.
// Each value starts from the lowest bit.
// Input:
//		a: the start index of the first value
//		b: the start index of the second value
// Output: the start index of the calculated value, length: INTEGER_BITS+RADIX_BITS
int FixPoint::fPAddition(int a, int b)
{
	//TODO: do addition.
	return PutAddGate(a, b, INTEGER_BITS + RADIX_BITS, false);
}


int FixPoint::fPSubtraction(int a, int b)
{
	int bComplement = fPTwoComplement(b);
	return fPAddition(a, bComplement);
}





//-------------------<function fPFilter>-------------------
// This function filters the provided value with the set bound, an absolute value.
int FixPoint::fPFilter(int a, int bound)
{
	int rep=INTEGER_BITS+RADIX_BITS;
	int negative=PutXORGate(a+rep-1,0);

	int a_abs=PutMUXGate(fPTwoComplement(a),a,negative,rep,false);
	int a_absGTBound= PutGTGate(a_abs,bound, rep);
	int a_absGTBoundANDNeg=PutANDGate(a_absGTBound,negative);
	int a_absGTBoundANDNegNeg=PutANDGate(a_absGTBound,PutXORGate(negative,1));
	int GTMUXNegNeg=PutMUXGate(bound, a, a_absGTBoundANDNegNeg,rep,false);
	int out=PutMUXGate(fPTwoComplement(bound),GTMUXNegNeg,a_absGTBoundANDNeg,rep, false);
	return out;

}


//-----------------<function fPExponenetial>-----------------
// This function calculates the exponential value of x by using Taylor's expansion method.
// Input:
//		exponent: the exponent
//		iteration: the iterations used in calculating the exponential value.
// Note: if the absolute value of exponent is larger than 1, the iteration should also be very large.
// To calculate the exponential, a lookup table is needed. Right now we just support iterations fewer than 15.
// If more iterations are needed, the user has to extend the lookup table.
int FixPoint::fPExponenetial(int exponent, int iteration)
{
	
	int rep=INTEGER_BITS+RADIX_BITS;
	// Set exponent bound to be 4.

	vector<int> array;
	array.resize(rep,0);
	//array[RADIX_BITS]=1;
	array[RADIX_BITS+2]=1;
	int bound=m_nFrontier;
	for(int i=0;i<INTEGER_BITS+RADIX_BITS;i++)
		PutXORGate(array[i], 0);

	// Build look-up table for Taylor's expension.
	vector< vector<int> > lookUp(16);
	vector<int> lookUpTable(16,0);
	for(int i=0;i< 16;i++)
	{
		lookUp[i].resize(rep,0);

	} 

	lookUp[0][RADIX_BITS]=lookUp[1][RADIX_BITS]= 1;
	lookUp[2][RADIX_BITS-1]=1;
	lookUp[3][RADIX_BITS-2]=lookUp[3][RADIX_BITS-4]=lookUp[3][RADIX_BITS-6]=lookUp[3][RADIX_BITS-8]=lookUp[3][RADIX_BITS-10]=1;
	lookUp[4][RADIX_BITS-2]=1;
	lookUp[5][RADIX_BITS-3]=lookUp[5][RADIX_BITS-4]=lookUp[5][RADIX_BITS-7]=lookUp[5][RADIX_BITS-8]=lookUp[5][0]=1;
	
	lookUp[6][RADIX_BITS-3]=lookUp[6][RADIX_BITS-5]=lookUp[6][RADIX_BITS-7]=lookUp[6][RADIX_BITS-9]=lookUp[6][0]=1;
	lookUp[7][RADIX_BITS-3]=lookUp[7][RADIX_BITS-6]=lookUp[7][RADIX_BITS-9]=lookUp[7][0]=1;
	lookUp[8][RADIX_BITS-3]=1;
	lookUp[9][RADIX_BITS-4]=lookUp[9][RADIX_BITS-5]=lookUp[9][RADIX_BITS-6]=lookUp[9][0]=lookUp[9][1]=1;
	
	lookUp[10][RADIX_BITS-4]=lookUp[10][RADIX_BITS-5]=lookUp[10][RADIX_BITS-8]=lookUp[10][RADIX_BITS-9]=1;

	lookUp[11][RADIX_BITS-4]=lookUp[11][RADIX_BITS-6]=lookUp[11][RADIX_BITS-7]=lookUp[11][RADIX_BITS-8]=lookUp[11][RADIX_BITS-10]=1;
	lookUp[12][RADIX_BITS-4]=lookUp[12][RADIX_BITS-6]=lookUp[12][RADIX_BITS-8]=lookUp[12][RADIX_BITS-10]=1;
	lookUp[13][RADIX_BITS-4]=lookUp[13][RADIX_BITS-7]=lookUp[13][RADIX_BITS-8]=lookUp[13][RADIX_BITS-9]=lookUp[13][RADIX_BITS-11]=1;
	lookUp[14][RADIX_BITS-4]=lookUp[14][RADIX_BITS-7]=lookUp[14][RADIX_BITS-10]=1;
	lookUp[15][RADIX_BITS-4]=lookUp[15][RADIX_BITS-8]=1;



	for(int i=0;i<16;i++)
	{
		lookUpTable[i]=m_nFrontier;
		for(int j=0;j<rep;j++)
			PutXORGate(lookUp[i][j],0);
	}



	vector<int> oneArray(rep, 0);
	oneArray[RADIX_BITS]=1;
	int one=m_nFrontier;
	for(int i=0;i<rep;i++)
		PutXORGate(oneArray[i],0);

	exponent=fPFilter(exponent,bound);
//return exponent;
	int out=one;
	for(int i=iteration;i>0;i--)
	{
		int temp=fPMultiplication(exponent,lookUpTable[i]);
		int temp2=fPMultiplication(out,temp);
		out=fPAddition(temp2,one);
	}

	return out;


}




//-------------------<function fPTwoComplement>-----------------------
// This function calculates the 2's complement of the provided value.
// Each value starts from the lowest bit.
// Input:
//		a: the start index of the provided value
// Output: the start index of the calculated complement.
int FixPoint::fPTwoComplement(int a)
{
	//TODO: do the 2's complement
	int rep = INTEGER_BITS + RADIX_BITS;

	//get 1's complement
	int oneComplement = fPOneComplement(a);

	vector<int> add;
	add.resize(rep, 0);
	add[0] = 1;
	
	// left + right mod (2^Rep)
	// Construct C[i] gates
	vector<int> C(rep);
	int axc, bxc, acNbc;

	C[0] = 0;

	int i = 0;
	for (; i<rep - 1; i++)
	{
		//===================
		// New Gates
		// oneComplement[i] xor c[i]
		axc = PutXORGate(oneComplement + i, C[i]);

		// add[i] xor c[i]
		bxc = PutXORGate(add[i], C[i]);

		// axc AND bxc
		acNbc = PutANDGate(axc, bxc);

		// C[i+1]
		C[i + 1] = PutXORGate(C[i], acNbc);
	}
	// Construct oneComplement[i] xor add[i] gates
	vector<int> AxB(rep);
	for (int i = 0; i<rep; i++)
	{
		// a[i] xor b[i]
		AxB[i] = PutXORGate(oneComplement + i, add[i]);
	}

	// Construct Output gates of Addition
	int out = m_nFrontier;
	for (int i = 0; i<rep; i++)
		PutXORGate(C[i], AxB[i]);

	return out;
}

//-------------------<function fPOneComplement>-----------------------
// This function calculates the 1's complement of the provided value.
// Each value starts from the lowest bit.
// Input:
//		a: the start index of the provided value
// Output: the start index of the calculated complement.
int FixPoint::fPOneComplement(int a)
{
	int out = m_nFrontier;
	for (int i = 0; i < INTEGER_BITS + RADIX_BITS; i++)
		PutXORGate(a + i, 1);
	return out;
}



int FixPoint::fPMultiplication(int a, int b)
{
	//TODO: do multiplication
	int extendA=fPExtendOperand(a, INTEGER_BITS+RADIX_BITS);
	int extendB=fPExtendOperand(b, INTEGER_BITS+RADIX_BITS);
	int result=PutMultGate(extendA,extendB,2*(INTEGER_BITS+RADIX_BITS));
	return (result+RADIX_BITS);
}

//---------------------<function fPExtendOperand>--------------------
// This function doubles # of bits of  the operands for fixed point multiplication.
// If the highest bit is 1, set all the extra bits to 1, otherwise, set them to 0.
int FixPoint::fPExtendOperand(int a,int numExtra)
{
	int out=m_nFrontier;
	for(int i=0;i<INTEGER_BITS+RADIX_BITS;i++)
		PutXORGate(a+i, 0);
	for(int i=0;i<numExtra;i++)
		PutXORGate(a+INTEGER_BITS+RADIX_BITS-1, 0);
	return out;
}

//-------------------<funciton fPMatrixAddition>---------------------------
// Output:: the start index of the added matrix, each elemt starts from the lower bit.
int FixPoint::fPMatrixAddition(int a, int b, int row, int col)
{
	int rep=INTEGER_BITS+RADIX_BITS;
	vector< vector<int> > addedMatrix(row);
	for(int i=0;i<row;i++)
		addedMatrix[i].resize(col);
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
		{
			addedMatrix[i][j]= fPAddition(a+i*col*rep+j*rep,b+i*col*rep+j*rep);
		}
	}
	int out=m_nFrontier;
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			for(int k=0;k<rep;k++)
			{
				PutXORGate(addedMatrix[i][j]+k,0);
			}

	return out;
}


//-------------------------<function fPMatrixSubtraction>--------------------
// This function calculates the subtraction of a and b.
int  FixPoint::fPMatrixSubtraction(int a, int b, int row, int col)
{
	int rep=INTEGER_BITS+RADIX_BITS;
	vector< vector<int> > addedMatrix(row);
	for(int i=0;i<row;i++)
		addedMatrix[i].resize(col);
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
		{
			addedMatrix[i][j]= fPSubtraction(a+i*col*rep+j*rep,b+i*col*rep+j*rep);
		}
	}
	int out=m_nFrontier;
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			for(int k=0;k<rep;k++)
			{
				PutXORGate(addedMatrix[i][j]+k,0);
			}

	return out;
}


//-------------------<function transpos>----------------------------
int FixPoint::fPtranspose(int a, int row, int col)
{
	int rep=INTEGER_BITS+RADIX_BITS;
	std::vector< std::vector<int> > res(col);
	for (int i = 0; i < col; i++)
		res[i].resize(row);
	//do transpose....
	for (int i = 0; i < col; i++)
		for (int j = 0; j < row; j++)
		{
		res[i][j] = a + rep*(j*row + i);
		}
	int transposedMatrix = m_nFrontier;
	for (int i = 0; i < col; i++)
		for (int j = 0; j < row; j++)
			for (int k = 0; k < rep; k++)
				PutXORGate(res[i][j] + k, 0);
	return transposedMatrix;
}

//---------------<function vectorAddition>--------------------
int FixPoint::fPvectorAddition(int a, int b, int length)
{
	int rep= INTEGER_BITS+RADIX_BITS;
	vector<int>  addedVector(length);
	for( int i=0;i< length;i++)
		addedVector[i]= fPAddition(a+i*rep,b+i*rep);
	int out=m_nFrontier;
	for( int i=0;i<length;i++)
		for( int j=0;j<rep;j++)
			PutXORGate(addedVector[i]+j,0);
	return out;
}

//----------------<function vectorMultiplication>--------------
// This function calculates a horizontal vector multiplies a vertical vector
int FixPoint::fPvectorMultiplication(int a, int b, int length)
{
	int rep= INTEGER_BITS+RADIX_BITS;
	vector<int> multElement(length);
	for(int i=0;i<length;i++)
		multElement[i]=fPMultiplication(a+i*rep,b+i*rep);
	int temp=multElement[0];
	for(int i=1;i<length;i++)
		temp=PutAddGate(temp,multElement[i],rep,false);
	return temp;

}

//---------------<function fPvectorMultiplication>------------------
// This function calculates a horizontal vector multiplies a vertical vector
// The vertical vector is in an array.
int FixPoint::fPvectorMultiplication(int a, vector<int> b, int length)
{
	int rep= INTEGER_BITS+RADIX_BITS;
	vector<int> multElement(length);
	for(int i=0;i<length;i++)
		multElement[i]=fPMultiplication(a+i*rep,b[i]);
	int temp=multElement[0];
	for(int i=1;i<length;i++)
		temp=PutAddGate(temp,multElement[i],rep,false);
	return temp;
}

//------------------<function fPMatrixScalaProduct>------------------------------
// Input:
//		a: the start index of the matrix
//		row: # of rows in the matrix
//		col: # of columns in the matrix
//		b: the start index of the scala.
// Output: the start index of the calculated matrix
int FixPoint::fPMatrixScalaProduct(int a, int row, int col, int b)
{
	int rep=INTEGER_BITS+RADIX_BITS;
	vector< vector<int> > multMatrix(row);
	for( int i=0;i<row;i++)
		multMatrix[i].resize(col);
	for(int i=0;i<row;i++)
		for (int j=0;j<col;j++)
			multMatrix[i][j]= fPMultiplication(a+i*col*rep+j*rep,b);

	int out=m_nFrontier;
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			for(int k=0;k<rep;k++)
			{
				PutXORGate(multMatrix[i][j]+k,0);
			}

	return out;
}



//------------------------<function matrixInnerProduct>------------------------
// This function calculates the inner product or dot product of two matrix.
// The length of elements of both the matrix should be the same.
// Input:
//		a: the start index of the left matrix
//		b: the start index of the right matrix
//		m: the rows of the left matrix
//		n: the column of the left matrix, it is also the row of the right matrix
//		p: the column of the right matrix
// Output: the start index of the multiplied matrix
int FixPoint::fPMatrixInnerProduct(int a, int b, int m, int n,int p)
{
	int rep=INTEGER_BITS+RADIX_BITS;

	//TODO: do matrix inner multiplication.
	vector<int> leftMatrix(m);
	//vector<int> rightMatrix(p);
	vector<vector<int> > rightMatrix(p);
	vector<vector<int> > innerProduct(m);
	for(int i=0;i<m;i++)
		innerProduct[i].resize(p);

	for(int i=0;i<m;i++)
		leftMatrix[i]=a+i*n*rep;

	// arrange the right matrix to vertical vectors
/*
	for(int i=0;i<p;i++)
	{
		rightMatrix[i]=m_nFrontier;
		for(int j=0;j<n;j++)
		{
			for(int k=0;k<rep;k++)
			{
				PutXORGate(b+j*p*rep+i*rep+k,0);
			}
		}
	}

	for(int i=0;i<m;i++)
		for(int j=0;j<p;j++)
			innerProduct[i][j]=fPvectorMultiplication(leftMatrix[i],rightMatrix[j],n);

	int out=m_nFrontier;
	for(int i=0;i<m;i++)
		for(int j=0;j<p;j++)
			for(int k=0;k<rep;k++)
				PutXORGate(innerProduct[i][j]+k,0);
*/
	// Re-arrange the right matrix into p*n arrays

	for(int i=0;i<p;i++)
	{
		rightMatrix[i].resize(n);
		for(int j=0;j<n;j++)
		{
			rightMatrix[i][j]=b+j*p*rep+i*rep;
		}
	}


	// Do the multiplication
	for(int i=0;i<m;i++)
		for(int j=0;j<p;j++)
			innerProduct[i][j]=fPvectorMultiplication(leftMatrix[i],rightMatrix[j],n);

	int out=m_nFrontier;
	for(int i=0;i<m;i++)
		for(int j=0;j<p;j++)
			for(int k=0;k<rep;k++)
				PutXORGate(innerProduct[i][j]+k,0);



	return out;
}

//-----------------<function fPMatrixInnerProductStrassen>-----------------
// This function calculates the inner product of two matrix using Strassen Algorithm.
// The two matrix should both be square matrix, so the m, n and p should be the same and be even.
int FixPoint::fPMatrixInnerProductStrassen(int a, int b, int m, int n, int p)
{

	int rep=INTEGER_BITS+RADIX_BITS;
	int half=m/2;
	// Re-arrange the A and B matrix, split them each into 4 sub-matrixs.
	vector<vector<int> > matrixA(2);
	vector<vector<int> > matrixB(2);
	vector<vector<int> > matrixC(2);
	for(int i=0;i<2;i++)
	{
		matrixA[i].resize(2);
		matrixB[i].resize(2);
		matrixC[i].resize(2);
	}
	
	for(int i=0;i<2;i++)
		for(int j=0;j<2;j++)
		{
			matrixA[i][j]=m_nFrontier;
			for(int x=0;x<half;x++)
				for(int y=0;y<half;y++)
					for( int k=0;k<rep;k++)
					{
						PutXORGate(a+i*n*half*rep+x*n*rep+j*half*rep+y*rep+k,0);
					}

		}


	for(int i=0;i<2;i++)
		for(int j=0;j<2;j++)
		{
			matrixB[i][j]=m_nFrontier;
			for(int x=0;x<half;x++)
				for(int y=0;y<half;y++)
					for( int k=0;k<rep;k++)
					{
						PutXORGate(b+i*n*half*rep+x*n*rep+j*half*rep+y*rep+k,0);
					}

		}

		// Use Strassen Algorithm to calcualte the inner product.
		// calculate the intermedia results Ms.
		int B12SubB22=fPMatrixSubtraction(matrixB[0][1],matrixB[1][1],half,half);
		int M1=fPMatrixInnerProduct(matrixA[0][0],B12SubB22,half,half,half);

		int A11AddA12=fPMatrixAddition(matrixA[0][0],matrixA[0][1],half,half);
		int M2=fPMatrixInnerProduct(A11AddA12,matrixB[1][1],half,half,half);

		int A21AddA22=fPMatrixAddition(matrixA[1][0],matrixA[1][1],half,half);
		int M3=fPMatrixInnerProduct(A21AddA22,matrixB[0][0],half,half,half);

		int B21SubB11=fPMatrixSubtraction(matrixB[1][0],matrixB[0][0],half,half);
		int M4=fPMatrixInnerProduct(matrixA[1][1],B21SubB11,half,half,half);

		int A11AddA22=fPMatrixAddition(matrixA[0][0],matrixA[1][1],half,half);
		int B11AddB22=fPMatrixAddition(matrixB[0][0],matrixB[1][1],half,half);
		int M5=fPMatrixInnerProduct(A11AddA22,B11AddB22,half,half,half);

		int A12SubA22=fPMatrixSubtraction(matrixA[0][1],matrixA[1][1],half,half);
		int B21AddB22=fPMatrixAddition(matrixB[1][0],matrixB[1][1],half,half);
		int M6=fPMatrixInnerProduct(A12SubA22,B21AddB22,half,half,half);

		int A11SubA21=fPMatrixSubtraction(matrixA[0][0],matrixA[1][0],half,half);
		int B11AddB12=fPMatrixAddition(matrixB[0][0],matrixB[0][1],half,half);
		int M7=fPMatrixInnerProduct(A11SubA21,B11AddB12,half,half,half);

		int M5AddM4=fPMatrixAddition(M5,M4,half,half);
		int M5AddM4SubM2=fPMatrixSubtraction(M5AddM4,M2,half,half);
		matrixC[0][0]=fPMatrixAddition(M5AddM4SubM2,M6,half,half);

		matrixC[0][1]=fPMatrixAddition(M1,M2,half,half);
		matrixC[1][0]=fPMatrixAddition(M3,M4,half,half);

		int M5AddM1=fPMatrixAddition(M5,M1,half,half);
		int M5AddM1SubM3=fPMatrixSubtraction(M5AddM1,M3,half,half);
		matrixC[1][1]=fPMatrixSubtraction(M5AddM1SubM3,M7,half,half);

		// Arrange the output;
		int out=m_nFrontier;

		for(int i=0;i<m;i++)
			for(int j=0;j<m;j++)
			{
				int c=0;
				if(i/half==0)
				{
					if(j/half==0)
						c=matrixC[0][0];
					else
						c=matrixC[0][1];
				}
				else
				{
					if(j/half==0)
						c=matrixC[1][0];
					else
						c=matrixC[1][1];
				}
				for(int k=0;k<rep;k++)
				{
					PutXORGate(c+(i%half)*half*rep+(j%half)*rep+k,0);
				}

			}

		return out;	
}

//--------------------<function constructIdentity>-----------------------
// This function construct the identity matrix.
int FixPoint::constructIdentity(int row, int col)
{
	int rep=INTEGER_BITS+RADIX_BITS;
	vector<int> elementOne(rep,0);
	vector<int> elementZero(rep,0);
	int p=PutXORGate(1,0);//1
	int q=PutXORGate(0,0);//0
	elementOne[RADIX_BITS]=1;

	vector<vector<vector<int > > > identity(row);
	for(int i=0;i<row;i++)
	{
		identity[i].resize(col);
		for(int j=0;j<col;j++)
		{
			identity[i][j].resize(rep);
			if(i==j)
				identity[i][j]=elementOne;
			else
				identity[i][j]=elementZero;
		}
	}
	//return identity;
	int out=m_nFrontier;
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			for(int k=0;k<rep;k++)
			{
				if(identity[i][j][k]==0)
					PutXORGate(q,q);
				else
					PutXORGate(p,q);
			}
	return out;
}

//-----------------<function fPMatrixInversion>--------------------------
// This funciton calculates the inversion of a specific matrix.
// The row and col should be the same.
int FixPoint::fPMatrixInversion(int a, int row, int col, int iteration, int index)
{
	int identity=constructIdentity(row,col);
	//TODO: do the inversion.
	int aPrime=fPtranspose(a,row, col);
	// Allocate the value to devide
	vector<int> devide(INTEGER_BITS+RADIX_BITS,0);
	//devide[4]=devide[5]=devide[6]=devide[3]=1;
	devide[index]=1;
	// NOT FINISHED
	int p=PutXORGate(1,0);//1
	int q=PutXORGate(0,0);//0
	int b=m_nFrontier;
	for(int i=0;i<INTEGER_BITS+RADIX_BITS;i++)
		if(devide[i]==0)
			PutXORGate(q,q);
		else 
			PutXORGate(p,q);

	int inv_a=fPMatrixScalaProduct(aPrime,col,row,b);
	//inv_a=fPMatrixScalaProduct(inv_a,col,row,b);
		//int inv_a=aPrime;
	// allocate two
	vector<int> vectorTwo(INTEGER_BITS+RADIX_BITS,0);
	vectorTwo[RADIX_BITS+1]=1;
	int two=m_nFrontier;
	for(int i=0;i<INTEGER_BITS+RADIX_BITS;i++)
		if(vectorTwo[i]==0)
			PutXORGate(q,q);
		else 
			PutXORGate(p,q);


	int _2TimesIden=fPMatrixScalaProduct(identity,col,row,two);

	for(int i=0;i< iteration;i++)
	{	
		if(row%2==1)
		{
			int aTimesInv_a=fPMatrixInnerProduct(a,inv_a,col,row,col);
			int sub=fPMatrixSubtraction(_2TimesIden,aTimesInv_a,col,row);
			inv_a=fPMatrixInnerProduct(inv_a,sub,col,row,col);
		}
		// Try inner product through strassen algorithm.
		else{
		int aTimesInv_a=fPMatrixInnerProductStrassen(a,inv_a,col,row,col);
		int sub=fPMatrixSubtraction(_2TimesIden,aTimesInv_a,col,row);
		inv_a=fPMatrixInnerProductStrassen(inv_a,sub,col,row,col);
		}
	}
	return inv_a;
}


void FixPoint::InitGates()
{
	m_othStartGate = m_nFrontier;
	m_pGates = new GATE[m_nNumGates];
	m_nNumXORs = 0;

	GATE* gate;

	for (int i = 0; i<m_othStartGate; i++)
	{
		gate = m_pGates + i;
		gate->p_ids = 0;
		gate->p_num = 0;
		gate->left = -1;
		gate->right = -1;
		gate->type = 0;
	}
}




//------------------<function logRegNewton>-------------------------
// This function calculate the beta for logistic regression by using 
// Newton's method.
// Input:
//		inversion: the inversed matrix (col*col)
//		x: the matrix of information 
//		row: the # of rows in x
//		col: the # of columns in x
//		y: the vector of probability. its length is row
int FixPoint::logRegNewton(int inversion, int x, int row, int col, int y)
{
	int rep=INTEGER_BITS+RADIX_BITS;
	int iteration=20;

	// Initialize the beta
	int p=PutXORGate(0,0);
	int beta=m_nFrontier;
	for(int j=0; j<col;j++)
		for(int i=0;i<rep;i++)
			PutXORGate(p,p);


	int derivative=0;
	// calcuate the beta
	for(int i=0;i<iteration;i++)
	{
		//int temp=fPMatrixInnerProduct(inversion,derivative,row,row,1);
		// the length of derivative is col.
		derivative=fPMatrixFirstderiv( x, y,beta, row, col);
		int temp=fPMatrixInnerProduct(inversion, derivative, col, col,1);
		beta=fPMatrixAddition(beta,temp,col,1);
	}
	return beta;
}


//--------------------<function fPLogitFunction>-----------------------------
// This function calculates the logit function, given the x vector( horizontal), and beta vector( vertical).
// exp(-(x*beta))
int FixPoint::fPLogitFunction(int x, int beta, int length)
{

	int rep=INTEGER_BITS+RADIX_BITS;
	vector<int> oneArray(rep, 0);
	oneArray[RADIX_BITS]=1;
	int one=m_nFrontier;
	for(int i=0;i<rep;i++)
		PutXORGate(oneArray[i],0);


	int exponent=fPvectorMultiplication(x, beta, length);
	exponent=fPTwoComplement(exponent);
	int exp= fPExponenetial(exponent, 15);
	int expAddOne=fPAddition(exp,one);
	return fPMatrixInversion(expAddOne, 1, 1,11,3);

}

//--------------------<function fPMatrixFirstderiv>-------------------------------
// This function calculates the first dericative for the given matrix.
// Input:
//		x: the given matrix
//		y: the vector containing probability.
//		beta: the beta vector
//		row: the # of rows in x, and the lenght of vector y.
//		col: the # of columns in x
int FixPoint::fPMatrixFirstderiv(int x, int y ,int beta, int row, int col)
{
	int rep=INTEGER_BITS+RADIX_BITS;
	vector<int> logFuncVector(row,0);
	vector<int> firstDerivVector(col,0);

	// calculate the logit function value for each record.
	for (int i=0;i<row;i++)
	{
		logFuncVector[i]=fPLogitFunction(x+i*col*rep,beta,col);
	}

	// allocate zero array
	int p=PutXORGate(0,0);
	int zero=m_nFrontier;
	for(int i=0;i<rep;i++)
		PutXORGate(p,p);

	// calculate the fist derivative.

	vector<int> ySubLogFuncVector(row,0);
	for(int i=0;i<row;i++)
	{
		ySubLogFuncVector[i]= fPSubtraction(y+i*rep, logFuncVector[i]);
	}



	for(int i=0;i<col;i++)
	{
		int out=zero;
		for(int j=0;j<row;j++)
		{
			
			int temp2=fPMultiplication(ySubLogFuncVector[j], x+j*col*rep+i*rep);
			out=fPAddition(out,temp2);
		}
		firstDerivVector[i]=out;
	}

	int out=m_nFrontier;
	for(int i=0;i<col;i++)
		for(int j=0;j<rep;j++)
			PutXORGate( firstDerivVector[i]+j,0);

	return out;

}