#ifndef _MYSCHEDULER_H_
#define _MYSCHEDULER_H_

#include <string>
#include <vector>
#include <queue>

// threading libraries
extern "C"
{
    #include <pthread.h>
    #include <unistd.h>
}

using namespace std;

class myScheduler
{
	
	public:
		myScheduler(){}; // Default Constructor
    	myScheduler(int dT, int nT);
		bool handleCommand(string *command, int TID);
        
	private:
        int detectTime;
				int numThreads;
				vector<string> *lockTable;
				vector<int> *wfgMatrix;
        void detectDeadlock();
				bool checkLock(int type, int TID, string *dataItem);
				bool reqLock(int type, int TID, string *dataItem);
				void releaseLocks(int TID);
};

#endif