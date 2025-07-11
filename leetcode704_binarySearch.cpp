#include "./includes.cpp"

/*
    right + left / 2
    
    left = 0
    right = 9
    result = 4

    left = 8
    right = 9
    result = 8

*/

class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;

        while(left <= right) {
            int pivot = (right + left) / 2;
            
            if(target > nums[pivot]) {
                left = pivot + 1;
            } else if (target < nums[pivot]) {
                right = pivot - 1;
            } else {
                return pivot;
            }
        }
        return -1;
    }
};