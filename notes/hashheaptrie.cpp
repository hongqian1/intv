#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "ctype.h"
#include "stack"
#include <netinet/in.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <memory>
#include <list>

#include "hashheaptrie.h"

using namespace std;
/* LRU cache
  Design and implement a data structure for Least Recently Used (LRU) cache. It should support the following operations: get and set.

get(key) - Get the value (will always be positive) of the key if the key exists in the cache, otherwise return -1.
set(key, value) - Set or insert the value if the key is not already present. When the cache reached its capacity, it should invalidate the least recently used item before inserting a new item.
 */
// First define the item in the list. It must include both key and value. 
struct Item{
    Item(int k, int v) : key(k), value(v) {}
    int key;
    int value;
};

class LRUCache{
public:
    // @param capacity, an integer
    LRUCache(int capacity) {
        limit = capacity;
    }
    
    // @return an integer
    int get(int key) {
        // The return of find() is an iterator to the map. It contains first
        // and second. 
        auto ret = keyMap.find(key);
        if (ret == keyMap.end())
            return -1;
        else {
            // Use splice() to move the iterator to the end. 
            cacheList.splice(cacheList.end(), cacheList, ret->second);
            return ret->second->value;
        }
    }

    // @param key, an integer
    // @param value, an integer
    // @return nothing
    void set(int key, int value) {
        auto ret = keyMap.find(key);
        if (ret == keyMap.end()) {
            // Don't forget to check the limit.
            if (cacheList.size() == limit) {
                // drop the first one due to capacity limit.
                // Also remove the key from the hash map first. 
                auto it = cacheList.begin();
                keyMap.erase(it->key);
                cacheList.pop_front();
            }
            cacheList.push_back(Item(key, value));

            // can not use cacheList.end()-1 directly. The '-' operator
            // is not overloaded for list. But you can use it for vectors
            // because its memory is contiguous. 
            auto it = cacheList.end();
            --it;
            // Use make_pair or [] operator to insert into hash map.
            // The type is implicit in make_pair. 
            keyMap.insert(make_pair(key, it));
            //keyMap[key] = it;
        } else {
            ret->second->value = value;

            // use splice! Get familiar with the syntax. 
            cacheList.splice(cacheList.end(), cacheList, ret->second);
        }
    }
private:
    int limit;

    // The item in the list should contain both the key and value.
    // The key is needed to refer back to the hash table. 
    list<Item> cacheList;
    unordered_map<int, list<Item>::iterator> keyMap;
};

/* Heapify
  Given an integer array, heapify it into a min-heap array.
For a heap array A, A[0] is the root of heap, and for each A[i], A[i * 2 + 1] is the left child of A[i] and A[i * 2 + 2] is the right child of A[i].
Example
Given [3,2,1,4,5], return [1,2,3,4,5] or any legal heap array.

Challenge
O(n) time complexity
 */
// Note: Check http://www.sanfoundry.com/cpp-program-implement-binary-heap/ 
class HeapImpl{
public:
    /**
     * @param A: Given an integer array
     * @return: void
     */
    int leftChild(int parent, vector<int> & A) {
        int c = parent*2+1;
        if (c>=A.size()) // check boundary
            return -1;
        else
            return c;
    }
    
    int rightChild(int parent, vector<int> & A) {
        int c = parent*2+2;
        
        if (c>=A.size())
            return -1;
        else
            return c;
    }
    
    int parent(int child) {
        int p = (child-1)/2;
        if (child == 0) // No parent for root
            return -1;
        else
            return p;
    }

    void heapifyDown(vector<int> &A, int i) {
        int lc = leftChild(i, A);
        int rc = rightChild(i, A);
        
        // bondry checking is very important for array. 
        if (lc<0 && rc<0)
            return;
            
        int child;
        if (lc < 0)
            child = rc;
        else if (rc <0)
            child = lc;
        else
            child = A[lc] < A[rc] ? lc:rc;
        
        if (A[child] < A[i]) {
            swap(A[child], A[i]);
            heapifyDown(A, child);
        }
    }

    // heapify up/down is basically swapping, keep swapping. 
    void heapifyUp(vector<int> & A, int i) {
        int p = parent(i);
        if (i>0 && p >= 0 && A[i] < A[p]) {
            swap(A[i], A[p]);
            heapifyUp(A, p);
        }
    }

    // heapify the whole array. Treat first item as a heap. Then insert
    // each item after that. Use heapifyUp for each insertion from back. 
    void heapify(vector<int> &A) {
        for (int i=1; i<A.size(); ++i) {
            heapifyUp(A, i);
        }
    }
};

int main() {
    return 0;
}
