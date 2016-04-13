#ifndef PATIENTLNK_H
#define PATIENTLNK_H
//////////////////////////////////////////////////////////////////////////
// patientLnk.h - Patient Linkage project for GMW						//
// version 1.0															//
//----------------------------------------------------------------------//
// Haoyi Shi (c) copyright 2015					                        //
// hshi05@syr.edu														//
// All rights granted provided this copyright notice is retained        //
//----------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////


#include "circuit.h"
#include <cassert>
class PatientLnk : public CCircuit
{
public:
	BOOL Create(int nParties, const vector<int>& vParams);
	int compareHash(int a, int b, int rep);
	int distCounter(vector<int> distVector);
	int GetNumBits(int decimal);
	int compareRecordHashs(int record1, int record2, int n_hash, int hashBits);
	int getLargestHashRecord(vector<int> sameHashVector);

	void InitGates();
	void putOutputLayer();
private:
	int m_nRep;
};
#endif