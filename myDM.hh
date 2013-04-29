#ifndef _MYDM_
#define _MYDM_

// #ifdef __GNUC__
// #include <tr1/unordered_map>
// #else
// #include <unordered_map>
// #endif

#include <tr1/unordered_map>

#include "myPAGE.hh"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// struct index_record {
// 	int offset; // offset into file (page?)
// 	int page; // page of file (?)
// };

class myDM{
	
	public:
		myDM(){}; // Default constructor
		myDM(int searchMode, int maxRecords, int numPages, string filename);
		
		// struct tuple read(int id);
		// int multRead(int area);
		// void write(struct tuple tup);
		// void load();
		// int read_tuple(int id);
		// void writeOut();
		
		int read(int id);
		int multRead(int areaCode);
		int write(tuple newTuple);
		int deleteData();
		
		void loadTuples();
		void writeOut();
		
	private:
		
		int SEARCH_MODE;
		int MAX_NUM_RECORDS;
		int NUM_PAGES;
		int tuple_length;
    string filename;
		fstream datafile;
		
		vector<string> fp;
		//vector<fstream *> file_pointers;
		vector<string> file_names;
		
		tr1::unordered_map<int,int> hash_index;
		tr1::unordered_map<int,struct tuple> tuple_list;
		vector<int> linear_index;
		
		int next_line;
		void goToLine(int num);
		
		//vector<struct index_record> linear_offset;
		
		// void deleteData(int id);
		// bool exists(int id);
		// bool rExists(int id);
	
		
};
#endif