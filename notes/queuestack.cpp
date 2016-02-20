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

#include "queuestack.h"

using namespace std;

/* Min Stack
  Implement a stack with min() function, which will return the smallest number in the stack.

It should support push, pop and min operation all in O(1) cost.

Note
min operation will never be called if there is no number in the stack
 */
class MinStack {
public:
    MinStack() {
    }

    void push(int number) {
        s1.push(number);

        if (s2.empty()) {
            s2.push(number);
        } else {
            // use ::min to avoid calling MinStack::min(). 
            s2.push(::min(number, s2.top()));
        }
    }

    int pop() {
        // pop both stacks. 
        int ret = s1.top();
        s1.pop();
        s2.pop();
        return ret;
    }

    int min() {
        // check empty stack. 
        if (s2.empty())
            return -1; // throw exception
        else
            return s2.top();
    }
    
private:
    stack<int> s1; // stores the values. 
    stack<int> s2; // stores the min values
};

/* Implement queue by stacks
  As the title described, you should only use two stacks to implement a queue's actions.
The queue should support push(element), pop() and top() where pop is pop the first(a.k.a front) element in the queue.

Both pop and top methods should return the value of first element.
 */
// Always push to s1 and pop from s2. If s2 is empty, move everything from s1 to s2. 
class Queue {
public:
    Queue() {
    }

    void push(int element) {
        // always push to s1
        s1.push(element);
    }
    
    int pop() {
        // always get from s2. If s2 is empty, move everything from s1 to s2. 
        if (s2.empty()) {
            // Use while loop instead of if. The condition is the same
            while (!s1.empty()) {
                int value = s1.top();
                s1.pop();
                s2.push(value);
            }
        }
        
        if (s2.empty())
            return -1; // should throw exception. 
        else {
            int value = s2.top();
            s2.pop();
            return value;
        }
    }

    int top() {
        // same as pop(). 
        if (s2.empty()) {
            while (!s1.empty()) {
                int value = s1.top();
                s1.pop();
                s2.push(value);
            }
        }
        
        if (s2.empty())
            return -1; // should throw exception. 
        else {
            return s2.top();
        }
    }
private:
    stack<int> s1;
    stack<int> s2;
};

/* Largest Rectangle in Histogram
  Given n non-negative integers representing the histogram's bar height where the width of each bar is 1, find the area of largest rectangle in the histogram.
 */
// Draw picture.
// You may solve this with two loops easily. But that is O(N^2).
int largestRectangleArea(vector<int> &height) {
    // write your code here
    stack<int> s1;
    int maxArea = 0;
    for (int i = 0; i<=height.size(); ++i) {
        // Loop to size(). But set the height of the last one to -1 so
        // that we can get the area with length of size. 
        int curHeight = (i == height.size())? -1 : height[i];

        // Do not push i into stack for now because we need remove larger
        // height from the stack. The height in the stack should be
        // in increasing order.
        // Note: we also include the case when height[s1.top()]==curHeight.
        //       we need keep the latest one to get the largest area if
        //       there is duplicate height.
        // Note: Do the difficult part first. The rest is easy.
        while (!s1.empty() && height[s1.top()] >= curHeight) {
            // The height of the rectangle. 
            int h = height[s1.top()];
            s1.pop();

            // The width of the rectangle is from i to the previous lower spot. 
            int w = s1.empty()?i:(i-s1.top()-1);
            maxArea = max(maxArea, h*w);
        }

        // Don't forget this!
        s1.push(i);
    }
    return maxArea;
}

/* Max tree
  Given an integer array with no duplicates. A max tree building on this array is defined as follow:

The root is the maximum number in the array
The left subtree and right subtree are the max trees of the subarray divided by the root number.
Construct the max tree by the given array.
Example
Given [2, 5, 6, 0, 3, 1], the max tree is

              6

            /    \

         5       3

       /        /   \

     2        0     1

Challenge
O(n) time complexity
 */
/*
  Notes
  Use a stack to keep the unfinished nodes.
  Smaller nodes before current node are linked to the current node as its left
  sub tree. So, you only need to keep the largest node in the stack to
  represent the left subtree.

  Smaller nodes after current node belong to right subtree of current node.
  But they are not finished. We don't know their root (or sub root). And they
  could be a left child in a right sub tree. So we keep them in the stack.
  We will find their root when we get a larger node.

  if we find a larger node than the bottom one, then all the nodes in the stack
  become the left sub tree of the largest node. And the largest one is always
  placed at the bottom of the stack. So the bottom item represents the roor
  of a left sub tree. We use a fake largest node at the end so that its left
  subtree is our result. 
 */
TreeNode* maxTree(vector<int> A) {
    // write your code here
    stack<TreeNode *> s1;
        
    for (int i=0; i<=A.size(); ++i) {
        // Use a fake largest node at the end. 
        int curValue = i == A.size()?numeric_limits<int>::max():A[i];
            
        TreeNode * current = new TreeNode(curValue);

        // Basically, we need process and remove all the nodes that are smaller
        // than the current node. Then we push the current node to the stack.
        // The nodes in the stack are always in descending order. 
        while (!s1.empty() && curValue > s1.top()->val) {
            TreeNode * top = s1.top();
            s1.pop();

            // The top node will be added to the next (right) in the stack or
            // the current node (left), which ever is smaller. The larger
            // node will stay or be added to the stack. 
            if (!s1.empty() && curValue > s1.top()->val) {
                // Need find the head of the left sub tree of the current node.
                // It is the largest node that is smaller than current.
                // All the smaller nodes belong to the right of the head. 
                TreeNode * next = s1.top();

                // The top node is linked or finished by being linked to
                // next->right. next is still in the stack so that we can
                // process it later. 
                next->right = top;
            } else {
                // The left points to the root of the left subtree.
                // The top node is linked or finished by being linked to
                // current->left. current will be pushed into the stack so that
                // we can process it later. 
                current->left = top;        
                break;
            }
        }
        s1.push(current);
    }
        
    TreeNode * last = s1.top();
    s1.pop();
    // the result is the left sub tree of the fake largest node. 
    TreeNode * result = last->left;
    delete last;
    return result;
}

int main() {
    return 0;
}
