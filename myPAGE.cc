#include "myPAGE.hh"


Page::Page(long file_size)
{
    long default_size = 512;
    m_pageSize = default_size;
    long num_files = file_size / default_size;
    m_pages = num_files;
    long i = 0;
    for (i = 0; i < num_files; i++)
    {
        buildPage(i,default_size);
    }
}

int Page::buildPage(long i, long pageSize)
{
    //fopen a page....  which essentially is the db file.
    //create a string the size of pageSiz
    //write string to file....   creates pageSize characters in the file.
    char * b = (char*)malloc(sizeof(char) * 255);
    sprintf(b, "%ld", i);
    string ret(b);
    free(b);
    
    FILE * fp = fopen(ret.c_str(), "w+");
    string data("0");
    for( long j = 0; j < m_pageSize; j++)
    {
        fwrite(data.c_str(),sizeof(char), 1, fp);
    }
    fclose(fp);
    return 0;
}

int Page::insertBuffer(void* buffer, long size)
{
    //fwrite buffer length size into on hashOf(...)
}

