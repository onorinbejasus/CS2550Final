    
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
	
		// return true if empty, false otherwise
		bool checkQueue(int TID) { return command_queue[TID].empty(); };
		
		// get the next item in the queue
		string popQueue(int TID){
			
			string command = command_queue[TID].front();
			command_queue[TID].pop();
			return command;
		};
	
	private:
		
		vector < vector<string> > currTrans;
		
		int readMode;
		
		/* array of threads */
		pthread_t *threads;
		
		/* command queues */
		queue<string> *command_queue;
				
		vector<string> *transactionLog;
		
		void parseCommands(string *script, int numCommands);
		void undoEffects(int TID);
};

#endif