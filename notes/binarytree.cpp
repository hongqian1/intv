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

#include "binarytree.h"

using namespace std;

// LintCode Binary tree maximum path sum.
/*
  Given a binary tree, find the maximum path sum.

The path may start and end at any node in the tree.

Example
Given the below binary tree,

       1
      / \
     2   3
Return 6.
*/
// Use D&C
// Two tricks. 1. convert negative single sum to 0.
// 2. Need keep single sum. It is different from total sum. 
int maxSum(TreeNode * root, int & maxSingleSum);

int maxPathSum(TreeNode *root) {
    int maxSingleSum = 0;
    return maxSum(root, maxSingleSum); // return
}

int maxSum(TreeNode * root, int & maxSingleSum) {
    if (root == NULL) {
        maxSingleSum = 0;
        return numeric_limits<int>::min(); // min()
    }

    int left, right, leftSingle=0, rightSingle=0; // init to 0
    left = maxSum(root->left, leftSingle);
    right = maxSum(root->right, rightSingle);

    maxSingleSum = max(leftSingle, rightSingle);

    // reset to 0 if it is negative. Remove values that are not helpful. 
    if (maxSingleSum < 0)
        maxSingleSum = 0;

    // We need this even if val is negative. A negative val can also be a max.
    maxSingleSum += root->val; 

    return max(max(left, right), leftSingle + rightSingle + root->val);
};

/*
Given the root and two nodes in a Binary Tree. Find the lowest common ancestor(LCA) of the two nodes.

The lowest common ancestor is the node with largest depth which is the ancestor of both nodes.

Example
        4

    /     \

  3         7

          /     \

        5         6

For 3 and 5, the LCA is 4.

For 5 and 6, the LCA is 7.

For 6 and 7, the LCA is 7.
*/
// The following does not return error when a node is not found!
// It returns the other node as a result. You can add a boolean flag
// in the argument list to indicate if both child nodes are found. 
Node * lowestCommonAncestor(Node * root, Node * node1, Node * node2, bool & both)
{
    if (root == NULL || root==node1 || root == node2)
        return root;

    Node * left, * right;
    left = lowestCommonAncestor(root->left, node1, node2, both);
    right = lowestCommonAncestor(root->right, node1, node2, both);

    if (left && right)
    {
        both = true;
        return root;
    }
    
    if (left)
        return left;
    if (right)
        return right;
    return NULL;
}

Node * findLowestCommonAncestor(Node * root, Node * node1, Node * node2)
{
    bool both = false;
    Node * common = lowestCommonAncestor(root, node1, node2, both);
    if (both && common)
        return common;
    else
        return NULL;
}

// Another solution is to find the path from the root. Then compare the paths
// from the root.

/*
Given a binary tree, determine if it is a valid binary search tree (BST).

Assume a BST is defined as follows:

The left subtree of a node contains only nodes with keys less than the node's key.
The right subtree of a node contains only nodes with keys greater than the node's key.
Both the left and right subtrees must also be binary search trees.
*/
bool isBST(TreeNode * root, TreeNode * & prev);

bool isValidBST(TreeNode *root) {
    // write your code here
    TreeNode * prev = NULL;
    return isBST(root, prev);
}

bool isBST(TreeNode * root, TreeNode * & prev){
    if (root == NULL)
        return true;

    if (!isBST(root->left, prev)) return false;

    if (prev && prev->val >= root->val) return false;
    prev = root;
    return isBST(root->right, prev);
}

// A more popular solution is to use D&C pre order traversal.
// But it can not pass the test case with value 2147483647 because
// it equals numeric max().
// Use 64 bit long long to avoid this problem.
// numeric_limits<long long>::max(). 
bool isBST2(TreeNode * root, long long minValue, long long maxValue)
{
    // always check NULL. Simple base case.
    if (root == NULL)
        return true;

    // If it is false, return right away, no need check more. 
    if (root->val <= minValue || root->val >= maxValue)
        return false;

    if (!isBST2(root->left, minValue, root->val))
        return false;
    
    return isBST2(root->right, root->val, maxValue);
}

/*
Given two values k1 and k2 (where k1 < k2) and a root pointer to a Binary Search Tree. Find all the keys of tree in range k1 to k2. i.e. print all x such that k1<=x<=k2 and x is a key of given BST. Return all the keys in ascending order.
*/
// in order traversal with conditions
void helper(TreeNode * root, int k1, int k2, vector<int> & result) {
    if (root == NULL) return;

    if (k1 < root->val)
        helper(root->left, k1, k2, result);

    if (root->val >= k1 && root->val <= k2)
        result.push_back(root->val);

    if (k2 > root->val)
        helper(root->right, k1, k2, result);
}

vector<int> searchRange(TreeNode* root, int k1, int k2) {
    // write your code here
    vector<int> result;
    helper(root, k1, k2, result);
    return result;
}

