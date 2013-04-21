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
	
	printf("inside myPTM constructor\n");
	
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
		vector<string>::iterator myIt = currTrans.at(i).begin();
		it.push_back(myIt);	
	}
	
	if(readMode == 0){ // round robin
		
		printf("readmode\n");
		printf("beginning size %i\n", (int)currTrans.size() );
		while(!done){
			
			for(int i = 0; i < currTrans.size(); it[i]++, i++){
				
				// if out of commands in current script, remove
				if( it[i] == currTrans.at(i).end() ){
				
					printf("end of list\n");
					currTrans.erase(currTrans.begin() + i);
					printf("after erase\n");
					printf( "size %i\n", (int)currTrans.size() );
					// if all scripts are removed, we are done
					if(currTrans.size() == 0){
					
						printf("done!\n");
						done = true;
						break;
					
					}else
						continue;
				} // end if
								
				/* get commands and pass them to the parser */
				commands.push_back(*it[i]);
				
			} // end for
			
			/* for the case where we run out of commands */
			if(commands.size() != 0){
			
		//		parseCommands(&commands[0], commands.size());
				commands.clear();
			
			} // end if
			
		} // end while
		
	}else{ // random
					
		int scriptSeed, commandSeed;
		// iterate until all commands have been parsed
		while(!done){
			
			/* get the seends */
			scriptSeed = rand() % currTrans.size();
			commandSeed = rand() % distance( currTrans.at(scriptSeed).begin(), it[scriptSeed]) + ( currTrans.at(scriptSeed).end() - it[scriptSeed] );
			
			printf("scriptSeed %i commandSeed %i\n", (int)scriptSeed, (int)commandSeed);
			
			/* get commands and pass them to the parser */
			commands = vector<string>( it[scriptSeed], it[scriptSeed] + commandSeed );
			printf("command size: %i\n", (int)commands.size());
			
	//		parseCommands(&commands[0], commands.size());
			commands.clear();
			
			// if out of commands in current script, remove
			if(it[scriptSeed] == currTrans[scriptSeed].end())
				currTrans.erase(currTrans.begin() + scriptSeed);
			
			// if all scripts are removed, we are done
			if(currTrans.size() == 0)
				done = true;
				
		} // end while
				
	} // end else
	
}

void myPTM::parseCommands(string *script, int numCommands){
	
	printf("numCommands %i\n", numCommands);
	
//	for(int i = 0; i < numCommands; i++){
		
	//	printf( "%s \n", script[i] );
		
//	}
	
}

void myPTM::undoEffects(int TID){
	
	/* will the transactions not writing out until commit,
		all we really need to do is drop the transaction log */
	
		
		
	
}

void myPTM::handleCommand(string command, int TID ){
	
	
	
}