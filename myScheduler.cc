#include "myScheduler.hh"
#include <sstream>
#include <iostream>
#include <fstream>

/* 
	Ensures strict 2PL and then passes command on to Data Manager
	Return value passed back by Data Manager or blocked status
*/

// Mutexes
pthread_mutex_t sched_log_mutex = PTHREAD_MUTEX_INITIALIZER;

// Scheduler log in memory
vector<string> schedulerLog;

myScheduler::myScheduler(int dT, int nT):
	detectTime(dT), numThreads(nT)
{
	// Keep track if myPTM is done and then once done with queues scheduler is done
	bool tmDone = false;
	bool schedDone = false;
	
	schedulerLog.push_back("Initializing Scheduler.");

	// How to access array of threads? Or just expose sched_command_queue?
}

bool myScheduler::handleCommand(string *command, int TID) 
{
	cout << "thread id: " << TID << "command: " << command << "\n";
	//@TODO Use command to determine which lock needed if read/multiple read/write/delete then
	int type = 0;
	string * dataItem = NULL;
	
	//@TODO Command is read/write/delete 
	if (type == 0)
	{
		// TID has necessary lock
		if (checkLock(type,TID, dataItem)) 
		{
			// Pass on to Data Manager
			return true;
		
		}// end if TID has lock
	
		// TID doesn't have lock
		else 
		{
			bool lockStatus = reqLock(type,TID,dataItem); 
			
			if (!lockStatus)
			{
				// Add to lockTable, wfgMatrix return blocked
				return true;
			}
			else 
			{
				// Pass on to Data Manager
				return true;
			}
			
		} // end else TID tries to get lock	
	} // End command is read/write/delete
	
	//@TODO Command is commit/abort
	else if (type == 1) 
	{
		// Pass on to Data Manager
		releaseLocks(TID);
		return true;
	} // End is commit/abort
	
	// Error - unknown command
	else {
		return false;
	}
}

// Release the locks that TID has
void myScheduler::releaseLocks(int TID) 
{
	
}
	
// Check if TID has lock of type on dataItem; Return false no / true yes
bool myScheduler::checkLock(int type, int TID, string *dataItem) 
{
	return false;
}

// Attempt to acquire lock of type on dataItem; Return false failure / true success
bool myScheduler::reqLock(int type, int TID, string *dataItem)
{
	return false;
}

// Use the wfgMatrix to detect deadlocks
void myScheduler::detectDeadlock() 
{
	//@TODO Check for deadlock
	bool deadlock = false;
	
	if (deadlock) {
		//@TODO Select arbitrary transaction involved in cycle
		int TID = 0;
		// Let TM know to abort transaction
		// Let DB know about aborted transaction
		releaseLocks(TID);
		// Update time between detections
		detectTime = detectTime/2;
	}
	else {
		detectTime = detectTime*4;
	}
}