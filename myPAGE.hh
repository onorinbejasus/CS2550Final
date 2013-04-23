#ifndef _MYPAGE_H_
#define _MYPAGE_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>

using namespace std;

struct tuple{

	int ID; // unique
	char ClientName[18];
	char Phone[12];
};

class Page
{
    public:
        Page();

        void insertTuple(struct tuple tup);
		bool isFull(int size);
		
    private:
		
		int MAX_TUPLES; // max number of records a file can hold
		int page_size; // the default page size
		int curr_size; // the current size of the page
		
		int bot_range;
		int top_range;
		
		vector <struct tuple> tuples;
		
};

#endif
