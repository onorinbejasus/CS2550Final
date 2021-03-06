#ifndef _MYSCHEDULER_H_
#define _MYSCHEDULER_H_

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <iostream>
#include <tr1/unordered_map>
#include "myPAGE.hh"
#include <cstdio>
#include <cstdlib>

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
	string command; // W, D, R, M
	bool intention; // true = intention | false = actual
 	int mode; // true = process | false = transaction
};
// int record id is key
struct record_lock {
	vector <struct lock_tuple> *waitList; // TIDs waiting for this record lock
	vector <struct lock_tuple> *currList; // TIDs currently holding this record lock

};
// string filename is key
struct file_lock {

	tr1::unordered_map<int, struct record_lock> *record_locks; // locks at record level
	vector <struct lock_tuple> *waitList; // TIDs waiting for this file lock
	vector <struct lock_tuple> *currList; // TIDs currently holding this file lock
	bool blocked;
	
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
				bool checkLock(string type, int TID,  string dataItem);
				bool reqLock(string type, int TID, int mode, string dataItem, string filename);
				void releaseLocks(int TID, string filename);
				//void releaseLock(int TID, string filename);
				vector<string> currDataFiles;
				
				tr1::unordered_map<string, struct file_lock> file_locks; // locks at file level
				bool checkGetLock(int TID, string base, bool process, string filename, int recordID);
};

#endif