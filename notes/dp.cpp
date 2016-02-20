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

#include "dp.h"

using namespace std;

/* Triangle
  Given a triangle, find the minimum path sum from top to bottom. Each step you may move to adjacent numbers on the row below.

Note
Bonus point if you are able to do this using only O(n) extra space, where n is the total number of rows in the triangle.

Example
For example, given the following triangle

  [
     [2],
    [3,4],
   [6,5,7],
   [4,1,8,3]
   ]
*/
// Note: this uses minSum to store the memoization search results.
// 
int minimumTotal(vector<vector<int> > &triangle) {

    // get the len because it will be used in many places. 
    int len = triangle.size();
    vector<vector<int>> minSum(len, vector<int>(len, 0));

    // initialize the obvious values. 
    minSum[0][0] = triangle[0][0];

    // i starts from 1 because 0 has been initialized
    for (int i=1; i<len; ++i) {
        for (int j=0; j<triangle[i].size(); ++j) {
            int minValue=numeric_limits<int>::max();

            // always check boundary before using array[]
            // Do not confuse minSum with triangles. Know when to use which.
            if (j>0)
                minValue = minSum[i-1][j-1];
            if (j<triangle[i-1].size())
                minValue = min(minValue, minSum[i-1][j]);
            minSum[i][j] = minValue + triangle[i][j];
        }
    }

    // use minSum instead of triangle. 
    int minValue = minSum[len-1][0];
    for (int i=1; i<len; ++i)
        minValue = min(minValue, minSum[len-1][i]);
    return minValue;
}

// Here is a version with O(N) space. Basically, you allocate an array
// with two rows for minSum. Then you use %2 operator to alternate between
// them. You only need two rows for memoization!
int minimumTotal2(vector<vector<int> > &triangle) {
    int len = triangle.size();
    vector<vector<int>> minSum(2, vector<int>(len, 0));

    // initialize the obvious values. 
    minSum[0][0] = triangle[0][0];

    // i starts from 1 because 0 has been initialized
    for (int i=1; i<len; ++i) {
        for (int j=0; j<triangle[i].size(); ++j) {
            int minValue=numeric_limits<int>::max();

            // always check boundary before using array[]
            // Do not confuse minSum with triangles. Know when to use which.
            if (j>0)
                minValue = minSum[(i-1)%2][j-1];
            if (j<triangle[i-1].size())
                minValue = min(minValue, minSum[(i-1)%2][j]);
            minSum[i%2][j] = minValue + triangle[i][j];
        }
    }
    

    // use minSum instead of triangle. 
    int minValue = minSum[(len-1)%2][0];
    for (int i=1; i<len; ++i)
        minValue = min(minValue, minSum[(len-1)%2][i]);
    return minValue;

}

/*
  A robot is located at the top-left corner of a m x n grid (marked 'Start' in the diagram below).

The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid (marked 'Finish' in the diagram below).

How many possible unique paths are there?

Note
m and n will be at most 100.
 */
int uniquePaths(int m, int n) {
    if (m == 0 || n == 0)
        return 0;
            
    // wirte your code here
    vector<vector<int>> paths(m, vector<int>(n, 0));

    // initialize the first row. 
    for (auto & p : paths[0])
        p = 1;
    
    // initialize the first col. 
    for (auto & v : paths)
        v[0] = 1;
            
    for (int i=1; i<m; ++i) {
        for (int j=1; j<n; ++j) {
            paths[i][j] = paths[i-1][j] + paths[i][j-1];
        }
    }
    return paths[m-1][n-1];
}

/*
  Given a m x n grid filled with non-negative numbers, find a path from top left to bottom right which minimizes the sum of all numbers along its path.

Note
You can only move either down or right at any point in time.
*/
int minPathSum(vector<vector<int> > &grid) {
    //if (grid.size(<1 || grid[0].size() < 1))
    //    throw exception
            
    vector<vector<int>> results(grid.size(), vector<int>(grid[0].size()));

    // init the first row and col. This makes it easier for the main loops
    int sum = 0;
    for (int i=0; i<results[0].size(); ++i) {
        sum += grid[0][i];
        results[0][i] = sum;
    }
    sum = 0;
    for (int i=0; i<results.size(); ++i) {
        sum += grid[i][0]; 
        results[i][0] = sum;
    }
        
    int i,j;
    for (i=1; i < results.size(); ++i) {
        for (j=1; j<results[i].size(); ++j) {
            // Don't forget to add grid[i][j].
            results[i][j] = min(results[i-1][j], results[i][j-1]) + grid[i][j];
        }
    }
        
    return results[i-1][j-1];
}

