#ifndef _MYPAGE_H_
#define _MYPAGE_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <iterator>
#include <algorithm>

#include <string>
#include <sstream>

using namespace std;

struct tuple{

	int ID; // unique
	int AreaCode; 
	char ClientName[18];
	char Phone[12];
};

class Page
{
    public:
        Page();

        void insertTuple(struct tuple tup);
		
		// check if empty or if will be full with next add
		bool isFull(int size);
		bool isEmpty();
		
		struct tuple *getBuffer();
		
		// see if element is in the buffer
		struct tuple scan(int id);
		
	protected:
		void dumpPage();
		
    private:
		
		int MAX_TUPLES; // max number of records a file can hold
		int page_size; // the default page size
		int curr_size; // the current size of the page
		
		// vectors of the tuples and ids
		vector <struct tuple> tuples;
		vector<int> ids;
		
		
};

#endif
