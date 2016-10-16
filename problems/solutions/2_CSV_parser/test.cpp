/*
2. CSV Parser
   Airbnb
 
John,Smith,john.smith@gmail.com,Los Angeles,1
Jane,Roberts,janer@msn.com,"San Francisco, CA",0
"Alexandra ""Alex""",Menendez,alex.menendez@gmail.com,Miami,1
"""Alexandra Alex"""
John|Smith|john.smith@gmail.com|Los Angeles|1
Jane|Roberts|janer@msn.com|San Francisco, CA|0
Alexandra "Alex"|Menendez|alex.menendez@gmail.com|Miami|1
"Alexandra Alex"

Understand the problem:
For this problem, there are several cases need to consider:
1. For comma, transform to |
2. If comma is inside a quote, don't treat the comma as separated. Remove the comma and print the entire token. e.g. "San Francisco, CA" => San Francisco, CA
3. If there are double quotes, remove one. e.g. "Alexandra ""Alex""" => Alexandra "Alex". 
Note that """Alexandra Alex""" becomes "Alexandra Alex" because we first remove the outer-most quote, and then remove one quote of the double quote.

*/
/*
  Note: the function only need to parse one line at a time.
  Note: Only the out most quote means quote. Any quote inside is a valid char.
 */

#include <climits>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

// This solution does not use stack because we only care one level of quotes,
// which is the outmost quotes. So no need stack, although it is OK.
// Use two pointers, the right pointer only checks the ending quote. 
string parseCSV(const string &str) {
    int left = 0;
    int right = str.size() - 1;

    string result;
    // Run left pointer until it hits a quote.
    // Do not forget boundary check in while loop's condition!
    while (left < str.size() && str[left] != '"') {
        char c = str[left++];
        if (c == ',') {
            result.push_back('|');
        } else {
            result.push_back(c);
        }
    }

    // left now points to a quote or end.
    if (left == str.size())
        return result;

    while (str[right] != '"')
        right--;

    // Process chars inside the quotes.
    for (int i = left + 1; i < right; i++) {
        char c = str[i];
        result.push_back(c);
        if (c == '"' && str[i + 1] == '"')
            i++;
    }

    // process the rest, which is outside the quote.
    // copy the same code from the above.
    // No need to write a new function because it is too simple.
    for (int i = right + 1; i < str.size(); i++) {
        char c = str[i];
        if (c == ',') {
            result.push_back('|');
        } else {
            result.push_back(c);
        }
    }

    return result;
}

// This solution uses stack. 
string parseCSV2(const string &str) {
    string result;

    // Must use a stack because you have to process from back for quotes.
    // The data type inside quote is important.
    // Use string in this case because it covers all kinds of data.
    vector<string> quoteStack; // use vector for stack
    string sub; // use sub to keep data between quotes. 

    // parse the string. Store strings in stack if there is ".
    for (int i = 0; i < str.size(); ++i) {
        char c = str[i];
        if (c == '"') {
            if (!sub.empty()) {
                quoteStack.push_back(sub);
                sub.clear();
            }
            quoteStack.push_back("\"");
        } else if (c == ',') {
            if (quoteStack.empty())
                result.push_back('|');
            else
                sub.push_back(c);
        } else {
            if (quoteStack.empty())
                result.push_back(c);
            else
                sub.push_back(c);
        }
    }

    if (quoteStack.empty())
        return result;

    // You don't know if there are more " later, so you have to
    // keep sub to the end.
    // Note: Must replace ',' with '|' in the last substring,
    // which is not in a quote.
    // Note: sub may be empty, but that is OK. 
    for (auto &c : sub) {
        if (c == ',')
            c = '|';
    }

    // The first and last are outmost quotes and should be discarded.
    // Now use quoteStack as vector. 
    for (int i = 1; i < quoteStack.size() - 1; ++i) {
        result += quoteStack[i];
        if (quoteStack[i] == "\"" && quoteStack[i+1] == "\"") {
            i++;
        }
    }

    // don't forget to add sub.
    return result + sub;
}

int main() {
    // must use escape \" for quote inside quote.
    string input1 = "John,Smith,john.smith@gmail.com,Los Angeles,1";
    cout << parseCSV(input1) << endl;

    string input2 = "Jane,Roberts,janer@msn.com,\"San Francisco, CA\",0";
    cout << parseCSV(input2) <<endl;;

    string input3 = "\"Alexandra \"\"Alex\"\"\",Menendez,alex.menendez@gmail.com,Miami,1";
    cout << parseCSV(input3) << endl;
     
    string input4 = "\"\"\"Alexandra Alex\"\"\"";
    cout << parseCSV(input4) << endl;
}

