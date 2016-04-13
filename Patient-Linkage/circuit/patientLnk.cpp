//////////////////////////////////////////////////////////////////////////
// patientLnk.cpp - Patient Linkage project for GMW						//
// version 1.0															//
//----------------------------------------------------------------------//
// Haoyi Shi (c) copyright 2015					                        //
// hshi05@syr.edu														//
// All rights granted provided this copyright notice is retained        //
//----------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////

#include "patientLnk.h"
#include <vector>
#include <cmath>
using namespace std;
const int NUM_HASH=7;
const int HASH_BITS=512;
BOOL PatientLnk::Create(int nParties, const vector<int>& vParams)
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
	m_nNumGates = m_othStartGate + 1000000000;
	InitGates();


	// do calculation.
	// calculate the number of same hashs.
	int record_num_reference=1;

	int record_num=150;
	vector<vector<int> > sameHashVector(record_num_reference);
	vector<int> output;
	for( int i=0;i<record_num_reference;i++)
	{
		for(int j=0;j<record_num;j++)
		{
			sameHashVector[i].push_back( compareRecordHashs(m_vInputStart[0]+NUM_HASH*HASH_BITS*i,m_vInputStart[1]+NUM_HASH*HASH_BITS*j,NUM_HASH,HASH_BITS));
		}
		output.push_back( getLargestHashRecord(sameHashVector[i]));
	}

	//int o_start=getLargestHashRecord(sameHashVector);

	int distBits = GetNumBits(NUM_HASH)+1;
	int recdIdBits = GetNumBits(record_num);
	int o_start=m_nFrontier;
	for( int i=0;i<output.size();i++)
	{
		for(int j=0;j<distBits+recdIdBits;j++)
			PutXORGate(output[i]+j,0);
	}

	int o_end=m_nFrontier-1;
	m_vOutputStart.resize(m_nNumParties,o_start);
	m_vOutputEnd.resize(m_nNumParties,o_end);
	cout<<"o_end  "<<o_end<<endl;



	m_nNumGates = m_nFrontier;
	PatchParentFields();
	return TRUE;
}

//--------------------<function compareHash>-----------------
// This function compares two hash values, checking whether they are equal or not.
int PatientLnk::compareHash(int a, int b, int rep)
{
	return PutEQGate(a, b,rep);
}

void PatientLnk::InitGates()
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


void PatientLnk::putOutputLayer()
{
}


//--------------<function distCounter)---------------
// This fucntion counts the 1s in a vector, the result starts from the lower bit
// eg. 2 is 01, 6 is 011.
int PatientLnk::distCounter(vector<int> distVector)
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
int PatientLnk::GetNumBits(int decimal)
{
	int num_bits = 1;
	int tmp;

	if (decimal < 0){
		puts("PatientLnk::GetNumBits(int): negative input");
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


//--------------<function compareRecordHashs>--------------------
// This function calculates the number of the same hashs between two records.
// Input:
//		record1: the start index of the first record.
//		record2: the start index of the second record.
//		n_hash: the number of hashs of each record.
//		hashBits: the number of bits in each hash.
int PatientLnk::compareRecordHashs(int record1, int record2, int n_hash, int hashBits)
{
	vector<int> hashEQVector;
	for( int i=0;i<n_hash;i++)
	{
		hashEQVector.push_back(compareHash(record1+hashBits*i,record2+hashBits*i,hashBits));

	}
	return distCounter(hashEQVector);
}

//---------------<function getLargestHashRecord>-------------------
// This function finds the record which has the highest similarity with the reference record.
// Input:
//		sameHashVector: the vector containing the similarty between the compared records with the reference record.
// Output: The id of the record with the highest similarity.
// The ids start from 1 rather than 0.
int PatientLnk::getLargestHashRecord(vector<int> sameHashVector)
{
	int distBits = GetNumBits(NUM_HASH)+1;
	int num_record=sameHashVector.size();
	int recdIdBits = GetNumBits(num_record);

	//cout<<"::getLargestHashRecord  number of records :"<<num_record<<endl;
	//cout<<"::getLargestHashRecord  number of record bits: "<< recdIdBits<<endl;

	int largest=sameHashVector[0];
	int id_largest=m_vInputStart[1]+NUM_HASH*HASH_BITS*num_record;
	for( int i=1;i<num_record;i++)
	{
		int gt=PutGTGate(largest, sameHashVector[i], distBits);
		largest=PutMUXGate(largest, sameHashVector[i],gt, distBits, false);
		id_largest=PutMUXGate(id_largest,m_vInputStart[1]+NUM_HASH*HASH_BITS*num_record+recdIdBits*i,gt,recdIdBits,false);
	}
	//return id_largest;
	int out=m_nFrontier;
	for( int i=0;i<distBits;i++)
		PutXORGate(largest+i,0);
	for(int i=0;i<recdIdBits;i++)
		PutXORGate(id_largest+i,0);
	return out;
}