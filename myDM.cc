#include "myDM.hh"
#include <stdio.h>

myDM::myDM()
{
    SEARCH_MODE = _LINEAR_;
}
myDM::myDM(int searchMode, long bufferSize)
{
    SEARCH_MODE = searchMode;
}


int myDM::read(char* filename, void* id)
{
    //if(!exists(filename)) return -2;
    if(!exists(filename)) return -2;
    //if(!rExists(record)) return -1;
    //retrieve records with areacode in filename.
    return 0;
}


int myDM::multRead(char* filename, void* areaCode)
{
    //if(!exists(filename)) return -2;
    if(!exists(filename)) return -2;
    //if(!rExists(record)) return -1;
    return 0;
}

//write record to file.
int myDM::write(char* filename, void* record)
{
    FILE* fp;
    //if(!exists(filename)) create the file.
    if(!exists(filename))
    {
        fp = fopen(filename, "w+");
        fclose(fp);
    }
    fp = fopen(filename,"r+");
    //write record
    fclose(fp);
    return 0;
}

//delete filename
int myDM::deleteData(char* filename)
{
    //deletes filename if it is a file OR directory.
    return remove(filename);
}

//if filename exists return true, else false;
bool myDM::exists(char* filename)
{
    //if file exists, return true.
    //else return false.
    FILE* fp = fopen(filename, "r+");
    if(fp != NULL){
        fclose(fp);
        return true;
    }
    return false;
}

//if record exists, return true, else false
bool myDM::rExists(char*filename,void* record)
{
    bool ret = false;
    FILE* fp = fopen(filename,"r");
    //search for record.
    if(SEARCH_MODE == _LINEAR_)
    {
        //search linearly.
        //set 'ret' to true if found
    }
    else if (SEARCH_MODE == _HASH_)
    {
        //search hash-early.
        //set 'ret' to true if found
    }
    fclose(fp);
    return ret;
}