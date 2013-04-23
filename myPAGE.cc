#include "myPAGE.hh"

Page::Page()
{	
    page_size = 512;
	MAX_TUPLES = page_size / sizeof(struct tuple);
	curr_size = 0;
}

void Page::insertTuple(struct tuple tup)
{
	
	// insert into tuple into page
	tuples.push_back(tup);

}

bool Page::isFull(int size){
	
	if( (curr_size+size) > page_size)
		return true;
	else
		return false; 
}


