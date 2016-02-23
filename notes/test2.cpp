#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "ctype.h"
#include "stack"
#include <netinet/in.h>
#include <unordered_set>
#include <list>
#include <vector>
#include <algorithm>
#include <stack>
#include <map>
#include <unordered_map>
#include <iostream>
#include <limits>
#include <utility> // for swap()
#include <queue>

#include "test2.h"

using namespace std;

void deleteChars(char * str, const char * str2)
{
    if (!str || !str2)
        return;
    
    bool helper[256];
    memset(helper, 0, sizeof(helper));

    while(*str2)
    {
        helper[*str2] = true;
        ++str2;
    }

    char * ptr = str;
    while (*str)
    {
        if (!helper[*str])
        {
            *ptr++=*str;
        }
        ++str;
    }
    *ptr = 0;
}

void sortString(char * str)
{
    if (!str) return;
    while (*str)
    {
        char * ptr = str+1;
        while(*ptr)
        {
            if (*ptr<*str)
            {
                // swap the chars
                char tmp = *str;
                *str=*ptr;
                *ptr=tmp;
            }
            ++ptr;
        }
        ++str;
    }
}

void sortString2(char * str)
{
    if (!str)
        return;
    
    int helper[256];
    memset(helper, 0, sizeof(helper));

    char * ptr=str;
    while (*ptr)
    {
        helper[*ptr]++;
        ++ptr;
    }

    ptr = str;
    for (int i=0;i<256;++i)
    {
        for (int j=0; j<helper[i]; ++j)
            *ptr++=i;
    }
}

void sortString3(char * str)
{
    if (!str) return;

    int len = strlen(str);
    for (int i=0;i<len;++i)
    {
        for (int j=i+1;j<len;++j)
        {
            if (str[j]<str[i])
            {
                char tmp = str[i];
                str[i]=str[j];
                str[j]=tmp;
            }
        }
    }
}

void sortString4(char * str)
{
    if (!str) return;

    int helper[256];
    memset(helper, 0, sizeof(helper));
    for (int i=0; i<strlen(str); ++i)
    {
        ++helper[str[i]];
    }

    int k=0;
    for (int i=0;i<256;++i)
    {
        for (int j=0; j<helper[i]; ++j)
            str[k++] = i;
    }
}

void removeDupSortedString(char * str)
{
    if (str == NULL) return;

    char * ptr = str;
    str++;
    
    // compare with the next char.
    while (*ptr)
    {
        if (*ptr != *(ptr+1))
            *str++ = *(ptr+1);

        ++ptr;
    }
    *str = 0;
}

void removeDupSortedString2(char * str)
{
    if (str == NULL) return;

    char * ptr = str;
    int len = strlen(str);
    int index = 1;
    for (int i=0;i<len;++i)
    {
        // It is OK to use i+1 because there is an ending 0
        if (ptr[i]!=ptr[i+1])
            str[index++] = ptr[i+1];
    }
}

char * my_strstr(char * str, const char * sub)
{
    if (str==NULL || sub == NULL)
        return NULL;

    while (*str)
    {
        char * str2=str;
        const char * sub2=sub;

        while(*str2&&*sub2&&*str2==*sub2)
        {
            ++str2;
            ++sub2;
        }

        if (*sub2 == 0)
            return str;

        ++str;
    }
    return nullptr;
}

void removeSubstr(char * str, const char* sub)
{
    if (str==NULL || sub==NULL)
        return;

    char * ptr = str;
    while(*str)
    {
        char * str2=ptr;
        const char * sub2=sub;

        while(*str2 && *sub2&&*str2==*sub2)
        {
            ++str2;
            ++sub2;
        }

        if (*sub2==0)
        {
            ptr=str2;
        }
        else
        {
            *str++=*ptr++;
        }
    }
    *str=0;
}

// Print prime numbers less than N.
void printPrime(int N)
{
    vector<int> primes;
    primes.reserve(1000);
    primes.push_back(2);
    cout<<2<<' ';

    for (int i=3;i<N;++i)
    {
        bool isPrime=false;
        for (int j=0;j<primes.size();++j)
        {
            if (primes[j]>i/2)
            {
                isPrime=true;
                break;
            }
            if (i%primes[j]==0)
                break;
        }
        if (isPrime)
        {
            primes.push_back(i);
            cout<<i<<' ';
        }
    }
}

// No. 1 Binary search tree and double linked link
// http://codercareer.blogspot.com/p/binary-tree-interview-questions.html
void btreetodlink(Node * root, Node ** prev)
{
    if (root == NULL || prev == NULL)
    {
        return;
    }

    btreetodlink(root->left, prev);
    if (prev && *prev) // 
        (*prev)->right = root; // 
    root->left = *prev; // 
    *prev = root;// 
    btreetodlink(root->right, prev);
}

Node * btreeconvertdlink(Node * root)
{
    if (!root) return NULL;
    Node * prev = NULL; // 
    btreetodlink(root, &prev);

    while (root && root->left)
        root = root->left;

    return root;
}

