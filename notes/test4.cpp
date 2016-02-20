#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "ctype.h"
#include "stack"
#include <netinet/in.h>

#include "test.h"

/*
bool isprime( unsigned n)
{
    if (n < 2) return false;
    std:: vector < bool > known_prime( sqrt( n), true);
    for (size_t val = 2; val < = known_prime.size(); + + val)
    {
        if (known_prime[ val])
        {
            if (0 = = (n % val))
                return false;
            
            // Not Val+Val because they were covered by smaller factors already. Think about Val+Val as 2*Val, and it is covered by 2. 3*Val is covered by 3 and so on, till (Val-1)*Val.
            
            for (auto multiple = val ⋆ val; multiple < known_prime.size(); multiple + = val)
            {
                known_prime[ multiple] = false;
            }
        }
    }
    return true;
}
*/
/*
bool isprime2( unsigned n)
{
    if (n == 2) return true;

    if (n < 2 | | 0 == (n % 2)) return false;

    auto sqroot = sqrt( n);
    typedef std:: pair < unsigned, unsigned > seive_entry;
    std:: vector < seive_entry > primes;
    primes.push_back({4,2});
    unsigned current = primes.front().second;
    while (current < = sqroot)
    {
        auto front = primes.front(). first;
        if (( front - 2) == current)
        {
            unsigned prime = front - 1;
            if (0 = = (n % prime)) return false;
            primes.push_back({ prime ⋆ prime, prime});
            std:: make_heap( primes.begin(), primes.end(), std:: greater < seive_entry >());
        }

        current = front;
        std:: pop_heap( primes.begin(), primes.end(), std:: greater < seive_entry >());
        primes.back().first += primes.back().second;
        std:: push_heap( primes.begin(), primes.end(), std:: greater < seive_entry >());
    }
    return true;
}

*/

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

/*
  Given an array 'A' of 'N' elements (all integers), compute an array 'B'
  such that B[i] = prod(A[j]), j = 0,...,N-1 & j!=i, i= 0,..,N-1
  without using division

  Constraints: O(N) time and O(N) space

  Analysis:
  First note that O(N) means O(2N) or O(C*N), not O(1*N). So you may use
  more than one loops, but not embedded.

  example: B[1]=A[0]*A[2]*A[3]...A[N-1]
  B[i] = A[0]*A[1]*A[i-1]* A[i+1]*..*A[N-1]

  To calculate B[i], you need multiple the first half of A! and sencond part
  of A!. The first half is A[0]*A[1]*A[i-1], and the second half is
  A[i+1]*..*A[N-1].

  You can store the first half for each B[i] in an Array[N], and store the
  second half in another array. Then loop through i to get B[i].

  The implementation uses optimization. It store the second half in B,
  and uses one running variable to store the first half because you don't
  need to memorize the first half for all the i.

*/

int * computeArray(int * A, int lenA)
{
    int * B = new int[lenA];

    /*
    int firstHalfProduct[N];
    int secondHalfProduct[N];
    
    firstHalfProduct[0]=1;
    for (int i =1; i<N; ++i)
    {
        firstHalfProduct[i] = firstHalfProduct[i-1]*A[i-1];
    }
    */

    // Store the second half in B 
    B[lenA-1] = 1;
    for (int i=lenA-2; i>=0; --i)
    {
        B[i]=B[i+1]*A[i+1];
    }

    // Store the first half in firstHalf
    int firstHalf = 1;

    // Loop through i to calculate B and firstHalf. 
    for (int i=0; i<lenA; ++i)
    {
        B[i] = firstHalf*B[i];
        firstHalf *= A[i];
    }

    return B;
}

// calculate x^y with complexity < O(N)
// Use the successive squaring in section 6.2 of "Programming Problems: Advanced Algorithms", 2nd, location 1028. 

// Use recursion
int power(int x, int y)
{
    if (y==0)
        return 1;

    // Use tmp to same one duplicate recursion.
    auto tmp = power(x, y/2);
    if (y%2==0)
        return tmp*tmp;
    else
        return tmp*tmp*x;

}

// Iterative calculation of x^y from section 6.2 of "Programming Problems: Advanced Algorithms", 2nd.
/*
int power2( int x, unsigned y)
{
    // log_x() with base of 2
    std:: vector<int> memo( log_x( y) + 2);
    memo[0] = 1;
    memo[1] = x;

    for (auto index = 2; index < memo.size(); ++ index)
    {
        memo[index] = memo[index - 1] ⋆ memo[ index - 1];
    }

    int retval = memo[ 0];
    while(y)
    {
        auto pos = 1 + log_x( lowest_set_bit( y));
        retval ⋆ = memo[ pos];
        y = clear_last_bit( y);
    };

    return retval;
}
*/

