#ifndef _MYSCHEDULER_H_
#define _MYSCHEDULER_H_

#include <string>
#include <vector>

using namespace std;

class myScheduler
{
	
	public:
		myScheduler(){}; // Default Constructor
    	myScheduler(int dT);
		int handleCommand(string *command, int TID, string *dataItem);
        
	private:
        int detectTime;
				vector<string> *lockTable;
				vector<int> *wfgMatrix;
        void releaseLocks(int TID); 
        int checkLock(int type, int TID, string *dataItem);
        int reqLock(int type, int TID, string *dataItem);
        void detectDeadlock();
};

#endif