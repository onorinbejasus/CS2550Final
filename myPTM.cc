#include "myPTM.hh"

struct thread_args
 {
    myPTM *ptr;
    int ID;
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handleCommand(void *args){
	
	bool done = true;
	
	 struct thread_args myArgs = *(reinterpret_cast<struct thread_args*>(args));
	
	int TID = myArgs.ID;
	myPTM *myClass = myArgs.ptr;
	
	pthread_mutex_lock( &mutex);
	cout << TID << endl;
	pthread_mutex_unlock( &mutex);
	
	while(!done){
		
		if( !myClass->checkQueue(TID) ){
			done = false;
		}else{
			// continue to wait for input
			done = true;
			continue;
		}
	}
	
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
	
	// flag to determine when done with all transactions
	bool done = false;
	
	// commands to be executed	
	vector<string> commands;
	
	/* iterators for the scripts to keep track of the position */
	vector < vector<string>::iterator > it;
	
	struct thread_args *myArgs = new struct thread_args[currTrans.size()];
	// assign the iterators to their corresponding scripts
	for(int i = 0; i < currTrans.size(); i++){
		
		vector<string>::iterator myIt = (currTrans.at(i)).begin();
		it.push_back(myIt);	
		
		// initialize variables to pass into thread		
		myArgs[i].ID = i;
		myArgs[i].ptr = this;
				
		// create a thread for each script
		pthread_create( &threads[i], NULL, handleCommand, reinterpret_cast<void*>(&myArgs[i]) );
	}
	// clean up
	
	if(readMode == 0){ // round robin
		
		int loop_size = currTrans.size();
		
		while(!done){
			
			for(int i = 0; i < loop_size; i++){
								
				/* serial from here */
				if(loop_size == 1){
					
					for(; it[0] != ( currTrans.at(0) ).end(); it[0]++ )
						commands.push_back(*it[0]);
					
					parseCommands( &commands[0], commands.size() );
					done = true;
					break;
				}
				
				// if out of commands in current script, remove
				if( it[i] == ( currTrans.at(i) ).end() ){
									
					// remove iterators when done
										
					iter_swap( it.begin() + (i), it.begin() + loop_size-1 );
					iter_swap( currTrans.begin() + (i), currTrans.begin() + loop_size-1 );
					
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
				++(it[i]);			
				
			} // end for
			
			/* for the case where we run out of commands */
			if(!commands.empty()){
				
				parseCommands(&commands[0], commands.size());
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
				
				parseCommands( &commands[0], commands.size() );
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
			
			// increment iterator
			it[scriptSeed] += commandSeed;
			
			parseCommands( &commands[0], (int)(commands.size()) );
			commands.clear();
			
			cur_index = (int)( it[scriptSeed] - currTrans.at(scriptSeed).begin() );
			
			// if out of commands in current script, remove
			if( it[scriptSeed] >= currTrans.at(scriptSeed).end() ){
				iter_swap( it.begin() + (scriptSeed), it.begin() + loop_size-1 );
				iter_swap( currTrans.begin() + (scriptSeed), currTrans.begin() + loop_size-1 );
				loop_size--;
			}
			
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

void myPTM::parseCommands(string *script, int numCommands ){
			
	for(int i = 0; i < numCommands; i++){
		
		//cout << script[i].c_str() << endl;
	}
	
}

void myPTM::undoEffects(int TID){
	
	/* with the transactions not writing out until commit,
		all we really need to do is drop the transaction log */
	
		
		
	
}
