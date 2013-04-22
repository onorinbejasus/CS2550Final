#include "myPTM.hh"
/*
#define _EXECUTION_ERROR_ -4
#define _INSERTION_ERROR_ -2
#define _LOCKFILE_ERROR_  -3
*/

/*

  Point of transactions:
  Grouping a set of operations together such that they are not interrupted.  
  Essentially locking the files until the transaction is done.
  
*/

myPTM::myPTM(vector< vector<string> > cT, int rM):
	currTrans(cT), readMode(rM)
{
		
	// create the logfiles for each transaction
//	transactionLog = vector<string>[currTrans.size()];
	
	// flag to determine when done with all transactions
		bool done = false;
	
	// commands to be executed	
	vector<string> commands;
	
	/* iterators for the scripts to keep track of the position */
	vector < vector<string>::iterator > it;
	
	// assign the iterators to their corresponding scripts
	for(int i = 0; i < currTrans.size(); i++){
		vector<string>::iterator myIt = (currTrans.at(i)).begin();
		it.push_back(myIt);	
	}
	
	if(readMode == 0){ // round robin
		
		int loop_size = currTrans.size();
		
		while(!done){
			
			for(int i = 0; i < loop_size; i++){
								
				/* serial from here */
				if(loop_size == 1){
					
					parseCommands( &( currTrans.at(0) )[0], ( currTrans.at(0) ).size() );
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
		// iterate until all commands have been parsed
		while(!done){		
						
			/* get the seends */
			/* random number = rand % max items + start */
			scriptSeed = rand() % currTrans.size();
			
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
				currTrans.erase(currTrans.begin() + scriptSeed);
				it.erase(it.begin() + scriptSeed);
			}
			
			// if all scripts are removed, we are done
			if(currTrans.empty())
				done = true;
				
		} // end while
				
	} // end else
	
}

void myPTM::parseCommands(string *script, int numCommands){
	
	printf("number of commands %i\n", numCommands);
		
	for(int i = 0; i < numCommands; i++){
		
		printf( "%s \n", script[i].c_str() );
	}
	
}

void myPTM::undoEffects(int TID){
	
	/* will the transactions not writing out until commit,
		all we really need to do is drop the transaction log */
	
		
		
	
}

void myPTM::handleCommand(string command, int TID ){
	
	
	
}