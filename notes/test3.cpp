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
#include <list>

#include "test3.h"

using namespace std;

// From Nine Chapter
// No. 1 
// strstr()
/**
 * Returns a index to the first occurrence of target in source,
 * or -1  if target is not part of source.
 * @param source string to be scanned.
 * @param target string containing the sequence of characters to match.
 */
// Can also use pointer to do this. More efficient. Use pointer as sample!
int mystrstr(const string &source, const string &target) {
    // write your code here
    if (target.empty())
        return 0;
            
    int slen = source.size();
    int tlen = target.size();
        
    for (int i=0; i<slen; ++i) {
        int j;
        for (j=0; j<tlen; ++j) {
            if (source[i+j] != target[j])
                break;
        }
        if (j == tlen)
            return i;
    }
    return -1;
}

// There are two types of permutation. This is the first type. It basically
// iterate through all the possible combinations. The second type is the
// palindrome problem and Vungle problem later. 
/*
  Given a set of distinct integers, return all possible subsets.
Note: Elements in a subset must be in non-descending order.
The solution set must not contain duplicate subsets.

Example
If S = [1,2,3], a solution is:

[
  [3],
  [1],
  [2],
  [1,2,3],
  [1,3],
  [2,3],
  [1,2],
  []
]
 */
// This is a typical "permutation/combination" problem. 
// It requires to print all permutation with all lengths. So we need add
// to the result for every new element.
// Sometimes it requires permutation of equal size. In that case, we add
// result when subset.size() == a.size()
/*
  Analysis:
  Try to use tree to analyze. It is much easier. 
  
  -> while loop goes to the right 
  1 2 3 |
  1 2 3 V
  1 2 3 recursion is downward

  One loop adds each item in the same indexed position of the array.
  It creates varieties on the same index.
  One recursion adds items to the next indexed position of the array.
  It increments the array's size by one.

  The loop adds more varieties to the current position. Don't worry, all
  the varieties will be tested by the loop. It is like BFS search that
  will cover all the possibilities at the same position or level. 
  The recursion is like Depth first search that will reach a solution
  (there are many solutions) or no solution.

  This is like deep/ultimate search that searches for both breadth and depth.
  We normally search only depth or breadth. But this one searches all. It does
  not miss anything. 

  Draw the diagram on paper!!!
  After drawing the diagram, you can add restrictions or remove items in
  each loop or recursion based on the requirements. For example, you need
  increment "next" in each recursion to remove duplicates. 
 */
// We will not need "next" if the question does not care about order of
// the subset.
// Note: there is also a DP/induction solution for this. Did that in Soundhound.
//       Please check the next function subsets(). 
//       This is power set problem. 
void allSubsets(vector<int>& a, int next, vector<int> & subset, vector<vector<int>> & result)
{
    // This base case is included when next == 3. So no need add extra here.
    /*
    if (next == a.size())
        return;
    */

    // next is used for order of subset.
    // For loop is for the index of the array or string
    for (int i=next; i<a.size(); ++i)
    {
        subset.push_back(a[i]);
        result.push_back(subset);
        allSubsets(a, i+1, subset, result);
        subset.pop_back();
    }
}

void allSubsetsWrapper(vector<int> &a, vector<vector<int>> & result)
{
    // Need sort because it requires non descending order and no dup subsets.
    sort(a.begin(), a.end());
    vector<int> subset;
    result.push_back(subset);
    allSubsets(a, 0, subset, result);    
}

// This is a DP/induction solution. It is a simple induction from an empty subset.
vector<vector<int> > subsets(vector<int> &nums) {
    // write your code here
    vector<vector<int>> result;
    vector<int> subset;

    // start from an empty subset
    result.push_back(subset);
    	
    for (int i=0; i<nums.size(); ++i) {
        // Always loop through all the subsets for the next level of induction.
        // This loop is from 0 to existing size of the result.
        int size = result.size();
        for (int j=0; j<size ; ++j) {
            subset.clear();
            subset = result[j];
            subset.push_back(nums[i]);
            result.push_back(subset);
        }
    }
    return result;
}

/*
  Given a list of numbers that may have duplicate numbers, return all possible subsets

Note: Each element in a subset must be in non-descending order.
The ordering between two subsets is free.
The solution set must not contain duplicate subsets.

Example
If S = [1,2,2], a solution is:

[
  [2],
  [1],
  [1,2,2],
  [2,2],
  [1,2],
  []
  ]
*/
// Note: To remove duplicate subset, you need order the original array.
//       Then you need check duplicate in the loop because it adds items to
//       the same indexed position in the array.
// Note: Can also use DP for this. 
void combinations( vector<int> &S, vector<int>&subset, int next, vector<vector<int>>&result) {
    for (int i = next; i< S.size(); ++i) {
        // This removes duplicate. It happens on the loop instead of recursion.
        if (i != next && S[i] == S[i-1])
            continue;
                
        subset.push_back(S[i]);
        result.push_back(subset);
            
        combinations(S, subset, i+1, result);
        subset.pop_back();
    }
}
    
vector<vector<int> > subsetsWithDup( vector<int> &S) {
    // Sort is important for two reasons. 1. non-descending order. 2. remove duplicate subsets.
    sort(S.begin(), S.end());
    vector<int> subset;
    vector<vector<int>> result;
    result.push_back(subset);
    combinations(S, subset, 0, result);
    return result;
}

// New puzzle. Linkedin interview.
/*
Given a list of numbers, return all possible permutations.

Example
For nums [1,2,3], the permutaions are: 

[

    [1,2,3],

    [1,3,2],

    [2,1,3],

    [2,3,1],

    [3,1,2],

    [3,2,1]

]

Challenge
Do it without recursion
*/
/**
 * @param nums: A list of integers.
 * @return: A list of permutations.
 */
// Note: We do not need "int next" parameter as an argument of the recursion
// function because it does not require order. We basically check all the
// permutations and excluding the existing items in perm.
// Note: We use a helper array "visited" to check duplicate.
// Note: The index of the visited array is the index of nums array. We have to
// use index for two reasons. 1. It is limited. 2. There could be dup items.
// We do not need care about dup items if we use index. 
void permutations(vector<int> &nums, vector<int>&perm, vector<bool>&visited, vector<vector<int>>&result) {
    if (perm.size()==nums.size()) {
        result.push_back(perm);
        return;
    }
        
    for (int i=0; i<nums.size(); ++i) {
        if (visited[i])
            continue;
                
        perm.push_back(nums[i]);
        visited[i]=true; // set the marker. 
        permutations(nums, perm, visited, result);
        perm.pop_back();
        visited[i] = false; // reset the marker after the last recursion.
    }
}
vector<vector<int> > permute(vector<int> &nums) {
    // write your code here
    vector<int> perm;
    vector<vector<int>> result;

    // edge case checking
    if (nums.empty()) {
        //result.push_back(perm);
        return result;
    }
    vector<bool> visited(nums.size(), false);
    permutations(nums, perm, visited, result);
    return result;
}

