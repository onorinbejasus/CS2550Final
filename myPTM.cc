#include "myPTM.hh"

myPTM::myPTM()
{
    
}

myPTM::myPTM(vector<string> cmd)
{
    int i = 0;
    for(i = 0; i < cmd.size(); i++)
    {
        string subCmd = cmd[i];
        //execute subCmd
    }
}

int myPTM::lockRows(int start, int end)
{
    m_start = start;
    m_end = end;
    
    
    return -1;
}

int myPTM::unlockRows(void)
{
    m_start = -1;
    m_end = -1;
    
    return -1;
}

int myPTM::writeOut(void* buffer, void* dataset)
{
    
    
    return -1;
}