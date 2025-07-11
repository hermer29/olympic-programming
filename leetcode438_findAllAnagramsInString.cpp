#include "includes.cpp"

class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> results;
        
        unordered_map<char, int> entries;
        unordered_map<char, int> pattern;
        int p_n = p.size();
        int s_n = s.size();

        for(char c : p) {
            pattern[c]++;
        }

        int anagram_start_index = 0;
        for(int j = 0; j < s_n; ++j) {
            entries[s[j]]++;

            if(j >= p_n - 1) {
                if(entries == pattern) {
                    results.push_back(anagram_start_index);
                }
                if(entries.count(s[anagram_start_index]) != 0) {
                    entries[s[anagram_start_index]]--;
                    if(entries[s[anagram_start_index]] == 0)
                        entries.erase(s[anagram_start_index]);
                }
                anagram_start_index++;
            }
        }
        return results;
    }
};