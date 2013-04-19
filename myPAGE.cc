#include "myPAGE.hh"

Page::Page()
{
    long default_size = 256;
    buildPage(default_size);
}

Page::Page(long pageSize)
{
    buildPage(pageSize);
}

int Page::buildPage(pageSize)
{
    m_pageSize = pageSize;
    //fopen a page....  which essentially is the db file.
    //create a string the size of pageSize
    //write string to file....   creates pageSize characters in the file.
    return -1;
}

int Page::insertBuffer(void* buffer, long size)
{
    //fwrite buffer length size into on hashOf(...)
}