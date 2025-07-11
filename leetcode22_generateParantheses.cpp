#include "includes.cpp"

class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> result;
        backtrack(result, n, "", 0, 0);
        return result;
    }

private:
    void backtrack(vector<string>& result, int n, string current, int open, int close) {
        if(current.size() == n * 2) {
            result.push_back(current);
            return;
        }

        if(open < n) {
            backtrack(result, n, current + "(", open + 1, close);
        }

        if(close < open) {
            backtrack(result, n, current + ")", open, close + 1);
        }
    }
};