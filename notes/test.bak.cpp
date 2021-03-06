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

bool getNodePath(Node * head, Node * node, std::list<Node *> & path)
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

void removeSubStr(char * str, char * sub)
{
    if (str == NULL || sub == NULL)
        return;

    char * output = str;
    while (*str)
    {
        char * strptr = str;
        char * subptr = sub;

        while (*strptr && *subptr && *strptr == *subptr)
        {
            ++strptr;
            ++subptr;
        }

        if (*subptr==0)
        {
            str=strptr;
        }
        else
        {
            *output++=*str++;
        }
    }

    *output=0;
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

int longestPath(Node * root, int & diameter)
{
    if (root == NULL)
        return 0;

    int leftLen=longestPath(root->left, diameter);
    int rightLen=longestPath(root->right, diameter);

    if ((leftLen+rightLen+1)>diameter)
        diameter = leftLen+rightLen+1;

    return 1+leftLen>rightLen?leftLen:rightLen;
}

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
    if (root=NULL)
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
void quicksort(int * array, int start, int end)
{
    if (array == NULL || start>=end)
        return;

    int mid = partition(array, start, end);
    quicksort(array, start, mid);
    quicksort(array, mid+1, end);
}
*/
int main()
{
//    printf("power 3^5 = %d\n", power(3,5));
    printf("is Little Endian %d.\n", isLittleEndian());

    printPascal(10);
    
    return 0;
}

