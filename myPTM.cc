#include "myPTM.hh"
#include "myDM.hh"
#include <iostream>
#include <fstream>
#include "time.h"

int timeOut = 5; // wait 5 loops then kill myself
struct thread_args
 {
	myPTM *ptr;
    int ID;
		int EMode; // EMode=1 Transaction | EMode=0 Process
		int blocked;
};

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t *queue_mutex;

pthread_mutex_t sched_log_mutex = PTHREAD_MUTEX_INITIALIZER;

extern std::string getTime();

/* command queues */
queue<string> *command_queue;
vector<string> transactionLog;

// return true if empty, false otherwise
bool checkQueue(int TID) { 
	return command_queue[TID].empty(); 
} // end check

// get the next item in the queue
string popQueue(int TID){
	string command = command_queue[TID].front();
	command_queue[TID].pop();
	return command;
} // end pop

void *handleCommand(void *args){
	
	bool done = false;
	
	struct thread_args myArgs = *(reinterpret_cast<struct thread_args*>(args));
	
	int TID = myArgs.ID;
	myPTM *myClass = myArgs.ptr;
	int TID_type = myArgs.EMode;
	int blocked = myArgs.blocked;
		
	while(!done){
		
		pthread_mutex_lock( &queue_mutex[TID] );
		bool empty = checkQueue(TID);
		pthread_mutex_unlock( &queue_mutex[TID] );
		
		if (blocked != 0) {
			if (blocked > timeOut) {
				//Abort itself
				//myClass->undoEffects(int TID);
			}
			blocked++;
		}
		else if(empty == false){
			
			stringstream ss;
			ss << TID;
			
			pthread_mutex_lock( &queue_mutex[TID] );
			string command =popQueue(TID);			
			pthread_mutex_unlock( &queue_mutex[TID] );
			
			pthread_mutex_lock( &log_mutex );
			transactionLog.push_back(getTime() + " : Popping command off stack[TID]: " + ss.str() + command); 
			pthread_mutex_unlock( &log_mutex );
			
			// pthread_mutex_lock( &print_mutex );
			// 		cout << "thread id: " << pthread_self() << "command: " << command << endl;
			// 		pthread_mutex_unlock( &print_mutex );
			
    	istringstream iss(command);
			string parsed_command[3];
			for (int i = 0; iss; i++) {
				string sub;
				iss >> sub;
				parsed_command[i] = sub;
			}
			
			pthread_mutex_lock( &log_mutex );
			transactionLog.push_back(getTime() + " : Passing command to scheduler"); 
			pthread_mutex_unlock( &log_mutex );
			
			bool result = false;
			if (parsed_command[0] == "B") {
				myArgs.EMode = atoi(parsed_command[1].c_str());
				TID_type = myArgs.EMode;
			}
			else {
				pthread_mutex_lock( &sched_log_mutex );
				result = myClass->scheduler->handleCommand(TID, parsed_command, TID_type);
				pthread_mutex_unlock( &sched_log_mutex );
			}

			if (result) {
				blocked = 0;
				if (parsed_command[0] == "R") {
					//myDM.read(parse_command[2]);
				} 
				if (parsed_command[0] == "W") {
					
				}
			}
			else {
				blocked++;
			}
			
			if(command.compare("done") == 0){
				pthread_mutex_lock( &log_mutex );
				transactionLog.push_back(getTime() + " : Transaction Done, exiting thread: " + ss.str()); 
				pthread_mutex_unlock( &log_mutex );
				done = true;
			}
			
		}else{
			// continue to wait for input
			continue;
		}
	} // end while
	
	pthread_exit(NULL);
	
	return 0;	
} // end handleCommand