/*
  Given a binary tree, return the preorder traversal of its nodes' values.
 */
// Do not use recursive. That is too easy. 
vector<int> preorderTraversal(TreeNode *root) {
    // write your code here
    vector<int> result;
    if (!root) return result;

    stack<TreeNode *> tasks;
    tasks.push(root);
    while (!tasks.empty()) {
        TreeNode * current = tasks.top();
        tasks.pop();
        result.push_back(current->val);

        if (current->right)
            tasks.push(current->right);
        if (current->left)
            tasks.push(current->left);
    }

    return result;
}

/*
  Design an iterator over a binary search tree with the following properties:
Elements are visited in ascending order (i.e. an inorder traversal)
next() and hasNext() queries run in O(1) time in average.
 */
class BinaryTree {
public:
    // @param root: The root of binary tree.
    BinaryTree(TreeNode *root) {
        // write your code here
        current = root;
    }

    // @return: True if there has next node, or false
    bool hasNext() {
        // write your code here
        if (current || !tasks.empty())
            return true;
        else
            return false;
    }

    // @return: return next node
    TreeNode* next() {
        // write your code here
        while (current) {
            // This processes a subtree. It pushes all the tasks to a stack. 
            tasks.push(current);
            current = current->left;
        }

        if (!tasks.empty()) {
            TreeNode * result = tasks.top();
            tasks.pop();

            // This starts another sub tree with a valid current
            if (result->right)
                current = result->right; 
            return result;
        }

        return NULL;
    }

private:
    TreeNode * current;
    stack<TreeNode *> tasks;
};

// Given a binary tree, return the inorder traversal of its nodes' values.
vector<int> inorderTraversal(TreeNode *root)
{
    vector<int> result;
    // write your code here
    stack<TreeNode *> tasks;
    bool done = false;
    TreeNode * current = root;
    while (!done) {
        while (current) {
            tasks.push(current);
            current = current->left;
        }

        if (!tasks.empty()) {
            current = tasks.top();
            tasks.pop();
            result.push_back(current->val);

            if (current->right)
                current = current->right;
            else
                current = NULL;
        } else {
            done = true;
        }
    }
    return result;
}

// BFS traversal of binary tree. Each level is separated by a new line.
// May use list if not familiar with queue. 
void printTree2(Node * root)
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

/*
- You are given a binary tree. The diameter of the tree is defined to be the longest distance between any two nodes. The distance is the number of nodes. 
 Write a function. The input is a binary tree. The output is the diameter (integer).


        root
        /  \        
      lc1   rc1
            /    
          lc2
            
4: lc1 to lc2
*/
// post order, get the longest depth of each subtree of the node.
// return diameter, keep height in paraemter list.
// Do not forget to keep the value of the reference type height. 
int getDiameter(Node * root, int & height)
{
    if (root == NULL)
    {
        height = 0;
        return 0;
    }

    int lh = 0;
    int rh = 0;

    int ld = getDiameter(root->left, lh);
    int rd = getDiameter(root->right, rh);
    height = 1+max(lh, rh);

    // Need compare left diameter, right diameter and current diameter!
    return max(max(ld, rd), lh+rh+1);
}

/*
- Convert a binary tree to a doubly linked list
// Analysis: use in order traversal, Keep updating prev pointer and head.
// Set prev=NULL before calling the function.
// After calling, traverse backward from head to the beginning node.
*/
// Note: prev is a reference to pointer because it will be changed. 
// This is a solution for in order tree. Need check if the question is for
// pre-order or in order. 
void treeToList(Node * head, Node * &prev)
{
    if (head==NULL)
        return;

    treeToList(head->left, prev);

    // For single list, need use "head->left = NULL" here. 
    head->left = prev;
    if (prev)
        prev->right = head;
    
    prev = head;
    
    treeToList(head->right, prev);
}

// Here is a solution for pre-order tree. It is also called flatten binary
// tree in leed coode. Check the following link for more details.
// https://oj.leetcode.com/problems/flatten-binary-tree-to-linked-list/
void flattenNodesPreOrder(TreeNode * root, TreeNode *& prev) {
    if (root == NULL)
        return;

    // Need keep left and right pointers before they are changed. 
    TreeNode * left = root->left;
    TreeNode * right = root->right;

    // This is for single list, so set root->left = NULL.
    // For double list, root->left = prev.
    root->left = NULL; 
    if (prev)
        (prev)->right = root;
    prev = root;
        
    flattenNodesPreOrder(left, prev);
    flattenNodesPreOrder(right, prev);
}

