#include "./includes.cpp"

/*
Example:
[1,2,3, 0,0,0] [4,5,6]


[1,2,3, 0,0,0] [2,5,6]


[1,2,3,10,200,0,0,0] [100,200,300]


[2,0] [1]
*/

class Solution {
    public:
        void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
            if(m == 0) {
                nums1.assign(nums2.begin(), nums2.end());
                return;
            }
            if(n == 0) {
                return;
            }

            auto extended_m = m;
            auto nums1_i = 0; 
            for(int nums2_i = 0; nums1_i < n + m; ) {
                if(extended_m == n + m)
                    return;
                if(nums1_i >= extended_m) {
                    nums1[nums1_i] = nums2[nums2_i];
                    nums2_i++;
                } else if(nums1[nums1_i] >= nums2[nums2_i]) {
                    for(int shift = extended_m - 1; shift >= nums1_i; --shift) { //shift [nums1_i, m - 1]
                        nums1[shift + 1] = nums1[shift];
                    }
                    extended_m++;
                    nums1[nums1_i] = nums2[nums2_i];
                    nums2_i++;
                }
                nums1_i++;
            }
        }
    };