/*
  You are climbing a stair case. It takes n steps to reach to the top.

Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?

Example
Given an example n=3 , 1+1+1=2+1=1+2=3
*/
// Note: Find the function from the last step. Same as Fib.
//       Only need to memorize two previous steps. 
int climbStairs(int n) {
    // write your code here
    if (n<3) return n;
        
    int current = 2;
    int previous = 1;
        
    for (int i=3; i<=n; ++i) {
        int tmp = current;
        current = current + previous;
        previous = tmp;
    }
        
    return current;
}

/* Jump Game
  Given an array of non-negative integers, you are initially positioned at the first index of the array.

Each element in the array represents your maximum jump length at that position.

Determine if you are able to reach the last index.
 */
// This can be improved by using O(N) time and O(2) space. See the next
// function jump() for more details.
bool canJump(vector<int> A) {
    // write you code here
    vector<bool> results(A.size(), false);

    // init value
    results[0] = true;

    // Use induction or DP for the rest
    for (int i=0; i<results.size(); ++i) {
        if (!results[i]) // i is not reachable. 
            continue;
        for (int j=0; j<A[i]; ++j) {
            // boundary check before operator []
            if (j+i+1 > results.size())
                break;
            results[j+i+1] = true;   
        }
    }
    return results[results.size()-1];
}

// Greedy solution
// May improve the perf by returning immediately upon a solution.
// Check "furthest >= A.size()-1" whenever it is changed.  
bool canJumpGreedy(vector<int> A) {
    // Think it as merging N intervals
    int furthest = A[0];
            
    for (int i=1; i<A.size(); ++i) {
        // Need check i<=furthest to make sure i is reachable. 
        if (i<=furthest && i+A[i] > furthest) {
            furthest = i+A[i];
        }
    }
    return furthest >= A.size()-1;
}

/* Jump game II
 Given an array of non-negative integers, you are initially positioned at the first index of the array.

Each element in the array represents your maximum jump length at that position.

Your goal is to reach the last index in the minimum number of jumps.

Example
Given array A = [2,3,1,1,4]

The minimum number of jumps to reach the last index is 2. (Jump 1 step from index 0 to 1, then 3 steps to the last index.)
*/
// Note: It will exceed time limit if using DP. Use greedy solution instead.
// Note: we do not need create a separate results vector for this.
// We can actually keep a running steps and furthest index.
// This saves a lot of time and space when the input is huge and requires
// a lot of calculation.
// We can also change the previous canJum() function to the same approach.
int jumpGreedy(vector<int> A) {
    if (A.size() == 0)
        return -1;

    int steps = 0;
    int furthest = 0;

    // use start and end to store the range of each loop.
    // Update them after each loop. Increment steps once for each loop. 
    // start <= end
    int start = 0;
    int end = furthest;

    // Only inrecment steps once in each while loop because each while loop
    // is one step to the current furthest. 
    bool needInc = true;
    while (start <= end && end < A.size()-1)
    {
        // Loop to the current furthest/end to find the next furthest
        // with one more step. 
        for (int i=start; i<=end; ++i) {
            int newFurthest = i+A[i];

            if (newFurthest > furthest) {
                furthest = newFurthest;
                if (needInc) {
                    ++steps; // only inc once for each while loop
                    needInc = false;
                }
                // check result after each update of furthest.
                // Might not needed. But can save some time. 
                if (furthest >= A.size()-1)
                    return steps;
            }
        }
        start = end+1;
        end = furthest;
        needInc = true;
    }
    
    if (furthest >= A.size()-1)
        return steps;
    else
        return -1;
}

/*
  Given a string s, cut s into some substrings such that every substring is a palindrome.
Return the minimum cuts needed for a palindrome partitioning of s.

Example
For example, given s = "aab",
Return 1 since the palindrome partitioning ["aa","b"] could be produced using 1 cut.
*/
// Note: it is very important to define the state of DP. In this case, the
// state i is the min cuts for the first i chars. The initial state is also
// important. In this case, state 0 is -1, not 0.
//       
bool isPal(const string&str, int start, int end) {
    for (; start<end; ++start, --end) {
        if (str[start] != str[end])
            return false;
    }
    return true;
}
    
int minCut(string s) {
    vector<int> cuts(s.size()+1, numeric_limits<int>::max());

    // The initial state is -1, not 0. This is important. It is easy for
    // later calculation. 
    cuts[0] = -1;
    for (int i=1; i<=s.size(); ++i) {
        for (int j=0; j<i; ++j) {
            if (isPal(s, j, i-1)) {
                // Take the minimum from the loop. 
                cuts[i] = min(cuts[i], cuts[j]+1);
            }

            // The following is optimization when #cuts is 0. No need further
            // check. 
            if (j==0)
                break;
        } 
    }
    return cuts.back();
}

/* Word Segmentation
  Given a string s and a dictionary of words dict, determine if s can be segmented into a space-separated sequence of one or more dictionary words.

Example
Given
s = "lintcode",
dict = ["lint", "code"].
Return true because "lintcode" can be segmented as "lint code".
 */
