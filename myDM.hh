class myDM{
	
	public:
		myDM(); // Default constructor
        myDM(int searchMode, long bufferSize);
	private:
        int _SEARCH_MODE_;
        int read(char*filename, void*id);
        int multRead(char*filename,void* areaCode);
        void* write(char* filename, void* record);
        void* deleteData(char* filename);
        bool exists(char*filename);
        bool rExists(char*filename,void*record);
	
};