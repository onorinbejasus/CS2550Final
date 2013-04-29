#ifndef _MYSCHEDULER_H_
#define _MYSCHEDULER_H_

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <tr1/unordered_map>
#include "myPAGE.hh"

// threading libraries
extern "C"
{
    #include <pthread.h>
    #include <unistd.h>
}

struct args
 {
    int ID;
	int EMode; // EMode=1 Transaction | EMode=0 Process
	int blocked;
};

using namespace std;

// Generic lock tuple with information common to record or file lock
struct lock_tuple {
	int TID; // TID
	char command; // true = read | false = write
	bool intention; // true = intention | false = actual
 	int mode; // true = process | false = transaction
};

// string filename is key
struct file_lock {
	tr1::unordered_map<int, struct lock_tuple> record_locks; // locks at record level
	queue <struct lock_tuple> waitList; // TIDs waiting for this file lock
	queue <struct lock_tuple> currList; // TIDs currently holding this file lock
};

// int record id is key
struct record_lock {
	queue <struct lock_tuple> waitList; // TIDs waiting for this record lock
	queue <struct lock_tuple> currList; // TIDs currently holding this record lock
};

class myScheduler
{
	
	public:
		myScheduler(){}; // Default Constructor
    	myScheduler(int dT, int nT);
		bool handleCommand(int TID, string parsedCommand[], int TID_type, struct args myArgs);
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
				
				tr1::unordered_map<string, struct file_lock> file_locks; // locks at file level
				bool checkGetLock(int TID, char base, bool process, string filename, int recordID);
};

#endif