#include "includes.cpp"

class Solution {
public:
    bool isAnagram(string s, string t) {
        unordered_map<char, int> result;

        if(s.size() != t.size())
            return false;

        int n = s.size();

        for(int i = 0; i < n; ++i) {
            result[s[i]] += 1;
        }

        for(int i = 0; i < n; ++i) {
            result[t[i]] -= 1;

            if(result[t[i]] < 0) {
                return false;
            }
        }
        return true;
    }
};