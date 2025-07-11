#include "includes.cpp"

class Solution {
public:
    int majorityElement(vector<int>& nums) {
        unordered_map<int, int> counts;

        float n = (float)nums.size() / 2;
        for(int num : nums){
            counts[num]++;
            if(counts[num] > n){
                return num;
            }
        }
        return 0;
    }
};