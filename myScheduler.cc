#include "myScheduler.hh"

/* 
	Ensures strict 2PL and then passes command on to Data Manager
	Return value passed back by Data Manager or blocked status
*/

myScheduler::myScheduler(int dT):
	detectTime(dT)
{
	
}

int myScheduler::handleCommand(string *command, int TID, string *dataItem) 
{
	//@TODO Use command to determine which lock needed if read/multiple read/write/delete then
	int type = 0;
	
	//@TODO Command is read/write/delete 
	if (type == 0)
	{
		// TID has necessary lock
		if (checkLock(type,TID, dataItem) == 1) 
		{
			// Pass on to Data Manager
			return 1;
		
		}// end if TID has lock
	
		// TID doesn't have lcok
		else 
		{
			int lockStatus = reqLock(type,TID,dataItem); 
			
			if (lockStatus == -1)
			{
				// Add to lockTable, wfgMatrix return blocked
				return -1;
			}
			else 
			{
				// Pass on to Data Manager
				return 1;
			}
			
		} // end else TID tries to get lock	
	} // End command is read/write/delete
	
	//@TODO Command is commit/abort
	else if (type == 1) 
	{
		// Pass on to Data Manager
		releaseLocks(TID);
		return 1;
	} // End is commit/abort
	
	// Error - unknown command
	else {
		return -1;
	}
}

// Release the locks that TID has
void myScheduler::releaseLocks(int TID) 
{
	
}
	
// Check if TID has lock of type on dataItem; Return -1 no / 1 yes
int myScheduler::checkLock(int type, int TID, string *dataItem) 
{
	return -1;
}

// Attempt to acquire lock of type on dataItem; Return -1 failure / 1 success
int myScheduler::reqLock(int type, int TID, string *dataItem)
{
	return -1;
}

// Use the wfgMatrix to detect deadlocks
void myScheduler::detectDeadlock() 
{
	//@TODO Check for deadlock
	int deadlock = -1;
	
	if (deadlock == 1) {
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