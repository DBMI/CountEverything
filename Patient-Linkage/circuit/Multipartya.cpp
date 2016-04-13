
#include "Multipartya.h"
#include <vector>
#include <cmath>
using namespace std;
/*
#define GenderBits 1
#define FamilyNameBits 120
#define GivenNameBits 120
#define BirthDateBits 25
#define CityBits 125
#define SSNBits 30
#define StateBits 125
#define EmailBits 270
#define ZipcodeBits 17
#define AddressBits 270

*/
#define HashBits 49
#define Bits 8


BOOL Multipartya::Create(int nParties, const vector<int>& vParams)
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
  
   mystart.resize(nParties);


  
	m_vNumVBits.resize(m_nNumParties, 1);
	m_vInputStart.resize(nParties);
	m_vInputEnd.resize(nParties);

	// allocate gates for inputs
  m_nFrontier = 2;
	for (int i = 0; i < nParties; i++)
	{
		m_vInputStart[i] = m_nFrontier;
    mystart[i]=m_vInputStart[i];
		m_nFrontier = m_nFrontier + HashBits*vParams[i];
		m_vInputEnd[i] = m_nFrontier - 1;
	}
  
  cout<<1<<endl;
	//calculate the gates for circuit.
	m_othStartGate = m_nFrontier;
	m_nNumGates = m_othStartGate + 80000000;
	InitGates();

	vector<int> output;
	int record_sum_num=1;
	vector<vector <int> > RecordResult;
  RecordResult.resize(vParams[0]);
  
	for(int i=0;i<nParties;i++)
		record_sum_num=record_sum_num*record_num[i];

	vector <vector<int> > Record_ind=RecordIndex(record_sum_num,record_num,nParties);
  
  vector<vector<vector<int > > > Result_temp=CompareTwoParty(record_num,nParties);
  
	vector<int > Result_ind(nParties-1);
	for (int i=0;i<record_sum_num;i++)
	{
  //  cout<<Record_ind[i][0]<<" "<<Record_ind[i][1]<<" "<<Record_ind[i][2]<<endl;
  //   cout<<i<<endl;
		for(int j=0;j<nParties-1;j++)
		{
		  Result_ind[j]=Result_temp[j][Record_ind[i][j]][Record_ind[i][j+1]];
  
		}
   
		  RecordResult[Record_ind[i][0]].push_back(Computation(Result_ind));
     
      
	}
 cout<<1<<endl;
  int result=Myaddition(RecordResult);
	int o_start=m_nFrontier;
  for (int i=0;i<Bits+1;i++)
     PutXORGate( result+i,0);
	



  int o_end=m_nFrontier-1;
 
	m_vOutputStart.resize(m_nNumParties,o_start);
	m_vOutputEnd.resize(m_nNumParties,o_end);
	cout<<"o_end  "<<o_end<<endl;
	

	m_nNumGates = m_nFrontier;
	PatchParentFields();
 


	return TRUE;
}
int Multipartya::CompareCategory(int a,int b,int rep)
{
	return PutEQGate(a, b,rep);
}


vector<vector<vector<int> > > Multipartya::CompareTwoParty(vector<int> record, int nParties)
{
   int i=0,j=0,k=0;
   
   vector<vector<vector<int> > >result(nParties-1);
   vector<int> record_temp(2);
   for (i=0;i<nParties-1;i++)
   {
      result[i].resize(record[i]);
      for(j=0;j<record[i];j++)
      {
        result[i][j].resize(record[i+1] );
         
        for(k=0;k<record[i+1];k++)
        {
           
           
           record_temp[0]=mystart[i]+j*HashBits;
           record_temp[1]=mystart[i+1]+k*HashBits;
           result[i][j][k]=CompareCategory(record_temp[0],record_temp[1],HashBits);
          
        }
      }
   }
   return result;
}

        



vector<vector <int> > Multipartya::RecordIndex(int n,vector<int> record_num,int nParties)
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

void Multipartya::InitGates()
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

int Multipartya::Computation(vector< int  > data)
{
   int i=0;
   
   int k=data.size();
   vector<int> temp(k);
   int result;
  
   
    for(i=0;i<k;i++)
      temp[i]=data[i];
    result=PutWideGate(G_AND,temp);

  
   return result;
}

void Multipartya::putOutputLayer()
{
}

vector<int> Multipartya::myxor(vector< vector<int> > data)
{
   int s=data.size();
   
   vector <int> result;
   int k;
   for (int i=0;i<s;i++)
    {  k=PutWideGate(G_XOR, data[i]);
      result.push_back(k);
    }
    return result;
}
    
    



int Multipartya::Myaddition(vector< vector<int> > data1)
{
   int result;
   
   vector <int> data=myxor(data1);
   
   
   int s=data.size();
   vector <int> temp(s);
   
   for(int i=0;i<s;i++)
   {
     temp[i]=PutXORGate( data[i],0);
     for(int j=0;j<Bits;j++)
       PutXORGate( 0,0);
   }
   result= PutWideAddGate(temp,Bits+1);
   return result;

}


