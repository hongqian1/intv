/*
  Given a sorted array of integers, find kth smallest square of the integer.
*/

/*
  Notes:
  1. There are negative numbers.
  2. Need find kth, not first. 
  3. k starts from 0, not 1.

  The problem with this solution is that it uses O(N) in the worst case.
  To use O(log N), must use binary search to find kth smallest element
  in two sorted array. Explain how to do that.
  See Lintcode "Median of two sorted arrays."
*/

#include <climits>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int findKthSmallestSquare(vector<int> &nums, int k) {
    // Find the index of the first non-negative number.
    auto it = lower_bound(nums.begin(), nums.end(), 0);
    int right = it - nums.begin(); // convert to index.
    int left = right - 1; // for negative numbers.

    // Use two pointers. right is for positive numbers, and left for negative.
    // loop for k + 1 times to find the kth.
    int result = INT_MAX;
    for (int i = 0; i <= k; ++i) {
        // boundary check first. 
        int rightNum = (right < nums.size()) ? nums[right] : INT_MAX;
        int leftNum = (left >= 0) ? -nums[left] : INT_MAX;
        if (rightNum < leftNum) {
            result = rightNum;
            right++;
        } else {
            result = leftNum;
            left--;
        }
    }
    return result * result;
}

int main() {
    vector<int> nums = {-8, -4, -3, -1, 0, 3, 4, 6, 10};
    int k = 4;
    cout << findKthSmallestSquare(nums, k) << endl;
    return 0;
}

