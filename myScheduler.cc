#include "myScheduler.hh"
#include <sstream>
#include <iostream>
#include <fstream>

/* 
	Ensures strict 2PL and then passes command on to Data Manager
	Return value passed back by Data Manager or blocked status
*/

myScheduler::myScheduler(int dT, int nT):
	detectTime(dT), numThreads(nT)
{
	// Keep track if myPTM is done and then once done with queues scheduler is done
	// bool tmDone = false;
	// bool schedDone = false;
	
	schedulerLog.push_back("Initializing Scheduler.");
	
	
}

bool myScheduler::handleCommand(int TID, string parsedCommand[], int TID_type) 
{
	// used to output TID to log
	stringstream ss;
	ss << TID;
	string base = parsedCommand[0]; // base command
	string one = parsedCommand[1]; // 1st substring -- filename/Emode
	string two = parsedCommand[2]; // 2nd substring -- val / record
	
	// Command is read/mult_read/write/delete 
	if (base == "R" || base == "M" || base == "W" || base == "D") {
		// TID has necessary lock
		if (checkLock(base,TID,one)) 
		{
			// Pass on to Data Manager
			schedulerLog.push_back(ss.str() + " already has lock for command: " + base + " on " + one);
			return true;
		
		}// end if TID has lock
	
		// TID doesn't have lock
		else 
		{
			bool lockStatus = reqLock(base,TID,one); 
			
			if (!lockStatus)
			{
				// Add to lockTable, wfgMatrix return blocked
				schedulerLog.push_back(ss.str() + " blocked on lock for command: " + base + " on " + one);
				return true;
			}
			else 
			{
				schedulerLog.push_back(ss.str() + " obtains lock for command: " + base + " on " + one);
				// Pass on to Data Manager
				return true;
			}
			
		} // end else TID tries to get lock	
	} // End command is read/write/delete
	
	else if (base == "C" || base == "A") 
	{
		// Pass on to Data Manager
		releaseLocks(TID);
		return true;
	} // End is commit/abort
	
	// Error - unknown command
	else {
		//@TODO	cout << "Unknown command: " + parsedCommand[0] + endl;
		return true;
	}
}

// Release the locks that TID has
void myScheduler::releaseLocks(int TID) 
{
	
}

//@TODO -- RIGHT NOW ONLY RETURNS TRUE
bool myScheduler::checkGetLock(int TID, bool read, bool process, string filename, int recordID) {
	// Check if filename lock is in file_locks
	tr1::unordered_map<string, struct lock_tuple>::const_iterator got_file = file_locks.find(filename);
	// If not found, then add it and give TID requested lock
	if (got_file == file_locks.end()) {
		// 1. Add filename lock to file_locks
		
		// 2. Add record lock to record_locks
		
		return true;
	}
	else {
		// If recordID == -1 then TID wants record_level lock so put intention at file_level
		if (recordID == -1) {
			// 1. Add file_level lock
			
			// 2. Add record_level lock
			
			return true;
		}
		// Regular lock at file_level
		else {
			
			return true;
		} 
	}
	return true;
}
	
// Check if TID has lock of type on dataItem; Return false no / true yes
bool myScheduler::checkLock(string type, int TID, string dataItem) 
{
	int int_type = 0;
	if (type == "R" || type == "M") int_type = 0;
	if (currDataFiles.size() == 0) {

		return true;
	}
	else {
		// record-level read/write
		if (type == "R" || type == "W") {
			
		}
		// file-level read/write
		// type == "M" || type == "D"
		else {
	
		}
	}
	return false;
}

// Attempt to acquire lock of type on dataItem; Return false failure / true success
bool myScheduler::reqLock(string type, int TID, string dataItem)
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