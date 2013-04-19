#ifndef _MYPTM_H_
#define _MYPTM_H_

#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

//#include ""
using namespace std;

class myPTM{
	
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
};

#endif