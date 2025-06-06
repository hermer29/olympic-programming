#include "./includes.cpp"

/*
Example:
[1,2,3, 0,0,0] [4,5,6]


[1,2,3, 0,0,0] [2,5,6] !!!


[1,2,3,10,200,0,0,0] [100,200,300]


[2,0] [1]
*/

class Solution {
    public:
        void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
            int i = m - 1;
            int j = n - 1;
            int k = m + n - 1;

            while(j >= 0) {
                if(i >= 0 && nums1[i] > nums2[j]) {
                    nums1[k--] = nums1[i--];
                } else {
                    nums1[k--] = nums2[j--];
                }
            }
        }
    };