myPTM::myPTM(vector< vector<string> > cT, int rM):
	currTrans(cT), readMode(rM)
{
			
	transactionLog.push_back(getTime() + " : Initializing Transaction Manager.");
	transactionLog.push_back(getTime() + " : Initializing command queues, threads, iterators and mutexes.");
	
	// initialize the threads
	threads = new pthread_t[currTrans.size()];
	command_queue = new queue<string>[currTrans.size()];
	
	/* iterators for the scripts to keep track of the position */
	vector < vector<string>::iterator > it;
	
	// initialize queue mutexes
	queue_mutex = new pthread_mutex_t[currTrans.size()];
		
	// struct holding thread parameterss
	struct thread_args *myArgs = new struct thread_args[currTrans.size()];
	
	// vector of ids
	vector<int> myIDS;
	
	// init the scheduler
	scheduler = new myScheduler(2000, currTrans.size());
	transactionLog.push_back(getTime() + " : Initializing command iterators");
	
	// assign the iterators to their corresponding scripts
	for(int i = 0; i < currTrans.size(); i++){
		
		vector<string>::iterator myIt = (currTrans.at(i)).begin();
		it.push_back(myIt);	
				
		// initialize variables to pass into thread		
		myArgs[i].ID = i;
		myArgs[i].ptr = this;
		myArgs[i].EMode = 0;
		myArgs[i].blocked = 0;
		
		// init mutex
		pthread_mutex_init(&queue_mutex[i], NULL);
		
		// add TID		
		myIDS.push_back(i);
		
		ss << i;
		transactionLog.push_back(getTime() + " : Launching thread TID:" + ss.str());
		ss.clear();		
		// create a thread for each script
		pthread_create( &threads[i], NULL, handleCommand, reinterpret_cast<void*>(&myArgs[i]) );
	}
	
	// flag to determine when done with all transactions
	bool done = false;
	
	// commands to be executed	
	vector<string> commands;
	
	// id's of the transactions
	vector<int> ids;
	
	clock_t startTime = clock();
	if(readMode == 0){ // round robin
		
		pthread_mutex_lock( &log_mutex );
		transactionLog.push_back(getTime() + " : Beginning Round Robin Scheduling");
		pthread_mutex_unlock( &log_mutex );
		
		int loop_size = currTrans.size();
		
		while(!done){
			
			for(int i = 0; i < loop_size; i++){
								
				/* serial from here */
				if(loop_size == 1){
					
					pthread_mutex_lock( &log_mutex );
					transactionLog.push_back(getTime() + " : One command set remaining, queuing");
					pthread_mutex_unlock( &log_mutex );
					
					for(; it[0] != ( currTrans.at(0) ).end(); it[0]++ )
						commands.push_back(*it[0]);
					
					// specify end of input
					commands.push_back("done");
					ids.push_back(myIDS[0]);
					
					parseCommands( &commands[0], commands.size(), &ids[0], (int)ids.size() );
					done = true;
					break;
				}
				
				// if out of commands in current script, remove
				if( it[i] == ( currTrans.at(i) ).end() ){
					
					pthread_mutex_lock( &log_mutex );
					stringstream ss;
					ss << i;
					transactionLog.push_back(getTime() + " : Command set: " + ss.str() + "has finished queuing"); 
					pthread_mutex_unlock( &log_mutex );
						
					// specify end of input
					commands.push_back("done");
					ids.push_back(myIDS[i]);
					
					pthread_mutex_lock( &log_mutex );
					transactionLog.push_back(getTime() + " : Concatinating remaining command sets"); 
					pthread_mutex_unlock( &log_mutex );
					
					// remove iterators when dones					
					iter_swap( it.begin() + (i), it.begin() + loop_size-1 );
					iter_swap( currTrans.begin() + (i), currTrans.begin() + loop_size-1 );
					iter_swap( ids.begin() + (i), ids.begin() + loop_size-1 );
					
					// revert to previous, then progress
					loop_size--;
					
					i -= 1;
										
					// if all scripts are removed, we are done
					if(loop_size == 0){
						
						pthread_mutex_lock( &log_mutex );
						transactionLog.push_back(getTime() + " : No command sets to queue.");
						transactionLog.push_back(getTime() + " : Begin waiting on threads to complete"); 
						pthread_mutex_unlock( &log_mutex );
						
						done = true;
						break;	
					}else // if not keep going
						continue;
						
				} // end if
				
				pthread_mutex_lock( &log_mutex );
				transactionLog.push_back(getTime() + " : Passing commands to be queued: " + *it[i] ); 
				pthread_mutex_unlock( &log_mutex );
				
				/* get commands and pass them to the parser */		
				commands.push_back(*it[i]);
				ids.push_back(myIDS[i]);
				
				++(it[i]);			
				
			} // end for
			
			/* for the case where we run out of commands */
			if(!commands.empty()){
				
				parseCommands(&commands[0], commands.size(), &ids[0], (int)ids.size());
				ids.clear();
				commands.clear();
			
			} // end if
			
		} // end while
		
	}else{ // random
		
		pthread_mutex_lock( &log_mutex );
		transactionLog.push_back(getTime() + " : Beginning Random Scheduling");
		pthread_mutex_unlock( &log_mutex );
		
		int scriptSeed, commandSeed, cur_index, size;
		int loop_size = currTrans.size();
		
		// iterate until all commands have been parsed
		while(!done){		
			
			/* serial from here */
			if(loop_size == 1){
				
				pthread_mutex_lock( &log_mutex );
				transactionLog.push_back(getTime() + " : One command set remaining, queuing");
				pthread_mutex_unlock( &log_mutex );
				
				for(; it[0] != ( currTrans.at(0) ).end(); it[0]++ )
					commands.push_back(*it[0]);
				
				commands.push_back("done");
				ids.push_back(myIDS[0]);
				
				parseCommands( &commands[0], commands.size(), &ids[0], (int)ids.size() );
				done = true;
				break;
		
			} // end if
						
			/* get the seends */
			/* random number = rand % max items + start */
			
			scriptSeed = rand() % loop_size;
			
			ss << scriptSeed;
			
			pthread_mutex_lock( &log_mutex );
			transactionLog.push_back(getTime() + " : Selecting the random transaction id: " + ss.str());
			pthread_mutex_unlock( &log_mutex );
			ss.clear();
			
			/* total number of commands */
			size = (int)( currTrans.at(scriptSeed).size() );
			
			/* current index of the iterator */
			cur_index = (int)( it[scriptSeed] - currTrans.at(scriptSeed).begin() );
			
			commandSeed = rand() % (size - cur_index) + cur_index;
										 
			/* watch for overflow */
			if( (size - cur_index) < commandSeed)
				commandSeed = ( size - cur_index);
			
			ss << commandSeed;
			
			pthread_mutex_lock( &log_mutex );
			transactionLog.push_back(getTime() + " : Selecting the number commands to queue: " + ss.str());
			pthread_mutex_unlock( &log_mutex );
			
			ss.clear();
								
			/* get commands and pass them to the parser */			
			commands = vector<string>( it[scriptSeed], it[scriptSeed] + commandSeed );
			ids.push_back(myIDS[scriptSeed]);
			
			pthread_mutex_lock( &log_mutex );
			transactionLog.push_back(getTime() + " : Passing commands to be queued: ");
			for(int ii = 0; ii < commands.size(); ii++)
				transactionLog.push_back(commands[ii]);
			pthread_mutex_unlock( &log_mutex );
			
			// increment iterator
			it[scriptSeed] += commandSeed;
						
			parseCommands( &commands[0], (int)(commands.size()), &ids[0], (int)ids.size() );
			commands.clear();
			ids.clear();
			
			cur_index = (int)( it[scriptSeed] - currTrans.at(scriptSeed).begin() );
			
			// if out of commands in current script, remove
			if( it[scriptSeed] >= currTrans.at(scriptSeed).end() ){
				
				pthread_mutex_lock( &log_mutex );
				stringstream ss;
				ss << scriptSeed;
				transactionLog.push_back(getTime() + " : Command set: " + ss.str() + "has finished queuing"); 
				pthread_mutex_unlock( &log_mutex );
				
				// specify end of input
				commands.push_back("done");
				ids.push_back(myIDS[scriptSeed]);
				
				pthread_mutex_lock( &log_mutex );
				transactionLog.push_back(getTime() + " : Concatinating remaining command sets"); 
				pthread_mutex_unlock( &log_mutex );
				
				iter_swap( it.begin() + (scriptSeed), it.begin() + loop_size-1 );
				iter_swap( currTrans.begin() + (scriptSeed), currTrans.begin() + loop_size-1 );
				iter_swap( ids.begin() + (scriptSeed), ids.begin() + loop_size-1 );
				
				loop_size--;
			} // end if
			
			// if all scripts are removed, we are done
			if(loop_size == 0){
				
				pthread_mutex_lock( &log_mutex );
				transactionLog.push_back(getTime() + " : No command sets to queue.");
				transactionLog.push_back(getTime() + " : Begin waiting on threads to complete");
				pthread_mutex_unlock( &log_mutex );
				
				done = true;
			}
				
		} // end while
				
	} // end else
	
	/* wait for threads to join */
	for(int i = 0; i < currTrans.size(); i++){
		pthread_join(threads[i], NULL);
	}
	
	pthread_mutex_lock( &log_mutex );
	transactionLog.push_back(getTime() + " : All scripts complete");
	pthread_mutex_unlock( &log_mutex );

	cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;
	/* writing transaction log out */
	
	ofstream log;
	log.open ("TMLog.txt");
	
	for(int i = 0; i < transactionLog.size(); i++)
		log << transactionLog[i] << "\n";
	 
	 log.close();
	
	// Writing Scheduler log out 
	log.open("SchedLog.txt");
	
	for(int i = 0; i < scheduler->schedulerLog.size(); i++)
		log << scheduler->schedulerLog[i] << "\n";
	 
	log.close();
}

void myPTM::parseCommands(string *script, int numCommands, int* id, int numScripts){
	
	pthread_mutex_lock( &log_mutex );
	transactionLog.push_back(getTime() + " : Parsing commands"); 
	pthread_mutex_unlock( &log_mutex );
	
	// keep track of current script
	// if random, only one script will be passed into here
	int cur_script = 0;
						
	for(int i = 0; i < numCommands; i++){
		
		pthread_mutex_lock( &log_mutex );
		transactionLog.push_back(getTime() + " : Queuing command: " + script[i]); 
		pthread_mutex_unlock( &log_mutex );
		
		command_queue[ id[cur_script] ].push( script[i] );
		
		// basically, if random
		if(numScripts > 1) 
			cur_script++;
			
	} // end for
}

void myPTM::undoEffects(int TID){
	
	ss << TID;
	
	pthread_mutex_lock( &log_mutex );
	transactionLog.push_back(getTime() + " : Undoing effects of TID: " + ss.str()); 
	pthread_mutex_unlock( &log_mutex );
	
	ss.clear();
	
	/* with the transactions not writing out until commit,
		all we really need to do is drop the transaction log */	
}
