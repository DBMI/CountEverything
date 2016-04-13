#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H
//////////////////////////////////////////////////////////////////////////
// fixedPoint.h - Fixed point operations for GMW						//
// version 1.0															//
//----------------------------------------------------------------------//
// Haoyi Shi (c) copyright 2015					                        //
// hshi05@syr.edu														//
// All rights granted provided this copyright notice is retained        //
//----------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////

/*
* Module Operations:
* =================
* This file provides a single class for fixed point operations based on GMW.
*
* Required Files:
* ================
* fixedPoint.h, fixedPoint.cpp, circuit.h, circuit.cpp
*
*/
#include "circuit.h"
#include <cassert>
class FixPoint : public CCircuit
{
public:
	BOOL Create(int nParties, const vector<int>& vParams);
	int fPAddition(int a, int b);
	int fPSubtraction(int a, int b);
	int fPTwoComplement(int a);
	int fPOneComplement(int a);
	int fPMultiplication(int a, int b);
	int fPExtendOperand(int a, int numExtra);
	int fPExponenetial(int exponent, int iteration);
	// Matrix operations
	int fPMatrixAddition(int a, int b, int row, int col);
	int fPMatrixSubtraction(int a, int b, int row, int col);
	int fPtranspose(int a, int row, int col);
	int fPvectorAddition(int a, int b, int length);
	int fPvectorMultiplication(int a, int b, int length);
	int fPvectorMultiplication(int a, vector<int> b, int length);
	int fPMatrixScalaProduct(int a, int row, int col, int b);
	int fPMatrixInnerProduct(int a, int b, int m, int n,int p);
	int fPMatrixInnerProductStrassen(int a, int b, int m, int n, int p);
	int fPMatrixInversion(int a, int row, int col,int iteration, int index);
	int constructIdentity(int row, int col);
	int logRegNewton(int inversion, int x, int row, int col, int y);
	int fPLogitFunction(int x, int beta, int length);
	int fPMatrixFirstderiv(int x, int y,int beta, int row, int col);

	int fPFilter(int a, int bound);

	void InitGates();
	void putOutputLayer();
private:
	int m_nRep;
};
#endif