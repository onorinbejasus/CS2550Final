#include "myPTM.hh"
/*
#define _EXECUTION_ERROR_ -4
#define _INSERTION_ERROR_ -2
#define _LOCKFILE_ERROR_  -3
*/

/*
  Point of transactions:
  Grouping a set of operations together such that they are not interrupted.  Essentially locking the files until the transaction is done.
 
  
 
*/

myPTM::myPTM()
{
    
}

myPTM::myPTM(vector<string> cmd)
{
    //LOCK FILE OR WAIT
    FILE * lf = fopen("lockfile","r+");
    char* lock;
    fread(lock,sizeof(char), 1, lf);
    if(strcmp(lock,"0")==0)
    {
        fseek(lf,0, SEEK_SET);
        fwrite("1", sizeof(char), 1, lf);
    }
    else
    {
        //if it IS already 1.
        //WAIT();
    }
    fclose(lf);
    
    //execute tasks
    int i = 0;
    int retVal = 0;
    for(i = 0; i < cmd.size(); i++)
    {
        string subCmd = cmd[i];
        retVal = executeCommand(subCmd);
        if(retVal == -1)
        {
            break;
        }
    }
    if (retVal == -1 )
    {
        //rollback
    }
    else
    {
        //carry out injections.
    }
    
    //unlock the file.
    lf = fopen("lockfile","r+");
    fread(lock,sizeof(char), 1, lf);
    if(strcmp(lock,"1")==0)
    {
        fseek(lf,0, SEEK_SET);
        fwrite("0", sizeof(char), 1, lf);
    }
    else
    {
        //a fatal error.  Somehow the lock was released before it should have.
        //THROW SOME SORT OF FLAG OR ERROR
    }
    fclose(lf);
}

int myPTM::executeCommand(string cmd)
{
    //here is where we are going to execute a series of commands;
    //if there is any sort of conflict, return an errorCode.
    
    return -1;
}

int myPTM::lockRows(int start, int end)
{
    m_start = start;
    m_end = end;
    
    
    return -1;
}

int myPTM::unlockRows(void)
{
    m_start = -1;
    m_end = -1;
    
    return -1;
}

int myPTM::writeOut(void* buffer, void* dataset)
{
    
    
    return -1;
}