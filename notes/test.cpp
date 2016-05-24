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
#include <queue>
#include <unordered_map>
#include <iostream> // For std::cout, etc. 

#include "test.h"

using namespace std;

/*
Singleton * Singleton::pInstance=NULL;
Singleton * Singleton::getInstance()
{
    if (pInstance == NULL)
    {
        pthread_mutex_lock(mutex);
        if (pInstance == NULL)
        {
            pInstance = new Singleton();
        }
        pthread_mutex_unlock(mutex);
    }
    return pInstance;
}
*/

bool isLittleEndian()
{
    int x = 0xaabbccdd;

    unsigned char * p = (unsigned char *)&x;
    return *p == 0xdd;
}

int findFirstDupe(int * myarray, int len)
{
    if (myarray==NULL)
        return -1;

    std::unordered_set<int> helper;
    for (int i=0; i<len; ++i)
    {
        if (helper.find(myarray[i]) != helper.end())
            return myarray[i];
        helper.insert(myarray[i]);
    }
    return -1;
}

// i: 0123456
// v: 1223456

int findDupe(int * array, int len)
{
    if (!array)
        return -1;
    if (len<2)
        return -1;
    
    int begin = 0;
    int end = len - 1;

    while (begin < end)
    {
        int mid = begin + (end-begin)/2;
        if ((mid >0 &&array[mid]==array[mid-1])
            || (mid<len-1 && array[mid] == array[mid+1]))
            return array[mid];
        if (array[mid]>mid)
        {
            begin = mid+1;
        }
        else
            end = mid-1;
    }
    return -1;
}

bool findLoop(SingleNode * head)
{
    SingleNode * p1 = head;
    SingleNode * p2 = head;

    while (p1 != NULL && p1->next != NULL && p2!=NULL)
    {
        if (p1!=p2 && p1->next == p2->next)
            return true;

        p1=p1->next->next;
        p2=p2->next;
        
        if (p1 == p2)
            return true;
    }
    return false;
}

void reverseStr(char * str)
{
    if (!str)
        return;
    
    char * begin = str;
    char * end = str + strlen(str)-1;

    while (begin < end)
    {
        char tmp = *begin;
        *begin=*end;
        *end = tmp;
        ++begin;
        --end;
    }
}

void reverseWords(char * str)
{
    if (!str)
        return;

    char * begin = str;
    char * end = str;

    while (*begin)
    {
        while (!isalpha(*begin))
        {
            ++begin;
            if (*begin == '\0')
                return;
        }
        
        end = begin;
        while (*end && isalpha(*end))
            ++end;

        char tmp = *end;
        *end = '\0';
        reverseStr(begin);
        *end = tmp;
        begin = end;
    }
}

bool getNodePath(Node * head, Node * node, vector<Node *> & path)
{
    if (head==NULL || node == NULL)
        return false;

    path.push_back(head);

    if (head == node)
        return true;

    if (getNodePath(head->left, node, path))
        return true;
    if (getNodePath(head->right, node, path))
        return true;
    
    path.pop_back();
    return false;
}

bool getNodePath(TreeNode *head, TreeNode * node, std::list<TreeNode *> &path)
{
    if (head == NULL || node == NULL)
        return false;

    path.push_back(head);

    if (head == node)
        return true;

    std::vector<TreeNode *>::iterator cit=head->children_.begin();
    for (; cit!=head->children_.end(); ++cit)
    {
        if (getNodePath(*cit, node, path))
            return true;
    }

    path.pop_back();
    return false;
}

int myatoi(char * str)
{
    assert(str);
    int value = 0;

    bool isPositive = true;
    while (*str && isspace(*str))
        ++str;

    if (*str == '+') ++str;

    if (*str == '-')
    {
        isPositive = false;
        ++str;
    }

    while (*str && isdigit(*str))
    {
        value = 10*value + *str - '0';
        ++str;
    }

    return isPositive?value:-value;
}

