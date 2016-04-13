
#include "Multiparty.h"
#include <vector>
#include <cmath>
using namespace std;

#define GenderBits 1
#define FamilyNameBits 150
#define GivenNameBits 150
#define BirthDateBits 25
#define CityBits 125
#define SSNBits 30
#define StateBits 125
#define EmailBits 270
#define ZipcodeBits 17
#define AddressBits 270



BOOL Multiparty::Create(int nParties, const vector<int>& vParams)
{
	m_nNumParties = nParties;

	if (vParams.size() < (unsigned)1)
	{
		cout << "Error! This circuit needs " << 1
			<< "parameters for vector length"
			<< endl;
		return FALSE;
	}
	for(int i=0;i<vParams.size();i++)
		cout<<vParams[i]<<" ";
	cout<<endl;
  
	vector<int> record_num(nParties);
	for(int i=0;i<nParties;i++)
		record_num[i]=vParams[i];
  
	m_vNumVBits.resize(m_nNumParties, 1);
	m_vInputStart.resize(nParties);
	m_vInputEnd.resize(nParties);

	// allocate gates for inputs
  m_nFrontier = 2;
	for (int i = 0; i < nParties; i++)
	{
		m_vInputStart[i] = m_nFrontier;
		m_nFrontier = m_nFrontier + record_num[i]*(GenderBits+FamilyNameBits+GivenNameBits+BirthDateBits+CityBits+SSNBits+StateBits+EmailBits+AddressBits+ZipcodeBits);
		m_vInputEnd[i] = m_nFrontier - 1;
	}
 
	//calculate the gates for circuit.
	m_othStartGate = m_nFrontier;
	m_nNumGates = m_othStartGate + 2000000000;
	InitGates();

	vector<int> output;
	int record_sum_num=1;
	vector <int> RecordResult;

	for(int i=0;i<nParties;i++)
		record_sum_num=record_sum_num*record_num[i];
	vector <vector<int> > Record_ind=RecordIndex(record_sum_num,record_num,nParties);

	vector<int> Result_ind(nParties);
	for (int i=0;i<record_sum_num;i++)
	{
  //  cout<<Record_ind[i][0]<<" "<<Record_ind[i][1]<<" "<<Record_ind[i][2]<<endl;
		for(int j=0;j<nParties;j++)
		{
		
		  Result_ind[j]=m_vInputStart[j]+Record_ind[i][j]*(GenderBits+FamilyNameBits+GivenNameBits+BirthDateBits+CityBits+SSNBits+StateBits+EmailBits+AddressBits+ZipcodeBits);
		}
		  RecordResult.push_back(CompareRecords(Result_ind,nParties));
      
	}
  
	int o_start=m_nFrontier;
 
	for( int i=0;i<record_sum_num;i++)
	{
		
			PutXORGate( RecordResult[i],0);
	}
 

  int o_end=m_nFrontier-1;
 
	m_vOutputStart.resize(m_nNumParties,o_start);
	m_vOutputEnd.resize(m_nNumParties,o_end);
	cout<<"o_end  "<<o_end<<endl;
	

	m_nNumGates = m_nFrontier;
	PatchParentFields();
	





	return TRUE;
}
int Multiparty::CompareGender(int a,int b,int rep)
{
	return PutEQGate(a, b,rep);
}

int Multiparty::CompareFamilyName(int a,int b,int rep)
{
	return PutEQGate(a, b,rep);
}

int Multiparty::CompareGivenName(int a,int b,int rep)
{
	return PutEQGate(a, b,rep);
}

int Multiparty::CompareBirthDate(int a,int b,int rep)
{
	return PutEQGate(a,b,rep);
}

int Multiparty::CompareState(int a,int b,int rep)
{
  return PutEQGate(a, b,rep);
}

int Multiparty::CompareCity(int a, int b,int rep)
{
	return PutEQGate(a,b,rep);
}

int Multiparty::CompareZipcode(int a, int b,int rep)
{
	return PutEQGate(a,b,rep);
}

int Multiparty::CompareAddress(int a,int b, int rep)
{
	return PutEQGate(a,b,rep);
}

int Multiparty::CompareSSN(int a,int b,int rep)
{
	return PutEQGate(a,b,rep);
}
int Multiparty::CompareEmail(int a,int b,int rep)
{
	return PutEQGate(a,b,rep);
}

int Multiparty::CompareGenderGroup(vector<int> record,int nParties)
{
	int i=0;
	vector<int> Gender;
	for(i=1;i<nParties;i++)
	{
		Gender.push_back(CompareGender(record[0],record[i],GenderBits));
	}
	return  PutWideGate(G_AND, Gender);
}

int Multiparty::CompareFamilyNameGroup(vector<int> record,int nParties)
{
	int i=0;
	vector<int> FamilyName;
	for(i=1;i<nParties;i++)
	{
		FamilyName.push_back(CompareFamilyName(record[0],record[i],FamilyNameBits));
	}
	return  PutWideGate(G_AND, FamilyName);
}

