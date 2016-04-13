//////////////////////////////////////////////////////////////////////////
// matrixLib.cpp - Implementation of functions in matrixLib.h			//
// version 1.0															//
//----------------------------------------------------------------------//
// Haoyi Shi (c) copyright 2015					                        //
// hshi05@syr.edu														//
// All rights granted provided this copyright notice is retained        //
//----------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////
#include "matrixLib.h"
#include <cmath> 
#include <cassert>
using namespace std;
BOOL MatrixLib::Create(int nParties, const vector<int>& vParams)
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
cout<<"vParams[0] "<< vParams[0]<<endl;
	m_nRep = vParams[0];
	m_nPartyLength = GetNumBits(m_nNumParties-1	);
	m_vNumVBits.resize(m_nNumParties, 1);
	int distBits = GetNumBits(m_nRep)+1;
	m_vInputStart.resize(nParties);
	m_vInputEnd.resize(nParties);

	m_nFrontier = 2;
	for (int i = 0; i < nParties; i++)
	{
		m_vInputStart[i] = m_nFrontier;
		m_nFrontier = m_nFrontier+(m_nRep+m_nPartyLength);
		m_vInputEnd[i] = m_nFrontier - 1;
	}
	m_othStartGate = m_nFrontier;
	int gates_hamming =m_nNumParties*( m_nRep +4*distBits *pow(2, distBits));
	int gate_noninput = gates_hamming + m_nNumParties*m_nPartyLength+100;
	
	m_nNumGates = gate_noninput + m_nFrontier+500;

	InitGates();

	//calculate the hamming distance between party1 and other party.
	for (int i = 1; i < m_nNumParties; i++)
	{
		HammDistanceCalculator(0, i, m_nRep);
		
	}
	
	PutOutputLayer();

	PatchParentFields();
	return TRUE;
}


void MatrixLib:: HammDistanceCalculator(int partyId1, int partyId2, int rep)
{
	int start_1 = m_vInputStart[partyId1];
	int start_2 = m_vInputStart[partyId2];

	vector<int> layer1;
	for (int i = 0; i < rep; i++)
	{
		int t = PutXORGate((start_1 + i), (start_2 + i));
		layer1.push_back(t);
	}
	distCounter(layer1);
	
	
}

//------------------------<function distCounter>---------------------------
// count the 1s in a vector, the result starts from the lower bit
// eg. 2 is 01, 6 is 011.


int MatrixLib:: distCounter(vector<int> distVector)

{
	int vectorSize = distVector.size();
	int bitNum = GetNumBits(vectorSize);
	for (int i = vectorSize; i < pow(2, bitNum); i++)
	{
		distVector.push_back(0);
	}

	int newVectorSize = pow(2, bitNum);
	int length = 1;
	while (newVectorSize > 1)
	{ 
		vector<int> vectorTemp;
		for (int i = 0; i < newVectorSize ; i=i+2)
		{
			int out=PutAddGate(distVector[i], distVector[i + 1], length, true);
			vectorTemp.push_back(out);
		}
		newVectorSize = newVectorSize / 2;
		distVector.resize(newVectorSize);
		distVector = vectorTemp;
		length++;
	}
	
	return distVector[0];
}


//-----------------------<function GetNumBits>--------------------------
// gets # of bits for a decimal number
// input: decimal number
// output: # of bits for the input
// note: input should not be a negative int
int MatrixLib::GetNumBits(int decimal)
{
	int num_bits = 1;
	int tmp;

	if (decimal < 0){
		puts("CP2PCircuit::GetNumBits(int): negative input");
		exit(-1);
	}
	else if (decimal == 0){
		return 0;
	}
	else if (decimal == 1){
		return 1;
	}

	tmp = decimal;
	while (1 == 1){
		num_bits++;
		tmp = tmp / 2;
		if (tmp <= 1){
			break;
		}
	}

	return num_bits;
}

void MatrixLib::InitGates()
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
//--------------------<function PutOutputLayer>----------------
void MatrixLib::PutOutputLayer()
{
	//int o_start = matrixAddition(m_vInputStart[0],m_vInputStart[1],2,2,2,false);
	//transpose(m_vInputStart[1],2,2,2);


	//int o_start=PutMultGate(m_vInputStart[0],m_vInputStart[1],2);

	//int o_start=matrixScalaProduct(m_vInputStart[0],2,2,m_vInputStart[1],3);

	//int o_start=vectorAddition(m_vInputStart[0],m_vInputStart[1],5,2,false);
	//int o_start=vectorMultiplication(m_vInputStart[0],m_vInputStart[1],5,3);

	int o_start=matrixInnerProduct(m_vInputStart[0],m_vInputStart[1],3,4,2,2);

	int o_end = m_nFrontier - 1;
	m_vOutputStart.resize(m_nNumParties, o_start);
	m_vOutputEnd.resize(m_nNumParties, o_end);
	m_nNumGates = m_nFrontier;
}

