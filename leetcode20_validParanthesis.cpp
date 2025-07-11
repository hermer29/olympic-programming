#include "./includes.cpp"

class Solution {
    public:
        bool isValid(string s) {
            int n = s.size();
            unordered_map<char, char> matching = {
                {')', '('},
                {']', '['},
                {'}', '{'}
            };
            stack<char> stack;
            
            for(int i = 0; i < n; ++i) {
                if(s[i] == '[' || s[i] == '(' || s[i] == '{') {
                    stack.push(s[i]);
                    continue;
                }
                if(stack.size() == 0)
                    return false;
                auto matched = matching[s[i]];
                if(stack.top() != matched)
                    return false;
                stack.pop();
            }
            return stack.size() == 0;
        }
    };