void myitoa(int x, char * str)
{
    if (str == NULL) return;
    bool isNegative = false;

    if (x<0)
    {
        x=-x;
        isNegative = true;
    }

    char * ptr=str;
    do
    {
        *str++ = x%10+'0';
        x = x/10;
    }while (x);

    if (isNegative)
        *str++ = '-';

    *str=0;
    reverseStr(ptr);
}

// How do I declare an array of N pointers to functions returning pointers to 
//  functions returning pointers to characters?
typedef char * F1();
typedef F1* F2();
F2* a1[10];

char * (* (*a[10])())();

// reverse a linked list
DoubleNode * reverseList(DoubleNode * head)
{
    while (head)
    {
        DoubleNode * tmp = head->prev;
        head->prev = head->next;
        head->next = tmp;

        if (head->prev == NULL)
            return head;
        head = head->prev;
    }

    return NULL;
}

char * f(char *s)
{
   int a,b,c,e=3,f;
   for ( a=0, e<<=1; s[a]; a=f+1, e=6 ) {
      e = (e+26) & 0x0020;
      for ( b=a; s[b] && s[b] !=  e; f=++b );
      for ( c=a, b=b-1; b>c; --b, ++c) { s[b] ^= s[c]; s[c] ^=s[b]; s[b] ^= s[c]; }
   }
   return s;
}

bool my_compare(int i, int j) {return i<j;}

