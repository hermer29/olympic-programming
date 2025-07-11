#include "includes.cpp"

class Solution {
public:
    string triangleType(vector<int>& nums) {
        if(nums[0] == nums[1] && nums[1] == nums[2]) {
            return "equilateral";
        }

        sort(nums.begin(), nums.end());
        if(nums[0] + nums[1] <= nums[2]) {
            return "none";
        }

        if(nums[1] == nums[2] ^ nums[0] == nums[1]) {
            return "isosceles";
        }

        return "scalene";
    }
};