// No. 4
// http://codercareer.blogspot.com/p/binary-tree-interview-questions.html
#define TOTAL   22
void printPath(Node * current, int sum, vector<Node *> &path)
{
    if (!current)
        return;

    sum += current->value;
    path.push_back(current);
    if (current->left==NULL && current->right==NULL && sum==TOTAL)
    {
        //printf();
    }

    printPath(current->left, sum, path);
    printPath(current->right, sum, path);

    path.pop_back();
}

// No. 06
// http://codercareer.blogspot.com/p/binary-tree-interview-questions.html
bool isPostOrder(int * iarray, int len)
{
    if (!iarray || len <1)
        return false; 

    int rootValue = iarray[len-1];

    int i=0;
    for (; i<len-1; ++i)
    {
        if (iarray[i]>rootValue)
            break;
    }

    for (int j=i;j<len-1;++j)
    {
        if (iarray[j] < rootValue)
            return false;
    }

    if (i>0)
        if (isPostOrder(iarray, i)==false)
            return false;
    if (i<len-1)
        if (isPostOrder(iarray+i, len-i-1)==false)
            return false;
    
    return true;
}

// No. 31
bool isBST(Node * current, Node **prev)
{
    // in order
    if (!prev || !current)
        return false;

    if (current->left)
        if (isBST(current->left, prev)==false)
            return false;

    if (*prev && (*prev)->value>=current->value)
        return false;

    *prev = current;

    if (current->right)
        if (isBST(current->left, prev)==false)
            return false;
}

bool isBSTpre(Node * current, int min, int max)
{
    // pre order
    if (!current) return true;

    if (current->value<min || current->value>max)
        return false;

    return isBSTpre(current->left, min, current->value)
        &&isBSTpre(current->right, current->value, max);
}

// No. 45
// Given a binary search tree and a value k, please find a node in the binary search tree whose value is closest to k. 
Node * findNodeCloseToK(Node * current, int k)
{
    if (!current)
        return NULL;

    int minDiff=numeric_limits<int>::max();
    Node * closeNode = NULL;

    // Use iterative instead of recursion. It stops at a sub node.
    while (current)
    {
        if (abs(current->value-k)<minDiff)
        {
            minDiff = abs(current->value-k);
            closeNode = current;
        }

        if (k < current->value)
        {
            current = current->left;
        }
        else
        {
            current=current->right;
        }
    }
    return closeNode;
}

// No. 46
// 1. Do an in order traversal, then find s-n. Same complexity as the the example.
// 2. Add the numbers less than S to a hash table. Then traverse the tree and find S-n for each n. 

bool checkBST(Node * root)
{
    return isBSTpre(root, numeric_limits<int>::min(), numeric_limits<int>::max());
}

void removeSubStr(char * str, const char * sub)
{
    if (str == NULL || sub == NULL)
        return;
    
    char * output = str;
    char * ptr = str;

    while (*ptr)
    {
        const char * sub2 = sub;
        char * ptr2 = ptr;

        while (*ptr2 && *sub2 && *ptr2 == *sub2)
        {
            ++ptr2;
            ++sub2;
        }

        if (*sub2 == '\0')
        {
            ptr = ptr2;
        }
        else
        {
            *output++ = *ptr++;
        }
    }
    *output = '\0';
}

/* The function assumes that there are at least two
   elements in array.
   The function returns a negative value if the array is
   sorted in decreasing order.
   Returns 0 if elements are equal  */
int maxDiff(int arr[], int arr_size)
{
  int maxDiff = arr[1] - arr[0];
  int minValue = arr[0];
  int i;
  for(i = 1; i < arr_size; i++)
  {       
    if(arr[i] - minValue > maxDiff)                               
      maxDiff = arr[i] - minValue;
    if(arr[i] < minValue)
         minValue = arr[i];                     
  }
  return maxDiff;
}    

// Number complement 50->13

// Which one provides conceptual support for function calls?
// The system stack. 

/**
 * Sample input:
 *
 *          1
 *         / \
 *        3   5
 *       /   / \
 *      2   4   7
 *     / \   \
 *    9   6   8
 
 *
 * Expected output:
 *    1
 *    3 5
 *    2 4 7
 *    9 6 8
 *    ==========
 */

// A better solution without using NULL. 
void printTree(Node * root)
{
    if (!root)
        return;
    
    queue<Node * > tasks;
    tasks.push(root);
    while (!tasks.empty())
    {
        int size = tasks.size();
        for (int i=0; i<size; ++i)
        {
            Node * current = tasks.front();
            tasks.pop();
            cout<<current->value<<" ";
            if (current->left)
                tasks.push(current->left);
            if (current->right)
                tasks.push(current->right);
        }
        cout<<endl;
    }
}

void getPerm(const vector<int> & iarr, vector<int>& perm, vector<vector<int>> & output, vector<bool>&helper)
{
    if (perm.size()==iarr.size())
    {
        output.push_back(perm); // not efficient because mem copy. May change later
        return;    
    }
    
    // need loop through iarr and excluding items already in perm, then call the
    // recursive function for each item.
    for (int i=0;i<iarr.size();++i)
    {
        // Helper array is used to exclude the items already in perm.
        // If an item has been vistied, we mark the corresponding slot in
        // helper to true. 
        if (helper[i])
            continue;
        
        vector<int> newPerm(perm);
        newPerm.push_back(iarr[i]);
        vector<bool> newHelper(helper);
        newHelper[i]=true;
        getPerm(iarr, newPerm, output, newHelper);
    }
}

