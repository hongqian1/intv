#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "ctype.h"
#include "iostream"
#include <unistd.h>

#include "threads.h"

RwLock::RwLock()
        : nReaders(0)
        , nWaitingWriters(0)
        , mutex()
        , cond()
{
}

RwLock::~RwLock()
{
}

// if nreaders >= 0, indicates #readers.
// If nreaders<0, there is a write lock. 

// Trick: Implement the non priority writer version first, then add priority writer with nWaitingWriters.

void RwLock::getReadLock()
{
    pthread_mutex_lock(&mutex);
    while (nReaders == -1 || nWaitingWriters > 0) {
        pthread_cond_wait(&cond, &mutex);
    }

    nReaders++;
    pthread_mutex_unlock(&mutex);
}

void RwLock::getWriteLock()
{
    pthread_mutex_lock(&mutex);
    nWaitingWriters++;
    while (nReaders != 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    
    nWaitingWriters--;
    nReaders = -1;
    pthread_mutex_unlock(&mutex);
}

void RwLock::unLock()
{
    pthread_mutex_lock(&mutex);
    if (nReaders > 0) {
        --nReaders;
        if (nReaders == 0) {
            pthread_cond_broadcast(&cond);
        }
    }
    else if (nReaders == -1) {
        nReaders = 0;
        pthread_cond_broadcast(&cond);
    }
    
    pthread_mutex_unlock(&mutex);
}

// Test code
using namespace std;

RwLock rwlock;

void* reader(void * arg)
{
    cout << "Obtaining read lock.\n";
    rwlock.getReadLock();
    cout << "Obtained read lock.\n";

    sleep(5);
    
    cout<<"Releasing read lock.\n";
    rwlock.unLock();
    cout<<"Released read lock.\n";
}

void* writer(void * arg)
{
    cout<<"Obtaining write lock.\n";
    rwlock.getWriteLock();
    cout<<"Obtained write lock.\n";

    sleep(5);
    
    cout<<"releasing write lock.\n";
    rwlock.unLock();
    cout<<"Reelased write lock.\n";
}

int main(int argc, char ** argv)
{
    pthread_t r1, r2, w1, w2;
    int res;
    res=pthread_create(&r1, nullptr, reader, nullptr);
    if (res != 0) {
        cout<<"Failed to start thread.\n";
        exit(1);
    }

    sleep(1);
    res = pthread_create(&w1, nullptr, writer, nullptr);
    if (res != 0) {
        cout<<"Failed to start thread.\n";
        exit(3);
    }

    res=pthread_create(&r2, nullptr, reader, nullptr);
    if (res != 0) {
        cout<<"Failed to start thread.\n";
        exit(2);
    }

    res=pthread_create(&w2, nullptr, writer, nullptr);
    if (res != 0) {
        cout<<"Failed to start thread.\n";
        exit(4);
    }

    // Use pthread_exit() to ensures the spawned threads keep running.
    // May also use join for that. 
    pthread_exit(nullptr);

    /*
    pthread_join(r1, nullptr);
    pthread_join(r2, nullptr);
    pthread_join(w1, nullptr);
    pthread_join(w2, nullptr);
    */
}
