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

myPTM *ptm = new myPTM();
myScheduler *scheduler = new myScheduler();
myDM *dm = new myDM();

char** names;

// ====================
// = Main Entry Point =
// Takes input from command line
// Parses and launches the appr.
// Modules
// ====================

int main(int argc, char*argv[]){
	
	/* initialize random seed: */
	  srand (time(NULL));
	
	/* set the array of names */
	names = new char *[argc-1];
	
	/* copy names from command line */
	for(int i = 0; i < argc-1; i++){
		names[i] = new char[sizeof(argv[i+1])];
		strcpy(names[i], argv[i+1]);
	}
	
	
	/* open file and start parsing */
	for(int i = 0; i < argc-1; i++){
		
		std::string file; // variable to read file into		
		std::ifstream script(names[i]); // current files
		
//		char *p = strtok(script, " ");
		
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
			  	std::cout << line << std::endl; // output
			}
	
		} else{ // FAIL!
		
			std::cout << "Unable to open file"; 
		
		} // end else
	
	} // end for
	
	return 0;
} // end main