/* Linkedin
 Question Description: 
    Write a function that, given a list of integers (both positive and negative) returns the sum of 
    the contiguous subsequence with maximum sum. Thus, given the sequence
    (1, 2, -4, 1, 3, -2, 3, -1)
    it should return 5.
 
    (note: only return non-negative number)
*/

/*
 * Use dynamic programming. Compare sum(i-1) + a(i) and a(i), choose the larger
 * one. In this case, we only need to check if sum(i-1)<0. If it is less than
 * 0, then we choose a[i] only and reset sum. 
 */
int sumOfSubseq(int iarray[], int length)
{
    // iarray is passed in as a pointer, not an array. So we do not know its
    // length. Need pass in a length separately.
    int max=0;
    int sum=0;

    // maintain a running maximum and running sum. 
    for (int i=0;i<length;++i)
    {
        sum += iarray[i];
        if (sum > max)
            max=sum;
        if (sum <0)
            sum =0;
    }
    
    return max;
}

int getAllPermutations()
{
    vector<int> iarr = {1, 2, 3};
    vector<int> perm;
    vector<vector<int>> output;
    vector<bool> helper(iarr.size(), false); 
    
    getPerm(iarr, perm, output, helper);
}

    /**
     * Given two nodes of a tree,
     * method should return the deepest common ancestor of those nodes.
     *
     *          A
     *         / \
     *        B   C
     *       / \
     *      D   E
     *         / \
     *        G   F
     *
     *  commonAncestor(D, F) = B
     *  commonAncestor(C, G) = A
     */
// use D&C or post order
Node * lowestCommonAncestor(Node * root, Node * node1, Node * node2)
{
    if (root == NULL || root==node1 || root == node2)
        return root;

    Node * left = lowestCommonAncestor(root->left, node1, node2);
    Node * right = lowestCommonAncestor(root->right, node1, node2);

    if (left && right)
        return root;
    if (left)
        return left;
    if (right)
        return right;
    return NULL;
}

// Linkedin
/* This class will be given a list of words (such as might be tokenized
 * from a paragraph of text), and will provide a method that takes two
 * words and returns the shortest distance (in words) between those two
 * words in the provided text. 
 * Example:
 *   WordDistanceFinder finder = new WordDistanceFinder(Arrays.asList("the", "quick", "brown", "fox", "quick"));
 *   assert(finder.distance("fox","the") == 3);
 *   assert(finder.distance("quick", "fox") == 1);
 */
// Note: For this kind of questions, always create an index of the words first.
// The best index is a hash map. If order is needed, use binary tree map. 
class WordDistanceFinder
{
public:
    WordDistanceFinder(const vector<string> &wordList);
    ~WordDistanceFinder(){};
    
    int distance(const string & w1, const string & w2);
    
private:
    // Hash map. The key is the hash number of the word.
    // The value is a list of positions of the word. The word could appear
    // in more than one place. 
    unordered_map<string, vector<int>> wordMap;
};

WordDistanceFinder::WordDistanceFinder(const vector<string> &wordList)
{
    // parse the wordList
    for (int i=0; i<wordList.size();++i)
    {
        wordMap[wordList[i]].push_back(i);
    }
}

int WordDistanceFinder::distance(const string& w1, const string & w2)
{
    if (wordMap.find(w1)==wordMap.end() || wordMap.find(w2)==wordMap.end())
        return -1;

    const vector<int>& wp1 = wordMap[w1];
    const vector<int>& wp2 = wordMap[w2];

    int min = numeric_limits<int>::max();

    int i=0, j=0;
    while (i<wp1.size() && j<wp2.size())
    {
        if (abs(wp1[i]-wp2[j]) < min)
            min = abs(wp1[i]-wp2[j]);

        if (wp1[i]<wp2[j])
            ++i;
        else
            ++j;
    }
    return min;
}

void mergeList()
{
    vector<int> list1 = {1, 2, 3, 4, 5};
    vector<int> list2 = {-10, 11, 12, 13};

    vector<int> list3;
    list3.reserve(list1.size()+list2.size());

    int i1=0;
    int i2=0;
    while(i1<list1.size() && i2<list1.size())
    {
        if (list1[i1]<list2[i2])
            list3.push_back(list1[i1++]);
        else
            list3.push_back(list2[i2++]);
    }

    if (i1 == list1.size())
    {
        for (; i2<list2.size();++i2)
            list3.push_back(list2[i2]);
    }

    if (i2 == list2.size())
    {
        for (; i1<list1.size();++i1)
            list3.push_back(list1[i1]);
    }

    for (int i=0;i<list3.size();++i)
        cout<<list3[i]<<' ';
    cout<<endl;
}

