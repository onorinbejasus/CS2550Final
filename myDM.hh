class myDM{
	
	public:
        int _LINEAR_ = 0;
        int _HASH_ = 1;
		myDM(); // Default constructor
        myDM(int searchMode, long bufferSize);
	private:
        int SEARCH_MODE;
        int read(char*filename, void*id);
        int multRead(char*filename,void* areaCode);
        void* write(char* filename, void* record);
        void* deleteData(char* filename);
        bool exists(char*filename);
        bool rExists(char*filename,void*record);
	
};