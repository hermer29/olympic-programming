#include "./includes.cpp"

class Solution {
    public:
        vector<int> twoSum(vector<int>& nums, int target) {
            int n = nums.size();
            unordered_map<int, int> memo;

            for(int i = 0; i < n; ++i) {
                auto sub = target - nums[i];
                if(memo.find(sub) != memo.end()) {
                    return {i, memo[sub]};
                } else {
                    memo[nums[i]] = i;
                }
            }
            return {};
        }
    };