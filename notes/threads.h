#include "pthread.h"

class RwLock
{
public:
    RwLock();
    ~RwLock();

    void getReadLock();
    void getWriteLock();
    void unLock();
    
private:
    // if nreaders >= 0, indicates #readers.
    // If nreaders<0, there is a write lock. 
    int nreaders;
    pthread_mutex_t rw_mutex;
    pthread_cond_t rw_cond;
};
