#ifndef _MYDM_
#define _MYDM_

#include "myPAGE.hh"

class myDM{
	
	public:
		myDM(){}; // Default constructor
        myDM(int searchMode, int maxRecords, int numPages);

	private:
		
        int SEARCH_MODE;
		int MAX_NUM_RECORDS;
		int NUM_PAGES;
        
		FILE **fp;

		int read(int id);
        int multRead();
        void write(struct tuple tup);
        void deleteData(int id);
        bool exists(int id);
        bool rExists(int id);
	
};
#endif