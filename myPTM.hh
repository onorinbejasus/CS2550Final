    
#ifndef _MYPTM_H_
#define _MYPTM_H_

#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

#define _EXECUTION_ERROR_ -1
#define _INSERTION_ERROR_ -2
#define _LOCKFILE_ERROR_  -3
//#include ""
using namespace std;

class myPTM
{
	
	public:
        
		myPTM(); // default constructor
        myPTM(vector<string> cmd);
	private:
        float m_tran_id;
        int m_start;
        int m_end;
    
        int lockRows(int start, int end);
        int unlockRows(void);
        int writeOut(void* buffer,void* dataset);
        int executeCommand(string cmd);
};

#endif