#include "./includes.cpp"

class Solution {
    public:
        string longestCommonPrefix(vector<string>& strs) {
            int i = 0;
        
            while(true) {
                char curr_char = strs[0][i];
                for(auto str : strs) {
                    if(i == str.length()) {
                        goto Exit;
                    }
                    
                    if(str[i] != curr_char) {
                        goto Exit;
                    }

                }
                i++;
            }

            Exit:
            return strs[0].substr(0, i);
        }
    };