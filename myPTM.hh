    
#ifndef _MYPTM_H_
#define _MYPTM_H_

#include <string>
#include <cstdio>
#include <iostream>
#include <vector>
#include <time.h>

#define _EXECUTION_ERROR_ -1
#define _INSERTION_ERROR_ -2
#define _LOCKFILE_ERROR_  -3

using namespace std;

class myPTM
{
	public:
        
		myPTM(){}; // default constructor
        myPTM(vector < vector<string> > cT, int rM);
	
	private:
		vector < vector<string> > currTrans;
		int readMode;
		
		vector<string> *transactionLog;
		
		void parseCommands(string *script, int numCommands);
		void handleCommand(string command, int TID );
		void undoEffects(int TID);
};

#endif