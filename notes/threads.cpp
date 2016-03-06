#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "ctype.h"
#include "iostream"
#include <unistd.h>

#include "threads.h"

RwLock::RwLock()
{
    nreaders = 0;
    pthread_mutex_init(&rw_mutex, nullptr);
    pthread_cond_init(&rw_cond, nullptr);
}

RwLock::~RwLock()
{
    pthread_mutex_destroy(&rw_mutex);
    pthread_cond_destroy(&rw_cond);
}

// What if we want to give writers higher priority.
// If there are many readers, writers may wait forever...
// Add nwriters counter. 
void RwLock::getReadLock()
{
    pthread_mutex_lock(&rw_mutex);
    while (-1==nreaders) {
        pthread_cond_wait(&rw_cond, &rw_mutex);
    }

    nreaders++;
    pthread_mutex_unlock(&rw_mutex);
}

void RwLock::getWriteLock()
{
    pthread_mutex_lock(&rw_mutex);
    while (0 != nreaders) {
        pthread_cond_wait(&rw_cond, &rw_mutex);
    }

    nreaders = -1;
    pthread_mutex_unlock(&rw_mutex);
}

void RwLock::unLock()
{
    pthread_mutex_lock(&rw_mutex);
    if (nreaders > 0) {
        nreaders--;

        // May do this after unlock the mutex. 
        if (nreaders == 0) {
            pthread_cond_broadcast(&rw_cond);
        }
    }
    else if (nreaders == -1) {
        nreaders = 0;

        // May do this after unlock the mutex.
        pthread_cond_broadcast(&rw_cond);
    }

    pthread_mutex_unlock(&rw_mutex);
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

    res=pthread_create(&r2, nullptr, reader, nullptr);
    if (res != 0) {
        cout<<"Failed to start thread.\n";
        exit(2);
    }

    sleep(1);
    res = pthread_create(&w1, nullptr, writer, nullptr);
    if (res != 0) {
        cout<<"Failed to start thread.\n";
        exit(3);
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
