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

// module initialization (May need to be an array of instances for each script)
myPTM *ptm = NULL; 
myDM *dm = NULL;

// log of the actions in the main module
vector<string> mainLog;

std::string getTime(){
	
	// get and return time in milliseconds
	stringstream ss;
	ss << "SysTime (ms):" << " " << time(0) * 1000;
	
	return ss.str();
}

// ====================
// = Main Entry Point =
// Takes input from command line
// Parses and launches the appr.
// Modules
// ====================

vector<string> extract(std::string scr){
	
	mainLog.push_back(getTime() + ": Read complete script file into memory.");
	
	std::string file; // variable to read file into		
	std::ifstream script(scr.c_str()); // current files
	
	std::vector<string> commands;
	
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
		
		mainLog.push_back(getTime() + ": Break script up into lines: main memory again.");
		
		while ( !lines.eof() ){ // continue until EOF

			// get current line
			getline(lines,line);
			commands.push_back(line);
			
			mainLog.push_back(getTime() + ": Line: " + line);
			
		}

	} else{ // FAIL!

		mainLog.push_back(getTime() + ": Failed to open script: " + scr);
		std::cout << "Unable to open file \n"; 

	} // end else	

	return commands;
}

int main(int argc, char*argv[]){
	
	/*
	1. the maximum number of records/tuples that could be held in a data file (such as 2000);
	2. the number of buffer pages;
	3. the search method (scan or hash);
	4. the method of concurrent reading from program files (round robin or random).
	*/
	
	mainLog.push_back(getTime() + ": Read in command line arguments");
	stringstream ss;
	int maxRecords = atoi(argv[1]);
	ss << maxRecords;
	mainLog.push_back(getTime() + ": Records: " + ss.str());
	ss.clear();
	int numBufferPages = atoi(argv[2]);
	ss << numBufferPages;
	mainLog.push_back(getTime() + ": Number of Buffer Pages: " + ss.str());
	ss.clear();
	int searchMode = atoi(argv[3]);
	ss << searchMode;
	mainLog.push_back(getTime() + ": Search Mode: " + ss.str());
	ss.clear();
	int readMode = atoi(argv[4]);
	ss << readMode;
	mainLog.push_back(getTime() + ": Read Mode: " + ss.str());
	ss.clear();
	
	// initialize the data manager
	dm = new myDM(searchMode, maxRecords, numBufferPages, "fixed.txt" );
	mainLog.push_back(getTime() + ": Initialized Data Manager");
	
	// number of threads to launch
	const int NUM_THREADS = argc-5;
	
	ss << NUM_THREADS;
	mainLog.push_back(getTime() + ": Number of Scripts: " + ss.str());
	ss.clear();
	
	mainLog.push_back(getTime() + ": Seed random number generator");
	/* initialize random seed: */
	srand (time(NULL));
	
	std::string names[NUM_THREADS];
	
	/* launch threads to work with the scripts*/
	std::vector< std::vector<string> > transactions;
		
	mainLog.push_back(getTime() + ": Parse script files.");
	
	for(int i = 0; i < NUM_THREADS; i++){
		
		// get the name of the script
		names[i] = std::string(argv[i+5]);
		mainLog.push_back(getTime() + ": Script: " + names[i]);
		transactions.push_back( extract( names[i] ) );
	}
	
	mainLog.push_back(getTime() + ": Launch Transaction Manager");
	string mode = (readMode == 0) ? "Round Robin" : "Random";
	mainLog.push_back(getTime() + ": Mode: " + mode);
	
	// launch transaction manager
	ptm = new myPTM(transactions, readMode);
	
	/* writing transaction log out */
	
	ofstream log;
	log.open ("mainLog.txt");
	
	for(int i = 0; i < mainLog.size(); i++)
		log << mainLog[i] << "\n";
	 
	 log.close();
	
	return 0;
} // end main