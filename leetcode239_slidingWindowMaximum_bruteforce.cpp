#include "./includes.cpp"

/*
    This solution cannot be accepted because of time restrictions
*/

class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> results;

        for(int i = 0; i < n - k + 1; i++) {
            int max_found = nums[i];
            int max_found_index = i;
            for(int j = 1; j < k; j++) {
                int current_index = i + j;
                if(nums[current_index] > max_found) {
                    max_found = nums[current_index];
                    max_found_index = current_index;
                }
            }
            results.push_back(max_found);
        }
        return results;
    }
};