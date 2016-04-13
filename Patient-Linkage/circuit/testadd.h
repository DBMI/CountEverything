// testadd.h
#ifndef __TEST_ADD_H__BY_SGCHOI_
#define __TEST_ADD_H__BY_SGCHOI_
 
#include "circuit.h"
#include <cassert>

class CTestAddCircuit : public CCircuit
{
public:
	BOOL Create(int nParties, const vector<int>& vParams);
 
public:
	void PutAdditionLayer();
	void PutOutputLayer();
	void PutEqGateLayer();
	int GetNumBits(int);
	void PutXORLayer();
private:
	int m_nWidth;
	int	m_nDepth;
	int m_nRep;
	int num_inputs;
	int outputBits;
	vector<int> eqGateOutput;
	vector<int> inputToAdd;
	vector<int>	m_vLayerInputs;
	vector<int> m_vLayerOutputs;
};

#endif //__TEST_ADD_H__BY_SGCHOI_
