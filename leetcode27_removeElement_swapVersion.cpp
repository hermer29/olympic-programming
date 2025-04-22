#include "./includes.cpp"

/*
[1, 3, 3, 4, 5] target - 3


[4, 5] target - 5
*/

class Solution {
    public:
        int removeElement(vector<int>& nums, int val) {
            int n = nums.size();
            while(n - 1 >= 0 && nums[n - 1] == val) {
                n--;
            } 

            for(int i = 0; i < n; ++i) {
                if(nums[i] == val) {
                    
                    std::swap(nums[i], nums[n - 1]);
                    n--;
                }
                while(n - 1 >= 0 && nums[n - 1] == val) {
                    n--;
                } 
            }
            return n;
        }
    };