void sortVector(std::vector<int> &ivec, int len=100)
{
    if (ivec.size()<100)
        return;
    
    sort(ivec.begin(), ivec.begin()+len, my_compare);
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

char * my_strstr(char * str, const char * sub)
{
    if (str == NULL || sub == NULL)
        return NULL;

    while (*str)
    {
        char * strptr = str;
        const char * subptr = sub;

        while (*strptr && *subptr && *strptr == *subptr)
        {
            ++strptr;
            ++subptr;
        }

        if (*subptr == 0)
            return str;
        else
            ++str;
    }
    return NULL;
}

/*
- Pascal's triangle and how to print it with right format, Facebook. 
         1        n=1
        1 1       n=2
       1 2 1      n=3
      1 3 3 1     n=4
     1 4 6 4 1    n=5

// It is hard to make a perfect pyramid because of multiple digit numbers.
// But we can make a nice slope at the left side.
*/
// Use one vector plus a variable to save space. 
void printPascal(int n)
{
    vector<int> row;
    row.reserve(2*n);
    row.push_back(1);
    int leadingSpaces = n-1;

    for (int i=0; i<n; ++i)
    {
        for (int j=0; j< leadingSpaces; ++j)
            printf(" ");
        
        int prev=0;
        for (vector<int>::iterator it=row.begin(); it != row.end(); ++it)
        {
            printf("%d ", *it);
            int tmp = *it;
            *it = prev + *it;
            prev=tmp;
        }

        printf("\n");
        --leadingSpaces;
        row.push_back(1);
    }
}

#define TARGET_SUM 22

bool btreeSum(Node * root, int sum)
{
    if (root == NULL)
        return false;

    sum += root->value;
    if (root->left == NULL && root->right == NULL)
        return sum==TARGET_SUM?true:false;

    if (btreeSum(root->left, sum))
        return true;
    if (btreeSum(root->right, sum))
        return true;

    return false;
}

/*
// You are given a binary tree. The diameter of the tree is defined to be the longest distance between any two nodes.
// Write a function. The input is a binary tree. The output is the diameter (integer).


        root
        /  \        
      lc1   rc1
            /    
          lc2
            
3: lc1 to lc2
*/
// post order, get the longest depth of each subtree of the node.

int getDiameter(Node * root, int * height)
{
    if (root == NULL)
    {
        *height = 0;
        return 0;
    }

    int lh = 0;
    int rh = 0;

    int ld = getDiameter(root->left, &lh);
    int rd = getDiameter(root->right, &rh);
    *height = 1+lh>rh?lh:rh;
    int subd = ld>rd?ld:rd;

    int diam = lh + rh + 1;
    return subd>diam?subd:diam;
}

// print prime numbers less than N

SingleNode * reverseList(SingleNode * head)
{
    SingleNode * prev = NULL;

    while (head)
    {
        // keep head->next because it will be re-assigned to prev.
        SingleNode * tmp = head->next;
        head->next = prev;
        prev = head;
        head = tmp;
    }

    return prev;
}

// in order, Keep updating prev pointer and head.
// Set prev=NULL before calling the function.
// After calling, traverse backward from head to the beginning node. 
void treeToList(Node * head, Node * &prev)
{
    if (head==NULL)
        return;

    treeToList(head->left, prev);
    head->left = prev;
    if (prev)
        prev->right = head;
    prev = head;
    treeToList(head->right, prev);
}

/*
All nodes along children pointers from root to leaf nodes form a path in a binary tree. Given a binary tree and a number, please print out all of paths where the sum of all nodes value is same as the given number.
*/
#define TARGET_SUM 22
void printPath(Node * root, int sum, vector<int> &path)
{
    if (root==NULL)
        return;

    sum += root->value;
    path.push_back(root->value);
    
    if (root->left==NULL && root->right==NULL && sum == TARGET_SUM)
    {
        // print path
    }

    printPath(root->left, sum, path);
    printPath(root->right, sum, path);

    // No need to deduct root->value from sum because it is passed by value
    // path is a reference, need remove this node before return.
    path.pop_back();
}

// Determine whether an input array is a post-order traversal sequence of a binary tree or not. If it is, return true; otherwise return false. Assume all numbers in an input array are unique.
bool isPostOrder(int * array, int len)
{
    // Don't forget len<=0
    if (!array || len<=0) return false;

    // The last one is the root value. 
    int rootValue = array[len-1];

    // Nodes in left sub tree are less than root node. 
    int i;
    for (i=0; i<len-1; ++i)
    {
        if (array[i]==rootValue)
            return false;
        if (array[i]>rootValue)
            break;
    }

    // Nodes in right sub tree are greater than root node. 
    for (int j=i; j<len-1; ++j)
    {
        if (array[j] <= rootValue)
            return false;
    }

    // A sub tree might not exist. This will be handled by len<=0
    return isPostOrder(array, i) && isPostOrder(array+i, len-i);
}

void btreePreOrder_iterative(Node * root)
{
    if (!root)
        return;
    
    stack<Node *> tasks;
    Node * current = root;

    tasks.push(current);
    while(!tasks.empty())
    {
        current=tasks.top();
        tasks.pop();
        printf("%d, ", current->value);

        if (current->right)
            tasks.push(current->right);
        if (current->left)
            tasks.push(current->left);
    }
}

void btreePostOrder_iterative(Node * root)
{
    if (!root)
        return;
    stack<Node *> tasks;
    tasks.push(root);
    Node * prev = NULL;

    while (!tasks.empty())
    {
        Node * current = tasks.top();
        // going down
        if (prev == NULL || prev->left == current || prev->right==current)
        {
            if (current->left)
            {
                tasks.push(current->left);
            }
            else if (current->right)
            {
                tasks.push(current->right);
            }
            else
            {
                printf("%d, ", current->value);
                tasks.pop();
            }
        }
        else if (current->left==prev)
        {
            if (current->right)
            {
                tasks.push(current->right);
            }
            else
            {
                printf("%d, ", current->value);
                tasks.pop();
            }
        }
        else if (current->right==prev)
        {
            printf("%d, ", current->value);
            tasks.pop();
        }
        prev=current;
    }
}

void btree_inorder_traversal_iterative2(Node * root)
{
    stack<Node *> tasks;
    Node * current = root;
    bool done = false;

    while (!done)
    {
        if (current)
        {
            tasks.push(current);
            current=current->left;
        }
        else
        {
            if (tasks.empty())
                done=true;
            else
            {
                current = tasks.top();
                tasks.pop();
                printf("%d, ", current->value);
                current = current->right;
            }
        }
    }
}

/*
- Given an integer 'N', return count of all binary strings of length 'N',
without consecutive 1's.

Ex. if N=2 , return 3 (since we have 00, 10, 01)
Constraints: O(N) time and O(N) space

Note: O(N) can be 2N or 3N, the constant can be ignored. But you can not
      use embedded loops, which will be O(N^2).
*/
int countStrings(int n)
{
    int numZero=1;
    int numOne=1;
    int count = 2;

    for (int i=2; i<=n; ++i)
    {
        int tmp = numZero;
        numZero = numZero+numOne;
        numOne = numZero;
        count *= numZero + numOne;
    }

    return count;
}

void btree_inorder_iterative(Node * root)
{
    stack<Node *> tasks;
    Node * current = root;

    bool done=false;
    if (!done)
    {
        if (current)
        {
            tasks.push(current);
            current = current->left;
        }
        else
        {
            if (tasks.empty())
                done=true;
            else
            {
                current=tasks.top();
                tasks.pop();

                printf("%d, ", current->value);
                current=current->right;
            }
        }
    }
}

/*
  - Given an array 'A' of 'N' elements (all integers), compute an array 'B'
  such that B[i] = prod(A[j]), j = 0,...,N-1 & j!=i, i= 0,..,N-1
  without using division

  Constraints: O(N) time and O(N) space
*/
int * computeArray(int * A, int len)
{
    int * B = new int[len];

    // store the second half in B
    B[len-1]=1;
    for (int i=len-2;i>=0;--i)
    {
        B[i]=A[i+1]*B[i+1];
    }

    int firstHalf=1;
    for (int i=0; i<len; ++i)
    {
        B[i]= B[i]*firstHalf;
        firstHalf*=A[i];
    }
    return B;
}

/*
  Given two sorted integer arrays A and B, merge B into A as one sorted array.

Note
You may assume that A has enough space (size that is greater or equal to m + n) to hold additional elements from B. The number of elements initialized in A and B are mand n respectively.

Example
A = [1, 2, 3, empty, empty] B = [4,5]

After merge, A will be filled as [1,2,3,4,5]
 */
// Need merge from back to front due to cost of moving elements in A.
// It saves a lot of memory copy.
void mergeSortedArray(int A[], int m, int B[], int n) {
    // write your code here
    int last = m + n -1;
    int alast = m - 1;
    int blast = n - 1;

    while (alast >= 0 && blast >= 0) {
        if (A[alast] > B[blast])
            A[last--] = A[alast--];
        else
            A[last--] = B[blast--];
    }

    if (alast < 0) {
        while (blast >= 0)
            A[last--] = B[blast--];
    } else {
        while (alast >= 0)
            A[last--] = A[alast--];
    }
}

/* Single Number II
  Given 3*n + 1 numbers, every numbers occurs triple times except one, find it.
Example
Given [1,1,2,3,3,3,2,2,4,1] return 4
*/
// Note: For each bit in a specific number, it will appear 3 times.
//       Use cancellation approach. Add all from a bit and mod 3. Then
//       set the bit in result.
// Note: You can always use hash table if there is no space restriction.
//       Just use a hash map to store the number of appearance for each number.
//       Remove it from hash map if it reaches 3. 
int singleNumberII(vector<int> &A) {
    // write your code here
    int result=0;
        
    for (int i=0; i<32; ++i) {
        int bit = 0;
        for (int j=0; j<A.size(); ++j) {
            bit += (A[j]>>i) &1;
        }
        bit %= 3; // May move this into the for j loop to avoid number overflow.
        result |= bit << i; 
    }
    return result;
}

/* Single Number III
  Given 2*n + 2 numbers, every numbers occurs twice except two, find them.

Example
Given [1,2,2,3,4,4,5,3] return 1 and 5
 */
vector<int> singleNumberIII(vector<int> &A) {
    // write your code here
    int n = 0;
    for (int i=0; i<A.size(); ++i)
        n ^= A[i];
        
    // Set the first non-zero bit with a short cut. May use a loop and
    // shift operator instead.
    // This bit differentiates the two numbers. Because the two numbers
    // must be different, there must be one bit set after XOR operation.
    // If they are the same, XOR will yield 0. 
    n = n & (-n);
        
    vector<int> result(2, 0);
    for (int i=0; i<A.size(); ++i) {
        // Must use parentheses becaues & does not precede operator ==. 
        if ((n & A[i]) == 0)
            result[0] ^= A[i];
        else
            result[1] ^= A[i];
    }
    return result;
}

/* Majority Number
  Given an array of integers, the majority number is the number that occurs more than half of the size of the array. Find it.

Example
For [1, 1, 1, 1, 2, 2, 2], return 1

Challenge
O(n) time and O(1) space
 */
// Note: use cancellation approach as in "single number" puzzles.
// For O(N) and O(1) space, always think about cancellation.
// The key is that majority number happens more than half times. 
int majorityNumber(vector<int> nums) {
    // Need two variables. So the space complexity is O(C). 
    int majority = 0;
    int count = 0;
        
    for (int i=0; i<nums.size(); ++i) {
        if (count == 0) {
            majority = nums[i];
            ++count;
        } else if (majority == nums[i]) {
            ++count;
        } else {
            // cancel if it is different. 
            --count;
        }
    }
    return majority;
}

/* Majority Number II
  Given an array of integers, the majority number is the number that occurs more than 1/3 of the size of the array.

Find it.

Note
There is only one majority number in the array

Example
For [1, 2, 1, 2, 1, 3, 3] return 1

Challenge
O(n) time and O(1) space
 */
// Note: Use cancellation approach, but cancel three items each time.
//       Need keep two numbers because of that. 
int majorityNumber2(vector<int> nums) {
    int num1=0, num2=0, count1=0, count2=0;
    for (int i=0; i<nums.size(); ++i) {
        if (count1==0) {
            num1 = nums[i];
            ++count1;
        } else if (count2 == 0 && num1 != nums[i]) {
            // Must make sure num1 != nums[i] here to avoid dup. 
            num2 = nums[i];
            ++count2;
        } else if (num1 == nums[i]) {
            ++count1;
        } else if (num2 == nums[i]) {
            ++count2;
        } else {
            --count1;
            --count2;
        }
    }

    // Must go through the array again to compare count1 and count2.
    // You can not compare them now. The count may be misleading because
    // the majority number may have been cancelled with other numbers and show
    // a small count here. 
    count1 = 0;
    count2 = 0;
    for (int i=0; i<nums.size(); ++i) {
        if (nums[i] == num1)
            ++count1;
        else if (nums[i] == num2)
            ++count2;
    }
        
    return count1>count2?num1:num2;
}

/* Majority Number III
  Given an array of integers and a number k, the majority number is the number that occurs more than 1/k of the size of the array. Find it.

Note
There is only one majority number in the array.

Example
For [3,1,2,3,2,3,3,4,4,4] and k = 3, return 3

Challenge
O(n) time and O(k) extra space
 */
// Use cancellation approach. But add k-1 numbers to a hash map to keep
// the number and count. 
int majorityNumber(vector<int> nums, int k) {
    unordered_map<int, int> candidates;
        
    for (int i=0; i<nums.size(); ++i) {
        // Always use it to keep the result for a map because it will be used
        // later. There might be exceptions. 
        auto it = candidates.find(nums[i]);
        if (it != candidates.end()) {
            ++it->second; // increment the count
            continue;
        }
            
        if (candidates.size() == k-1) {
            // reduce one count for all numbers in hash map.
            // Use while loop instead of for because iterator will be
            // invalidated after deletion. Need special treatment. 
            it = candidates.begin();
            while (it != candidates.end()) {
                --it->second;
                if (it->second == 0) { // Need delete the item from map. 
                    int key = it->first;
                    ++it;
                    candidates.erase(key);
                }
                else
                    ++it;
            }
        } else {
            // Add to hash map. 
            candidates[nums[i]] = 1;
        }
    }

    // We still need loop the array and find the number with max occurences. 
    for (auto it = candidates.begin(); it != candidates.end(); ++it) {
        it->second=0; // reset count to 0.
    }
        
    int maxCount=0, maxNum=0; // Find number with maxCount
    for (int i=0; i<nums.size(); ++i) {
        auto it = candidates.find(nums[i]);
        if (it != candidates.end()) {
            ++it->second;
            if (it->second > maxCount) {
                maxCount = it->second;
                maxNum = nums[i];
            }
        }
    }
    return maxNum;
}

/* Best Time to Buy and Sell Stock
  Say you have an array for which the ith element is the price of a given stock on day i.

If you were only permitted to complete at most one transaction (ie, buy one and sell one share of the stock), design an algorithm to find the maximum profit.

Example
Given an example [3,2,3,1,2], return 1
 */
// Note: When there are two changing numbers, choose one to keep with the for
//       loop. In this case, we have to choose the selling price and keep a
//       running min buying price.
int maxProfit(vector<int> &prices) {
    // write your code here
    int minPrice = numeric_limits<int>::max();
    int maxValue = 0;
    for (int i=0; i<prices.size(); ++i) {
        if (prices[i] < minPrice)
            minPrice = prices[i];
            
        int profit = prices[i] - minPrice;
        maxValue = max(maxValue, profit);
    }
    return maxValue;
}

/* Best Time to Buy and Sell Stock II
  Say you have an array for which the ith element is the price of a given stock on day i.

Design an algorithm to find the maximum profit. You may complete as many transactions as you like (ie, buy one and sell one share of the stock multiple times). However, you may not engage in multiple transactions at the same time (ie, you must sell the stock before you buy again).

Example
Given an example [2,1,2,0,1], return 2
 */
// Note: Add profit for every rise. But have to reset low price after a sell
//       because you can not do multiple transactions.
int maxProfit2(vector<int> &prices) {
    // write your code here
    int low = numeric_limits<int>::max(); 
    int profits = 0;
        
    for (int i=0; i<prices.size(); ++i) {
        if (prices[i] < low) 
            low = prices[i];
        if (prices[i] > low) {
            // Do not forget to reset low. 
            profits += prices[i]-low;
            low = prices[i];
        }
    }
    return profits;
}

/* Best Time to Buy and Sell Stock III
  Say you have an array for which the ith element is the price of a given stock on day i.

Design an algorithm to find the maximum profit. You may complete at most two transactions.

Note
You may not engage in multiple transactions at the same time (ie, you must sell the stock before you buy again).

Example
Given an example [4,4,6,1,1,4,2,5], return 6
 */
// Note: Use forward-backward traversal method. This is very useful.
//       Add the max profit till the point in a separate forward array.
//       Then traverse from back and add two profits to find the max.
//       This is similar to DP except there are two forward traversals in DP. 
int maxProfit3(vector<int> &prices) {
    // write your code here
    vector<int> forward(prices.size(), 0);

    int low = numeric_limits<int>::max();
    int maxP = 0;
    // forward traversal. 
    for (int i=0; i<prices.size(); ++i) {
        if (prices[i] < low)
            low = prices[i];
            
        maxP = max(maxP, prices[i]-low);
        forward[i]=maxP;
    }

    // backward traversal. 
    int high = numeric_limits<int>::min();
    for (int i=prices.size()-1; i>0; --i) {
        if (prices[i] > high)
            high = prices[i];

        // Add two profits and compare with max. 
        maxP = max(maxP, forward[i-1] + high - prices[i]);
    }
    
    return maxP;
}

/* Maximum Subarray
   Given an array of integers, find a contiguous subarray which has the largest sum.
Note
The subarray should contain at least one number
Example
For example, given the array [−2,2,−3,4,−1,2,1,−5,3], the contiguous subarray [4,−1,2,1] has the largest sum = 6.
*/
int maxSubArray(vector<int> nums) {
    int sum = 0;

    // sum could be negative or very small. So set to min(). 
    int maxSum = numeric_limits<int>::min();
    for (int i=0; i<nums.size(); ++i) {
        sum += nums[i];
        if (sum > maxSum)
            maxSum = sum;
        
        // reset to 0 because the next sum will be larger without a
        // negative sum. 
        if (sum < 0)
            sum = 0;
    }
    return maxSum;
}

// Lint code
void DFSsearchPaths(unordered_map<string, vector<string>> & reverseWordMap, vector<string> & path, const string & start, const string & end, vector<vector<string>>&result) {
    if (start == end) {
        // 从后往前加入result，等于是reverse ladder string了。
        result.push_back(vector<string>());
        for (int i = path.size()-1; i >= 0; --i) {
            result.back().push_back(path[i]);
        }
            
        /* 不需要reverse了
        // reverse path.
        for (int first=0, last=path.size()-1; first<last; ++first, --last) {
        swap(path[first], path[last]);
        }
        result.push_back(path);
            
        // Must reverse back because path will be used later. 
        for (int first=0, last=path.size()-1; first<last; ++first, --last) {
        swap(path[first], path[last]);
        }
        */
        return;
    }
        
    // 这个也可以减去很多时间
    if (reverseWordMap.find(start) == reverseWordMap.end())
        return;

    for (auto & word : reverseWordMap[start]) {
        // push_back和pop_back都在loop里call，避免复杂情况。如果在外面，会有麻烦。例如，这个for loop根本不被call，hash table里是空的。我们还必须在call 这个function前把start/word push_back().
        path.push_back(word);
        DFSsearchPaths(reverseWordMap, path, word, end, result);
        path.pop_back();
    }
}
    
int BFSladderLength(string start, string end, unordered_set<string> &dict, unordered_map<string, vector<string>> & reverseWordMap) {
    // write your code here
    // Use BFS for shortest search. Use a queue.
    queue<string> tasks;
    tasks.push(start);
    int ladderLen = 0;
        
    unordered_set<string> prevWords;
    // This is important to avoid searching the duplicate words
    // But it is OK to have duplicate words in the same level for this question.
    prevWords.insert(start);

    while (!tasks.empty()) {
        ++ladderLen;
            
        unordered_set<string> wordsInOneLevel; // no duplicates in one set
        // Every time loop through the size of the tasks. 
        int len = tasks.size();
        for (int i=0; i<len; ++i ){
            string & word = tasks.front();
            if (word == end)
                return ladderLen;
                
            for (int j=0; j<word.size(); ++j) {
                for (int k='a'; k<='z'; ++k) {
                    if (word[j] == k)
                        continue;
                            
                    string newWord = word;
                    newWord[j] = k;
                    if (dict.find(newWord) == dict.end() || prevWords.find(newWord) != prevWords.end())
                        continue;
                            
                    // Do not add to tasks due to duplicate words
                    // tasks.push_back(newWord);
                    wordsInOneLevel.insert(newWord);
                        
                    // You may get duplicate new words here. But the word is always different when new words are duplicate. 
                    reverseWordMap[newWord].push_back(word);
                        
                    // Do not add to prevWords now so that we allow dupicate strings to be added to reverseWordMap in the current level
                    // There can be different mappings in reverseWordMap for the same new words!
                    //prevWords.insert(newWord);
                }
            }
            tasks.pop();
        }
            
        // Use auto to save time. Insert into prevWords to avoid duplicates
        for (auto it = wordsInOneLevel.begin(); it != wordsInOneLevel.end(); ++it) {
            tasks.push(*it);
            prevWords.insert(*it);
        }
        wordsInOneLevel.clear();
    }
    return 0;
}

vector<vector<string>> findLadders(string start, string end, unordered_set<string> &dict) {
    // write your code here
    // Use BFS to find the length shortest path and store the reverse paths in a map. 
    unordered_map<string, vector<string>> reverseWordMap;
    int len = BFSladderLength(start, end, dict, reverseWordMap);
        
    vector<vector<string>> result;
    if (len==0)
        return result;
            
    // Search reverseWordMap and get all the paths. 
    vector<string> path;
    // The start and end are reversed because the map is from end to start. 
    path.push_back(end);
    DFSsearchPaths(reverseWordMap, path, end, start, result);
    return result;
}

string simplifyPath(string& path) {
    string result;
    if (path.empty())
        return result;
    
    stack<char> tasks;
    tasks.push(path[0]);
    for (int i = 1; i < path.size(); ++i) {
        if (path[i] == '/') {
            if (tasks.top() != '/')
                tasks.push(path[i]);
        }
        else if (path[i] == '.' && (i+1 >= path.size() || path[i+1] != '.')) {
            if (tasks.top() == '/') {
                tasks.pop();
            }
        }
        else if (path[i] == '.' && i+1 < path.size() && path[i+1] == '.') {
            if (tasks.top() == '/')
                tasks.pop();

            // pop a directory name. 
            while (!tasks.empty() && tasks.top() != '/')
                tasks.pop();
            
            i++; // skip the next '.'
        }
        else {
            tasks.push(path[i]);
        }
    }
    
    if (tasks.top() == '/' && tasks.size() > 1) {
        tasks.pop();
    }

    result.resize(tasks.size());
    for (int i = result.size()-1; i >= 0; --i) {
        result[i] = tasks.top();
        tasks.pop();
    }
    return result;
}

class Solution {
public:

};

class LRUCache {
public:
    LRUCache(int c) : capacity(c) {}

    // Remember 3 things
    // 1. capacity 2. move to the front of cache. 3. update hashMap. 
    int get(int key) {
        auto it = hashMap[key];
        cache.splice(cache.begin(), cache, it);
        return cache.begin()->second;
    }
    
    void set(int key, int value) {
        // D&C. Check if the key already exists.
        if (hashMap.find(key) == hashMap.end()) {
            if (cache.size() == capacity) {
                hashMap.erase(cache.back().first);
                cache.pop_back();
            }
            cache.emplace_front(key, value);
            hashMap[key] = cache.begin();
        }
        else {
            auto it = hashMap[key];
            it->second = value;
            cache.splice(cache.begin(), cache, it);
        }
    }
    
private:
    int capacity;
    list<pair<int, int>> cache;
    unordered_map<int, list<pair<int, int>>::iterator> hashMap;
};

int main()
{
    Solution sol;

    /*
    string start = "qa";
    string end = "sq";
    unordered_set<string> dict = {"si","go","se","cm","so","ph","mt","db","mb","sb","kr","ln","tm","le","av","sm","ar","ci","ca","br","ti","ba","to","ra","fa","yo","ow","sn","ya","cr","po","fe","ho","ma","re","or","rn","au","ur","rh","sr","tc","lt","lo","as","fr","nb","yb","if","pb","ge","th","pm","rb","sh","co","ga","li","ha","hz","no","bi","di","hi","qa","pi","os","uh","wm","an","me","mo","na","la","st","er","sc","ne","mn","mi","am","ex","pt","io","be","fm","ta","tb","ni","mr","pa","he","lr","sq","ye"};
    vector<vector<string>> result = findLadders(start, end, dict);
    for (auto & ladder : result) {
        for (auto & word : ladder) {
            cout << word << " ";
        }
        cout << endl;
    }
    */

//    printf("power 3^5 = %d\n", power(3,5));
    printf("is Little Endian %d.\n", isLittleEndian());

    printPascal(10);

    cout <<  "\ncheck smallest integer, -2^31\n";
    // Note that 0xffffffff is -1. x negate plus 1?
    int x = 0x80000000;
    cout << "x = " << x << ", x % 11 = " << (x%11 + 11)%11 << endl;

    return 0;
}