// New puzzle
/*
Given a list of numbers with duplicate number in it. Find all unique permutations.

Example
For numbers [1,2,2] the unique permutations are:

[

    [1,2,2],

    [2,1,2],

    [2,2,1]

]

Challenge
Do it without recursion.
*/
// Note: This one is tricky because you need care about both dup in visited
//       and dup in loop. And they have a conflict. You need check
//       visited[i-1]==false as shown below.
void permutationUnique(vector<int> & nums, vector<int> &perm, vector<bool>&visited, vector<vector<int>>&result) {
    if (perm.size() == nums.size()) {
        result.push_back(perm);
        return;
    }
        
    for (int i = 0; i<nums.size(); ++i) {
        // Note: Need add "visited[i-1]==false" condition. This indicates
        // that the previous item is already added. So we do not need
        // add its duplicate nums[i] in this loop.
        // Otherwise, we can add nums[i] 
        if (i!=0 && nums[i] == nums[i-1] && visited[i-1]==false)
            continue;
               
        if (visited[i])
            continue;

        perm.push_back(nums[i]);
        visited[i] = true;
        permutationUnique(nums, perm, visited, result);
        perm.pop_back();
        visited[i] = false;
    }
}
vector<vector<int> > permuteUnique(vector<int> &nums) {
    // write your code here
    sort(nums.begin(), nums.end());
    vector<int> perm;
    vector<bool> visited(nums.size(), false);
    vector<vector<int>> result;
    if (nums.empty())
        return result;
    permutationUnique(nums, perm, visited, result);
    return result;
}
/*
  Given a set of candidate numbers (C) and a target number (T), find all unique combinations in C where the candidate numbers sums to T.

The same repeated number may be chosen from C unlimited number of times.

For example, given candidate set 2,3,6,7 and target 7, 
A solution set is: 
[7] 
[2, 2, 3] 

Note
All numbers (including target) will be positive integers.
Elements in a combination (a1, a2, … , ak) must be in non-descending order. (ie, a1 ≤ a2 ≤ … ≤ ak).
The solution set must not contain duplicate combinations.
*/
/**
 * @param candidates: A list of integers
 * @param target:An integer
 * @return: A list of lists of integers
 */
// Note: start from a sample diagram. Add conditions and restrictions
// according to the requirements. In this case, the main requirement is
// sum == 7. So stop recursion when sum >= 7.
void combinations(vector<int>& candidates, int target, vector<int>&comb, int next, int &sum, vector<vector<int>>&result) {
    for (int i=next; i<candidates.size(); ++i) {
        if (sum + candidates[i] > target) {
            continue;
        }

        sum += candidates[i];
        comb.push_back(candidates[i]);
        if (sum == target) {
            result.push_back(comb);
            sum -= candidates[i];
            comb.pop_back();
            continue;
        }
            
        combinations(candidates, target, comb, i, sum, result);
        sum -= comb[comb.size()-1];
        comb.pop_back();
    }
}
    
vector<vector<int> > combinationSum(vector<int> &candidates, int target) {
    // write your code here
    sort(candidates.begin(), candidates.end());
    vector<int> comb;
    int next = 0;
    int sum = 0;
    vector<vector<int>> result;
    combinations(candidates, target, comb, next, sum, result);
    return result;
}

/*
  Given a string s, partition s such that every substring of the partition is a palindrome.

Return all possible palindrome partitioning of s.

Example
given s = "aab",
Return

  [
   ["aa","b"],
   ["a","a","b"]
  ]
*/
// This is slightly different from the previous permutation problems.
// It requires ultimate search concept. 
bool isPalindrome2(const string & str, int start, int end)
{
    for (; start<end; ++start, --end) {
        if (str[start] != str[end]) // use [] operator for string
            return false;
    }
    return true;
}
    
void partitionCombs(const string &s, int next, vector<string> & comb, vector<vector<string>> & result) {
    // condition for finishing one solution
    if (next == s.size()) {
        result.push_back(comb);
        return;
    }

    // i means check chars up to ith char. 
    for (int i=next; i<s.size(); ++i) {
        // If not palindrome, do not consider further, no more recursion.
        if (!isPalindrome2(s, next, i))
            continue; 

        // get familiar with string::substr() method. It needs the first
        // pos and length of the sub string.
        comb.push_back(s.substr(next, i-next+1));

        // We just finish a sub strings with palindromes,
        // now start from the next char. 
        partitionCombs(s, i+1, comb, result);

        // Do not forget to pop the last sub string so that it can be used for
        // next search. 
        comb.pop_back(); 
    }
}
    
vector<vector<string>> partition(string s) {
    // write your code here
    vector<string> comb;
    vector<vector<string>> result;
    partitionCombs(s, 0, comb, result);
    return result;
}

/*
 * Intervewed at Vungle, a Facebook guy asks for this.
 * You have mapping from a~z to 1~26. Input a string of numbers,
 * Figure out how many different combinations of string of chars.
 *
 * For example: "111" -> aaa, ka, ak, total 3 combinations
 * "999" -> iii, one comb.
 * "000" -> no comb. 
 */
void stringCombs(string & str, int next, int & count)
{
    // If the search goes beyond the string, that means we have one comb.
    if (next == str.size()) {
        ++count;
        return;
    }
    
    // The for loop is for the index of the string or array
    // You can reduce the #search by using i<next+2. 
    for (int i=next; i< min(next+2, (int)str.size()); ++i)
    {
        // The search will stop if number is not valid. No need go further.
        // Must use "i-next+1". 
        int num = atoi(str.substr(next, i-next+1).c_str());
        if (num<1 || num>26)
            continue; // Do not forget this "continue" to remove the unmatched. 

        stringCombs(str, i+1, count);
    }
}

int findStringCombs(string & str)
{
    int count = 0;
    stringCombs(str, 0, count);
    return count;
}

// Altiscale match strings
/*
Problem:
Input: A set of strings, d
       A single string, s
Ouput: T/F depending on whether the string s can be written as a concatenation of strings from d

Examples: d = {'aa', 'bb', 'cc'} s = 'aabbaa' --> T
          d = {'aa', 'bb', 'cc'} s = 'aabba' --> F
          d = {'aa', 'bb', 'cc', 'aab', 'ba'} s = 'aabba' --> ?
*/
// Question: how to improve to O(N).
// Use DP. Recursions can be improved by DP or induction. See matchStrings2()
// This problem matches conditions for DP. 

