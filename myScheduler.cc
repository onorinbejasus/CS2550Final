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
		
		// used to output TID to log
		stringstream ss;
		ss << TID;
		string base = parsedCommand[0]; // base command
		string one = parsedCommand[1]; // 1st substring -- filename/Emode
		string two = parsedCommand[2]; // 2nd substring -- val / record
				
		// Command is read/mult_read/write/delete 
		if (base == "R" || base == "M" || base == "W" || base == "D") {
			
			// TODO -- find file corresponding to ID
			
			if(base == "D" || base == "M"){ // need to check for file locks
				
				tr1::unordered_map<string, struct file_lock>::const_iterator got_file = file_locks.find("fixed.txt");
				
				if(got_file != file_locks.end() && got_file->second.blocked == false)
				{
					struct lock_tuple lock = { TID, base, true, myArgs.EMode };
					
					got_file->second.currList->push_back(lock);
				
				//	got_file->second.blocked = true;
					
				}else if (got_file != file_locks.end() && got_file->second.blocked){
					
						struct lock_tuple lock = { TID, base, true, myArgs.EMode};
						got_file->second.waitList->push_back(lock);				
							
				} // end else
				
			} // end if file lock
			
			if (checkGetLock(TID, base.substr(0,1), myArgs.EMode, "fixed.txt", myArgs.ID ) ) 
			{
				
				// Pass on to Data Manager
				schedulerLog.push_back(ss.str() + " already has lock for command: " + base + " on " + one);
				
				// add DM here
				
				return true;
			
			}// end if TID has lock
		
			// TID doesn't have lock
			else {
				bool lockStatus = reqLock(base,TID, myArgs.EMode, one, "fixed.txt"); 
				
				if (!lockStatus){ // blocking
					
					schedulerLog.push_back(ss.str() + " blocked on lock for command: " + base + " on " + one);
					
					tr1::unordered_map<string, struct file_lock>::const_iterator got_file = file_locks.find("fixed.txt");
					tr1::unordered_map<int, struct record_lock>::const_iterator got_rec = got_file->second.record_locks->find(TID);
					
					/* add to the wait list */
					struct lock_tuple wait = { TID, base, true, myArgs.EMode};
					got_rec->second.waitList->push_back(wait);
										
					return false;
					
				}else { // add locks to table
					
					tr1::unordered_map<string, struct file_lock>::const_iterator got_file = file_locks.find("fixed.txt");
					tr1::unordered_map<int, struct record_lock>::const_iterator got_rec = got_file->second.record_locks->find(TID);
					
					if(got_rec == got_file->second.record_locks->end()){ // no record for it yet
						
						struct lock_tuple curr = { TID, base, false, myArgs.EMode};
						
						vector<struct lock_tuple> temp1;
						vector<struct lock_tuple> temp2;
						
						struct record_lock rec = {&temp1, &temp2};
						rec.currList->push_back(curr);
						
						pair <int,struct record_lock> myPair;
						myPair = make_pair (TID,rec);
						
						got_file->second.record_locks->insert(myPair);
						
					}
							
					schedulerLog.push_back(ss.str() + " obtains lock for command: " + base + " on " + one);
					
					// Pass on to Data Manager
					// need DM call HERE
					
					return true;
					
				} // else
				
			} // end else TID tries to get lock	
		} // End command is read/write/delete
		
		else if (base == "C" || base == "A") 
		{
			// Pass on to Data Manager
			releaseLocks(TID, "fixed.txt");
			return true;
		} // End is commit/abort
		
		// Error - unknown command
		else {
			//@TODO	cout << "Unknown command: " + parsedCommand[0] + endl;
		return true;
	 } // end else
}

// Release the locks that TID has
void myScheduler::releaseLocks(int TID, string filename) 
{
	tr1::unordered_map<string, struct file_lock>::const_iterator got_file = file_locks.find(filename);
	tr1::unordered_map<int, struct record_lock>::const_iterator got_rec = got_file->second.record_locks->find(TID);
	
	if(got_rec != got_file->second.record_locks->end())
		got_file->second.record_locks->erase(TID);
}

void myScheduler::releaseLock(int TID, string filename) {
	
	// tr1::unordered_map<string, struct file_lock>::const_iterator got_file = file_locks.find(filename);
	// tr1::unordered_map<int, struct record_lock>::const_iterator got_rec = got_file->second.record_locks->find(TID);
	// 
	// if(got_rec == got_file->second.record_locks->end())
	// 	return;
	// 	
	// got_rec->second.currList->erase(TID);
	// 
	// struct lock_tuple n = got_rec.waitList->at(0);
	// got_rec->second.waitList->erase(TID);
	// 
	// n.blocked = false;
	// 
	// got_rec->second.currList->push(n);
 
}

//@TODO -- RIGHT NOW ONLY RETURNS TRUE
bool myScheduler::checkGetLock(int TID, string base, bool process, string filename, int recordID) {
	
	// Check if filename lock is in file_locks
	if(file_locks.empty()) 
		return false;
		
	tr1::unordered_map<string, struct file_lock>::const_iterator got_file = file_locks.find(filename);
	
	tr1::unordered_map<int, struct record_lock>::const_iterator got_rec = got_file->second.record_locks->find(TID);
	
	 if(got_rec == got_file->second.record_locks->end()) {// not added yet
	 	return false;
	 }
	 
	return true;
}
	
// Check if TID has lock of type on dataItem; Return false no / true yes
bool myScheduler::checkLock(string type, int TID,  string dataItem) 
	{return false;}

// Attempt to acquire lock of type on dataItem; Return false failure / true success
bool myScheduler::reqLock(string type, int TID, int mode, string dataItem, string filename)
{
	// lock found in all, now check intention
	
	tr1::unordered_map<string, struct file_lock>::const_iterator got_file = file_locks.find(filename);
	
	bool ret = false;
	
	if(file_locks.empty()){  // first one
		
		vector<struct lock_tuple> rec1;
		vector<struct lock_tuple> rec2;
		
		struct record_lock rec_temp = {&rec1, &rec2 };
		
		tr1::unordered_map<int, struct record_lock> *temp_map;
		pair <int,struct record_lock> myPair;
		myPair = make_pair (TID,rec_temp);	
		temp_map->insert(myPair); 
		
		vector<struct lock_tuple> temp1;
		vector<struct lock_tuple> temp2;
		
		struct file_lock temp_file = {temp_map, &temp1, &temp2 , false};
		pair <string,struct file_lock> myPair1;		
		myPair1 = make_pair (filename, temp_file);
		file_locks.insert(myPair1);
		
		ret = true;

	} // end if 
	else{ // we need to search 
		
		tr1::unordered_map<string, struct file_lock>::const_iterator got_file = file_locks.find(filename);
		if(got_file == file_locks.end())
			ret = true;
	
		tr1::unordered_map<int, struct record_lock>::const_iterator got_rec = got_file->second.record_locks->find(TID);
		if(got_rec != got_file->second.record_locks->end())
			ret = false;
		
	}
		
	return ret;
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
		releaseLocks(TID, "fixed.txt");
		// Update time between detections
		detectTime = detectTime/2;
	}
	else {
		detectTime = detectTime*4;
	}
}