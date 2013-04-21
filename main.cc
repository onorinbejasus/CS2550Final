// ======================================
// = Timothy Luciani and Rebecca Hachey =
// 				 main.cc
// ======================================
#include <cstdio>
#include <cstdlib>
#include <time.h>

// String/File IO
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// Module Class Definitions
#include "myPTM.hh"
#include "myScheduler.hh"
#include "myDM.hh"

// threading libraries
extern "C"
 {
    #include <pthread.h>
    #include <unistd.h>
 }

// module initialization (May need to be an array of instances for each script)
myPTM *ptm = NULL; 
myScheduler *scheduler = NULL;
myDM *dm = NULL;

// ====================
// = Main Entry Point =
// Takes input from command line
// Parses and launches the appr.
// Modules
// ====================

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

void *execute(void *arg){
	
	std::string scr = *(static_cast<std::string*>(arg));
		
	std::string file; // variable to read file into		
	std::ifstream script(scr.c_str()); // current files

	if (script.is_open()){ //  if able to open file

		/* read entire file */

		script.seekg(0, std::ios::end);   
		file.reserve(script.tellg());
		script.seekg(0, std::ios::beg);

		file.assign((std::istreambuf_iterator<char>(script)),
		            std::istreambuf_iterator<char>());

		script.close(); // close file

		std::istringstream lines(file.c_str()); // iterate over the file lines
		std::string line;
		
		while ( !lines.eof() ){ // continue until EOF

			// get current line
			getline(lines,line);
			
			
			// lock write here:  YOU SHALL NOT PASS!!
			pthread_mutex_lock( &print_mutex );
			
		  	std::cout << pthread_self() << ":  " << line << std::endl; // output
			
			// Unlock write here: Ok, now you can go :)
			pthread_mutex_unlock( &print_mutex );
		}

	} else{ // FAIL!

		std::cout << "Unable to open file"; 

	} // end else	
	
	/* kill thread and return value */
	pthread_exit(NULL);
	return 0;
}

int main(int argc, char*argv[]){
	
	/*
	1. the maximum number of records/tuples that could be held in a data file (such as 2000);
	2. the number of buffer pages;
	3. the search method (scan or hash);
	4. the method of concurrent reading from program files (round robin or random).
	*/
	
	const int records = atoi(argv[1]);
	const int numBufferPages = atoi(argv[2]);
	const int searchMode = atoi(argv[3]);
	const int readMode = atoi(argv[4]);

	// number of threads to launch
	const int NUM_THREADS = argc-5;
	
	/* initialize random seed: */
	srand (time(NULL));
	
	/* array of threads */
	pthread_t threads[NUM_THREADS]; 
	
	std::string names[NUM_THREADS];
	/* launch threads to work with the scripts*/
	for(int i = 5; i < NUM_THREADS; i++){
		
		// get the name of the script
		names[i] = std::string(argv[i+1]);
		
		/* launch thread */
		pthread_create( &threads[i], NULL, execute, static_cast<void*>( &names[i] ) );
	}
	
	/* wait for all threads to complete */
	for(int i = 0; i < NUM_THREADS; i++){
		pthread_join( threads[i], NULL);
	}
	
	return 0;
} // end main