bool match(unordered_set<string>&d, const string &str)
{
    return d.find(str) != d.end(); // It is easy to compare with hash table. 
}

// Use hash map to store d for easy match. 
bool matchStrings(unordered_set<string> & d, const string & s, int next)
{
    if (next == s.size())
        return true;
    
    for (int i=next; i<s.size();++i)
    {
        if (!match(d, s.substr(next, i-next+1))) // Easy to get the substr. 
            continue;

        // Recursion starts from the next posisition.
        if (matchStrings(d, s, i+1)) 
            return true; // return true right away, no need do further. 
    }
    return false;
}

bool matchStrSet(vector<string> d, const string &s)
{
    unordered_set<string> dset;
    for (auto &t: d) // C++11
        dset.insert(t);

    return matchStrings(dset, s, 0);
}

// Use DP. It matches the conditions of DP. 
bool matchStrings2(const unordered_set<string> & d, const string &s) {
    vector<bool> f (s.size(), false);
    for (int i=0; i< s.size(); ++i) {
        // First check the whole sub string.
        if (d.find(s.substr(0, i+1)) != d.end()) {
            f[i] = true;
            continue;
        }

        // Then check the later part of the sub based on f[j]. 
        for (int j=0; j<i; ++j) {
            if (!f[j])
                continue;
            if (d.find(s.substr(j+1, i-j)) != d.end()) {
                f[j]=true;
                break;
            }
        }
    }
    return f.back();
}

/* Another permutation problem. Print divisors
  Write all the possible numbers which multiply to given number. Eg : 12 can be written as 12*1, 2*6,3*4,2*2*4. Dont include 6*2 again or 4*3 as its duplication of 2*6 and 3*4 resp.
*/
// If you can solve it in a few loops. Think about recursion with a loop in it.
// Also use sqrt() to prevent duplicate divisors.
// Use induction analysis for recursion. Start from the most simple base case.
// In this case, you can start from
// divisor 1, then add more divisors on top of that.
// Use deep/ultimate search thinking approach.

// Note: Need determine the start and end of the for loop. We use next for
// start. If start is always the same, no need next.
// Note: For all permutation problems, use tree to analyze it. 
void printDivisors(int num, int next, vector<int>& divisors)
{
    // base case. We do not need base case here because num will become smaller
    // and next become larger as the recursion goes deeper. Eventually it
    // will skip the for loop and return!
    // if (n<2) return...
    
    // use sqrt to prevent duplicate divisors.
    int upperLimit = sqrt(num);
    for (int i=next; i<=upperLimit; ++i)
    {
        if (num%i != 0)
            continue;

        for (auto &t:divisors)
            cout << t <<" ";
        cout << i << " "<<num/i <<endl; // These two numbers are also valid!

        divisors.push_back(i);
        printDivisors(num/i, i==1?i+1:i, divisors);
        divisors.pop_back();
    }
}

// No. 2
/* New puzzle
   Binary search is a famous question in algorithm.

For a given sorted array (ascending order) and a target number, find the first index of this number in O(log n) time complexity.

If the target number does not exist in the array, return -1.

Example
If the array is [1, 2, 3, 3, 4, 5, 10], for given target 3, return 2.

Challenge
If the count of numbers is bigger than MAXINT, can your code work properly?
 */
int binarySearch(vector<int> &A, int target) {
    if (A.size() == 0) {
        return -1;
    }

    int start = 0; // index only
    int end = A.size() - 1; // index only

    while (start + 1 < end) {
        int mid = start + (end - start) / 2;
        if (A[mid] == target) {
            // Do not return right away because we need find the first one.
            end = mid;
            //return mid; 
        } else if (A[mid] < target) {
            start = mid;
        } else //if (A[mid] > target)
            end = mid;
    }

    // Don't forget the following.
    if (A[start] == target) {
        return start;
    }
    if (A[end] == target) {
        return end;
    }

    return -1;
}

// New puzzle
/* http://www.ninechapter.com/solutions/search-for-a-range/
  Given a sorted array of integers, find the starting and ending position of a given target value. 
Your algorithm's runtime complexity must be in the order of O(log n). 
If the target is not found in the array, return [-1, -1]. 

For example, 
Given [5, 7, 7, 8, 8, 10] and target value 8, 
return [3, 4].
 */
vector<int> searchRange(vector<int> &a, int target)
{
    vector<int> output = {-1, -1};
    if (a.size()<1)
        return output;

    int start = 0;
    int end = a.size()-1;
    int mid;

    while (start+1<end)
    {
        mid = start + (end - start)/2;
        if (a[mid] == target)
            end = mid;
        else if (a[mid]<target)
            start = mid;
        else
            end = mid;
    }

    if (a[start]==target)
        output[0] = start;
    else if (a[end] ==target)
        output[0] = end;
    else
        return output;

    // found the first one. Find the last one now. Do not use linear search!
    // The complexity is O(2logn) = O(logn).
    start=0;
    end = a.size()-1;
    while (start+1<end)
    {
        mid = start + (end - start)/2;
        if (a[mid] == target)
            start= mid;
        else if (a[mid]<target)
            start = mid;
        else
            end = mid;
    }

    if (a[end]==target)
        output[1] = end;
    else if (a[start] ==target)
        output[1] = start;

    return output;
}

// New puzzle
/* http://www.ninechapter.com/solutions/search-insert-position/
Given a sorted array and a target value, return the index if the target is found. If not, return the index where it would be if it were inserted in order.

You may assume no duplicates in the array.

Example
[1,3,5,6], 5 → 2
[1,3,5,6], 2 → 1
[1,3,5,6], 7 → 4
[1,3,5,6], 0 → 0
*/
int searchInsert(vector<int> &a, int target)
{
    if (a.size()<1)
        return 0;

    // There are two types of cases, -1 or +1 index cases. 
    // After taking care of this -1 case, we only need to care about +1 cases,
    // no need to worry about -1 cases. This simplifies the problem. 
    if (target<a[0])
        return 0;

    int start=0;
    int end = a.size()-1;
    int mid;

    // Don't confuse index with value. know when to use a[mid] or mid. 
    while (start+1<end){
        mid = start + (end-start)/2;
        if (a[mid] == target)
            return mid;
        else if (a[mid] > target)
            end = mid;
        else
            start=mid;
    }

    if (a[end]==target)
        return end;
    if (a[end]<target)
        return end+1;
    if (a[start]==target)
        return start;

    // don't forget this. This function never returns -1.
    // This is different from end because a[end] != target in this case,
    // although the return values are the same. 
    return start+1; // same as "return end", verified. 
}

