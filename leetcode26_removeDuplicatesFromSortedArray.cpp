#include "./includes.cpp"

/*
    Отсортированность массива позволяет нам полагать, что проходя по элементам, 
    все изменяющиеся числа будут уникальными
*/

class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int n = nums.size();
        int write = 1;
        int previous = nums[0];
        for(int i = 1; i < n; ++i) {
            if(nums[i] != previous) {
                previous = nums[i];
                nums[write++] = nums[i];
            }
        }
        return write;
    }
};