#ifndef _MYSCHEDULER_H_
#define _MYSCHEDULER_H_

#include <string>
#include <vector>
#include <queue>
#include <map>

// threading libraries
extern "C"
{
    #include <pthread.h>
    #include <unistd.h>
}

using namespace std;

/*
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
*/

struct lock_elem {
	queue <int> prop_TIDs; // first TID is one that has it, unless read then all have it
	int lock_type; // read = 0 or write = 1
	int Emode; // proc = 0 or trans = 1
	int record_level; // record_level = 1 @ record or record_level = 0 @ file
	queue <int> prop_IDs; // record ID's that TID's want to read
};

class myScheduler
{
	
	public:
		myScheduler(){}; // Default Constructor
    	myScheduler(int dT, int nT);
		bool handleCommand(int TID, string parsedCommand[], int TID_type);
		vector<string> schedulerLog;
        
	private:
        int detectTime;
				int numThreads;
				vector<int> *wfgMatrix;
        void detectDeadlock();
				bool checkLock(string type, int TID, string dataItem);
				bool reqLock(string type, int TID, string dataItem);
				void releaseLocks(int TID);
				vector<string> currDataFiles;
				vector<lock_elem> dataLocks;
};

#endif