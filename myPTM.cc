#include "myPTM.hh"
#include "myDM.hh"
#include <iostream>
#include <fstream>
#include "time.h"
#include <exception>
#include <signal.h>
#include <errno.h>

#define DEBUG

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
pthread_mutex_t myPTM_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mxq = PTHREAD_MUTEX_INITIALIZER;

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

int checkStatus(pthread_mutex_t *mtx, pthread_t id)
{
  switch(pthread_mutex_trylock(mtx)) {
    case 0: /* if we got the lock, unlock and return 1 (true) */
      pthread_mutex_unlock(mtx);
		#ifdef DEBUG
			pthread_mutex_lock( &print_mutex );
			cout << "if we got the lock, unlock and return 1" << endl; 
			pthread_mutex_unlock( &print_mutex );
		#endif
      return 1;
    case EBUSY: /* return 0 (false) if the mutex was locked */
	  	#ifdef DEBUG
			pthread_mutex_lock( &print_mutex );
			cout << "cancelling thread" << endl; 
			pthread_mutex_unlock( &print_mutex );
		#endif
		pthread_cancel(id);
      return 0;
  }
  return 1;
}

void *thread_kill(void *tid){
	
	pthread_t T = *(reinterpret_cast<pthread_t*>(tid));
	
	int i = 0;  
		
	while( !checkStatus(&queue_mutex[i++%2], T) &&
				!checkStatus(&print_mutex, T) &&
					!checkStatus(&log_mutex, T) ) {}
	
	#ifdef DEBUG
		pthread_mutex_lock( &print_mutex );
		cout << "out of loop" << endl; 
		pthread_mutex_unlock( &print_mutex );
	#endif
	
	  return NULL;
}

void *handleCommand(void *args){
	
	bool done = false;
	
	stringstream tt;

	struct thread_args myArgs = *(reinterpret_cast<struct thread_args*>(args));
	
	int TID = myArgs.ID;
	myPTM *myClass = myArgs.ptr;
	int TID_type = myArgs.EMode;
	int TID_blocked = myArgs.blocked;
		
	while(!done){
		
		pthread_mutex_lock( &queue_mutex[TID] );
		bool empty = checkQueue(TID);
		pthread_mutex_unlock( &queue_mutex[TID] );
		
		if(empty == false){
			
			string command;
		
			tt << TID;
		
			stringstream em;
			em << myArgs.EMode;
		
			pthread_mutex_lock( &queue_mutex[TID] );
			command = popQueue(TID);			
			pthread_mutex_unlock( &queue_mutex[TID] );
		
			pthread_mutex_lock( &log_mutex );
			transactionLog.push_back(getTime() + " : Popping command off stack: " + tt.str() + command); 
			pthread_mutex_unlock( &log_mutex );
			
			#ifdef DEBUG
				pthread_mutex_lock( &print_mutex );
				cout << "Popping command off stack " << TID << " " << command << endl; 
				pthread_mutex_unlock( &print_mutex );
			#endif
		
			tt.clear();
		
			#ifdef DEBUG
			
				pthread_mutex_lock( &print_mutex );
				cout << "thread id: " << pthread_self() << "command: " << command << endl;
				pthread_mutex_unlock( &print_mutex );
							
			#endif
			
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
				if (parsed_command[0] == "R") {
					//myDM.read(parse_command[2]);
					pthread_mutex_lock( &myPTM_mutex );
					myClass->num_reads++;
					pthread_mutex_unlock( &myPTM_mutex );
				} 
				else if (parsed_command[0] == "W") {
					pthread_mutex_lock( &myPTM_mutex );
					myClass->num_writes++;
					pthread_mutex_unlock( &myPTM_mutex );
				}
				else if (parsed_command[0] == "M") {
					pthread_mutex_lock( &myPTM_mutex );
					myClass->num_reads++;
					pthread_mutex_unlock( &myPTM_mutex );
				}
				else if (parsed_command[0] == "D") {
					pthread_mutex_lock( &myPTM_mutex );
					myClass->num_writes++;
					pthread_mutex_unlock( &myPTM_mutex );
				}
			}
			
			#ifdef DEBUG
				pthread_mutex_lock( &print_mutex );
				cout << command << endl;
				pthread_mutex_unlock( &print_mutex );
			#endif
			
			if(command.compare("done") == 0){
			
				#ifdef DEBUG
					pthread_mutex_lock( &print_mutex );
					cout << "parsed done " << endl;
					pthread_mutex_unlock( &print_mutex );
				#endif
				
				
				pthread_mutex_lock( &log_mutex );
				transactionLog.push_back(getTime() + " : Transaction Done, exiting thread: " + tt.str()); 
				pthread_mutex_unlock( &log_mutex );
				
				#ifdef DEBUG
					pthread_mutex_lock( &print_mutex );
					cout << "after transaction log" << endl;
					pthread_mutex_unlock( &print_mutex );
				#endif
				
				done = true;
				
			}
			
		}else{
			// continue to wait for input
			continue;
		}
		
		#ifdef DEBUG
			pthread_mutex_lock( &print_mutex );
			cout << "while loop" << endl;
			pthread_mutex_unlock( &print_mutex );
		#endif
				
	} // end while
	
	int ret = 0;
	pthread_exit(&ret);
	
	#ifdef DEBUG
		pthread_mutex_lock( &print_mutex );
		cout << "before return" << endl;
		pthread_mutex_unlock( &print_mutex );
	#endif
	
	return 0;
	
} // end handleCommand