//------------------<function transpose>--------------------------------
// This function provides support for transposing a matrix.
// Inputs:
//		a: the starting index of the matrix in m_pGates
//		row: rows of the original matrix
//		col: columns of the original matrix
//		rep: the length (bits) of each element in the matrix
// Output: the starting index of the transposed matrix in m_pGates
int MatrixLib::transpose(int a, int row, int col, int rep)
{

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
//-------------------------<function matrixAddition>----------------------
// Output:: the start index of the added matrix, each elemt starts from the lower bit.
// Eg., 6 is 0 1 1
int MatrixLib::matrixAddition(int a, int b, int row, int col, int rep, BOOL bCarry)
{
	vector< vector<int> > addedMatrix(row);
	for(int i=0;i<row;i++)
		addedMatrix[i].resize(col);
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
		{
			addedMatrix[i][j]= PutAddGate(a+i*col*rep+j*rep,b+i*col*rep+j*rep,rep,bCarry);
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

//-------------------------<function matrixScalaProduct>--------------------------------
// This function calculate the product of a matrix and  a scala.
// The length of each element and the scala are the same, and length of the elements of the
// product are the same as that of the scala.
// Input:
//		a: the start index of the matrix
//		row: the rows of the matrix
//		col: the columns of the matrix
//		b: the start index of the scala
//		rep: the length of the element in the matrix and the scala
// Output: the start index of the product.
// Note: each element starts from the lowest bit, eg. 6 is 0 1 1.
int MatrixLib::matrixScalaProduct(int a, int row, int col, int b, int rep)
{
	vector< vector<int> > multMatrix(row);
	for( int i=0;i<row;i++)
		multMatrix[i].resize(col);
	for(int i=0;i<row;i++)
		for (int j=0;j<col;j++)
			multMatrix[i][j]= PutMultGate(a+i*col*rep+j*rep,b,rep);

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
//		rep: the length of the elements of the matrix
// Output: the start index of the multiplied matrix
// Note: each element starts from the lowest bit, eg. 6 is 0 1 1.
int MatrixLib::matrixInnerProduct(int a, int b, int m, int n, int p, int rep)
{
	//TODO: do matrix inner multiplication.
	vector<int> leftMatrix(m);
	vector<int> rightMatrix(p);
	vector<vector<int> > innerProduct(m);
	for(int i=0;i<m;i++)
		innerProduct[i].resize(p);

	for(int i=0;i<m;i++)
		leftMatrix[i]=a+i*n*rep;

	// arrange the right matrix to vertical vectors
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
			innerProduct[i][j]=vectorMultiplication(leftMatrix[i],rightMatrix[j],n,rep);

	int out=m_nFrontier;
	for(int i=0;i<m;i++)
		for(int j=0;j<p;j++)
			for(int k=0;k<rep;k++)
				PutXORGate(innerProduct[i][j]+k,0);
	return out;
}


//---------------------<function vectorAddition>---------------------------
// This function calculate the addition of two vectors.
// The lengths of the two vectors are the same.
// Input:
// 		a: the start index of the first vector
//		b: the start index of the second vector
//		length: the length of the vectors
//		rep: the bit length of each element in the vectors
//		bCarry: whether the carrier is needed or not for the highest bit, suggested to be false
// Output: the start index of the added vector
int  MatrixLib::vectorAddition(int a, int b, int length, int rep, BOOL bCarry)
{
	vector<int>  addedVector(length);
	for( int i=0;i< length;i++)
		addedVector[i]= PutAddGate(a+i*rep,b+i*rep,rep,bCarry);
	int out=m_nFrontier;
	for( int i=0;i<length;i++)
		for( int j=0;j<rep;j++)
			PutXORGate(addedVector[i]+j,0);
	return out;
}

//------------------<function vectorMultiplication>---------------------
// This function calculate the multiplication of a horizontal vector and a vertical vector.
// The horizontal vector is the first vector, while the vertical one is the second.
// Input:
//		a: the start index of the horizontal vector
//		b: the start index of the vertical vector
//		length: the length of the vector ( # of elements in the vectors)
//		rep: the bit length of each element
// Output: the start index of the calculated the result. The result is a number rather than a vector.
int MatrixLib::vectorMultiplication(int a, int b, int length, int rep)
{
	vector<int> multElement(length);
	for(int i=0;i<length;i++)
		multElement[i]=PutMultGate(a+i*rep,b+i*rep,rep);
	int temp=multElement[0];
	for(int i=1;i<length;i++)
		temp=PutAddGate(temp,multElement[i],rep,false);
	return temp;
}