#include "pthread.h"

class RwLock
{
public:
    RwLock();
    ~RwLock();

    // Three main functions. 
    void getReadLock();
    void getWriteLock();
    void unLock();
    
private:
    // if nreaders >= 0, indicates #readers.
    // If nreaders<0, there is a write lock. 
    int nReaders;
    int nWaitingWriters;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};
