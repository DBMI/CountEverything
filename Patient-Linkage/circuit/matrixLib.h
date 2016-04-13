#ifndef MATRIXLIB_H
#define MATRIXLIB_H
//////////////////////////////////////////////////////////////////////////
// matrixLib.h - Matrix manipulation libraty for GMW					//
// version 1.0															//
//----------------------------------------------------------------------//
// Haoyi Shi (c) copyright 2015					                        //
// hshi05@syr.edu														//
// All rights granted provided this copyright notice is retained        //
//----------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////

/*
* Module Operation:
* =================
* This module provides a single class for 2-dimentional matrix manipulations.
* 
* Required files:
* ===============
* circuit.h, circuit.cpp
*/
#include "circuit.h"
#include <cassert>
class MatrixLib :public CCircuit{
public:
	BOOL Create(int nParties, const vector<int>& vParams);
	int	GetNumBits(int decimal);
	void PutOutputLayer();
	void InitGates();
	void HammDistanceCalculator(int partyId1, int partyId2, int rep);
	int distCounter(vector<int> distVector);


	int matrixAddition(int a, int b, int row, int col, int rep, BOOL bCarry);
	int transpose(int a, int row, int col, int rep);
	int matrixScalaProduct(int a, int row, int col, int b, int rep);
	int matrixInnerProduct(int a, int b, int m, int n,int p, int rep);
	int vectorAddition(int a, int b, int length, int rep, BOOL bCarry);
	int vectorMultiplication(int a, int b, int length, int rep);
private:
	int m_nRep;
	int m_nPartyLength;
};
#endif
