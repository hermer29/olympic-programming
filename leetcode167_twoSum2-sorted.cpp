#include "./includes.cpp"

class Solution {
    public:
        vector<int> twoSum(vector<int>& nums, int target) {
            int n = nums.size();
            auto right = n - 1;
            auto left = 0;

            for(int i = 0; i < n; ++i) {
                auto sum = nums[right] + nums[left];
                if(sum == target) {
                    return {left + 1, right + 1};
                }
                if(sum > target) {
                    right--;
                } else if(sum < target) {
                    left++;
                }
            }
            return {};
        }
    };