void findMax()
{
    vector<int> iarray = {-1, 2, -3, -4, 5};

    int max = -999999;
    int i1=1;
    int i2=max;
    for (int i =0; i<iarray.size(); ++i)
    {
        for (int j=i+1; j<iarray.size(); ++j)
        {
            int value = iarray[i]*iarray[j];
            if (value>max)
            {
                i1=iarray[i];
                i2=iarray[j];
                max = value;
            }
        }
    }

    cout<<i1<<'*'<<i2<<'='<<max<<endl;
}

// add error checking later
void compareWord(vector<const char *> wordList,  const char * input)
{
    for (int i=0; i <wordList.size(); ++i)
    {
        const char * word = wordList[i];
//        cout<<word<<endl;

        // Now compare
        int count =0;
        const char * ptr = input;
        while (*word && *ptr)
        {
//            cout<<*word;
//            cout<<*ptr;
            if (*word != *ptr)
            {
                ++count;
                if (count >1)
                    break;
                
            }
            ++word;
            ++ptr;
        }

        if (*word == 0 && *ptr ==0 && count ==1)
        {
            // got one match. 
            cout<<wordList[i]<<' ';
        }
    }
    cout<<endl;
}

// May use another commonly used partition alg. 
int partition(vector<int>&array, int start, int end)
{
    // Use index for pivot instead using value. This can avold confusion
    // because we always use array[] for comparison. 
    int pivot = start;
    int pi = start;

    for (int i=start+1;i<=end;++i)
    {
        // Even if swap same element, ++pi still happens.
        // This ensures all elements at or before pi are less than pivot. 
        if (array[i]<array[pivot])
            swap(array[i], array[++pi]);
    }

    swap(array[pi], array[pivot]);
    return pi;
}

// If partition returns the middle index every time, it is O(nlogn).
// However, if partition returns the first index every time, it is O(n^2). 
void myquicksort(vector<int> &iarray, int start, int end)
{
    if (start<end)
    {
        int k = partition(iarray, start, end); // end is valid
        
        // Omit kth element in recursions because it is already sorted!
        // This is the key. You don't want to change the already sorted item. 
        // Otherwise, it will crash!
        myquicksort(iarray, start, k-1);
        myquicksort(iarray, k+1, end); 
    }
}

void insertionSort(vector<int> & a)
{
    for (int i = 1; i<a.size(); ++i)
    {
        int j=i;
        // all elements before j is sorted. Exit the following loop when
        // a[j] >= a[j-1]. 
        while(j>0 && a[j]<a[j-1])
        {
            swap(a[j], a[j-1]);
            j--;
        }
    }
}

void selectionSort(vector<int> &a)
{
    for (int i=0;i<a.size();++i)
    {
        int min = i;
        for (int j=i+1;j<a.size();++j)
        {
            if (a[j]<a[min])
                min = j;
        }
        swap(a[min], a[i]);
    }
}

void bubbleSort(vector<int> &a)
{
    bool swapped=true;

    while(swapped)
    {
        swapped=false;
        for (int i=1;i<a.size();++i)
        {
            if (a[i]<a[i-1])
            {
                swap(a[i], a[i-1]);
                swapped=true;
            }
        }
    } 
}

/* Kth Largest Element
  Find K-th largest element in an array.

Note
You can swap elements in the array

Example
In array [9,3,2,4,8], the 3rd largest element is 4

In array [1,2,3,4,5], the 1st largest element is 5, 2nd largest element is 4, 3rd largest element is 3 and etc.
 */
// Note: this implements std::nth_element(). It uses selection or k partition.
// We include start and end in the parameters to avoid array copy and easy
// comparison with k. 
int kSelection(vector<int> & nums, int k, int start, int end) {
    // First implement partition. 
    int pivot = start;
    int pi = start;
    for (int i = start+1; i<=end; ++i) {
        if (nums[i] > nums[pivot]) {
            swap(nums[i], nums[++pi]);
        }
    }
    swap(nums[pi], nums[pivot]);

    // check the pivot. It may return or call recursion function. 
    if (pi+1 == k)
        return nums[pi];
    else if (pi+1 < k)
        return kSelection(nums, k, pi+1, end);
    else
        return kSelection(nums, k, start, pi-1);
}
    
int kthLargestElement(int k, vector<int> nums) {
    if (k<0 || k>nums.size())
        return -1; // assume -1 is an invalid answer. 

    // Note the parameters for easy recursion. 
    return kSelection(nums, k, 0, nums.size()-1);
}

/*
Given a array of positive integers, find all possible triangle triplets that can be formed from this array. 
eg: 9 8 10 7 
ans: 9 8 10, 9 8 7, 9 10 7, 7 8 10 
Note : array not sorted, there is no limit on the array length
*/
// This is for combination, not for permutation because order is not needed.
// So use triple loop instead of recursion. 
void findAllTriangles(vector<int> & a)
{
    if (a.size()<3)
        return;

    for (int i=0; i<a.size(); ++i)
        for (int j=i+1; j<a.size(); ++j)
            for (int k=j+1; k<a.size(); ++k)
            {
                if (a[i]+a[j]>a[k] &&
                    a[i]+a[k]>a[j] &&
                    a[j]+a[k]>a[i])
                {
                    cout<<a[i]<<" "<<a[j]<<" "<<a[k]<<", ";
                }
            }
    cout << endl;
}

