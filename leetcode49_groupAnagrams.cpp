#include "includes.cpp"

class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> groups;

        for(string str : strs) {
            vector<int> thisAnagram(26, 0);
            
            for(char c : str) {
                thisAnagram[c - 'a']++;
            }
            
            string key;
            for(int num : thisAnagram) {
                key += to_string(num) + "#";
            }

            groups[key].push_back(str);
        }

        vector<vector<string>> result;

        for(const auto& pair : groups) {
            result.push_back(pair.second);
        }
        return result;
    }
};