/*
// Find an approximation int sqrt. Section 6.3 of Green's book, 2nd.
// Use binary search.
// All positive, use unsigned. 
// Listing 6.4: Integer Square Root
unsigned int sqrt( unsigned int x)
{
    if (0 = = x) return 0;
    int lower = 1;
    int higher = x;

    while (higher > lower)
    {
        int test = lower + (higher-lower) / 2;
        if (test ⋆ test > x)
        {
            if (test == higher) break;
            higher = test;
        }
        else
        {
            if (test == lower) break;
            lower = test;
        }
    }
    return lower;
}

// From Green's book, 2nd.
// Listing 6.5: Decimal Square Root for floating point. 
double sqrt2( double x)
{
    if (x < 0) return -1.0;
    if (x == 1) return 1.0;

    double higher = (x < 1) ? 1 : x;
    double lower = (x < 1) ? x : 1;
    
    while (( higher - lower) > epsilon)
    {
        double test = lower + (higher - lower) / 2;
        double eval = test ⋆ test;
        if (eval > x)
        {
            higher = test;
        }
        else
        {
            lower = test;
        }
    }
    return lower;
}

// From Green's Book, 2nd
//Listing 9.2: strcpy
void strcpy( char ⋆ destination, const char ⋆ source, int len)
{
    if (destination == source) return;
    if (! source | | !destination) return;
    if (source < destination && destination < (source + len))
    {
        while(len--) ⋆(destination + len) = ⋆(source + len);
    }
    else
    {
        while(len--) ⋆ destination + + = ⋆ source + +;
    }
}

// From Green's book, 2nd ed. 
// Listing 9.3: strtok
char ⋆ strtok( char ⋆ str, const char ⋆ delim)
{
    static char ⋆ next_str = nullptr;
    if (! delim) return nullptr;
    if (str) { next_str = str; }
    if (! next_str | | !⋆ next_str) return nullptr;

    auto span = strcspn( next_str, delim);
    while (! span && ⋆ next_str)
    {
        ++ next_str;
        span = strcspn( next_str, delim);
    }

    if (next_str && ⋆ next_str)

    {
        str = next_str;
        next_str + = std:: min( span + 1, strlen( next_str));
        str[ span] = '\ 0';
    }
    return str;
}

// From Green's book, 2nd ed.
// Listing 9.4: split
std:: vector < std:: string > split( const std:: string& str, const std:: string& delim)
{
    std:: vector < std:: string > tokens;
    size_t begin = 0;
    size_t end = 0;
    while (end < str.length())
    {
        while (end < str.length() && (! delim.length() || (delim.find( str[ end]) == std:: string:: npos)))
        {
            end ++;
        }

        auto len = end - begin;
        if (len > 0)
        {
            tokens.push_back( std:: string( str, begin, len));
        }
        begin = ++ end;
    }
    return tokens;
}

// From Green's book, 2nd ed. 
// Listing 9.7: Simple Rotationally Equivalent
bool rot_equiv( const char ⋆ a, const char ⋆ b)
{
    if (a = = b) return true;
    if (! a | | !b) return false;
    if (strlen( a) != strlen( b)) return false;
    std:: string b2 = b;
    b2 + = b;
    return nullptr != strstr( b2. c_str(), a);
}

// From Green's book, 2nd ed. 
// Listing 9.8: Determine if Two Strings are Anagrams
bool anagrams( const char ⋆ a, const char ⋆ b)
{
    if (! a && !b) return true;
    if (! a | | !b) return false;
    std:: array < int, 128 > histogram = {};

    while (⋆ a) { histogram[⋆ a + +] + = 1; }
    while (⋆ b) { histogram[⋆ b + +] -= 1; }
    for (auto& entry: histogram)
    {
        if (entry) return false;
    }
    return true;
}

// From Green's book, 2nd ed. 
// Listing 9.10: match

bool match( const char ⋆ str, const char ⋆ expr)
{
    // First consider the base cases.
    if (⋆ str == '\0' && ⋆ expr == '\0') return true;
    if (⋆ str == '\0' && ⋆(expr + 1) != '⋆') return false;
    if (⋆ expr == '\0') return false;

    // Then consider all the other non base cases, starting from *(expr+1)
    // being * or not. 
    if (⋆(expr + 1) != '⋆')
    {
        if (⋆expr != '.' && ⋆str != ⋆expr) return false;
        return match( str + 1, expr + 1);
    }

    // Wrong! What if expr="a*" where a != *str?
    // Here, you need match str with *expr and *(expr+1) first, then move
    // to expr+2
    if (match( str, expr + 2)) return true;

    if (⋆str == '\0') return false;
    
    if (⋆expr != '.' && ⋆ str != ⋆expr) return false;
    return match( str+1, expr);
}

// You are given a binary tree. The diameter of the tree is defined to be the longest distance between any two nodes.
// Write a function. The input is a binary tree. The output is the diameter (integer).

static int result;
int treeDepth(Node * node)
{
    if (node == NULL)
        return 0;

    int leftDepth = treeDepth(node->left);
    int rightDepth = treeDepth(node->right);
    
    int distance = leftDepth+rightDepth;
    if (result<distance)
        result=distance;

    return 1+leftDepth>rightDepth?leftDepth:rightDepth;
}
*/

