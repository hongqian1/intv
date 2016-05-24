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

/**
 * Uber
 * Time Travelling Hash Table/Map/Dict 
 * 
 * put(foo, 10) = bar
 * put(foo, 20) = bar2
 * put(foo, 30) = bar3
 * 
 * get(foo, 20) == bar2
 * get(foo, 30) == bar3
 * get(foo, 25) == bar2
 *
 * Note: get(foo, 25) returns the value for 20 because 20 is already inserted
 *       when querying 25, but 30 is not.
 */

// Note: need add include headers and namespace because Hackerrank does not
//       include them.
#include <string>
#include <unordered_map>
#include <map>
#include <iostream>

using namespace std;
class TimeTraveller {
public:
    TimeTraveller() {}
    void put(const string &foo, int ftime, const string& bar) {
        hashMap[foo].emplace_back(ftime, bar);
    }
    
    string get(const string &foo, int ftime) {
        if (hashMap.find(foo) == hashMap.end()) {
            return "NULL";
        }
        else {
            // Check the definition of hashMap, we are handleing
            // vector<pair<int, string>>
            // Need a binary search function
            int index = search(hashMap[foo], ftime);
            return index == -1 ? "NULL" : hashMap[foo][index].second;
        }
    }
 
private:
    unordered_map<string, vector<pair<int, string>>> hashMap;

    // Note that it returns the previous index if not found.
    // It may return -1 if the target ftime is smaller than the first one.
    int search(vector<pair<int, string>> & timeArray, int ftime) {
        if (timeArray.empty())
            return -1;
        if (ftime < timeArray[0].first)
            return -1;
        if (ftime > timeArray.back().first)
            return timeArray.size() - 1;

        int start = 0;
        int end = timeArray.size() - 1;
        while (start + 1 < end) {
            int mid = start + (end - start) / 2;
            if (ftime == timeArray[mid].first)
                return mid;
            else if (ftime < timeArray[mid].first)
                end = mid;
            else
                start = mid;
        }

        if (ftime == timeArray[start].first)
            return start;
        if (ftime == timeArray[end].first)
            return end;
        return start;
    }
};

int main() {
    // testing code. Check normal cases + edge cases. 
    TimeTraveller timeTraveller;
    timeTraveller.put("foo", 10, "bar");
    timeTraveller.put("foo", 20, "bar2");
    timeTraveller.put("foo", 30, "bar3");
    
    cout << "foo, 20 : " << timeTraveller.get("foo", 20) << endl;
    cout << "foo, 30 : " << timeTraveller.get("foo", 30) << endl;
    cout << "foo, 25 : " << timeTraveller.get("foo", 25) << endl;
    cout << "foo,  1 : " << timeTraveller.get("foo", 1) << endl;
    
    return 0;
}