/*
All nodes along children pointers from root to leaf nodes form a path in a binary tree. Given a binary tree and a number, please print out all of paths where the sum of all nodes value is same as the given number.
*/
// Use Preorder D&C
#define TARGET_SUM 22 // Add this as an argument of the function. 
void printPath(Node * root, int sum, vector<int> &path)
{
    if (root==NULL)
        return;

    sum += root->value;
    path.push_back(root->value);
    
    if (root->left==NULL && root->right==NULL && sum == TARGET_SUM)
    {
        // print path, or add the path to a result vector
    }

    printPath(root->left, sum, path);
    printPath(root->right, sum, path);

    // No need to deduct root->value from sum because it is passed by value
    // path is a reference, need remove this node before return.
    path.pop_back();
}

// Determine whether an input array is a post-order traversal sequence of a binary search tree or not. If it is, return true; otherwise return false. Assume all numbers in an input array are unique.

bool isPostOrder(int * array, int len)
{
    // Don't forget len<=0
    if (!array || len<=0) return false;

    // The last value is the root value. 
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

// Find Kth largest element in a BST.
// The fastest way is to do a reverse in order traversal, traversing from
// right child to left child.
// Note: count is used to count elements from backward.
//       We do not need to finish the traversal. We can stop after finding
//       the element. So need return bool to indicate if it is found.
bool findKthLargest(Node * root, int &count, int K, int & result)
{
    if (root == NULL)
        return false; // pay attention to return 

    // start from the right child because it is reversal traversal.
    // No need to traverse further if we find it.
    if (findKthLargest(root->right, count, K, result))
        return true;

    ++count;
    if (count == K)
    {
        result = root->value;
        return true;
    }

    return findKthLargest(root->left, count, K, result);
}

int findKthLargestWrapper(Node * root, int K)
{
    int count=0;
    int result=0;
    findKthLargest(root, count, K, result);
    return result;
}

/* Construct Binary Tree from Inorder and Postorder Traversal
  Given inorder and postorder traversal of a tree, construct the binary tree.

Note
You may assume that duplicates do not exist in the tree.

Example
Given inorder [1,2,3] and postorder [1,3,2]

return a tree

  2
 /  \
1    3
*/
TreeNode *buildTree(vector<int> &inorder, vector<int> &postorder) {
    if (inorder.empty()||postorder.empty()||inorder.size() != postorder.size()) {
        return NULL;
    }

    // It is easy to use pre-order to construct a tree.
    // The last one in postorder is the root of the tree (first in pre-order). 
    TreeNode * current = new TreeNode(postorder.back());
    int i=0;
    for (i=0; i<postorder.size(); ++i) {
        if (inorder[i] == postorder.back())
            break;
    }

    // i is the middle one in the inorder vector. It can also be used
    // to separate the postorder vector. 
    vector<int> leftInOrderVec(inorder.begin(), inorder.begin()+i);
    vector<int> rightInOrderVec(inorder.begin()+i+1, inorder.end());
        
    vector<int> leftPostOrderVec(postorder.begin(), postorder.begin()+i);
    vector<int> rightPostOrderVec(postorder.begin()+i, postorder.end()-1);
        
    current->left = buildTree(leftInOrderVec, leftPostOrderVec);
    current->right = buildTree(rightInOrderVec, rightPostOrderVec);

    // don't forget to return. 
    return current;
}

/* Remove Node in Binary Search Tree 
Given a root of Binary Search Tree with unique value for each node.  Remove the node with given value. If there is no such a node with given value in the binary search tree, do nothing. You should keep the tree still a binary search tree after removal.
*/
TreeNode* removeNode(TreeNode* root, int value) {
    // Need keep parent node to manipulate a node. 
    TreeNode * current = root;
    TreeNode * parent = NULL;
    while (current) {
        if (current->val == value)
            break;
        else if (value < current->val) {
            parent = current;
            current = current->left;
        }
        else {
            parent = current;
            current = current->right;
        }
    }
    // found the node and its parent
    if (current == NULL)
        return root;
    
    // For binary tree node, use D&C. Divide into left and right.
    // Don't try to divide further, which will make it more complex. 
    // Process left node first, check if it is NULL.
    // It is more complex to process the left node. Need connect its smallest
    // node (right most) to the right child. 
    // The right node is easy because it does not need to worry about the other
    // node. When you work on left node, you need think about the right one. 
    // Draw a picture to analyze diff cases.
    TreeNode * newNode = NULL; // The node to replace current node. 
    if (current->left) {
        newNode = current->left;

        TreeNode * newRight = newNode;
        while (newRight->right) {
            newRight = newRight->right;
        }
        newRight->right = current->right;
    } else {
        // if left child is null, just use the right child.
        // newNode could be NULL. And that is no problem. 
        newNode = current->right;
    }

    // assign the new node to parent or root. The deleted node could be root!
    if (parent) {
        if (parent->left == current) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
    } else { // current == root 
        root = newNode;
    }
            
    delete current; // Don't forget to delete the node. 
    return root;
}

int main()
{
    return 0;
}