/*
I recently, appeared for second phone interview with CloudEra, the interviewer asked me to 
write a function which takes in an array of integers and returns the highest positive product 
possible by multiplying 3 distinct numbers. NO SORTING is ALLOWED
*/
/*
example: 

[1, 3, 5, 2, 8, 0, -1, 3] 

=> 8 * 5 * 3 = 120 

[0, -1, 3, 100, -70, -5] 

=> -70*-50*100=350000
*/
/* Solution
1) Find top 3 and bottom 2 values from the list in O(n) 
2) Let the list be [a, b, c ..... d, e] 

modification:
3) find maximum of b*c and d*e.
prod1 = max(b*c, d*e);

4. output: a * prod1.

Reason: if the first three are positive, we only need the first 3 numbers.
If there are negatives, we need compare the last two numbers with b*c.

The first one must be positive to get a solution.
If it is negative, no solution.
Then, there are only two possible solutions, either b and c are both positive,
or d and e are both negative. 

Tip: although no sorting, you can still find the top 3 and bottom two numbers in
O(1) time. So do not ignore this option when no sorting. 
 */
void highestPositiveProduct(vector<int> &a)
{
    if (a.size()<3)
    {
        cout<<"no solution"<<endl;
        return;
    }

// Need consider 4, 5 cases
    
    int max1=numeric_limits<int>::min();
    int max2=max1, max3=max1;
    int min1=numeric_limits<int>::max();
    int min2=min1;

    for (int i=0; i<a.size(); ++i)
    {
        // Note: the order of maxn and minn is important.
        // max1 is the largest and min2 is the lowest.
        // You have to compare with the largest or lowest first. 
        if (a[i]>max1)
        {
            max3 = max2;
            max2 = max1;
            max1=a[i];
        }
        else if (a[i]>max2)
        {
            max3=max2;
            max2=a[i];
        }
        else if (a[i]>max3)
            max3=a[i];

        // for min
        if (a[i]<min2)
        {
            min1=min2;
            min2=a[i];
        }
        else if (a[i]<min1)
            min1=a[i];
    }

    int prod1 = max2*max3;
    int prod2 = min1*min2;

    // No need to consider the case of 4 numbers in the array.
    // Overlapping numbers in max and min are OK, not affecting the result. 
    if (max1<0||(prod1<0 && prod2<0))
    {
        cout<<"no solution"<<endl;
        return;
    }
    
    if (prod1>prod2)
        cout<<max1<<"*"<<max2<<"*"<<max3<<"="<<max1*prod1<<endl;
    else
        cout<<max1<<"*"<<min1<<"*"<<min2<<"="<<max1*prod2<<endl;
}

/*
  cloudera
  Give an matrix of m *n with all the elements=1, Also given is a list of (x,y) points. The question is to zero out all the elements of xth row and yth column. 
 */
// Change to use two elements vector for Point
struct Point
{
    Point(){x=0; y=0;}
    Point(int a, int b):x(a), y(b) {};
    
    int x;
    int y;
};

void fillZero(int** matrix, int m, int n, Point * plist, int len)
{
    // Fill rows first
    for (int i=0; i<len; ++i)
    {
        int row = plist[i].x;

        // Need boundary check 
        if (row >= 0 && row < m)
            memset(matrix[row], 0, sizeof(int)*n);
    }

    for (int i=0; i<m; ++i)
    {
        for (int j=0; j<len; ++j)
        {
            int col=plist[j].y;
            if (col>=0 &&col<n)
                matrix[i][col] = 0;
        }
    }
}

/*
  Array Pair Sum. Solve it in O(N) time complexity.
  There is a n element int array. How do you detect the two numbers that add up to S.
 */
vector< pair<int, int> > pair_sum(vector<int> arr, int s)
{
	vector< pair<int, int> > result;
	unordered_set< int> helper;
	
	for(int i=0; i<arr.size(); i++)
	{
		if(helper.find(s-arr[i]) != helper.end())
			result.push_back( make_pair(arr[i], s-arr[i]) );

        helper.insert(arr[i]);
	}
	
	return result;
}

/*
Given a binary tree where all the right nodes are leaf nodes, flip it upside down and turn it into a tree with left leaf nodes. 

Keep in mind: ALL RIGHT NODES IN ORIGINAL TREE ARE LEAF NODE.

 * for example, turn these:
 *
 *        1                 1
 *       / \               / \
 *      2   3             2   3
 *     / \
 *    4   5
 *   / \
 *  6   7
 *
 * into these:
 *
 *        1               1
 *       /               /
 *      2---3           2---3
 *     /
 *    4---5
 *   /
 *  6---7
 *
 * where 6 is the new root node for the left tree, and 2 for the right tree.
 * oriented correctly:
 *
 *     6                  2
 *    / \                / \
 *   7   4              3   1
 *        / \
 *       5   2
 *            / \
 *          3   1
 */
Node * flipTree(Node * root)
{
    Node * current = root;
    Node * prevLeaf = NULL;
    Node * prevNode = NULL;
    
    while (current)
    {
        Node * left = current->left;
        current->left = prevLeaf;
        prevLeaf = current->right;
        current->right = prevNode;

        prevNode = current;
        current = left;
    }

    return prevNode;
}

