#include "includes.cpp"

class Solution {
public:
    int romanToInt(const string s) {
        static const unordered_map<char, int> numbers = {
            {'I', 1},
            {'V', 5},
            {'X', 10},
            {'L', 50},
            {'C', 100},
            {'D', 500},
            {'M', 1000}
        };
        
        const int n = s.size();
        int result = numbers.at(s[0]);
        int prev = result;

            const char value = s[i];
        int current = numbers.at(value);

        for(int i = 1; i < n; i++) {
            const int current = 

            if(current > )
        }
    }
};