myPTM::myPTM(vector< vector<string> > cT, int rM):
	currTrans(cT), readMode(rM)
{
	num_writes = 0;
	num_reads = 0;		
	transactionLog.push_back(getTime() + " : Initializing Transaction Manager.");
	transactionLog.push_back(getTime() + " : Initializing command queues, threads, iterators and mutexes.");
	
	// initialize the threads
	threads = new pthread_t[currTrans.size()];
	command_queue = new queue<string>[currTrans.size()];
	
	/* iterators for the scripts to keep track of the position */
	vector < vector<string>::iterator > it;
	
	// initialize queue mutexes
	queue_mutex = new pthread_mutex_t[currTrans.size()];
		
	// struct holding thread parameters
	struct thread_args *myArgs = new struct thread_args[currTrans.size()];
	
	// vector of ids
	vector<int> myIDS;
	
	// init the scheduler
	scheduler = new myScheduler(2000, currTrans.size());
	transactionLog.push_back(getTime() + " : Initializing command iterators");
	
	const int NUM_THREADS = (int)currTrans.size();
	
	#ifdef DEBUG
		cout << "number of scripts " << currTrans.size() << endl;
	#endif
	
	queue_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * NUM_THREADS);
	
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
					
					parseCommands( &commands[0], commands.size(), &ids[0], (int)ids.size() ,1);
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
				
				parseCommands(&commands[0], commands.size(), &ids[0], (int)ids.size(), 1);
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
				
				#ifdef DEBUG
					pthread_mutex_lock( &print_mutex );
					cout << "exit " << commands.size() << " " << ids.size() << endl;
					pthread_mutex_unlock( &print_mutex );
				#endif
				
				if(commands.size() > 0)
					parseCommands( &commands[0], commands.size(), new int(myIDS[0]), 1, 1);
				
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
			
			#ifdef DEBUG
				pthread_mutex_lock( &log_mutex );
				cout << "script seed " << scriptSeed << endl;
				pthread_mutex_unlock( &log_mutex );
			#endif
			
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
			
			parseCommands( &commands[0], (int)(commands.size()), new int(myIDS[scriptSeed]), 1, 1);
			commands.clear();
			ids.clear();
			
			cur_index = (int)( it[scriptSeed] - currTrans.at(scriptSeed).begin() );
			
			// if out of commands in current script, remove
			if( it[scriptSeed] >= currTrans.at(scriptSeed).end() ){
				
				stringstream ss;
				ss << scriptSeed;
				
				pthread_mutex_lock( &log_mutex );
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
				break;
			}
				
		} // end while
				
	} // end else
	
	#ifdef DEBUG
		pthread_mutex_lock( &print_mutex );
		cout << "number fo threads " << NUM_THREADS << endl;
		pthread_mutex_unlock( &print_mutex );
	#endif
	
		int *ptr;
	
	/* wait for threads to join */
	for(int i = 0; i < NUM_THREADS-1; i++){
	
		#ifdef DEBUG
			pthread_mutex_lock( &print_mutex );
			cout << "waiting on " << i << endl;
	 		pthread_mutex_unlock( &print_mutex );
		#endif
			
		pthread_join(threads[i], (void**)&(*ptr));	
		
		#ifdef DEBUG
			pthread_mutex_lock( &print_mutex );
			printf("\n return value from first thread is [%d]\n", *ptr);
			pthread_mutex_unlock( &print_mutex );
		#endif
	    
	}
	
	pthread_mutex_lock( &log_mutex );
	transactionLog.push_back(getTime() + " : All scripts complete");
	pthread_mutex_unlock( &log_mutex );

	pthread_mutex_lock( &print_mutex );
	cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;
	cout << num_reads;
	cout << "  = number of reads.";
	cout << endl;
	cout << num_writes;
	cout << "  = number of writes.";
	cout << endl;
	pthread_mutex_unlock( &print_mutex );
	
	//cout << ((float) num_reads / (num_reads + num_writes) << " = percent reads" << endl;
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

void myPTM::parseCommands(string *script, int numCommands, int* id, int numScripts, int type){
	
	pthread_mutex_lock( &log_mutex );
	transactionLog.push_back(getTime() + " : Parsing commands"); 
	pthread_mutex_unlock( &log_mutex );
	
	// keep track of current script
	// if random, only one script will be passed into here
	int cur_script = 0;
	
	if(type == 1) // random
		cur_script = *id;
	
	if(cur_script < 0)
		return;
	
	#ifdef DEBUG
		pthread_mutex_lock( &print_mutex );

		printf("num scripts: %i\n", numScripts);
		printf("cur scripts: %i\n", cur_script);
		
		pthread_mutex_unlock( &print_mutex );
	#endif
	
	for(int i = 0; i < (numCommands); i++){
		
		#ifdef DEBUG	
			pthread_mutex_lock( &print_mutex );
			cout << "before log " << endl;
			printf("cur: %d id: %d\n",i, cur_script);		
			cout << getTime() << " : Queuing command: " << script[i] << endl;
			pthread_mutex_unlock( &print_mutex );
		#endif
		
		pthread_mutex_lock( &log_mutex );
		transactionLog.push_back(getTime() + " : Queuing command: " + script[i]); 
		pthread_mutex_unlock( &log_mutex );
		
		if(type == 0){ // RR
			pthread_mutex_lock( &queue_mutex[cur_script] );
			command_queue[ id[cur_script] ].push( script[i] );
			pthread_mutex_unlock( &queue_mutex[cur_script] );	 
		}	
		else{ // random
			pthread_mutex_lock( &queue_mutex[cur_script] );
			command_queue[ cur_script ].push( script[i] );
			pthread_mutex_unlock( &queue_mutex[cur_script] );
		}
		
		#ifdef DEBUG
			pthread_mutex_lock( &print_mutex );
			cout << "after " << endl;
			pthread_mutex_unlock( &print_mutex );
		#endif
		
		// basically, if random
		if(type == 0) 
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