/**
 * Implement a method which takes an integer array and returns an integer array (of equal size) in
 * which each element is the product of every number in the input array with the exception of the
 * number at that index.
 *
 * Example:
 *   [3, 1, 4, 2] => [8, 24, 6, 12]
 */
// If divisionn is allowed, it will be easy. But I guess it is not allowed. 
// It is a good habit to draw picture before coding. It helps understand
// the problem. It can also be a reference for coding. Do not store everything
// in brain. Human brain can not hold that much info. 
void selfExcludingProduct(const vector<int>& input, vector<int>& output) {

    // First build the first half of the product.
    // Add them to the output.
    output.reserve(input.size());

    int firstHalf =1;
    for (int i=0;i<input.size();++i)
    {
        output.push_back(firstHalf);
        firstHalf *= input[i];
    }

    // build second half and multiply with the first half. Place the product
    // in output. Do it from the backward.
    int secondHalf = 1;
    for (int i=input.size()-1; i>=0; --i)
    {
        output[i] = output[i] * secondHalf;
        secondHalf *= input[i];
    }
}

/*
  Given a sequence, find the length of the longest palindromic subsequence in it. For example, if the given sequence is “BBABCBCAB”, then the output should be 7 as “BABCBAB” is the longest palindromic subseuqnce in it. “BBBBB” and “BBCBB” are also palindromic subsequences of the given sequence, but not the longest ones.
 */
// The following is a recursive implementation based on induction. 
// Check http://www.geeksforgeeks.org/dynamic-programming-set-12-longest-palindromic-subsequence/
int lps(char *seq, int i, int j)
{
   // Base Case 1: If there is only 1 character
   if (i == j)
     return 1;
 
   // Base Case 2: If there are only 2 characters and both are same
   if (seq[i] == seq[j] && i + 1 == j)
     return 2;
 
   // If the first and last characters match
   if (seq[i] == seq[j])
      return lps (seq, i+1, j-1) + 2;
 
   // If the first and last characters do not match
   return max( lps(seq, i, j-1), lps(seq, i+1, j) );
}

// modifed from lps() for integers and array convention 
int lps2(int * seq, int len)
{
    //   if (len == 0) return 0;
    // The two base case or building blocks are len == 1 and len ==2 with
    // same items. 
    if (len == 1) return 1;

    if (len == 2 && seq[0]==seq[1])
        return 2;

    if (seq[0] == seq[len - 1])
        return lps2(seq+1, len-2)+2;

    return max(lps2(seq, len-1), lps2(seq+1, len-1));
}

// lps and lps2 have Overlapping Subprolems property, some subproblems are
// solved repeatedly. Not efficient.

// This is from Linkedin second interview.
// Only one programming question with some questions about your previous
// projects. 
class Intervals {
    
public:
    /**
     * Adds an interval [from, to) into internal structure.
     */
    virtual void addInterval(int from, int to) = 0;

     /**
     * Returns a total length covered by the added intervals.
     * If several intervals intersect, the intersection should be counted only once.
     * Example:
     *
     * addInterval(3, 6)
     * addInterval(8, 9)
     * addInterval(1, 5)
     *
     * getTotalCoveredLength() -> 6
     * I.e. [1,5) and [3,6) intersect and give a total covered interval [1,6).
     * [1,6) and [8,9) don't intersect, so the total covered length is a sum of both intervals, that is 5+1=6.
     *
     *           _________
     *                          ___
     *     ____________
     *
     * 0  1  2  3  4  5  6  7  8  9  10
     *
     */
    virtual int getTotalCoveredLength() = 0;
};

class SubInterval : public Intervals
{
public:
    void addInterval(int from, int to);
    
    int getTotalCoveredLength();
    
private:
    map<int, int> allIntervals; // the key is the begin number, the value is the end number
};

void SubInterval::addInterval(int from, int to)
{
    // Never add raw data directly! You need analyze the data and adjust them.
    // There are 3 steps in the following code to merge overlapped interval.
    // 1. merge with the same node. 2. merge with the previous node.
    // 3. merge with the next node. 

    // Merge with the same node
    auto ret = allIntervals.insert(pair<int, int>(from, to));
    auto it = ret.first;
    if (ret.second == false) {
        it->second = max(it->second, to);
    }

    // compare it and itPrev to merge with previous node 
    if (it != allIntervals.begin())
    {
        // Can not use itPrev = it-1. Must do the following way.
        // you may also use advance(it, -1);
        auto itPrev = it;
        --itPrev;
        if (itPrev->second >= it->first) {
            itPrev->second = max(itPrev->second, it->second);
            allIntervals.erase(it); // No need this element.

            // This is important. Need update "it" after it is erased.
            // It will be used in the following code. 
            it = itPrev; 
        }
    }

    // You can not use itNext = it+1. You must do the following way. 
    auto itNext = it;
    ++itNext;

    // compare it and itNext to merge with next node.
    // Need a loop for all the next nodes because the new interval may
    // cover all.
    while (itNext != allIntervals.end())
    {
        if (it->second >= itNext->first)
        {
            it->second = max(it->second, itNext->second);
            allIntervals.erase(itNext);
        }
        itNext = it;
        ++itNext;
    }
}

