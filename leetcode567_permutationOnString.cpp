#include "includes.cpp"

class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        int n = s1.size();
        int m = s2.size();
        
        unordered_map<char, int> s1map;
        deque<char> s2mem;
        unordered_map<char, int> rolesPlayed;
        unordered_map<char, int> s2map;

        for(int i = 0; i < n; ++i) {
            rolesPlayed[s1[i]]++;
            if(s1map.count(s1[i]) > 0) {
                s1map[s1[i]] = -1;
            } else {
                s1map[s1[i]] = i;
            }
        }

        for(int i = 0; i < m; i++) {
            if(s2mem.size() > n) {
                s2mem.pop_front();
            }

            bool isInS1Map = s1map.count(s2[i]) == 1;
            if(isInS1Map) {
                s2mem.push_back(s2[i]);
            }

            if(s2mem.size() == n) {

                for(int j = 0; j < n; j++) {
                    s2map[s2mem[j]]++;
                    if(s1[j] == s2mem[j] && s1map[s1[j]] != -1) {
                        break;
                    }
                }
                if(s2map == rolesPlayed)
                {
                    return true;
                }
                s2map.clear();
            }

            if(!isInS1Map) {
                s2mem.clear();
            }

        }
        return false;
    }
};