#include "./includes.cpp"

void bubbleSort(vector<int>* nums) {
    bool is_sorted = true;

    do
    {
        is_sorted = true;
        for(int i = 0, j = 1; j < nums -> size(); i++)
        {
            if(nums[i] > nums[j])
            {
                swap(nums[i], nums[j]);
                is_sorted = false;
            }
        }
    } while (!is_sorted);
}