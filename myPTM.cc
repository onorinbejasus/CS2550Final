#include "myPTM.hh"
struct thread_args
 {
	myPTM *ptr;
    int ID;
};

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t *queue_mutex;

/* command queues */
queue<string> *command_queue;

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
		
	while(!done){
		
		pthread_mutex_lock( &queue_mutex[TID] );
		bool empty = checkQueue(TID);
		pthread_mutex_unlock( &queue_mutex[TID] );
		
		if( empty == false ){
			
			pthread_mutex_lock( &queue_mutex[TID] );
			string command =popQueue(TID);			
			pthread_mutex_unlock( &queue_mutex[TID] );
			
			pthread_mutex_lock( &print_mutex );
			cout << "thread id: " << pthread_self() << "command: " << command << endl;
			pthread_mutex_unlock( &print_mutex );
			
			if(command.compare("done") == 0)
				done = true;
			
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
		
	// create the logfiles for each transaction
//	transactionLog = vector<string>[currTrans.size()];
	
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
	//*scheduler = new myScheduler();
	
	// assign the iterators to their corresponding scripts
	for(int i = 0; i < currTrans.size(); i++){
		
		vector<string>::iterator myIt = (currTrans.at(i)).begin();
		it.push_back(myIt);	
				
		// initialize variables to pass into thread		
		myArgs[i].ID = i;
		myArgs[i].ptr = this;
		
		// init mutex
		pthread_mutex_init(&queue_mutex[i], NULL);
		
		// add TID		
		myIDS.push_back(i);
				
		// create a thread for each script
		pthread_create( &threads[i], NULL, handleCommand, reinterpret_cast<void*>(&myArgs[i]) );
	}
	
	// flag to determine when done with all transactions
	bool done = false;
	
	// commands to be executed	
	vector<string> commands;
	
	// id's of the transactions
	vector<int> ids;
	
	if(readMode == 0){ // round robin
		
		int loop_size = currTrans.size();
		
		while(!done){
			
			for(int i = 0; i < loop_size; i++){
								
				/* serial from here */
				if(loop_size == 1){
					
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
									
					// specify end of input
					commands.push_back("done");
					ids.push_back(myIDS[i]);
					
					// remove iterators when dones					
					iter_swap( it.begin() + (i), it.begin() + loop_size-1 );
					iter_swap( currTrans.begin() + (i), currTrans.begin() + loop_size-1 );
					iter_swap( ids.begin() + (i), ids.begin() + loop_size-1 );
					
					// revert to previous, then progress
					loop_size--;
					
					i -= 1;
										
					// if all scripts are removed, we are done
					if(loop_size == 0){
						done = true;
						break;	
					}else // if not keep going
						continue;
						
				} // end if
				
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
					
		int scriptSeed, commandSeed, cur_index, size;
		int loop_size = currTrans.size();
		
		// iterate until all commands have been parsed
		while(!done){		
			
			/* serial from here */
			if(loop_size == 1){
			
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
			
			/* total number of commands */
			size = (int)( currTrans.at(scriptSeed).size() );
			
			/* current index of the iterator */
			cur_index = (int)( it[scriptSeed] - currTrans.at(scriptSeed).begin() );
			
			commandSeed = rand() % (size - cur_index) + cur_index;
						 
			/* watch for overflow */
			if( (size - cur_index) < commandSeed)
				commandSeed = ( size - cur_index);
								
			/* get commands and pass them to the parser */			
			commands = vector<string>( it[scriptSeed], it[scriptSeed] + commandSeed );
			ids.push_back(myIDS[scriptSeed]);
			
			// increment iterator
			it[scriptSeed] += commandSeed;
						
			parseCommands( &commands[0], (int)(commands.size()), &ids[0], (int)ids.size() );
			commands.clear();
			ids.clear();
			
			cur_index = (int)( it[scriptSeed] - currTrans.at(scriptSeed).begin() );
			
			// if out of commands in current script, remove
			if( it[scriptSeed] >= currTrans.at(scriptSeed).end() ){
				
				// specify end of input
				commands.push_back("done");
				ids.push_back(myIDS[scriptSeed]);
				
				iter_swap( it.begin() + (scriptSeed), it.begin() + loop_size-1 );
				iter_swap( currTrans.begin() + (scriptSeed), currTrans.begin() + loop_size-1 );
				iter_swap( ids.begin() + (scriptSeed), ids.begin() + loop_size-1 );
				
				loop_size--;
			} // end if
			
			// if all scripts are removed, we are done
			if(loop_size == 0)
				done = true;
				
		} // end while
				
	} // end else
	
	/* wait for threads to join */
	for(int i = 0; i < currTrans.size(); i++){
		pthread_join(threads[i], NULL);
	}
	
}

void myPTM::parseCommands(string *script, int numCommands, int* id, int numScripts){
	
	// keep track of current script
	// if random, only one script will be passed into here
	int cur_script = 0;
						
	for(int i = 0; i < numCommands; i++){
		
		command_queue[ id[cur_script] ].push( script[i] );
		
		// basically, if random
		if(numScripts > 1) 
			cur_script++;
			
	} // end for
}

void myPTM::undoEffects(int TID){
	
	/* with the transactions not writing out until commit,
		all we really need to do is drop the transaction log */
	
		
		
	
}
