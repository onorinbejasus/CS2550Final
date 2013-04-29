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
	ids.push_back(tup.ID);
	
	curr_size += sizeof(tup);

}

struct tuple Page::scan(int id){
	
	// iterate over vector, get iterator if exists
	vector<int>::iterator it = find(ids.begin(), ids.end(), id);
	
	if (it != ids.end()) { // if exists
		
		// return value at that index
		int cur_index = (int)( it - ids.begin() );
		return tuples[ cur_index ];
			
	} else { // else return null
		
		struct tuple empty;
		empty.ID = -1;
		return  empty;
	}
}

/* check if page is full */
bool Page::isFull(int size){
	
	if( (curr_size+size) > page_size)
		return true;
	else
		return false; 
}

bool Page::isEmpty(){
	
	if(tuples.empty())
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