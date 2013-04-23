    
#ifndef _MYPTM_H_
#define _MYPTM_H_

#include <string>
#include <cstdio>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <time.h>
#include <queue>
#include <sstream>

//#include "myScheduler.hh"

// threading libraries
extern "C"
 {
    #include <pthread.h>
    #include <unistd.h>
 }

using namespace std;

class myPTM
{
	public:
        
		myPTM(){}; // default constructor
        myPTM(vector < vector<string> > cT, int rM);
	
	private:
		
		vector < vector<string> > currTrans;
		
		int readMode;
		
		/* array of threads */
		pthread_t *threads;
		
		// for printing numbers
		stringstream ss;
		
		/* scheduler */
		//myScheduler *scheduler;		
						
		void parseCommands(string *script, int numCommands, int* id, int numScripts);
		void undoEffects(int TID);
};

#endif