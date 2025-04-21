#include "./includes.cpp"

using ValueIndex = std::pair<int, int>;

class Solution {
    public:
        int partition(std::vector<ValueIndex>& arr, int low, int high) {
            ValueIndex pivot = arr[high];
            int i = low - 1;
        
            for (int j = low; j < high; j++) {
                if (arr[j].first <= pivot.first) {
                    i++;
                    std::swap(arr[i], arr[j]);
                }
            }
            std::swap(arr[i + 1], arr[high]);
            return i + 1;
        }

        void quickSort(std::vector<ValueIndex>& arr, int low, int high) {
            if (low < high) {
                int pivotIndex = partition(arr, low, high);
                quickSort(arr, low, pivotIndex - 1);
                quickSort(arr, pivotIndex + 1, high);
            }
        }

        vector<int> twoSum(vector<int>& nums, int target) {

            std::vector<ValueIndex> numsWithIndex;
            for (int i = 0; i < nums.size(); i++) {
                numsWithIndex.push_back({nums[i], i});
            }

            int n = nums.size();
            auto right = n - 1;
            auto left = 0;

            quickSort(numsWithIndex, 0, n - 1);

            for(int i = 0; i < n; ++i) {
                auto sum = numsWithIndex[right].first + numsWithIndex[left].first;
                if(sum == target) {
                    return {numsWithIndex[left].second, numsWithIndex[right].second};
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