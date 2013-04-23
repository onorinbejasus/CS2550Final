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

struct file_lock_args {
	string type; // type of lock
	int holding[]; // TID's of transactions holding lock
	char filename[1]; // data filename
	int waiting[]; // TID's of waiting transactions
};

struct record_lock_args {
	string type; // type of lock
	int holding[]; // TID's of transactions holding lock
	char filename[1]; // data filename
	int record_ID[]; // record ID
	int waiting[]; // TID's of waiting transactions
};

class myScheduler
{
	
	public:
		myScheduler(){}; // Default Constructor
    	myScheduler(int dT, int nT);
		bool handleCommand(int TID, string parsedCommand[]);
		vector<string> schedulerLog;
        
	private:
        int detectTime;
				int numThreads;
				vector<string> *lockTable;
				vector<int> *wfgMatrix;
        void detectDeadlock();
				bool checkLock(string type, int TID, string dataItem);
				bool reqLock(string type, int TID, string dataItem);
				void releaseLocks(int TID);
};

#endif