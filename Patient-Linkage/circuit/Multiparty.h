#ifndef MULTIPARTY_H
#define MULTIPARTY_H


#include "circuit.h"
#include <cassert>
class Multiparty : public CCircuit
{
public:
	BOOL Create(int nParties, const vector<int>& vParams);
	
	int CompareGivenName(int a, int b, int rep);
	int CompareFamilyName(int a, int b, int rep);
	int CompareBirthDate(int a, int b, int rep);
	int CompareGender(int a, int b, int rep);
  int CompareEmail(int a,int b,int rep);
  int CompareState(int a,int b,int rep);
  int CompareCity(int a, int b,int rep);
  int CompareZipcode(int a, int b,int rep);
  int CompareAddress(int a,int b, int rep);
  int CompareSSN(int a,int b,int rep);
	int CompareGivenNameGroup(vector<int> record,int nParties);
	int CompareFamilyNameGroup(vector<int> record,int nParties);
	int CompareBirthDateGroup(vector<int> record,int nParties);
	int CompareGenderGroup(vector<int> record,int nParties);
  int CompareEmailGroup(vector<int> record,int nParties);
  int CompareStateGroup(vector<int> record,int nParties);
  int CompareZipcodeGroup(vector<int> record,int nParties);
  int CompareAddressGroup(vector<int> record,int nParties);
  int CompareSSNGroup(vector<int> record, int nParties);
  int CompareCityGroup(vector<int> record,int nParties);
	int CompareRecords(vector<int> record,int nParties);
	vector <vector <int> > RecordIndex(int n,vector<int> record_num,int nParties);
	void InitGates();
	void putOutputLayer();

private:
	
	

};
#endif