// New puzzle
/*
  http://www.ninechapter.com/solutions/search-in-rotated-sorted-array/
  Suppose a sorted array is rotated at some pivot unknown to you beforehand.

(i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2).

You are given a target value to search. If found in the array return its index, otherwise return -1.

You may assume no duplicate exists in the array.

Example
For [4, 5, 1, 2, 3] and target=1, return 2

For [4, 5,1, 2, 3] and target=0, return -1
*/
int searchInRotated(vector<int> & a, int target)
{
    if (a.size()<1)
        return -1;

    int start = 0;
    int end = a.size()-1;
    int mid;

    while (start+1<end)
    {
        mid = start + (end-start)/2;
        if (a[mid] == target)
            return mid;
        else if (a[start] < a[mid])
        {
            // Note: the if condition is different from sample code!
            // When first half is in order. Check if target is in between.
            // Only when the numbers are in order, they can be compared or
            // checked. That's why we do if/else this way.
            // Note: the start or end may equal the target! Need use <= here.
            if (a[start]<=target && target<a[mid])
                end = mid;
            else
                start = mid;
        }
        else
        {
            // When second half is in order. Check if target is in between.
            if (a[mid]<target && target<=a[end])
                start = mid;
            else
                end = mid;
        }
    }
    if (a[start]==target)
        return start;
    if (a[end]==target)
        return end;

    return -1;
}

/* Find Minimum in Rotated Sorted Array
  Suppose a sorted array is rotated at some pivot unknown to you beforehand.

(i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2).

Find the minimum element.

You may assume no duplicate exists in the array.

*/
int findMin(vector<int> &num) {
    // write your code here
    // Check errors and edge cases such as two items only
    int start = 0;
    int end = num.size()-1;
    int size = num.size();

    while (start+1 < end) {
        int mid = start + (end-start)/2;
        // We don't need worry about out of boudary for mid+1 and mid-1
        // because the while condition (start+1 < end). 
        if (num[mid] < num[mid+1] && num[mid] < num[mid -1])
            return num[mid];
        else if (num[start] < num[mid]) {
            // The first half is sorted.
            // Check if the min value in the first half is larger than max
            // value in the second half. If so, the min is in the second half. 
            if (num[start] > num[end])
                start = mid;
            else
                end = mid;
        } else {
            // The second half is sorted
            if (num[mid] > num[mid-1])
                end = mid;
            else
                start = mid;
        }
    }

    // No need to compare with left and right. Sometimes it is at the edge.
    // Just compare them and choose the small one. 
    if (num[start] > num[end])
        return num[end];
    else
        return num[start];
}

// Note for the previous question. If there are duplicates in the previous
// question, we can not use binary search. We can only use linear for loop
// because we can not remove half. For example: 11111111011111111111111.
// It is OK to use binary search if not rotated. 

/*
There are two sorted arrays A and B of size m and n respectively. Find the median of the two sorted arrays. The overall run time complexity should be O(log (m+n)).

Example
For A = [1,2,3,4,5,6] B = [2,3,4,5], the median is 3.5

For A = [1,2,3] B = [4,5], the median is 3

Challenge
Time Complexity O(logn)
*/
// Draw pictures!
int findKth(vector<int> &A, int starta, vector<int>&B, int startb, int K);

double findMedian(vector<int> &A, vector<int> &B)
{
    int len = A.size()+B.size();
    if (len%2 == 0)
        return (findKth(A, 0, B, 0, len/2)+findKth(A, 0, B, 0, len/2+1))/2.0;
    else
        return findKth(A, 0, B, 0, len/2+1);
}

int findKth(vector<int> &A, int starta, vector<int>&B, int startb, int K) {
    // Do not use A.size()-1 if A.size() could be 0. A.size() is unsigned.
    // So A.size() -1 is actually a big number if A.size() is 0.
    // So you need check A.size()>0 before using A.size()-1. 
    /* 
    if (A.size() - 1 < starta)
        return B[startb + K - 1];
    if (B.size() - 1 < startb)
        return A[starta + K - 1];
    */

    // First check the base or edge cases. These are important in recursion.
    // There are three edge cases.
    
    // This happens when A is empty or its elements are all removed
    if (A.size() <= starta)
        return B[startb + K - 1];
    if (B.size() <= startb)
        return A[starta + K - 1];

    // This is when K/2 is reduced to 1. 
    if (K == 1)
        return min(A[starta], B[startb]);

    // Do not forget starta and startb when calculating mid. 
    int mida = K/2 + starta - 1;
    int midb = K/2 + startb - 1;

    // If the index is out of bound, set valueA to max so that elements in
    // A will not be "removed" later. 
    int aMidValue = mida < A.size()?A[mida]:numeric_limits<int>::max();
    int bMidValue = midb < B.size()?B[midb]:numeric_limits<int>::max();

    // It is safe to "remove" half from the array with lower value at K/2.
    if (aMidValue < bMidValue) {
        starta = mida + 1;
    } else {
        startb = midb + 1;
    }
    return findKth(A, starta, B, startb, K-K/2);
}

/*
  Given a rotated sorted array, recover it to sorted array in-place.

Example
[4, 5, 1, 2, 3] -> [1, 2, 3, 4, 5]

Challenge
In-place, O(1) extra space and O(n) time.
*/
// 3 steps reverse approach.
void reverseArray(vector<int> & nums, int start, int end) {
    for (; start<end; ++start, --end) {
        int tmp = nums[start];
        nums[start] = nums[end];
        nums[end] = tmp;
    }    
}
    
void recoverRotatedSortedArray(vector<int> &nums) {
    // write your code here
    // Find the middle index+1
    int i;
    for (i=1; i<nums.size(); ++i) {
        if (nums[i] < nums[i-1])
            break;
    }
    
    // This also covers 0 size array case. Always check or cover edge case
    // such as 0 size. 
    if (i >= nums.size())
        return;
        
    reverseArray(nums, 0, i-1);
    reverseArray(nums, i, nums.size()-1);
    reverseArray(nums, 0, nums.size()-1);
}


/*
  Given a string and an offset, rotate string by offset. (rotate from left to right)

Example
Given "abcdefg"

for offset=0, return "abcdefg"
for offset=1, return "gabcdef"
for offset=2, return "fgabcde"
for offset=3, return "efgabcd"
 */
// It is more useful to specify start and end in the function. 
void reverseString(string & A, int start, int end) {
    for (; start<end; ++start, --end) {
        char tmp = A[start];
        A[start] = A[end];
        A[end] = tmp;
    }
}