// Find first duplicate
// Example: Input: [1, 5, 3, 4, 3], Return value: 3
// Can not use an extra array to implement bucket sort because the number is
// too large. May use map or hash table. But need perfect hash. 
// It is better to use sort, then find. The performance overhead is not much
int findDupe(int *a, int len)
{
    std::unordered_set<int> numbers;
    for (int i=0;i<len;++i)
    {
        auto ret = numbers.insert(a[i]);
        if (ret.second==false)
            return a[i];
    }
    return 0;
}

/*
Given array of length n:
  - The elements in the array are sorted (ascending)
  - There is *exactly* one duplicate in the array.
  - The elements are in the range 1 ­ n-1 (inclusive).

Write a function that finds the dupl
i 0 1 2 3 4 5 6 7 8
  1 2 3 4 5 6 7 8 9
  1 2 3 3 4 5 6 7 8
*/
          
int findDupe2(int *a, int len)
{
    int low = 0;
    int high = len-1;

    while (high>low)
    {
        int mid = low + (high-low)/2;
        
        if ((mid>0 && a[mid]==a[mid-1]) || (mid<len-1 && a[mid]==a[mid+1]))
            return a[mid];
        if (a[mid]==mid)
        {
//            if (high==mid) break;
            high = mid;
        }
        else
        {
//            if (low==mid) break;
            low = mid;
        }
    }
    return 0;
}

// Green, Bradley (2012-03-29). Programming Problems: A Primer for The Technical Interview (Kindle Locations 944-960).  . Kindle Edition.

bool find_path(node⋆ root, node⋆ target, std:: vector <node⋆ > ⋆ path)
{
    while (nullptr != root && nullptr != target && (path->empty() || path->back() != target))
    {
        path->push_back( root);
        if (target->value < root->value)
        {
            root = root->left;
        }
        else
        {
            root = root->right;
        }
    }

    return !target || path->back() == target;
}

// Find a common ancestor of two values in a binary tree. The tree is not sorted
//
//       6
//     4   7
//   8   5   3
// 1           2
 
// 1,3 => 6
// 1,5 => 4

// Need find a path first. 
 class Node
 {
    Node * left;
    Node * right;
    int _value;
 }

static std::list<Node*> path;
// return true if the value is found.
// Start from depth first search.
// 
// When calling a function or the same function in this case, the info
// (variables, etc. )about the current function is pushed in the stack. It will
// be popped out and used after the called function is returned.
// So keep this in mind when calling recursive function. 
bool getPath(Node * node, int v)
{
    if (node == NULL)
        return false;

    if (node->_value == v)
    {
        path.push_front(node);
        return true;
    }

    bool found=getPath(node->left, v);
    if (found)
    {
        path.push_front(node);
        return found;
    }
    
    found = getPath(node->right, v);
    if (found)
        path.push_front(node);

    return found;
}

// Green, Bradley (2013-02-27). Programming Problems: Advanced Algorithms (Kindle Locations 2212-2229).  . Kindle Edition. 
// Listing 9.11: unique

void unique( char ⋆ str)
{
    std:: vector < bool > seen( 128);
    char ⋆ trailing = str;

    do
    {
        size_t char_value = (size_t) ⋆ str;
        if (seen[ char_value]) continue;
        seen[ char_value] = true;
        if (trailing != str)
        {
            ⋆ trailing = ⋆ str;
        }
        + + trailing;
    } while(⋆ str + +);

    return;
}

// Green, Bradley (2013-02-27). Programming Problems: Advanced Algorithms (Kindle Locations 3133-3159).  . Kindle Edition. 
// Listing 12.4: Find Values With Sum
std:: tuple < bool, int, int > find_sum_to_k(const std:: vector < int >& values, int k)
{
    auto double_trouble = 0;
    std:: set < int > differences;

    for (auto& val : values)
    {
        auto difference = k - val;
        if (difference == val) ++double_trouble;

        differences.insert( difference);
    }

    for (auto& val : values)
    {
        if (differences.end()! = differences.find( val))
        {
            if (val == (k-val) && double_trouble == 1)
            {
                continue;
            }

            return std:: make_tuple( true, val, k - val);
        }
    }

    return std:: make_tuple( false, 0, 0);
}