int SubInterval::getTotalCoveredLength()
{
    // If this function is called frequently, we can update the total
    // whenever we insert an interval into allInterval. Then we keep the
    // total as a member variable in the object.
    // Do not optimize for the first version. You need make it work first. 
    int total = 0;
    for (auto it = allIntervals.begin(); it != allIntervals.end(); ++it) {
        total += it->second - it->first;
    }
    return total;
}

// flatten a pre-order tree to a linked list.
// 
// the key is prev. You can always keep prev in the recursions. 
void flatten(Node * root, Node * &prev)
{
    if (root == NULL)
    {
        if (prev)
            prev->right = NULL;
        return;
    }

    Node * left = root->left;
    Node * right = root->right;

    root->left = prev;
    if (prev != NULL)
        prev->right = root;

    prev = root;
    
    flatten(left, prev);
    flatten(right, prev);
}

// Bloomberg, design problem
// You are writing an app for the chrome browser. This app stores URLs entered by users.
// At any point of time it can print a unique list of URLs in the most recent first
// order.
// e.g. the user enters the following URLSs -
// 1. www.google.com <== first input
// 2. www.wsj.com
// 3. www.apple.com
// 4. www.google.com <== last input
// The app will print the following -
// 1. www.google.com
// 2. www.apple.com
// 3. www.wsj.com
// Note: you need remove duplicates
class UrlList
{
public:
    UrlList() : index(numeric_limits<int>::max()) {};
    void insert(const string & url);
    void print();
private:
    unordered_map<string, int> uniqueUrls; // int is the index;
    map<int, string> urls; 
    int index;
};

// Use an index for each url. The index can start from a max number, then decrement for each new one. 
// The index can be used to sort the urls in a map. 
// I will also change the hash table to a hash map which maps url to index. 
void UrlList::insert(const string & url)
{
    unordered_map<string, int>::iterator it = uniqueUrls.find(url);
    if (it == uniqueUrls.end())
    {
        uniqueUrls[url] = index;
        urls[index] = url;
        --index;
    }
    else
    {
        urls.erase((*it).second);
        urls[index--] = url;
    }
}

void UrlList::print()
{
    map<int, string>::const_iterator cit;
    for (cit = urls.begin(); cit != urls.end(); ++cit)
    {
        cout<<(*cit).second<<" ";
    }
    cout<<endl;
}

// Note: remove() and erase() are different in STL containers.
// Remove() removes all the elements with the same value.
// Erase() erases only an item pointed to by an iterator or a key.
// For map, you can erase
// by key. And there is no remove() call in map because the items are unique
// in map. 

/* Bloomberg IQ test
you have a 64 bits double with a 53 bits mantissa. How many DECIMAL digits can you represent?

mantissa=0x8 exponent = 0x2

number=0x8*2^2

mantissa = 2^53-1 

1. Loop mantisa = mantissa / 10 until mantisa == 0. This one is OK.
2. Use a calculator logx+1 with base 10. This one is preferred.
*/

// Explain how do you use conditional variable
// 1. mutex. 2. auto release mutex 3. while loop to check if the condition
// is still valid. Write a sample code here.
/* The following is from Oracle doc.
Because the condition can change before an awakened thread returns from pthread_cond_wait(), the condition that caused the wait must be retested before the mutex lock is acquired. The recommended test method is to write the condition check as a while() loop that calls pthread_cond_wait().
No specific order of acquisition is guaranteed when more than one thread blocks on the condition variable.
*/
/*
void thread_lock()
{
    pthread_mutex_lock();
    while(condition_is_false)
        pthread_cond_wait();
    pthread_mutex_unlock();
}
*/

// From cloudera
// You have 1M ip addresses in a file. For each ip address, check if it is
// reachable and print "alive" or "dead". 
//
/*
  This looks like an open ended question. So you need discuss with the
  interviewer to clarify the details.
  First I told him I can use ping to check if it is reachable.
  Then I said the main problem must be 1M ip addresses. He agreed.
  He said ping is not important here. But 1M is.
  So according to that, I designed one thread pool and queue to send ping
  for each IP address. Assume that the queue contains 1M IP addresses.
  I designed the second thread pool and a second task queue to print
  alive or dead. Need mutex and cond var in the impl.

  So you need understand the interview's mind. You only need to write the
  important pieces and explain what is left need to be done.
  In this case, the important pieces are the thread pool and its impl.

  You don't have to write perfect code here. Just need the main idea.
  You may use psydo code sometime. Write the first version first. 
 */

