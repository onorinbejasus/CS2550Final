#ifndef _MYDM_
#define _MYDM_

#include "myPAGE.hh"
#include <iostream>
#include <fstream>

class myDM{
	
	public:
		myDM(){}; // Default constructor
        myDM(int searchMode, int maxRecords, int numPages);
		
		struct tuple read(int id);
        int multRead(int area);
        void write(struct tuple tup);
		
	private:
		
        int SEARCH_MODE;
		int MAX_NUM_RECORDS;
		int NUM_PAGES;
        
		vector<string> fp;
		
		void deleteData(int id);
        bool exists(int id);
        bool rExists(int id);
	
};
#endif