#include "includes.cpp"

class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> results;

        sort(candidates.begin(), candidates.end());
        backtrack(results, 0, {}, target, candidates, 0);

        return results;
    }

private:
    void backtrack(vector<vector<int>>& results, int currentSum, vector<int> currentCombination, const int& target, const vector<int>& candidates, int start) {

        for(int i = start; i < candidates.size(); i++) {
            int num = candidates[i];

            if(currentSum + num > target) {
                continue;
            }

            currentCombination.push_back(num);
            if(currentSum + num == target) {
                results.push_back(currentCombination);
            } else {
                backtrack(results, currentSum + num, currentCombination, target, candidates, i + 1);
            }
            currentCombination.pop_back();
        }
    }
};