// Note: Need convert offset to mid. Find the mid point + 1. This is the key.
// Then use 3 steps reverse approach. Draw picture to find it. 
string rotateString(string A, int offset) {
    // wirte your code here
    if (A.size()<2 || offset < 0)
        return A;

    // Use % to include larger offset. No need to worry about offset == A.size()
    // because of %. 
    int mid = A.size() - offset%A.size();
    if (mid == 0)
        return A;
        
    reverseString(A, 0, mid-1);
    reverseString(A, mid, A.size()-1);
    reverseString(A, 0, A.size()-1);
    return A;
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
  Given a sorted array of integers, find the starting and ending position of a given target value.

Your algorithm's runtime complexity must be in the order of O(log n).

If the target is not found in the array, return [-1, -1].

Example
Given [5, 7, 7, 8, 8, 10] and target value 8,
return [3, 4].
 */
vector<int> searchRange2(vector<int> &A, int target) {
    // write your code here
    vector<int> result(2, -1);
    if (A.size() < 1) return result;

    int start = 0;
    int end = A.size()-1;

    // Find the first appearance of target.
    while (start+1 < end) {
        int mid = start + (end - start)/2;
        if (A[mid] == target)
            end = mid;
        else if (A[mid] > target)
            end = mid;
        else
            start = mid;
    }

    if (A[start] == target)
        result[0] = start;
    else if (A[end] == target)
        result[0] = end;
    else
        return result;

    start = 0;
    end = A.size()-1;

    // Find the seccond appearance of target.
    while (start+1 < end) {
        int mid = start + (end-start)/2;

        //Always assign mid to start or end. 
        if (A[mid] == target)
            start = mid;
        else if (A[mid] > target)
            end = mid;
        else
            start = mid; // It is NOT "start = end". 
    }

    if (A[end] == target)
        result[1] = end;
    else if (A[start] == target)
        result[1] = start;

    return result;
}

/*
  Write an efficient algorithm that searches for a value in an m x n matrix.

This matrix has the following properties:

    * Integers in each row are sorted from left to right.

    * The first integer of each row is greater than the last integer of the previous row.

Example
Consider the following matrix:

[

    [1, 3, 5, 7],

    [10, 11, 16, 20],

    [23, 30, 34, 50]

]

Given target = 3, return true.

Challenge
O(log(n) + log(m)) time
 */
// Implemented with O(log(n) + log(m)) time
bool searchMatrix(vector<vector<int> > &matrix, int target) {
    // write your code here
    if (matrix.size() < 1) return false;

    int totalCols = matrix[0].size();
    int start = 0;
    int end = matrix.size()-1;
    int row = -1;

    while (start+1 < end) {
        int mid = start + (end - start)/2;
        if (matrix[mid][0] <= target && matrix[mid][totalCols-1] >= target) {
            row = mid;
            break;
        } else if (matrix[mid][0] > target) {
            end = mid;
        } else {
            start = mid;
        }
    }

    if (row == -1) {
        if (matrix[start][0] <= target && matrix[start][totalCols-1] >= target)
            row = start;
        else if (matrix[end][0] <= target && matrix[end][totalCols-1] >= target)
            row = end;
    }

    if (row == -1) return false;

    vector<int> & a = matrix[row];
    if (a.size() < 1) return false;
    start = 0;
    end = a.size()-1;

    while (start+1 < end) {
        int mid = start + (end - start)/2;
        if (a[mid] == target)
            return true;
        else if (a[mid] > target)
            end = mid;
        else
            start = mid;
    }

    if (a[start] == target) return true;
    if (a[end] == target) return true;
    return false;
}

// First Bad Version
/*
  The code base version is an integer and start from 1 to n. One day, someone commit a bad version in the code case, so it caused itself and the following versions are all failed in the unit tests.
You can determine whether a version is bad by the following interface: 

Java:
    public VersionControl {
        boolean isBadVersion(int version);
    }
C++:
    class VersionControl {
    public:
        bool isBadVersion(int version);
    };
Python:
    class VersionControl:
        def isBadVersion(version)

Find the first bad version.
*/

class VersionControl {
public:
    static bool isBadVersion(int k) {return true;};
};

int findFirstBadVersion(int n) {
    // write your code here
    if (n < 1) return -1;

    int start = 1;
    int end = n;
    while (start+1 < end) {
        int mid = start + (end-start)/2;
        bool isBadV = VersionControl::isBadVersion(mid);
        if (isBadV)
            end = mid;
        else
            start = mid;
    }

    if (VersionControl::isBadVersion(start))
        return start;
    if (VersionControl::isBadVersion(end))
        return end;
    return -1;
}

/*
  There is an integer array which has the following features:

    * The numbers in adjacent positions are different.

    * A[0] < A[1] && A[A.length - 2] > A[A.length - 1].

We define a position P is a peek if A[P] > A[P-1] && A[P] > A[P+1].

Find a peak element in this array. Return the index of the peak.

Note
The array may contains multiple peeks, find any of them.

Example
[1, 2, 1, 3, 4, 5, 7, 6]

return index 1 (which is number 2)  or 6 (which is number 7)
 */
int findPeak(vector<int> A) {
    // write your code here
    // Need error checking

    int start = 0;
    int end = A.size()-1;
    while (start+1 < end) {
        int mid = start + (end-start)/2;
        if (A[mid] > A[mid-1] && A[mid] > A[mid+1])
            return mid;
        else if (A[mid] < A[mid+1])
            start = mid;
        else
            end = mid;
    }

    if (start > 0 && start < A.size()-1 && A[start] > A[start-1] && A[start] > A[start+1])
        return start;
    if (end > 0 && end < A.size()-1 && A[end] > A[end-1] && A[end] > A[end+1])
        return end;
}

// The following are array questions.
/*
  Given a sorted array, remove the duplicates in place such that each element appear only once and return the new length.

Do not allocate extra space for another array, you must do this in place with constant memory.

For example,
Given input array A = [1,1,2],

Your function should return length = 2, and A is now [1,2].
 */
int removeDuplicates(vector<int> &nums) {
    // Must return nums.size() because it could be empty and return 0!
    if (nums.size() < 2) return nums.size();

    int index = 1; // start from th second item. 
    for (int i = 1; i < nums.size(); ++i) {
        if (nums[i] != nums[i-1])
            nums[index++] = nums[i];
    }

    // You can resize the vector.
    nums.resize(index);
    return index;
}

/*
  Follow up for "Remove Duplicates":
What if duplicates are allowed at most twice?

For example,
Given sorted array A = [1,1,1,2,2,3],

Your function should return length = 5, and A is now [1,1,2,2,3].
 */
int removeDuplicates2(vector<int> &nums) {
    // Check edge case
    if (nums.size() < 3) return nums.size();

    int index = 2;
    
    // Need keep nums[i-2] because it can be overwritten.
    // For example, 111222. 
    int minus2 = nums[0]; 
    for (int i = 2; i < nums.size(); ++i) {
        int minus1 = nums[i-1];
        if (nums[i] != nums[i-1])
            nums[index++] = nums[i]; // This may overwritten nums[i-1] or minus1
        else if (nums[i] != minus2)
            nums[index++] = nums[i];
        minus2 = minus1; // minus1 will become minus2 in the next loop. 
    }
    nums.resize(index);
    return nums.size();
}

/*
Merge two given sorted integer array A and B into a new sorted integer array.

Example
A=[1,2,3,4]

B=[2,4,5,6]

return [1,2,2,3,4,4,5,6]
*/
vector<int> mergeSortedArray(vector<int> &A, vector<int> &B) {
    // write your code here
    vector<int> result;
    result.reserve(A.size()+B.size());

    int ia = 0, ib = 0;
    while (ia < A.size() && ib < B.size()) {
        if (A[ia] < B[ib])
            result.push_back(A[ia++]); // Don't forget to increment index. 
        else
            result.push_back(B[ib++]);
    }

    if (ia == A.size()) {
        for (; ib < B.size(); ++ib)
            result.push_back(B[ib]); // no need to increment index. 
    } else {
        for (; ia < A.size(); ++ia)
            result.push_back(A[ia]);
    }
    return result;
}

/*
  Bloomberg phone screen. They do not use the same wording in the question
  Given an array of strings, return all groups of strings that are anagrams.
Note
All inputs will be in lower-case

Example
Given a string list: ["lint","intl","book", "inlt","eat", "ate", "at"]

return ["lint","inlt","intl", "eat", "ate"]
*/
// Note: the return type may change to vector<vector<string>>
// Need get the key and add to hash map.
vector<string> anagrams(vector<string> &strs) {
    // write your code here
    unordered_map<string, vector<string>> groups;
    for (int i=0; i<strs.size(); ++i) {
        string key = strs[i];
        sort(key.begin(), key.end()); // Need sort first, then hash map.
        groups[key].push_back(strs[i]); // push into a vector. 
    }
        
    vector<string> result;
    unordered_map<string, vector<string>>::iterator cit;
    for (cit = groups.begin(); cit != groups.end(); ++cit) {
        vector<string>& grp = (*cit).second;
        if (grp.size()>1)
        {
            // remember how to use vector insert with iterators. 
            result.insert(result.end(), grp.begin(), grp.end());
        }
    }
    return result;
}

/* Kth prime number lintCode
  Design an algorithm to find the kth number such that the only prime factors are 3, 5, and 7.

The eligible numbers are like 3, 5, 7, 9, 15 ...

Example
If k=4, return 9.

Challenge
O(n log n) or O(n) time
*/
// Note: you can not use it+i opertation. You need call advance(it, i).
//       ++it is OK because it is overloaded. But it+i is not.
// Note: you can use advance(it, 5) or distance(it1, it2)...
// Note: It seems something is wrong here. Need compare with 3^N to determine
//       that all numbers smaller than 3^N are already in the map. 
long long kthPrimeNumber(int k) {
    // write your code here
    set<long long> numbers;
    numbers.insert(3);
    numbers.insert(5);
    numbers.insert(7);
    int prevSize = 0;
    while (numbers.size() < k)
    {
        // For each new members in the set, multiply with 3, 5, and 7 and
        // add the products back to the set. They will be sorted by
        // the set. O(nlogn).
        // Keep the previous size so that we do not multiply same number
        // again. 
        int size = numbers.size();
        for (int i=prevSize;i<size;++i)
        {
            // use auto to avoid long code. 
            auto it = numbers.begin();
            advance(it, i);
            long long val = 3* (*it);
            numbers.insert(val);
            val = 5 * (*it);
            numbers.insert(val);
            val = 7 * (*it);
            numbers.insert(val);
        }
            
        prevSize = size;
    }
        
    auto it = numbers.begin();
    advance(it, k-1);
    return *it;
}

// Roblox test about unix path cd operation
class Path
{
public:
	Path(std::string path)
	{
		currentPath = path;
	}

	std::string getPath() const
	{
		return currentPath;
	}

	Path cd(std::string newPath) 
	{
        if (newPath[0] == '/')
        {
            currentPath=newPath;
            return currentPath;
        }
        
        while (!newPath.empty()) // empty()
        {
            if (newPath.substr(0, 2) == string("..")) // substr()
            {
                newPath.erase(0, 2);
                if (newPath[0] == '/')
                    newPath.erase(0,1); // erase
                int pos = currentPath.rfind('/'); // rfind();
                if (pos != string::npos) // string::npos
                    currentPath.resize(pos); // resize or erase
            }
            else
                break;
        }

        currentPath=currentPath+"/"+newPath;
        if (currentPath.empty())
            currentPath="/";
        return currentPath;
	}

private:
	std::string currentPath;
};

/*int main(int argc, const char* argv[])
{
	Path path("/a/b/c/d");
	std::cout << path.cd("../x").getPath();
}*/

/*
- Question: Given the following prototype: 
int compact(int * p, int size);  

Write a function that will take a sorted array, possibly with duplicates,
and compact the array, returning the new length of the array. That is, if p
points to an array containing: 1, 3, 7, 7, 8, 9, 9, 9, 10, when the function
returns, the contents of p should be: 1, 3, 7, 8, 9, 10, with a length of 5
returned.
*/
// "int p[]" == "int *p". Array is passed as a pointer. It is no longer
// an array after passing. It is just a pointer and sizeof(p) = 4. 
int compact(int p[], int size) 
{
    // size = sizeof(p)/sizeof(int);
    int current = 1;
    for (int i=1; i<size; ++i) { // start from 1. 
        if (p[i] == p[i-1])
            continue;
        else
            p[current++] = p[i]; 
    }
    return current; // don't forget the return. 
}

void heapPractice()
{
    vector<int> v = {10, 20, 30, 5, 15};
    make_heap(v.begin(), v.end());
    cout<<"initial max heap: "<<v.front()<<endl;

    pop_heap(v.begin(), v.end()); v.pop_back();
    cout << "max heap after pop: "<<v.front()<<endl;

    v.push_back(99); push_heap(v.begin(), v.end());
    cout <<"max heap after push: "<<v.front()<<endl;

    //heap sort

    auto it = v.end();
    while (it != v.begin())
    {
        pop_heap(v.begin(), it);
        --it;
    }

//    sort_heap(v.begin(), v.end());

    cout<<"final sorted range: ";
    for (int i = 0; i<v.size(); ++i)
        cout<<" "<<v[i];
    cout <<endl;
}

/* 3 sum. three sum. threesum 3sum
  Given an array S of n integers, are there elements a, b, c in S such that a + b + c = 0? Find all unique triplets in the array which gives the sum of zero.

Note
Elements in a triplet (a,b,c) must be in non-descending order. (ie, a ≤ b ≤ c)

The solution set must not contain duplicate triplets.

Example
For example, given array S = {-1 0 1 2 -1 -4}, A solution set is:

(-1, 0, 1)

(-1, -1, 2)
 */
vector<vector<int> > threeSum(vector<int> &nums) {
    // write your code here
    vector<vector<int>> result;
    if (nums.size()<3)
        return result;

    // sort first to check duplicate and move from both ends. 
    sort(nums.begin(), nums.end()); 
    vector<int> oneResult;
    for (int i = 0; i<nums.size()-2; ++i) {
        // removes duplicates when moving i. 
        if (i!=0 && nums[i] == nums[i-1])
            continue;

        // set target
        int target = 0-nums[i];
        int start = i+1;
        int end = nums.size()-1;
            
        while (start<end) {
            if (nums[start]+nums[end] == target) {
                oneResult.clear();
                oneResult.push_back(nums[i]);
                oneResult.push_back(nums[start]);
                oneResult.push_back(nums[end]);
                result.push_back(oneResult);

                // The following removes duplicates when moving start and end.
                ++start;
                while (start<end && nums[start] == nums[start-1])
                    ++start;
                --end;
                while (start<end && nums[end] == nums[end+1])
                    --end;
            } else if (nums[start]+nums[end] > target){
                --end;
            } else {
                ++start;
            }
        }
    }
        
    return result;
}

/*
- Google LA 1
input 2 arrays: [1, 8, 5], [2, 10, 4, 6]
Make a union of them. Do not have to be ordered. Don’t care about original
order. The original two set streams are not ordered, either.

Implement iterator operations
Note: these arrays are huge. We need bitset or vector<bool> for that. 

*/

class IntSetStream {
public:
//    virtual bool more() const = 0;
//    virtual int next() const = 0;
    virtual bool more() const {return true;}
    virtual int next() const {return 0;}
};

// assume the following is a non-STL bitset class. 
class BitSet {
public:
    void set(int) {}
    bool has(int) const {return true;}
};

class UnionIntSetStream : public IntSetStream {
public:
    UnionIntSetStream(const IntSetStream& a, const IntSetStream& b) {
        // need initialize the private members here. 
        while (a.more()) {
            int item = a.next();
            data.set(item);
            if (item>max)
                max = item;
            if (item < min)
                min = item;
        }

        while (b.more()) {
            int item = b.next();
            data.set(item);
            if (item>max)
                max = item;
            if (item < min)
                min = item;
        }
        currentInt = min;
    }

    virtual bool more() const {
        // This is important. You just need some boolean operation to make
        // sure there is more. No complex operation!
        return currentInt<=max;
    }

    virtual int next() {
        // assume max is not the largest int.
        while (currentInt <= max)
        {
            if (data.has(currentInt))
                return currentInt++;
            else
                ++currentInt;
        }

        // No more data, throw exception. 
    }
    
private:
    BitSet data;
    int currentInt;

    // Need min and max for performance. You don't want to start from min int. 
    int min; 
    int max;
};

// Amazon 2nd phone interview.
// Get all the friends of a person. Then get all his friend's friends. Then
// keep getting more friends like that. There is a limit of how many friends
// you can get. 
// Note: 3 things: 1. Exclude the person himself. 2. Remove duplicates
// because there are cyclic connections between friends. 3. Avoid infinite
// loop.
// Note: Amazon always asks how to test. Mock object. Try to find all the
//       edge cases.
// Note: Use a list. You keep reading from the list and
//       add new items at the end until you reach the end or the limit. It is
//       always better to use a list than a queue because there are more
//       flexible operations on a list.
// Note: Use iterations to write complex code. First implement the basics.
//       Then add code to remove duplicates. Then add code for limit...
//       Review the code at last before hand it in. 


class Person {
public:
    string & getId() {return id;}
    list<Person> &getFriends() {return friends;}
private:
    string id;
    list<Person> friends;
};

list<Person> getFriendList(Person & person, int limit)
{
    // Similar to BSF of binary tree. But no need to print each level.
    // So we do not need check size() for each inner loop or use a marker. 
    list<Person> results;

    // use the hash table to avoid duplicate
    unordered_set<string> visited;
    
    results.push_back(person);
    visited.insert(person.getId());
    
    auto it = results.begin();
    while (it != results.end())
    {
        list<Person> friends = it->getFriends();
        for (auto it2 = friends.begin(); it2 != friends.end(); ++it2)
        {
            if (visited.find(it2->getId()) != visited.end())
                continue;
            
            results.push_back(*it2);
            visited.insert(it2->getId());
            if (results.size() == limit+1)
            {
                results.pop_front();
                return results;
            }
        }
        ++it;
    }

    results.pop_front();
    return results;
}

/*
  You have 100 integers from 1 to 100. You randomly add them into an array
  of 99 size. 
  There are duplicate numbers in the array. Please find the numbers not
  in the array. Also need O(N).

  Hint: Imagine you can sort the numbers in the array. But you do not sort
        because it is >O(N). For each number, you change the number in the
        corresponding location to negative. Then, positive numbers indicate
        that the corresponding numbers are not inserted.
        Note that 100 can not fit the 99 array. 
 */
void printNotInserted(vector<int> & A) {
    // Always pay attention to boundary for array.
    // Must use abs() because the item may become negative.
    // Also check if it is 100 because it is also out of boundary.
    bool has100 = false;
    for (int i=0; i<A.size(); ++i) {
        // Can not use the same approach for 100 because it is out of boundary.
        // use a flag instead. 
        if (abs(A[i]) == 100) {
            has100 = true;
            continue;
        }
        if (A[abs(A[i])-1] > 0)
            A[abs(A[i])-1] = -A[abs(A[i])-1];
    }
    
    for (int i=0; i<A.size(); ++i) {
        if (A[i] > 0)
            printf("%d ", i+1);
    }
    
    if (!has100)
        printf("100");
    printf("\n");
}

/* From Facebook phone interview. 
Given a dictionary, find all pairs of words that, when concatenated together, form a palindrome. For instance, in English we would be looking for pairs like (cigar, tragic); (warrener, raw);

cigar + tragic = "cigartragic"
warrener + raw = "warrenerraw"

warrener = "war" ("raw") + "rener"
*/
/*
  Analysis: In the cigar case, the "t" in tragic is a palindrome. So we just
  need to find the reverse of the suffix after "t" in dict which is cigar.
  In the warrener case, the "rener" is a palindrome. So we just need to find
  the reverse of the prefix "war" which is raw.
  In the implementation, we only implement the warrener case, then use reverse
  string of tragic to go through the same routine of warrener in the second
  loop. You may also implement both cases. 
 */

// Note: It is not important to use "const" in interview. People do not care.
bool isPalindrome(const string & str, int start, int end) {
    for (; start<end; ++start, --end) {
        if (str[start] != str[end])
            return false;
    }
    return true;
}

void reverseStr(string & str) {
    int start = 0;
    int end = str.size()-1;
    for (; start<end; ++start, --end) {
        swap(str[start], str[end]);
    }
}

vector<vector<string>> findAllPairs(const unordered_set<string> & dict) {
    auto it = dict.begin();
    vector<vector<string>> result;
    
    for (auto it=dict.begin(); it != dict.end(); ++it) {
        string str = *it; // Do not use reference because it will be changed.

        // Use two loops. The first loop is for warrener type. The second
        // is for cigar type. Ignore the second loop when working on the
        // first one. Then add code for the second one.
        // Note: we have at least one duplicate here. The reverse of the
        // whole word is checked twice in the two loops. So we need dedup
        // here. Added dedup code. It is not required in the interview though.
        for (int j=0; j<2; ++j) {
            if (j == 1)
                reversStr(str); 
                
            for (int i=0; i<str.size(); ++i) {
                // remove dup in the second loop as discussed above.
                if (j==1 && i == str.size()-1)
                    continue;

                // it is important to check edge cases.
                // Here, you do not check if the whole word is a pal.
                // You check from the second char to the end.
                // But you include the edge case where i==str.size()-1.
                // In this case, isPalindrome() will return true and
                // you need find the reverse of the whole word in dict!
                if (isPalindrome(str, i+1, str.size()-1)) {
                    // Note it is i+1. Use an example to figure out. 
                    string prefix = str.substr(0, i+1);
                    if (j==0)
                        reverseStr(prefix);
                    if (dict.find(prefix) == dict.end())
                        continue;
                    
                    vector<string> oneresult;
                    if (j==1)
                        reverseStr(str);
                    oneresult.push_back(str);
                    if (j==1)
                        reverseStr(str);
                    oneresult.push_back(prefix);
                    result.push_back(oneresult);
                }
            }
        }
    }
    return result;
}

vector<vector<string>> findAllPairs2(const unordered_set<string> & dict) {
    vector<vector<string>> result;

    for (auto it = dict.begin(); it != dict.end(); ++it) {
        const string &str = *it;

        // for warrener and raw case.
        for (int i=0; i<str.size(); ++i) {
            // Note: Check Pal with empty sub str at the last loop.
            // The empty sub str is also a pal!
            if (isPalindrome(str, i+1, str.size()-1)) {
                string prefix = str.substr(0, i+1);
                reverseStr(prefix);
                if (dict.find(prefix) != dict.end()) {
                    vector<string> oneResult;
                    oneResult.push_back(str);
                    oneResult.push_back(prefix);
                    result.push_back(oneResult);
                }
            }
        }

        // for cigar and tragic case. 
        for (int i=1; i<str.size(); ++i) {
            // Note: Check Pal from -1! The empty sub str is also a pal.
            // We need check the reverse of the whole word in dict.
            // But we already checked the whole word in the previous prefix
            // case. We need remove the duplicate here. So we start the loop
            // from i=1!
            if (isPalindrome(str, 0, i-1)) {
                string suffix = str.substr(i);
                reverseStr(suffix);
                if (dict.find(suffix) != dict.end()) {
                    vector<string> oneResult;
                    oneResult.push_back(str);
                    oneResult.push_back(suffix);
                    result.push_back(oneResult);
                }
            }
        }
    }
    return result;
}

    long long permutationIndex(vector<int>& A) {
        // Write your code here
        
    }

int main()
{
    const string source = "abdopsdkdppb";
    const string target = "sdk";

    cout<<"mystrstr()=="<<mystrstr(source, target)<<endl;

    vector<int> a = {1, 3, 2};
    vector<vector<int>> result;
    allSubsetsWrapper(a, result);

    for (int i=0; i<result.size(); ++i)
    {
        for (int j=0; j<result[i].size(); ++j)
            cout<<result[i][j]<<" ";
        cout<<endl;
    }

    a.clear();
    a = {1, 2, 2};
    result.clear();
    result = permuteUnique(a);
    for (int i=0; i<result.size(); ++i)
    {
        for (int j=0; j<result[i].size(); ++j)
            cout<<result[i][j]<<" ";
        cout<<endl;
    }

    a.clear();
    a = {1, 2, 3, 3, 4, 5, 10};
    cout<<"binary search = "<<binarySearch(a, 10)<<endl;

    a.clear();
    a = {5, 7, 7, 8, 8, 9, 10, 10};
    vector<int> result2;
    result2 = searchRange(a, 10);
    cout<<"search range is "<<result2[0] << " "<<result2[1]<<endl<<endl;

    a.clear();
    a = {1,3,5,6};
    cout<<"search result = "<<searchInsert(a, 5)<<endl;
    cout<<"search result = "<<searchInsert(a, 2)<<endl;
    cout<<"search result = "<<searchInsert(a, 7)<<endl;
    cout<<"search result = "<<searchInsert(a, 0)<<endl;
    cout <<endl;

    a.clear();
    a = {4, 5, 1, 2, 3};
    cout<<"rotated search result = "<<searchInRotated(a, 1)<<endl;
    cout<<"rotated search result = "<<searchInRotated(a, 0)<<endl;
    cout <<endl;

    vector<int> A;
    vector<int> B = {1};
    cout<<"Median is "<<findMedian(A, B)<<endl;

    A = {1,2,3};
    B = {4,5};
    cout<<"Median is "<<findMedian(A, B)<<endl;

// Implement a two dimentional array with vector.
    /*
    int row=4, col=5;
    vector<vector<int>> array1(row, vector<int>(col, 10));
    cout<<array1[3][4]<<endl;
    */

    a.clear();
    a = {9,10,100,101,1002,10203};
    result2 = searchRange2(a, 10203);
    cout << "searchRange2() = "<<result2[0] <<", "<<result2[1]<<endl;

    string str = "111";
    cout << "findStringCombs(str)=" <<findStringCombs(str)<<endl;

	Path path("/a/b/c/d");
	std::cout << path.cd("../x").getPath()<<endl;

    int iarray[] = {1, 3, 7, 7, 8, 9, 9, 9, 10};
    int size = compact(iarray, sizeof(iarray)/sizeof(int));
    cout << "compact = " << size <<endl;
    for (int i = 0; i<size; ++i)
        cout << iarray[i] << " ";
    cout<<endl;

    heapPractice();

    vector<string> d = {"aa", "bb", "cc", "aab", "ba"};
    string s="aabba";
    cout <<"matchStrSet(d, s)=" << matchStrSet(d, s)<<endl;

    // printDivisors.
    vector<int> divisors;
    printDivisors(12, 1, divisors);
    cout<<endl;

    return 0;
}

