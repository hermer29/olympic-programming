#include "includes.cpp"

/*
    Различная сумма символов ASCII может дать одинаковую сумму, решение невалидное
*/

class Solution {
public:
    bool isAnagram(string s, string t) {
        int resultS;
        int resultT;

        if(s.size() != t.size())
            return false;

        int n = s.size();

        for(int i = 0; i < n; ++i) {
            resultS += (int)s[i];
            resultT += (int)t[i];
        }

        cout << resultS << " " << resultT;
        return resultS == resultT;
    }
};