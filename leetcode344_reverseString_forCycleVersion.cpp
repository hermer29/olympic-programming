#include "./includes.cpp"

class Solution {
    public:
        void reverseString(vector<char>& s) {
            int n = s.size();
    
            int tail = n - 1;
            for(int head = 0; head < n; ++head) {
                if(head >= tail)
                    return;
                
                swap(s[head], s[tail]);
                tail--;
            }
        }
    };