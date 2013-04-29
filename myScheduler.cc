#include "myScheduler.hh"
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm> 
/* 
	Ensures strict 2PL and then passes command on to Data Manager
	Return value passed back by Data Manager or blocked status
*/

// // function to find recordID
// struct find_id : std::unary_function<struct lock, bool> {
//     int id;
//     find_id(int id):id(id) { }
//     bool operator()(struct lock_tuple const& m) const {
//         return m.recordID == id;
//     }
// };

myScheduler::myScheduler(int dT, int nT):
	detectTime(dT), numThreads(nT)
{
	// Keep track if myPTM is done and then once done with queues scheduler is done
	// bool tmDone = false;
	// bool schedDone = false;
	//files.push_back("fixed.txt");
			
	schedulerLog.push_back("Initializing Scheduler.");
}

bool myScheduler::handleCommand(int TID, string parsedCommand[], int TID_type, struct args myArgs) 
{
	// 	
	// // used to output TID to log
	// stringstream ss;
	// ss << TID;
	// string base = parsedCommand[0]; // base command
	// string one = parsedCommand[1]; // 1st substring -- filename/Emode
	// string two = parsedCommand[2]; // 2nd substring -- val / record
	// 
	// cout << "1:" << parsedCommand[1] << " 2:" << parsedCommand[2] << endl;
	// 
	// // Command is read/mult_read/write/delete 
	// if (base == "R" || base == "M" || base == "W" || base == "D") {
	// 	// TID has necessary lock
	// 	
	// 	// TODO -- find file corresponding to ID
	// 	
	// 	if (checkGetLock(TID, base.substr(0,1), myArgs.EMode, "fixed.txt", myArgs.ID ) ) 
	// 	{
	// 		
	// 		// Pass on to Data Manager
	// 		schedulerLog.push_back(ss.str() + " already has lock for command: " + base + " on " + one);
	// 		
	// 		return true;
	// 	
	// 	}// end if TID has lock
	// 
	// 	// TID doesn't have lock
	// 	else {
	// 		bool lockStatus = reqLock(base,TID,one); 
	// 		
	// 		if (!lockStatus){ // blocking
	// 			
	// 			schedulerLog.push_back(ss.str() + " blocked on lock for command: " + base + " on " + one);
	// 			return false;
	// 			
	// 		}else { // add locks to table
	// 			
	// 			add.push_back(myArgs.ID);
	// 			locks[TID].push_back(struct locks{base, myArgs.ID, TID})
	// 			schedulerLog.push_back(ss.str() + " obtains lock for command: " + base + " on " + one);
	// 			
	// 			// Pass on to Data Manager
	// 			// need DM call
	// 			
	// 			return true;
	// 		} // else
	// 		
	// 	} // end else TID tries to get lock	
	// } // End command is read/write/delete
	// 
	// else if (base == "C" || base == "A") 
	// {
	// 	// Pass on to Data Manager
	// 	releaseLocks(TID);
	// 	return true;
	// } // End is commit/abort
	// 
	// // Error - unknown command
	// else {
	// 	//@TODO	cout << "Unknown command: " + parsedCommand[0] + endl;
		return true;
	// }
}

// Release the locks that TID has
void myScheduler::releaseLocks(int TID) 
{
	//locks[TID].clear();
}

//@TODO -- RIGHT NOW ONLY RETURNS TRUE
bool myScheduler::checkGetLock(int TID, char base, bool process, string filename, int recordID) {
	
	// Check if filename lock is in file_locks
//	tr1::unordered_map<string, struct lock_tuple>::const_iterator got_file = file_locks.find(filename);
	
	// if(got_file == file_locks.end()) {// not added yet
	// 	return false;
	// }
	// 
	return true;
}
	
// Check if TID has lock of type on dataItem; Return false no / true yes
bool myScheduler::checkLock(string type, int TID, string dataItem) 
{}

// Attempt to acquire lock of type on dataItem; Return false failure / true success
bool myScheduler::reqLock(string type, int TID, string dataItem)
{
		// lock found in all, now check intention
	
	// if(file:locks.empty){  // first one
	// 	
	// 	
	// 	struct lock_tuple temp{TID,type, ; // true = read | false = write
	// 	bool intention; // true = intention | false = actual
	//  	int mode; // true = process | false = transaction};
	// 	
	// 	
	// 	std::pair<string, struct lock_tuple> temp (filename );
	// 	
	// 	
	// 	
	// 	
	// 	
	// 	file_locks.insert();
	// 
	// 
	// 
	// } 
	
	// for(int i = 0; i < (int)locks.size(); i++){
	// 
	// 	vector<struct lock>::iterator it = std::find_if(locks[i].begin(), locks[i].end(), 
	//          	find_id(recordID));
	// 		
	// 	if(it != locks[i].end()){	
	// 	
	// 		if(*it.type != 'M' || *it.type != 'D'){
	// 			return true;
	// 		}else{
	// 			return false;
	// 		} // end else
	// 		
	// 	} // end if
	// 	
	// 
	// } // end for
		
	return false;
} // end req

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