/* No need to be perfect! comment out the code because it can not compile
   But you need comment out and read it. The main thing is the conditional
   variables, the queue and the mutex

struct Result
{
    // add a constructor for easy coding
    Result(int ip1, bool isAlive) : ip(ip1), alive(isAlive) {}
    unsigned int ip;
    bool alive;
};
    
class CheckIP
{
public:
    // Function to send ping 
    void sendPing();
    void printIP();
private:
    // will initialize them later. 
    pthread_mutex_t mutex1;
    deque<unsigned int> ipQ;
    pthread_mutex_t mutex2;
    pthread_cond_t cv;
    deque<Result> resultQ;
    bool done;
    bool printDone;
};

void CheckIP::sendPing()
{
    while(!done) // for thread
    {
        pthread_mutex_lock(&mutex1);
        if (!ipQ.empty())
        {
            unsigned int ip = ipQ.front();
            ipQ.pop_front();
        }
        else
        {
            done=true;
            pthread_mutex_unlock(&mutex1);
            break;
        }
        pthread_mutex_unlock(&mutex1);
        
        bool isAlive = ping(ip);

        pthread_mutex_lock(&mutex2);
        resultQ.push_back(Result(ip, isAlive));
        pthread_mutex_unlock(&mutex2);
        
        pthread_cond_signal(&cond);
    }
}

void CheckIP::printIP()
{
    while(!printDone) // for thread
    {
        Result res;
        pthread_mutex_lock(&mutex2);
        while (pthread_cond_wait(&cv, &mutex2))
        {
            if (!resultQ.empty())
            {
                res = resultQ.front();
                resultQ.pop_front();
                break;
            }
        }
        pthread_mutex_unlock(&mutex2);

        // This should also be put in mutex because all the threads are
        // printing. It will cause a mess. 
        cout<<res.ip<<" "<<res.alive() <<endl;
    }
}
*/

int main()
{

//    vector<char*> wordList ={"apple", "epple", "appla", "ell"};
    /*
    vector<const char*> wordList;
    wordList.push_back("apple");
    wordList.push_back("epple");
    wordList.push_back("appla");
    wordList.push_back("ell");
    
    const char * input = "apple";
    compareWord(wordList, input);
    */
    vector<int> myarray = {2, 20, 25, 3, 8, 1, -19, 6, 10};
//    myquicksort(myarray, 0, myarray.size()-1);
    insertionSort(myarray);
//    selectionSort(myarray);
//    bubbleSort(myarray);
    for (int i=0;i<myarray.size();++i)
        cout<<myarray[i]<<' ';
    cout<<endl;

    vector<string> asList = {"the", "quick", "brown", "fox", "quick", "the"};
    WordDistanceFinder finder (asList);
    assert(finder.distance("fox","the") == 2);
    assert(finder.distance("quick", "fox") == 1);

    vector<int> a = {9, 8, 10, 7};
    findAllTriangles(a);

    vector<int> a1 = {0, -1, 3, 100, -70, -50};
    highestPositiveProduct(a1);

    // fill zeros
    int m=4, n=5;

    // can not use matrix[4][5]. It can not be converted to int **.
    int ** matrix = (int**)malloc(m*sizeof(int *));
    for (int i=0; i<m; ++i)
    {
        matrix[i] = (int *)malloc(n*sizeof(int));

        // cannot use memset. Use memset for 0, not 1. If you set all memory
        // to 1, each item will be a negative number, not 1. 
        for (int j=0; j<n; ++j)
            matrix[i][j]=1; 
    }

    for (int i=0;i<m;++i)
    {
        for (int j=0; j<n; ++j)
            cout<<matrix[i][j]<<" ";
        cout<<endl;
    }
    cout<<endl;
        
    Point plist[2];
    plist[0]=Point(2,2);
    plist[1]=Point(1,1);
    fillZero(matrix, m, n, plist, 2);

    for (int i=0;i<m;++i)
    {
        for (int j=0; j<n; ++j)
            cout<<matrix[i][j]<<" ";
        cout<<endl;
    }
    cout<<endl;

    for (int i=0; i<m; ++i)
    {
        free(matrix[i]);
    }
    free(matrix);

    vector<int> input = {3, 1, 4, 2};
    vector<int> output;
    selfExcludingProduct(input, output);
    for (int i=0; i<output.size();++i)
        cout << output[i] << " ";
    cout<<endl;
    cout<<endl;
    
    return 0;
}

//
//Find second largest in BST. Bloomberg interview
/*
Node * findRightMostChild(Node *root, Node * & parent)
{
    Node * current = root;
    if (current == NULL)
        return NULL;
    
    if (current->right)
        current = current->right;
    return current;
}

int findSecondLargest(Node * root)
{
    if (root == NULL)
        return -1;
    
    Node * current = findRightMostChild(root);
    
    Node * secondNode = findRightMostChild(current->left);
    if (secondNode)
        return secondNode->val;
    else {
        // Need find the parent of current. 
    }
}
*/
/*
Question:
Given a set of words, find out the anagram words. What is anagram: Any word or phrase that exactly reproduces the letters in another order is an anagram.
Input: list of strings
output: list of string set

Example:
Input: tea, eat,ate, dog, but, tub
Output: {tea,eat,ate}, {but,tub}
*/
void findAnagrams(vector<string>&input, vector<vector<string>>&output)
{
    unordered_map<string, vector<string>> group;
    for (int i=0; i<input.size(); ++i)
    {
        string key = input[i];
        sort(key.begin(), key.end());
        group[key].push_back(input[i]);
    }
    
    vector<string> result;
    
}
