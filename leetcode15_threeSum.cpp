#include "./includes.cpp"

class Solution {
    /*

[0, 1, 2] - no solution
[-1, 0, 1] - [-1, 0, 1]
[-1,0,1,2,-1,-4] - sort - [-4, -1, -1, 0, 1, 2]
    */

    public:
    
        vector<vector<int>> threeSum(vector<int>& nums) {
            vector<vector<int>> results;
            int n = nums.size();

            sort(nums.begin(), nums.end());

            for(auto i = 0; i < n - 2; ++i) {
                
            }
            return results;
        }
    };