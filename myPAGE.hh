#ifndef _MYPAGE_H_
#define _MYPAGE_H_

#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;
class Page
{
    public:
        Page(){};
        Page(long file_size);
        int insertBuffer(void* buffer, long size);
    private:
        long m_pageSize;
        long m_pages;
        int buildPage(long i, long pageSize);
};

#endif
