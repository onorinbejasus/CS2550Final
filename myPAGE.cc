#include "myPAGE.hh"

Page::Page()
{	
    page_size = 512;
	MAX_TUPLES = page_size / sizeof(struct tuple);
	curr_size = 0;
}

/* insert tuple into page */
void Page::insertTuple(struct tuple tup)
{
	
	// insert into tuple into page
	tuples.push_back(tup);
	curr_size += sizeof(tup);

}

/* check if page is full */
bool Page::isFull(int size){
	
	if( (curr_size+size) > page_size)
		return true;
	else
		return false; 
}

/* return the array of tupples */
struct tuple* Page::getBuffer(){
	
	return &tuples[0];
}

/* empties the buffer of records */
void Page::dumpPage(){
	tuples.clear();
}