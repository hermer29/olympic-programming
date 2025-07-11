#include "includes.cpp"

class Solution {
public:
    vector<string> letterCombinations(string digits) {
        if(digits.size() == 0)
            return {};
        vector<string> results;
        backtrack(results, 0, digits.size(), digits, "");
        return results;
    }

private:
    unordered_map<char, vector<char>> digitsMapping {
        {'2', {'a', 'b', 'c'}},
        {'3', {'d', 'e', 'f'}},
        {'4', {'g', 'h', 'i'}},
        {'5', {'j', 'k', 'l'}},
        {'6', {'m', 'n', 'o'}},
        {'7', {'p', 'q', 'r', 's'}},
        {'8', {'t', 'u', 'v'}},
        {'9', {'w', 'x', 'y', 'z'}}
    };

    void backtrack(vector<string>& results, int current, int n, string digits, string combination) {
        if(current == n) {
            results.push_back(combination);
            return;
        }

        for(const char& c : digitsMapping[digits[current]]) {
            backtrack(results, current + 1, n, digits, combination + c);
        }
    }
};