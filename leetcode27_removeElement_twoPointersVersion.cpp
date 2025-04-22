#include "./includes.cpp"

/*
[1, 3, 3, 4, 5] target - 3 [1, 4, 5, -, -] - 3 


[4, 5] target - 5
*/

class Solution {
    public:
        int removeElement(vector<int>& nums, int val) {
            int k = 0;

            for(int elem : nums) {
                if(elem != val) {
                    nums[k++] = elem;
                }
            }
            return k;
        }
    };