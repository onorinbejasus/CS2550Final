#include <stdio.h>
#include "myDM.hh"

// pagess
Page *pages;

myDM::myDM(int searchMode, int maxRecords, int numPages):
	SEARCH_MODE(searchMode), MAX_NUM_RECORDS(maxRecords), NUM_PAGES(numPages)
{
	
	// create an array of pages to be used for manipulation
	pages = new Page[NUM_PAGES];
		
} // end constructor


int myDM::read(int id)
{
	return 0;
}


int myDM::multRead()
{
	return 0;
}

//write record to file.
void myDM::write(struct tuple tup)
{
    
}

//delete filename
void myDM::deleteData(int id)
{
   
}

//if filename exists return true, else false;
bool myDM::exists(int id)
{

	return false;
   
}

//if record exists, return true, else false
bool myDM::rExists(int id)
{
	return false;
}