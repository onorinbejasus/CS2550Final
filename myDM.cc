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


struct tuple myDM::read(int id)
{
	/* check pages to see if */
	if(SEARCH_MODE == 0){ // scan
		
		bool found = false;
		/* first check pages */
		for(int i = 0; i < NUM_PAGES; i++){
			
			if(!pages[i].isEmpty()){
				
				struct tuple ret = pages[i].scan(id);
				
				if(ret.ID == -1) // empty
					continue;
				else {
					return ret;
				}
			} // end if
			
		} // end for
		
		if(!found){ // we much iterate over the file
			
			for(int i = 0; i < fp.size(); i++ ){
				
				std::ifstream script( (fp[0]).c_str() ); // current files
				string file;
				
				if (script.is_open()){ //  if able to open file

					/* read entire file */

					script.seekg(0, std::ios::end);   
					file.reserve(script.tellg());
					script.seekg(0, std::ios::beg);

					file.assign((std::istreambuf_iterator<char>(script)),
					            std::istreambuf_iterator<char>());

					script.close(); // close file
					
					std::istringstream lines(file.c_str()); // iterate over the file lines
					
					string tuple;					
					while(!lines.eof()){
						
						getline(lines,tuple);
						
						char * cstr = new char [tuple.length()+1];
						std::strcpy (cstr, tuple.c_str());
						
						char * ptr;
						ptr = strtok (cstr,",");
						
						// continue
						if( atoi(ptr) != id)
							continue;
						else{
							// create a struct 
							struct tuple ret;
							// populate it
							ret.ID = atoi(ptr);
							ptr = strtok (NULL,",");
							strcpy(ptr, ret.ClientName);
							ptr = strtok (NULL,",");
							strcpy(ptr, ret.Phone);
							
							return ret;
						} // end else
						
					} // end while !eof
				
				} // if open
			
			} // for files
			
		} // not in cache
		
	} // end search mode scan
	
	else{ // hash search mode
		
		
	} // end if
	
	struct tuple empty;
	empty.ID = -1;
	return  empty;
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