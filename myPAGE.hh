#ifndef _MYPAGE_H_
#define _MYPAGE_H_

#include <iostream>


class Page
{
    public:
        Page( long pageSize );
        Page();
        int insertBuffer(void* buffer, long size);
    private:
        long m_pageSize;
        int buildPage(long pageSize);
}

#endif
