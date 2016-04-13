#ifndef MULTIPARTYA_H
#define MULTIPARTYA_H


#include "circuit.h"
#include <cassert>
class Multipartya : public CCircuit
{
public:
	BOOL Create(int nParties, const vector<int>& vParams);
	
	int CompareCategory(int a, int b, int rep);
	
  vector<vector<vector<int> > >CompareTwoParty(vector<int> record, int nParties);
  int Computation(vector<int>  data);
  
	vector <vector <int> > RecordIndex(int n,vector<int> record_num,int nParties);
  int Myaddition(vector< vector<int> >data);
  vector< int> myxor(vector< vector<int> >data);
	void InitGates();
	void putOutputLayer();
  int Compare(int a,int b,int rep);
  

private: 
  
  vector<int > mystart;


};
#endif