// state[i] is about the first i chars, not the subscript of s. 
// Dictionary word implies that there is a limit in word length. So we can
// use the word length to optimize. 
bool wordSegmentation(string s, unordered_set<string> &dict) {
    // write your code here
    vector<bool> state(s.size()+1, false);

    // The intial value of state[0] is very important. 
    state[0] = true;
        
    for (int i=1; i<=s.size(); ++i) {
        // If the max word length in dict is 20, we can set j = i-20;
        // Do not apply the optimization in the first draft. Add later. 
        for (int j = i>=20?i-20:0; j<i; ++j) {
            if (!state[j])
                continue;
            if (dict.find(s.substr(j, i-j)) != dict.end()) {
                state[i] = true;
                break;
            }
        }
    }
        
    return state.back();
}

/* Longest Increasing Subsequence
  Given a sequence of integers, find the longest increasing subsequence (LIS).
You code should return the length of the LIS.
Example
For [5, 4, 1, 2, 3], the LIS  is [1, 2, 3], return 3
For [4, 2, 4, 5, 3, 7], the LIS is [4, 4, 5, 7], return 4
 */
int longestIncreasingSubsequence(vector<int> nums) {
    // initialize all LIS to 1. 
    vector<int> lis(nums.size(), 1);
        
    int maxLis = 0; // The final max. 
    for (int i=1; i<nums.size(); ++i) {
        // We need two loops for this. The inner loop must go through the
        // all the previous states and check with the new state i. 
        for (int j=0; j<i; ++j) {
            if (nums[j] <= nums[i]) {
                lis[i] = max(lis[i], lis[j]+1);
                maxLis = max(maxLis, lis[i]);
            }
        }
    }
    return maxLis;
}

/* Longest Common Subsequence
Given two strings, find the longest comment subsequence (LCS).

Your code should return the length of LCS.

Example
For "ABCD" and "EDCA", the LCS is "A" (or D or C), return 1

For "ABCD" and "EACB", the LCS is "AC", return 2
*/
// Note: This DP problem involves two sequences. Need 2D array.
//       Two sequences are different from one sequence. You only need to
//       deduct result from adjacent items. In one sequence, you need
//       all the previous items in the 1D array. 
// Note: There is no magic. Increment it only when equal.
// Note: The initial values are important
int longestCommonSubsequence(string A, string B) {
    // init value to 0 for lcs[i][0] and lcs[0][j].
    // The value in lcs is the longest lcs so far. 
    vector<vector<int>> lcs(A.size()+1, vector<int>(B.size()+1, 0));

    // i and j mean that the first i and j chars, not their sub script.
    // So the lcs value is 0 when i or j is 0. 
    for (int i=1; i<=A.size(); ++i) {
        for (int j=1; j<=B.size(); ++j) {
            if (A[i-1]==B[j-1])
                lcs[i][j] = lcs[i-1][j-1]+1;
            else {
                // Pick the largest from the previous neighboring items.
                // It needs to be propogated for future calculation. And the
                // final result needs to be propogated to the end item. 
                lcs[i][j] = max(lcs[i-1][j], lcs[i][j-1]);
            }
        }
    }

    // This is the same as lcs[A.size()][B.size()]
    return lcs.back().back();
}

/*
  Given two strings, find the longest common substring.
  Return the length of it.
  Note
  The characters in substring should occur continiously in original string. This is different with subsequnce.

  Example
  Given A="ABCD", B="CBCE", return 2
*/
// Note: Must draw the 2D array picture for state(i, j).
// Note: Use maxLength because it is not at the end of the 2D array. 
int longestCommonSubstring(string &A, string &B) {
    // write your code here
    vector<vector<int>> state(A.size()+1, vector<int>(B.size()+1, 0));
    int maxLength = 0;
    for (int i=1; i<=A.size(); ++i) {
        for (int j=1; j<=B.size(); ++j) {
            if (A[i-1] == B[j-1]) {
                // Add from state(i-1, j-1) because it must be continuous.
                // You can not get continuity from the other two neighbors.
                // Study the 2D array picture to understand this. 
                state[i][j] = state[i-1][j-1] +1;
                maxLength = max(maxLength, state[i][j]);
            }
        }
    }
    return maxLength;
}

