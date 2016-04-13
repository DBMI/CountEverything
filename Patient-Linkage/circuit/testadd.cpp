// testadd.cpp

#include "testadd.h"
#include <cassert>
using namespace std;

BOOL CTestAddCircuit::Create(int nParties, const vector<int>& vParams)
{
    m_nNumParties = nParties;

    // parse params
    // params: width, depth, rep
    if( vParams.size() < (unsigned) 4 )
    {
        cout << "Error! This circuit needs " << 4  
             << "parameters: width depth rep num_inputs"
             << endl;
        return FALSE;
    }

    m_nWidth = vParams[0];
    m_nDepth = vParams[1];
    m_nRep = vParams[2];
    num_inputs = vParams[3];
    outputBits=GetNumBits(m_nWidth);
    m_vNumVBits.resize(m_nNumParties, 1);
    
    // gates for inputs: only the first party will have one input
    m_vInputStart.resize(nParties);
    m_vInputEnd.resize(nParties);
        
    m_nFrontier = 2;
    m_vInputStart[0] = m_nFrontier;
    m_nFrontier += m_nWidth*m_nRep;
    m_vInputEnd[0] =  m_nFrontier - 1;
    m_vInputStart[1] = m_nFrontier;
    m_nFrontier +=  m_nWidth*m_nRep;
    m_vInputEnd[1] = m_nFrontier - 1;
    for(int i=2; i<nParties; i++)
    {
        m_vInputStart[i] = 1;
        m_vInputEnd[i] = 0;
    }

    //===========================================================================
    // computes roughly the number of gates beforehand --- need this for allocating space
    // gates for each equality gate:
    // bitwise xor, bitwise negation, AND of each bitwise outputs
    int gates_eq = m_nRep*m_nRep + 2*m_nRep;
    int gates_add = 6*m_nRep;
    int xorlayer = m_nWidth*m_nWidth; 
    int gates_noninput = m_nDepth*m_nWidth*m_nWidth*gates_add + 2*m_nWidth*m_nRep + 2*gates_eq + xorlayer + 10000 + m_nWidth*outputBits;
         
    m_othStartGate = m_nFrontier;
    m_nNumGates = m_nFrontier + gates_noninput;
    m_pGates = new GATE[m_nNumGates];
    m_nNumXORs = 0;
     //   m_nNumGates += 4;
    GATE* gate;

    for(int i=0; i<m_othStartGate; i++)
    {
        gate = m_pGates + i;
        gate->p_ids = 0;
        gate->p_num = 0;
        gate->left = -1;
        gate->right = -1;
        gate->type = 0;
    }

    // Prepare 0-th input layer: 
    // all of them are the first party's input
    m_vLayerInputs.resize(m_nWidth*2);
    m_vLayerOutputs.resize(1,2);
    eqGateOutput.resize(m_nWidth);
    
         inputToAdd.resize(m_nWidth);
    
    for(int i=0; i<m_nDepth; i++)
    {
        PutEqGateLayer();
        PutXORLayer();
        m_vLayerOutputs[0]=PutWideAddGate(inputToAdd,outputBits);
//        inputToAdd[0]=PutXORGate(1,eqGateOutput[0]);
//        PutXORGate(0,0);    
//        inputToAdd[1]=PutXORGate(1,eqGateOutput[1]);
//              PutXORGate(0,0);
//        m_vLayerInputs = m_vLayerOutputs;
//        PutAdditionLayer();
    }

    PutOutputLayer();
    PatchParentFields();
    return TRUE;
};

void CTestAddCircuit::PutXORLayer()
{
    for(int i=0;i<m_nWidth;i++)
    {
     inputToAdd[i]=PutXORGate(eqGateOutput[i],1);
    //    cout<<outputBits<<"\n";
        for(int j=1;j<outputBits;j++)
        {
        PutXORGate(0,0);
        }
    }

}
/*
void CTestAddCircuit::PutAdditionLayer()
{
    int size = m_vLayerInputs.size();
        m_vLayerInputs[1]=7; 
//    for(int i=0; i<size; i++)
//    {
//        int j = (i+1) % size;
        m_vLayerOutputs[0] = PutAddGate(inputToAdd[0], inputToAdd[1],2 );
//    }       
}
*/
int CTestAddCircuit::GetNumBits(int decimal)
{
int num_bits = 1;
    int tmp;

    if (decimal < 0){
        puts("CP2PCircuit::GetNumBits(int): negative input");
        exit(-1);
    }
    else if(decimal == 0){
        return 0;
    }
    else if(decimal == 1){
        return 1;
    }

    tmp = decimal;
    while(1==1){
        num_bits++;
        tmp = tmp/2;
        if (tmp <= 1){
            break;
        }
    }

    return num_bits;
}

void CTestAddCircuit::PutEqGateLayer()
{
    int j=2;
    for(int i=0;i<m_nWidth;i++)
        {
            eqGateOutput[i]=PutEQGate(j,j+(m_nWidth*m_nRep),m_nRep);
        //    cout<<j <<" "<<j+m_nWidth*m_nRep<<"\n";            
            j += m_nRep;        
        }
}

void CTestAddCircuit::PutOutputLayer()
{
    int o_start = m_nFrontier;
    for(int i=0; i<1; i++)
    {
        for(int j=0; j<outputBits; j++)
        {
            PutXORGate(m_vLayerOutputs[i] + j, 0);     // did changes here m_vLayerOutputs
        }
    }
    int o_end = m_nFrontier -1;

    m_vOutputStart.resize(m_nNumParties, o_start);
    m_vOutputEnd.resize(m_nNumParties, o_end);
    m_nNumGates = m_nFrontier;
}

 
