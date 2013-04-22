#include "myDM.hh"
#include <stdio.h>

myDM::myDM()
{
    SEARCH_MODE = _LINEAR_
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
}


int myDM::multRead(char* filename, void* areaCode)
{
    //if(!exists(filename)) return -2;
    if(!exists(filename)) return -2;
    //if(!rExists(record)) return -1;
}

//write record to file.
void* myDM::write(char* filename, void* record)
{
    FILE* fp;
    //if(!exists(filename)) create the file.
    if(!exists(filename))
    {
        fp = fopen(filename, "w+");
        fclose(fp);
    }
    //fp = fopen(filename,"r+");
    //write record
}

//delete filename
void* myDM::deleteData(char* filename)
{
    remove(filename);
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
    FILE* fp = fopen(filename,"r");
    //search for record.
    if(SEARCH_MODE == _LINEAR_)
    {
        
    }
    else if (SEARCH_MODE == _HASH_)
    {
        
    }
    fclose(fp);
}