/* Edit Distance
  Given two words word1 and word2, find the minimum number of steps required to convert word1 to word2. (each operation is counted as 1 step.)
You have the following 3 operations permitted on a word:

a) Insert a character
b) Delete a character
c) Replace a character

Example
Given work1="mart" and work2="karma"
return 3
*/
// First define the state f(i, j). It means it takes f(i,j) steps to make
// both strings equal at word[i] and word[j]. 
// Note: i and j means the number of chars so far in the string, not subscript. 
// Note: There is no magic. Just need check word1[i] == word2[j].
//       The rest is simple
int minDistance(string word1, string word2) {
    vector<vector<int>> steps(word1.size()+1, vector<int>(word2.size()+1, 0));

    // Init first row and col. Make sure the length is correct.
    // Note that f(0,0) is already initialized to 0.
    for (int i=1; i<=word1.size(); ++i) {
        steps[i][0] = i;
    }
    for (int i=1; i<=word2.size(); ++i) {
        steps[0][i] = i;
    }
        
    for (int i=1; i<=word1.size(); ++i) {
        for (int j=1; j<=word2.size(); ++j) {
            // Note we use i-1 and j-1 here because they are the subscripts
            if (word1[i-1] == word2[j-1]) 
                // no need get min with neighboring nodes. This is the min
                // in this case. It saves one step from the other neighbors. 
                steps[i][j] = steps[i-1][j-1];
            else
                // Get min from all neighbors. They all need one operation
                // to become the same. no magic
                steps[i][j] = min(min(steps[i][j-1], steps[i-1][j]), steps[i-1][j-1])+1;
        }
    }
    return steps.back().back();
}

/* Regular Expression Matching
  Implement regular expression matching with support for '.' and '*'.

'.' Matches any single character.
'*' Matches zero or more of the preceding element.

The matching should cover the entire input string (not partial).

The function prototype should be:
bool isMatch(const char *s, const char *p)
Example
isMatch("aa","a") -> false
isMatch("aa","aa") -> true
isMatch("aaa","aa") -> false
isMatch("aa", "a*") -> true
isMatch("aa", ".*") -> true
isMatch("ab", ".*") -> true
isMatch("aab", "c*a*b") -> true
 */
// Note: Must draw the 2D match array and analyze. 
bool isMatch(const char *s, const char *p) {
    // write your code here
    int slen = strlen(s);
    int plen = strlen(p);
    vector<vector<bool>> match(slen+1, vector<bool>(plen+1, false));

    // initialize. Row 0 and column 0 are used for 0 chars.
    // The variable i and j means number of chars so far, not the sub script.
    match[0][0] = true;
    // Initialize match[0][j] for a*b*c... case. 
    for (int i=2; i<=plen; i=i+2) {
        if (p[i-1]=='*') {
            match[0][i] = true;
            match[0][i-1] = true;
        }
        else
            break;
    }
        
    for (int i=1; i<=slen; ++i) {
        // Always check j>=2 when accessing the array.
        // This avoid crash and handles the "*a..." case.
        // If * is before a char, it will not match anything and set to false. 
        for (int j=1; j<=plen; ++j) {
            // There are three situations from the 2D picture.


            // normal case without '*'. Check if the next pair match. 
            if (match[i-1][j-1]) {
                if (s[i-1] == p[j-1] || p[j-1] == '.') {
                    match[i][j] = true;
                    continue;
                }
            }

            // The following two cases are special with '*'
            // Propogate the value for a*, which can mean no char.
            // Do this first because it sets two values.
            if (p[j-1] == '*' && j>=2&& match[i][j-2]==true) {
                match[i][j] = true;
                match[i][j-1] = true;
                continue;
            }
            
            // A case with '*'. If there is *, check if it matches the char
            // before *
            if (p[j-1]=='*' && match[i-1][j]) {
                if (j>=2 && (s[i-1] == p[j-2] || p[j-2] == '.')) {
                    match[i][j] = true;
                    continue;
                }
            }
        }
    }

    // It must match to the both ends. 
    return match.back().back();
}

// This one is from a previous phone interview.
// It is a DP/induction problem.
/*
Given an integer 'N', return count of all binary strings of length 'N',
without consecutive 1's.

Ex. if N=2 , return 3 (since we have 00, 10, 01)
Constraints: O(N) time and O(N) space

Note: O(N) can be 2N or 3N, the constant can be ignored. But you can not
      use embedded loops, which will be O(N^2). 
*/
// Analysis:
// Use induction, starting from N=1. Looks like tail recursion. 
// N=1, 0 or 1. 
// count_0[1]=1; count_1[1]=1
// count_0[2]=2; count_1[2]=1
// You may create two arrays of count_0 and count_1 to record #0 and #1 in
// each bit, then use induction. But it is not necessary to memorize all the
// bits, you only need to memorize the previous bit.
// Note: This is induction, which is simpler than DP. You only need to
//       keep the previous one or two results. 
int countString(int len)
{
    // For the first bit
    int count_0 = 1;
    int count_1 = 1;
    int count = 2;

    // For the i-th bit,
    // use induction to calculate the values from the previous bit. 
    for (int i = 2; i<= len; ++i)
    {
        int tmp = count_0+count_1;
        count_1 = count_0;
        count_0 = tmp;
        count *= count_0+count_1;
    }
    return count;
}

int main() {
    return 0;
}
