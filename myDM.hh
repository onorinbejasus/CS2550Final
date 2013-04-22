#ifndef _MYDM_
#define _MYDM_
#define _LINEAR_ 0
#define _HASH_   1

class myDM{
	
	public:
		myDM(); // Default constructor
        myDM(int searchMode, long bufferSize);
	private:
        int SEARCH_MODE;
        int read(char*filename, void*id);
        int multRead(char*filename,void* areaCode);
        int write(char* filename, void* record);
        int deleteData(char* filename);
        bool exists(char*filename);
        bool rExists(char*filename,void*record);
	
};
#endif