// Green, Bradley (2013-02-27). Programming Problems: Advanced Algorithms (Kindle Locations 3205-3230).  . Kindle Edition. 
// Listing 12.6: Find Values With Sum Closest to 0
std::tuple<int, int, int> find_sum_closest_to_0( std:: vector < int >& values)
{
    if (values.size() < 2) throw std:: exception();
    radix_sort( values.begin(), values.end());

    std:: tuple < int, int, int > a_b_sum( 0, 0, std::numeric_limits<int>:: max());
    auto a = values.begin(), b = values.end() - 1;

    while (a < b)
    {
        auto sum = ⋆ a + ⋆ b;
        auto abs = std::abs( sum);
        if (abs < std::get<2>(a_b_sum))
        {
            a_b_sum = std::make_tuple(⋆ a, ⋆ b, abs);
        }

        if (sum < 0) ++a; else --b;
    }

    return a_b_sum;
}

// Green, Bradley (2013-02-27). Programming Problems: Advanced Algorithms (Kindle Locations 3243-3271).  . Kindle Edition. 
// Listing 12.7: Find Values With Sum to A Third
std:: tuple < bool, int, int, int > find_sum_to_c( std:: vector < int >& values){
    radix_sort( values.begin(), values.end()); 
    for (auto& c : values)
    {
        auto a = values.begin(), b = values.end() - 1; 
        while (a < b)
        {
            if (&c == &⋆a)
            {
                ++a; 
                continue; 
            }
            
            if (&c == &⋆b)
            {
                --b; 
                continue; 
            }

            auto sum = ⋆a + ⋆b - c; 

            if (0 = = sum)
            {
                return std:: make_tuple( true, ⋆ a, ⋆ b, c); 
            }
            else if (sum > 0) --b;
            else ++a; 
        }
    }

    return std:: make_tuple( false, 0, 0, 0); 
}

// Green, Bradley (2013-02-27). Programming Problems: Advanced Algorithms (Kindle Locations 3308-3312).  . Kindle Edition. 
// Listing 12.8: Find Max Difference

int find_max_difference( const std:: vector < int > values)
{
    if (values.size() < 2) throw std:: exception(); 
    int max = std::numeric_limits<int>::min(); 
    auto min = values.begin(); 
    auto i = values.begin() + 1; 
    while (i != values.end())
    {
        max = std::max( max, ⋆i - ⋆min);
        if (⋆i < ⋆min) min = i; 
        ++i; 
    }
    return max; 
}

// Green, Bradley (2013-02-27). Programming Problems: Advanced Algorithms (Kindle Locations 3334-3358).  . Kindle Edition. 
// Listing 12.9: Find Max Difference
std:: vector < bool >:: const_iterator partition_index( const std:: vector < bool >& v)
{
    auto total_sum = std:: accumulate( v.begin(), v.end(), 0); 
    auto best_sum = std:: max <int>( total_sum, v.size() - total_sum); 
    auto partition = v.end(); 
    auto current_sum = 0; 
    for (auto i = v.begin(); 
         i != v.end(); 
         ++i)
    {
        if (⋆i) ++current_sum; 
        auto left_zeros = 1 +( i-v.begin())-current_sum; 
        auto right_ones = total_sum - current_sum; 
        auto sum = std::max<int>( left_zeros + right_ones, v.size() - left_zeros - right_ones);
        
        if (sum > = best_sum)
        { best_sum = sum; 
            partition = i;
        }
    }

    return partition; 
}

// Green, Bradley (2013-02-27). Programming Problems: Advanced Algorithms (Kindle Locations 3403-3417).  . Kindle Edition. 
// Listing 12.11: Maximum Sub-Array
int max_subarray( const std:: vector < int >& v)
{
    if (v.empty()) return 0;
    auto sum = 0;
    auto max = std::numeric_limits<int>::min();

    for (auto& val : v)
    {
        sum + = val;
        max = std::max( max, sum);
        sum = std::max( sum, 0);
    }

    return max;
}

// Green, Bradley (2013-02-27). Programming Problems: Advanced Algorithms (Kindle Locations 3636-3647).  . Kindle Edition. 
// Listing 13.6: Towers of Hanoi Solution
void hanoi( int num, int src, int dest, int temp)
{
    if (num == 1)
    {
        move( src, temp);
        return;
    }

    hanoi( num-1, src, temp, dest);
    hanoi( 1, src, dest, temp);
    hanoi( num-1, temp, dest, src);
}

int main()
{
    printf("string count 6 = %d\n", countString(6));

    printf("power 3^5 = %d\n", power(3,5));

    return 0;
}

