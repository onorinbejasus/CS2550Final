    
#ifndef _MYPTM_H_
#define _MYPTM_H_

#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

#define _EXECUTION_ERROR_ -1
#define _INSERTION_ERROR_ -2
#define _LOCKFILE_ERROR_  -3

using namespace std;

class myPTM
{
	public:
        
		myPTM(){}; // default constructor
        myPTM(vector<string> cT, int rM);
	
	private:
		vector<string> currTrans
		int readMode;
		FILE *fp;
};

#endif