int Multiparty::CompareGivenNameGroup(vector<int> record,int nParties)
{
	int i=0;
	vector<int> GivenName;
	for(i=1;i<nParties;i++)
	{
		GivenName.push_back(CompareGivenName(record[0],record[i],GivenNameBits));
	}
	return  PutWideGate(G_AND, GivenName);
}
int Multiparty::CompareBirthDateGroup(vector<int> record,int nParties)
{
	int i=0;
	vector<int> BirthDate;
	for(i=1;i<nParties;i++)
	{
		BirthDate.push_back(CompareBirthDate(record[0],record[i],BirthDateBits));
	}
	return  PutWideGate(G_AND, BirthDate);
}

int Multiparty::CompareEmailGroup(vector<int> record,int nParties)
{
   int i=0;
   vector<int> Email;
   for(i=1;i<nParties;i++)
   {
     Email.push_back(CompareEmail(record[0],record[i],EmailBits));
   }
   return PutWideGate(G_AND,Email);
}

int Multiparty::CompareStateGroup(vector<int> record,int nParties)
{
   int i=0;
   vector<int> State;
   for(i=1;i<nParties;i++)
   {
     State.push_back(CompareState(record[0],record[i],StateBits));
   }
   return PutWideGate(G_AND,State);
}

int Multiparty::CompareZipcodeGroup(vector<int> record,int nParties)
{
   int i=0;
   vector<int> Zipcode;
   for(i=1;i<nParties;i++)
   {
     Zipcode.push_back(CompareZipcode(record[0],record[i],ZipcodeBits));
   }
   return PutWideGate(G_AND,Zipcode);
}

int Multiparty::CompareAddressGroup(vector<int> record,int nParties)
{
   int i=0;
   vector<int> Address;
   for(i=1;i<nParties;i++)
   {
     Address.push_back(CompareAddress(record[0],record[i],AddressBits));
   }
   return PutWideGate(G_AND,Address);
}

int Multiparty::CompareSSNGroup(vector<int> record, int nParties)
{
   int i=0;
   vector<int> SSN;
   for(i=1;i<nParties;i++)
   {
     SSN.push_back(CompareState(record[0],record[i],SSNBits));
   }
   return PutWideGate(G_AND,SSN);
}

int Multiparty::CompareCityGroup(vector<int> record,int nParties)
{
   int i=0;
   vector<int> City;
   for(i=1;i<nParties;i++)
   {
     City.push_back(CompareState(record[0],record[i],CityBits));
   }
   return PutWideGate(G_AND,City);
}

int Multiparty::CompareRecords(vector<int> record,int nParties)
{
	vector<int> record_temp(nParties);
	int i=0;
	vector<int> R_section;

	 for(i=0;i<nParties;i++)
	   record_temp[i]=record[i];
   R_section.push_back(CompareGivenNameGroup(record_temp,nParties));
   for(i=0;i<nParties;i++)
	   record_temp[i]=record_temp[i]+GivenNameBits;
   R_section.push_back(CompareFamilyNameGroup(record_temp,nParties));
   for(i=0;i<nParties;i++)
	   record_temp[i]=record_temp[i]+FamilyNameBits;
   R_section.push_back(CompareEmailGroup(record_temp,nParties));
   for(i=0;i<nParties;i++)
	   record_temp[i]=record_temp[i]+EmailBits;
   R_section.push_back(CompareAddressGroup(record_temp,nParties));
   for(i=0;i<nParties;i++)
	   record_temp[i]=record_temp[i]+AddressBits;
   R_section.push_back(CompareSSNGroup(record_temp,nParties));
   for(i=0;i<nParties;i++)
	   record_temp[i]=record_temp[i]+SSNBits;
   R_section.push_back(CompareGenderGroup(record_temp,nParties));
   for(i=0;i<nParties;i++)
	   record_temp[i]=record_temp[i]+GenderBits;
   R_section.push_back(CompareCityGroup(record_temp,nParties));
   for(i=0;i<nParties;i++)
	   record_temp[i]=record_temp[i]+CityBits;
   R_section.push_back(CompareZipcodeGroup(record_temp,nParties));
   for(i=0;i<nParties;i++)
	   record_temp[i]=record_temp[i]+ZipcodeBits;
   R_section.push_back(CompareStateGroup(record_temp,nParties));
   for(i=0;i<nParties;i++)
	   record_temp[i]=record_temp[i]+StateBits;
   R_section.push_back(CompareBirthDateGroup(record_temp,nParties));
   
   
   return  PutWideGate(G_AND, R_section);
}
vector<vector <int> > Multiparty::RecordIndex(int n,vector<int> record_num,int nParties)
{
	int i=0,j=0;
	vector<vector<int> >Result(n);
	vector<int> Temp(nParties);
	Temp[nParties-1]=record_num[nParties-1];
	for(i=nParties-2;i>=0;i--)
	{
		Temp[i]=Temp[i+1]*record_num[i];
	}
	int temp_num=n;
	int b=0;
	for(i=0;i<n;i++)
	{
    temp_num =i;
		for(j=0;j<nParties-1;j++)
		{
			b=temp_num;
			temp_num=temp_num/Temp[j+1];
			Result[i].push_back(temp_num);
			temp_num=b-Temp[j+1]*temp_num;
		}
		Result[i].push_back(temp_num);
	}
	return Result;
}

void Multiparty::InitGates()
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

void Multiparty::putOutputLayer()
{
}